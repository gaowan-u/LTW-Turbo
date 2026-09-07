/**
 * 文件功能：QUADS → TRIANGLES 转换实现。
 * GLES 3.x 移除了 GL_QUADS（桌面枚举 0x0007），MC 1.12（lwjglx 兼容层）
 * 的方块面却用基础版 glDrawArrays/glDrawElements 以 QUADS 提交，导致
 * 驱动报 "draw mode 7 is unknown" 且绘制被丢弃（黑屏）。
 * 这里拦截这些调用，把每 4 个顶点展开为 2 个三角形（a,b,c + a,c,d）。
 * 配合 EBO CPU 影子副本零同步读索引；客户端数组路径按 (first, count)
 * 缓存展开结果，避免每帧重传。
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "quads.h"
#include "proc.h"
#include "egl.h"
#include "debug.h"
#include "fixed_pipeline.h"
#include "unordered_map/int_hash.h"

#ifndef GL_QUADS
#define GL_QUADS 0x0007
#endif

// ---- EBO CPU 影子副本 ----
// MC 1.12 区块以 GL_QUADS + ELEMENT_ARRAY_BUFFER 提交，QUADS→TRIANGLES 展开
// 需要读取索引。旧实现每帧 glMapBufferRange 从 GPU 读回，造成同步停顿（帧率和
// 功耗的主要瓶颈）。这里在 glBufferData/glBufferSubData 时保留 CPU 副本，展开时
// 直接读影子内存，零同步；同时用数据版本号跳过重复上传展开结果。
#define EBO_SHADOW_MAX_SIZE   (256 * 1024)        // 单缓冲影子上限（超出回退 map）
#define EBO_SHADOW_TOTAL_CAP  (32 * 1024 * 1024)  // 影子总内存上限

typedef struct {
    uint8_t*   data;
    GLsizeiptr size;       // 有效数据长度
    GLsizeiptr alloc;      // 分配容量
    uint64_t   generation; // 数据版本（全局单调递增）
    bool       valid;      // 副本是否完整可信
} ebo_shadow_t;

static uint64_t ebo_shadow_seq = 0;

static ebo_shadow_t* ebo_shadow_find(GLuint buffer) {
    if(!current_context || !current_context->ebo_shadow_map) return NULL;
    return (ebo_shadow_t*)unordered_map_get(current_context->ebo_shadow_map, (void*)(intptr_t)buffer);
}

static void ebo_shadow_free_entry(ebo_shadow_t* sh) {
    if(!sh) return;
    if(sh->data) free(sh->data);
    free(sh);
}

void ltw_ebo_shadow_invalidate(GLuint buffer) {
    if(!current_context || !current_context->ebo_shadow_map) return;
    ebo_shadow_t* sh = (ebo_shadow_t*)unordered_map_remove(current_context->ebo_shadow_map, (void*)(intptr_t)buffer);
    if(sh) {
        if(current_context->ebo_shadow_total > (size_t)sh->alloc)
            current_context->ebo_shadow_total -= (size_t)sh->alloc;
        else
            current_context->ebo_shadow_total = 0;
        ebo_shadow_free_entry(sh);
    }
}

// 内存超限时整体清空（简单可靠；影子会在下次上传时重新建立）。
static void ebo_shadow_clear_all(void) {
    if(!current_context || !current_context->ebo_shadow_map) return;
    unordered_map_iterator* it = unordered_map_iterator_alloc(current_context->ebo_shadow_map);
    if(it) {
        void* key = NULL;
        void* value = NULL;
        while(unordered_map_iterator_has_next(it)) {
            if(unordered_map_iterator_next(it, &key, &value)) {
                ebo_shadow_free_entry((ebo_shadow_t*)value);
            }
        }
        unordered_map_iterator_free(it);
    }
    unordered_map_clear(current_context->ebo_shadow_map);
    current_context->ebo_shadow_total = 0;
}

void ltw_ebo_shadow_upload(GLenum target, GLsizeiptr size, const void* data, GLintptr offset, bool full) {
    if(!current_context || target != GL_ELEMENT_ARRAY_BUFFER) return;
    if(size <= 0 || size > EBO_SHADOW_MAX_SIZE) return;

    GLint eab = 0;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);
    if(eab == 0) return;
    // LTW 自己的 scratch/multidraw 缓冲不影子化（内容由展开/拷贝生成，无同步收益）
    if((GLuint)eab == current_context->quads_scratch_buffer ||
       (GLuint)eab == current_context->multidraw_element_buffer) return;

    ebo_shadow_t* sh = ebo_shadow_find((GLuint)eab);

    if(full) {
        if(!sh) {
            sh = (ebo_shadow_t*)calloc(1, sizeof(ebo_shadow_t));
            if(!sh) return;
            unordered_map_put(current_context->ebo_shadow_map, (void*)(intptr_t)eab, sh);
        } else if(current_context->ebo_shadow_total > (size_t)sh->alloc) {
            current_context->ebo_shadow_total -= (size_t)sh->alloc;
        }
        if(sh->alloc < size) {
            uint8_t* nd = (uint8_t*)realloc(sh->data, (size_t)size);
            if(!nd) {
                // 扩容失败：丢弃影子，回退到每帧 map
                ltw_ebo_shadow_invalidate((GLuint)eab);
                return;
            }
            sh->data = nd;
            sh->alloc = size;
        }
        sh->size = size;
        sh->valid = (data != NULL);
        if(data) memcpy(sh->data, data, (size_t)size);
        sh->generation = ++ebo_shadow_seq;
        current_context->ebo_shadow_total += (size_t)sh->alloc;
    } else {
        // glBufferSubData 增量更新
        if(!sh || !sh->valid) return;
        if(offset < 0 || offset + size > sh->size) { sh->valid = false; return; }
        if(data) memcpy(sh->data + offset, data, (size_t)size);
        sh->generation = ++ebo_shadow_seq;
    }

    if(current_context->ebo_shadow_total > EBO_SHADOW_TOTAL_CAP) {
        ebo_shadow_clear_all();
    }
}

// 取影子副本；命中返回 CPU 指针并写出数据版本号，否则返回 NULL。
static const void* ebo_shadow_get(GLuint buffer, GLintptr offset, GLsizeiptr bytes, uint64_t* generation) {
    if(!current_context || bytes < 0) return NULL;
    ebo_shadow_t* sh = ebo_shadow_find(buffer);
    if(!sh || !sh->valid || !sh->data) return NULL;
    if(offset < 0 || offset + bytes > sh->size) return NULL;
    if(generation) *generation = sh->generation;
    return sh->data + offset;
}

void ltw_ebo_shadow_destroy(context_t* ctx) {
    if(!ctx || !ctx->ebo_shadow_map) return;
    unordered_map_iterator* it = unordered_map_iterator_alloc(ctx->ebo_shadow_map);
    if(it) {
        void* key = NULL;
        void* value = NULL;
        while(unordered_map_iterator_has_next(it)) {
            if(unordered_map_iterator_next(it, &key, &value)) {
                ebo_shadow_free_entry((ebo_shadow_t*)value);
            }
        }
        unordered_map_iterator_free(it);
    }
    unordered_map_free(ctx->ebo_shadow_map);
    ctx->ebo_shadow_map = NULL;
    ctx->ebo_shadow_total = 0;
}

static GLsizei quads_type_bytes(GLenum type) {
    switch (type) {
        case GL_UNSIGNED_BYTE:  return 1;
        case GL_UNSIGNED_SHORT: return 2;
        case GL_UNSIGNED_INT:   return 4;
        default: return 0;
    }
}

// 读取源索引到 uint32 数组。来源可能是 CPU 指针或当前绑定的 ELEMENT_ARRAY_BUFFER（indices 为偏移）。
// 优先读 EBO 的 CPU 影子副本（零同步），无影子时才 map GPU 缓冲区。
// 返回 malloc 数组（调用方 free），失败返回 NULL；src_ebo/src_gen 记录数据来源用于跳过重复上传。
static uint32_t* quads_read_indices(GLenum type, GLsizei count, const void* indices, GLuint* src_ebo, uint64_t* src_gen) {
    GLsizei tbytes = quads_type_bytes(type);
    if(tbytes == 0) return NULL;
    *src_ebo = 0;
    *src_gen = 0;

    GLint eab = 0;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);

    const void* src = indices;
    void* mapped = NULL;
    GLint copy_ebo = 0;

    if(eab != 0) {
        *src_ebo = (GLuint)eab;
        // 影子副本命中：直接读 CPU 内存，避免每帧 map GPU 缓冲区造成同步停顿
        const void* shadow = ebo_shadow_get((GLuint)eab, (GLintptr)indices,
                                            (GLsizeiptr)count * tbytes, src_gen);
        if(shadow) {
            src = shadow;
        } else {
            es3_functions.glGetIntegerv(GL_COPY_READ_BUFFER_BINDING, &copy_ebo);
            es3_functions.glBindBuffer(GL_COPY_READ_BUFFER, (GLuint)eab);
            mapped = es3_functions.glMapBufferRange(GL_COPY_READ_BUFFER,
                                                    (GLintptr)indices,
                                                    (GLsizeiptr)count * tbytes,
                                                    GL_MAP_READ_BIT);
            if(!mapped) {
                es3_functions.glBindBuffer(GL_COPY_READ_BUFFER, (GLuint)copy_ebo);
                return NULL;
            }
            src = mapped;
        }
    }

    uint32_t* out = (uint32_t*)malloc((size_t)count * sizeof(uint32_t));
    if(out) {
        if(tbytes == 1) {
            const uint8_t* p = (const uint8_t*)src;
            for(GLsizei i = 0; i < count; i++) out[i] = p[i];
        } else if(tbytes == 2) {
            const uint16_t* p = (const uint16_t*)src;
            for(GLsizei i = 0; i < count; i++) out[i] = p[i];
        } else {
            const uint32_t* p = (const uint32_t*)src;
            for(GLsizei i = 0; i < count; i++) out[i] = p[i];
        }
    }

    if(mapped) {
        es3_functions.glUnmapBuffer(GL_COPY_READ_BUFFER);
        es3_functions.glBindBuffer(GL_COPY_READ_BUFFER, (GLuint)copy_ebo);
    }
    return out;
}

// 把展开后的索引上传到临时 ELEMENT_ARRAY_BUFFER 并绘制。
// first/count 仅客户端数组（非索引）路径使用：展开结果只取决于二者，
// 与顶点内容无关，相同参数时可跳过重复上传。client_arrays 为 true 表示
// 非索引 glDrawArrays（源索引即 first..first+count-1），false 表示索引路径
// （源索引每次调用内容可变，src_ebo==0 时一律重新上传）。
static void quads_draw_triangles(GLsizei quads, const uint32_t* indices, GLuint src_ebo, uint64_t src_gen,
                                 GLint first, GLsizei count, bool client_arrays) {
    context_t* ctx = current_context;
    if(!ctx || ctx->quads_scratch_buffer == 0) return;

    GLsizei tri_count = quads * 6;
    GLint eab = 0;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);

    // 无 program 时用固定管线默认 shader（MC 1.12 GUI 的 QUADS 即时模式路径）
    bool fp_bound = fp_bind_default_program();
    // GL_ELEMENT_ARRAY_BUFFER 绑定属于当前 VAO：fp_bind_default_program 已切到
    // 私有 fp_vao，必须在这里把 scratch EBO 绑进 fp_vao，再上传索引并绘制，
    // 否则 glDrawElements 会读到 fp_vao 里残留的旧 EBO（时序相关，偶发黑块）。
    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->quads_scratch_buffer);
    // 同一来源 EBO + 同一数据版本 + 同样顶点数时，scratch 内容未变，跳过重新上传
    bool skip_upload;
    if(src_ebo != 0) {
        skip_upload = (src_gen != 0 &&
                       ctx->quads_last_ebo == src_ebo &&
                       ctx->quads_last_gen == src_gen &&
                       ctx->quads_last_tri_count == tri_count);
    } else if(client_arrays) {
        // 客户端数组路径：缓存键是 (first, count)
        skip_upload = (ctx->quads_last_ebo == 0 &&
                       ctx->quads_last_first == first &&
                       ctx->quads_last_count == count);
    } else {
        // 客户端索引路径：内容随每次调用变化，不缓存
        skip_upload = false;
    }
    if(!skip_upload) {
        es3_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                   (GLsizeiptr)tri_count * 4,
                                   indices, GL_STREAM_DRAW);
        if(src_ebo != 0) {
            ctx->quads_last_ebo = src_ebo;
            ctx->quads_last_gen = src_gen;
            ctx->quads_last_tri_count = tri_count;
        } else if(client_arrays) {
            // 客户端数组路径：记录 (first, count)，并清空其他缓存键
            ctx->quads_last_ebo = 0;
            ctx->quads_last_gen = 0;
            ctx->quads_last_tri_count = 0;
            ctx->quads_last_first = first;
            ctx->quads_last_count = count;
        } else {
            // 客户端索引路径：标记 scratch 内容来自该路径，使其他缓存键失效
            ctx->quads_last_ebo = (GLuint)-1;
            ctx->quads_last_gen = 0;
            ctx->quads_last_tri_count = 0;
            ctx->quads_last_first = 0;
            ctx->quads_last_count = 0;
        }
    }
    if(fp_bound) {
        fp_prepare_client_arrays(quads * 4);
        es3_functions.glDrawElements(GL_TRIANGLES, tri_count, GL_UNSIGNED_INT, NULL);
        fp_ge_check("quads_de_bound");
        fp_unbind_default_program();
    } else {
        es3_functions.glDrawElements(GL_TRIANGLES, tri_count, GL_UNSIGNED_INT, NULL);
        fp_ge_check("quads_de_nobound");
    }

    // fp_unbind_default_program 已恢复应用 VAO，这里恢复它原来的 EAB 绑定。
    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)eab);
}

// 确保展开用的可复用缓冲区容量足够
static bool quads_ensure_expanded(GLsizei quads) {
    context_t* ctx = current_context;
    if(!ctx) return false;
    GLsizei need = (GLsizei)quads * 6;
    if(ctx->quads_expanded_cap < need) {
        uint32_t* nd = (uint32_t*)realloc(ctx->quads_expanded, (size_t)need * sizeof(uint32_t));
        if(!nd) return false;
        ctx->quads_expanded = nd;
        ctx->quads_expanded_cap = need;
    }
    return true;
}

// 逐 quad 展开：quad (a,b,c,d) -> triangles (a,b,c) (a,c,d)
static void quads_expand(const uint32_t* src, GLsizei count, uint32_t* dst) {
    GLsizei quads = count >> 2;
    for(GLsizei q = 0; q < quads; q++) {
        uint32_t a = src[q * 4 + 0];
        uint32_t b = src[q * 4 + 1];
        uint32_t c = src[q * 4 + 2];
        uint32_t d = src[q * 4 + 3];
        uint32_t* t = dst + q * 6;
        t[0] = a; t[1] = b; t[2] = c;
        t[3] = a; t[4] = c; t[5] = d;
    }
}

bool ltw_quads_draw_arrays(GLenum mode, GLint first, GLsizei count) {
    if(mode != GL_QUADS || count < 4 || (count & 3) != 0) return false;
    context_t* ctx = current_context;
    if(!ctx) return false;

    GLsizei quads = count >> 2;
    if(!quads_ensure_expanded(quads)) return false;
    // 非索引路径的源索引就是 first..first+count-1，直接生成展开结果，
    // 免去临时数组的 malloc/展开两趟开销
    for(GLsizei q = 0; q < quads; q++) {
        uint32_t a = (uint32_t)(first + q * 4 + 0);
        uint32_t b = (uint32_t)(first + q * 4 + 1);
        uint32_t c = (uint32_t)(first + q * 4 + 2);
        uint32_t d = (uint32_t)(first + q * 4 + 3);
        uint32_t* t = ctx->quads_expanded + q * 6;
        t[0] = a; t[1] = b; t[2] = c;
        t[3] = a; t[4] = c; t[5] = d;
    }
    quads_draw_triangles(quads, ctx->quads_expanded, 0, 0, first, count, true);
    return true;
}

bool ltw_quads_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if(mode != GL_QUADS || count < 4 || (count & 3) != 0) return false;
    context_t* ctx = current_context;
    if(!ctx) return false;

    GLsizei quads = count >> 2;
    GLuint src_ebo = 0;
    uint64_t src_gen = 0;
    uint32_t* src = quads_read_indices(type, count, indices, &src_ebo, &src_gen);
    if(!src) return false;
    if(!quads_ensure_expanded(quads)) { free(src); return false; }

    quads_expand(src, count, ctx->quads_expanded);
    quads_draw_triangles(quads, ctx->quads_expanded, src_ebo, src_gen, 0, count, false);

    free(src);
    return true;
}
