/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#include "proc.h"
#include "egl.h"
#include "mempool.h"
#include "debug.h"
#include <string.h>
#include "libraryinternal.h"
//#include <GL/glext.h>

#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46

static void swizzle_process_bgra(GLenum* swizzle) {
    GLenum red_src = swizzle[0];
    GLenum blue_src = swizzle[2];
    swizzle[0] = blue_src;
    swizzle[2] = red_src;
}

static void swizzle_process_endianness(GLenum* swizzle) {
    GLenum orig_swizzle[4];
    memcpy(orig_swizzle, swizzle, 4 * sizeof(GLenum));
    swizzle[0] = orig_swizzle[3];
    swizzle[1] = orig_swizzle[2];
    swizzle[2] = orig_swizzle[1];
    swizzle[3] = orig_swizzle[0];
}

static texture_swizzle_track_t* get_swizzle_track(GLenum target) {
    GLint texture;
    GLenum getter = get_textarget_query_param(target);
    if(getter == 0) return NULL;
    current_context->fast_gl.glGetIntegerv(getter, &texture);
    if(texture == 0) return NULL;
    texture_swizzle_track_t* track = unordered_map_get(current_context->texture_swztrack_map, (void*)texture);
    if(track == NULL) {
        track = mempool_alloc(current_context->swizzle_track_pool);
        if(!track) {
            LTW_ERROR_PRINTF("LTW: Failed to allocate swizzle track for texture %d", texture);
            return NULL;
        }
        current_context->fast_gl.glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_R, (GLint*)&track->original_swizzle[0]);
        current_context->fast_gl.glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_G, (GLint*)&track->original_swizzle[1]);
        current_context->fast_gl.glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_B, (GLint*)&track->original_swizzle[2]);
        current_context->fast_gl.glGetTexParameteriv(target, GL_TEXTURE_SWIZZLE_A, (GLint*)&track->original_swizzle[3]);
        // 部分驱动（Turnip/Zink 等）在纹理尚未上传数据时对 swizzle 查询
        // 返回全 0，而不是默认的 R/G/B/A。全 0 会让后续 BGRA 补偿把
        // 字形纹理的采样 swizzle 设成 ZERO，文字变成全透明/黑块。
        if(track->original_swizzle[0] == 0 && track->original_swizzle[1] == 0 &&
           track->original_swizzle[2] == 0 && track->original_swizzle[3] == 0) {
            track->original_swizzle[0] = GL_RED;
            track->original_swizzle[1] = GL_GREEN;
            track->original_swizzle[2] = GL_BLUE;
            track->original_swizzle[3] = GL_ALPHA;
        }
        // 初始化applied_swizzle和pending_swizzle为原始swizzle值
        memcpy(track->applied_swizzle, track->original_swizzle, sizeof(track->applied_swizzle));
        memcpy(track->pending_swizzle, track->original_swizzle, sizeof(track->pending_swizzle));
        track->goofy_byte_order = GL_FALSE;
        track->upload_bgra = GL_FALSE;
        track->has_pending_update = GL_FALSE;
        track->texture_target = target;
        unordered_map_put(current_context->texture_swztrack_map, (void*)texture, track);
    }
    return track;
}

static void apply_swizzles(GLenum target, texture_swizzle_track_t* track) {
    GLenum new_swizzle[4];
    memcpy(new_swizzle, track->original_swizzle, 4 * sizeof(GLenum));
    if(track->goofy_byte_order) swizzle_process_endianness(new_swizzle);
    if(track->upload_bgra) swizzle_process_bgra(new_swizzle);

    // 检查是否需要更新
    if(memcmp(new_swizzle, track->applied_swizzle, 4 * sizeof(GLenum)) == 0) {
        return;  // 已经是这个状态，跳过
    }

    {
        // 诊断：确认实际写入驱动的 swizzle 值（字形纹理 tex>=24）
        GLenum getter = get_textarget_query_param(target);
        if(getter != 0) {
            GLint tex = 0;
            current_context->fast_gl.glGetIntegerv(getter, &tex);
            if(tex >= 24 && tex <= 64) {
                printf("[LTW DIAG] apply swizzle tex=%d -> 0x%x,0x%x,0x%x,0x%x\n",
                       tex, (unsigned)new_swizzle[0], (unsigned)new_swizzle[1],
                       (unsigned)new_swizzle[2], (unsigned)new_swizzle[3]);
                fflush(stdout);
            }
        }
    }

    // 批量更新模式：只标记为待更新，不立即应用
    if(current_context->swizzle_batch_mode) {
        memcpy(track->pending_swizzle, new_swizzle, 4 * sizeof(GLenum));
        track->has_pending_update = GL_TRUE;

        // 获取纹理ID并添加到待更新列表
        GLint texture;
        GLenum getter = get_textarget_query_param(target);
        if(getter != 0) {
            current_context->fast_gl.glGetIntegerv(getter, &texture);
            if(texture != 0 && current_context->pending_swizzle_count < 64) {
                // 检查是否已经在列表中
                bool already_pending = false;
                for(int i = 0; i < current_context->pending_swizzle_count; i++) {
                    if(current_context->pending_swizzle_textures[i] == (GLuint)texture) {
                        already_pending = true;
                        break;
                    }
                }
                if(!already_pending) {
                    current_context->pending_swizzle_textures[current_context->pending_swizzle_count++] = (GLuint)texture;
                }
            }
        }
        return;
    }

    // 立即更新模式：直接应用
    memcpy(track->applied_swizzle, new_swizzle, 4 * sizeof(GLenum));
    current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, new_swizzle[0]);
    current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, new_swizzle[1]);
    current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, new_swizzle[2]);
    current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, new_swizzle[3]);
}

