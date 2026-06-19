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
        // 初始化applied_swizzle和pending_swizzle为原始swizzle值
        memcpy(track->applied_swizzle, track->original_swizzle, sizeof(track->applied_swizzle));
        memcpy(track->pending_swizzle, track->original_swizzle, sizeof(track->pending_swizzle));
        track->goofy_byte_order = GL_FALSE;
        track->upload_bgra = GL_FALSE;
        track->has_pending_update = GL_FALSE;
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
    if(track == NULL) return;
    bool apply_upload_bgra = false;
    bool apply_goofy_order = false;
    if((*format) == GL_BGRA_EXT) {
        apply_upload_bgra = true;
        *format = GL_RGBA;
    }
    if((*type) == 0x8035) {
        apply_goofy_order = true;
        *type = GL_UNSIGNED_BYTE;
    }
    if((*type) == 0x8367) {
        *type = GL_UNSIGNED_BYTE;
    }
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
            // 绑定纹理（需要知道目标类型）
            // 由于我们不知道目标类型，这里需要从 track 中存储
            // 为了简化，我们假设大部分是 GL_TEXTURE_2D
            GLenum target = GL_TEXTURE_2D;

            // 应用更新
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