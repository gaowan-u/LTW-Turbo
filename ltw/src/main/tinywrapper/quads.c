/**
 * QUADS -> TRIANGLES 转换实现。
 * GLES 3.x 移除了 GL_QUADS（桌面枚举 0x0007），MC 1.12（lwjglx 兼容层）
 * 的方块面却用基础版 glDrawArrays/glDrawElements 以 QUADS 提交，导致
 * 驱动报 "draw mode 7 is unknown" 且绘制被丢弃（黑屏）。
 * 这里拦截这些调用，把每 4 个顶点展开为 2 个三角形（a,b,c + a,c,d）。
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "quads.h"
#include "proc.h"
#include "egl.h"
#include "debug.h"

#ifndef GL_QUADS
#define GL_QUADS 0x0007
#endif

static GLsizei quads_type_bytes(GLenum type) {
    switch (type) {
        case GL_UNSIGNED_BYTE:  return 1;
        case GL_UNSIGNED_SHORT: return 2;
        case GL_UNSIGNED_INT:   return 4;
        default: return 0;
    }
}

// 读取源索引到 uint32 数组。来源可能是 CPU 指针或当前绑定的 ELEMENT_ARRAY_BUFFER（indices 为偏移）。
// 返回 malloc 数组（调用方 free），失败返回 NULL。
static uint32_t* quads_read_indices(GLenum type, GLsizei count, const void* indices) {
    GLsizei tbytes = quads_type_bytes(type);
    if(tbytes == 0) return NULL;

    GLint eab = 0;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);

    const void* src = indices;
    void* mapped = NULL;
    GLint copy_ebo = 0;

    if(eab != 0) {
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
static void quads_draw_triangles(GLsizei quads, const uint32_t* indices) {
    context_t* ctx = current_context;
    if(!ctx || ctx->quads_scratch_buffer == 0) return;

    GLsizei tri_count = quads * 6;
    GLint eab = 0;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);

    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx->quads_scratch_buffer);
    es3_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)tri_count * 4, indices, GL_STREAM_DRAW);
    es3_functions.glDrawElements(GL_TRIANGLES, tri_count, GL_UNSIGNED_INT, NULL);
    {
        // 诊断：绘制后检查错误与关键状态，首次 16 次打印
        static unsigned int dn = 0;
        if((++dn & 0xF) == 0) {
            GLenum de = es3_functions.glGetError();
            GLint dbuf[4] = {0};
            es3_functions.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, dbuf);
            GLint vp[4] = {0};
            es3_functions.glGetIntegerv(GL_VIEWPORT, vp);
            GLint program = 0;
            es3_functions.glGetIntegerv(GL_CURRENT_PROGRAM, &program);
            printf("[LTW ERROR] QUADS draw: err=0x%x fb=%d vp=%d,%d,%dx%d prog=%d\n",
                   de, dbuf[0], vp[0], vp[1], vp[2], vp[3], program);
        }
    }

    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)eab);
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

// 诊断：统计 QUADS 转换调用次数，每 1024 次打印一次，确认路径被触发
static void quads_stat(const char* src) {
    static unsigned int n = 0;
    if((++n & 0x3FF) == 0) {
        printf("[LTW ERROR] QUADS converted via %s (total ~%u)\n", src, n);
    }
}

bool ltw_quads_draw_arrays(GLenum mode, GLint first, GLsizei count) {
    if(mode != GL_QUADS || count < 4 || (count & 3) != 0) return false;

    GLsizei quads = count >> 2;
    uint32_t* src = (uint32_t*)malloc((size_t)count * sizeof(uint32_t));
    uint32_t* dst = (uint32_t*)malloc((size_t)quads * 6 * sizeof(uint32_t));
    if(!src || !dst) {
        free(src);
        free(dst);
        return false;
    }
    for(GLsizei i = 0; i < count; i++) src[i] = (uint32_t)(first + i);

    quads_stat("glDrawArrays");
    quads_expand(src, count, dst);
    quads_draw_triangles(quads, dst);

    free(src);
    free(dst);
    return true;
}

bool ltw_quads_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if(mode != GL_QUADS || count < 4 || (count & 3) != 0) return false;

    GLsizei quads = count >> 2;
    uint32_t* src = quads_read_indices(type, count, indices);
    uint32_t* dst = (uint32_t*)malloc((size_t)quads * 6 * sizeof(uint32_t));
    if(!src || !dst) {
        free(src);
        free(dst);
        return false;
    }

    quads_stat("glDrawElements");
    quads_expand(src, count, dst);
    quads_draw_triangles(quads, dst);

    free(src);
    free(dst);
    return true;
}