INTERNAL void swizzle_process_upload(GLenum target, GLenum* format, GLenum* type) {
    texture_swizzle_track_t* track = get_swizzle_track(target);
    bool apply_upload_bgra = false;
    bool apply_goofy_order = false;
    if((*format) == GL_BGRA_EXT) {
        apply_upload_bgra = true;
        *format = GL_RGBA;
    }
    // 桌面遗留的单通道/双通道格式：GLES 3 只接受对应的 RED/RG 上传格式。
    // GL_ALPHA 是 MC 1.12 及更早版本字形纹理的另一种常见路径（glformats.c
    // 已把 internalformat 映射为 GL_R8），这里必须同步转换 SubImage。
    if((*format) == GL_ALPHA) {
        *format = GL_RED;
    } else if((*format) == GL_LUMINANCE) {
        *format = GL_RED;
    } else if((*format) == GL_LUMINANCE_ALPHA) {
        *format = GL_RG;
    }
    if((*type) == 0x8035) {
        apply_goofy_order = true;
        *type = GL_UNSIGNED_BYTE;
    }
    if((*type) == 0x8367) {
        *type = GL_UNSIGNED_BYTE;
    }
    // 拿不到跟踪记录时也必须完成上面的格式/类型转换，否则 GLES 驱动会收到
    // GL_BGRA/GL_ALPHA 等桌面格式导致上传失败，纹理保持白色/未初始化。
    if(track == NULL) return;
    if(apply_goofy_order != track->goofy_byte_order || apply_upload_bgra != track->upload_bgra) {
        track->goofy_byte_order = apply_goofy_order;
        track->upload_bgra = apply_upload_bgra;
        apply_swizzles(target, track);
    }
}

INTERNAL void swizzle_process_swizzle_param(GLenum target, GLenum swizzle_param, const GLenum* swizzle) {
    switch (swizzle_param) {
        case GL_TEXTURE_SWIZZLE_R:
        case GL_TEXTURE_SWIZZLE_G:
        case GL_TEXTURE_SWIZZLE_B:
        case GL_TEXTURE_SWIZZLE_A:
        case GL_TEXTURE_SWIZZLE_RGBA:
            break;
        default:
            return;
    }
    texture_swizzle_track_t* track = get_swizzle_track(target);
    if(track == NULL) return;
    switch(swizzle_param) {
        case GL_TEXTURE_SWIZZLE_R:
        case GL_TEXTURE_SWIZZLE_G:
        case GL_TEXTURE_SWIZZLE_B:
        case GL_TEXTURE_SWIZZLE_A:
            track->original_swizzle[swizzle_param - GL_TEXTURE_SWIZZLE_R] = *swizzle;
            apply_swizzles(target, track);
            break;
        case GL_TEXTURE_SWIZZLE_RGBA:
            memcpy(track->original_swizzle, swizzle, 4 * sizeof(GLenum));
            apply_swizzles(target, track);
            break;
    }
}

// 开始批量更新模式
INTERNAL void swizzle_begin_batch_update(void) {
    if(!current_context) return;
    current_context->swizzle_batch_mode = true;
    current_context->pending_swizzle_count = 0;
}

// 结束批量更新模式并应用所有待处理的更新
INTERNAL void swizzle_end_batch_update(void) {
    if(!current_context || !current_context->swizzle_batch_mode) return;

    // 应用所有待处理的更新
    for(int i = 0; i < current_context->pending_swizzle_count; i++) {
        GLuint texture = current_context->pending_swizzle_textures[i];
        texture_swizzle_track_t* track = unordered_map_get(current_context->texture_swztrack_map, (void*)texture);
        if(track && track->has_pending_update) {
            GLenum target = track->texture_target;

            memcpy(track->applied_swizzle, track->pending_swizzle, 4 * sizeof(GLenum));
            current_context->fast_gl.glBindTexture(target, texture);
            current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, track->pending_swizzle[0]);
            current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, track->pending_swizzle[1]);
            current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, track->pending_swizzle[2]);
            current_context->fast_gl.glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, track->pending_swizzle[3]);
            track->has_pending_update = GL_FALSE;
        }
    }

    // 退出批量更新模式
    current_context->swizzle_batch_mode = false;
    current_context->pending_swizzle_count = 0;
}
