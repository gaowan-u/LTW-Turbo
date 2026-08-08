/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

/**
 * 文件功能：帧缓冲拷贝器与纹理上传兼容。
 *
 * 通过临时 FBO + glBlitFramebuffer 实现 glGetTexImage 与深度纹理读回；
 * 并在 glTexSubImage2D 中处理 GL_BGRA + UNSIGNED_INT_8_8_8_8_REV 的
 * CPU 字节交换（绕开驱动 swizzle 异常）。
 */
#include "proc.h"
#include "egl.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "swizzle.h"
#include "debug.h"
void buffer_copier_init(context_t* context) {
    framebuffer_copier_t* copier = &context->framebuffer_copier;
    while(es3_functions.glGetError() != 0) {}
    es3_functions.glGenTextures(1, &copier->temp_texture);
    es3_functions.glGenFramebuffers(1, &copier->tempfb);
    es3_functions.glGenFramebuffers(1, &copier->destfb);
    es3_functions.glBindTexture(GL_TEXTURE_2D, copier->temp_texture);
    es3_functions.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    es3_functions.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    es3_functions.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    es3_functions.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLenum error = es3_functions.glGetError();
    if(error != 0) {
        LTW_ERROR_PRINTF("LTW: error while initializing buffer-copier: %x", error);
        return;
    }
    copier->ready = true;
}

static void buffer_copier_store(GLint x, GLint y, GLsizei w, GLsizei h) {
    framebuffer_copier_t* copier = &current_context->framebuffer_copier;
    if(!copier->ready) return;
    GLint current_texbind;
    es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &current_texbind);
    es3_functions.glBindTexture(GL_TEXTURE_2D, copier->temp_texture);
    es3_functions.glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    es3_functions.glBindTexture(GL_TEXTURE_2D, current_texbind);
    es3_functions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, copier->tempfb);
    es3_functions.glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, copier->temp_texture, 0);
    es3_functions.glBlitFramebuffer(x, y, x+w, y+h, 0, 0, w, h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    es3_functions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_context->draw_framebuffer);
}

static void buffer_copier_release(GLenum target, GLint level, GLint x, GLint y, GLsizei w, GLsizei h) {
    framebuffer_copier_t* copier = &current_context->framebuffer_copier;
    if(!copier->ready) return;
    GLint current_texbind;
    GLenum target_query = get_textarget_query_param(target);
    if(target_query == GL_NONE) return;
    es3_functions.glGetIntegerv(target_query, &current_texbind);
    es3_functions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, copier->destfb);
    es3_functions.glBindFramebuffer(GL_READ_FRAMEBUFFER, copier->tempfb);
    es3_functions.glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, current_texbind, level);
    es3_functions.glBlitFramebuffer(0, 0, w, h, x, y, x+w, y+h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    es3_functions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_context->draw_framebuffer);
    es3_functions.glBindFramebuffer(GL_READ_FRAMEBUFFER, current_context->read_framebuffer);
}

void glGetTexImage( 	GLenum target,
                       GLint level,
                       GLenum format,
                       GLenum type,
                       void * pixels) {
    if(!current_context) return;
    if(!current_context->es31) goto unsupported_esver;
    // 白名单是“格式合法”且“类型合法”；之前的表达式用 && 串起 format 和 type
    // 判断，导致 (合法格式 + 非法类型) 之类组合漏判，混进 glReadPixels 报错。
    if(!(format == GL_RGBA || format == GL_RGBA_INTEGER) ||
       !(type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_INT || type == GL_INT || type == GL_FLOAT)) goto unsupported;
    framebuffer_copier_t* copier = &current_context->framebuffer_copier;
    GLint texture;
    es3_functions.glGetIntegerv(get_textarget_query_param(target), &texture);
    es3_functions.glBindFramebuffer(GL_READ_FRAMEBUFFER, copier->tempfb);
    es3_functions.glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, texture, level);
    GLint w, h;
    es3_functions.glGetTexLevelParameteriv(target, level, GL_TEXTURE_WIDTH, &w);
    es3_functions.glGetTexLevelParameteriv(target, level, GL_TEXTURE_HEIGHT, &h);
    if(!pixels) {
        LTW_ERROR_PRINTF("LTW: glGetTexImage called with NULL pixels");
        es3_functions.glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
        return;
    }
    es3_functions.glReadPixels(0, 0, w, h, format, type, pixels);
    es3_functions.glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
    return;
    unsupported_esver:
    LTW_ERROR_PRINTF("LTW: glGetTexImage only supported on OpenGL ES 3.1");
    return;
    unsupported:
    LTW_ERROR_PRINTF("LTW: unsupported parameters for glGetTexImage");
}

void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data) {
    if(!current_context) return;
    if(format == GL_DEPTH_COMPONENT) {
        framebuffer_copier_t* copier = &current_context->framebuffer_copier;
        copier->depthData = data;
        copier->depthWidth = width;
        copier->depthHeight = height;
        buffer_copier_store(x, y, width, height);
        return;
    }
    GLTRACE_CALL(glReadPixels, es3_functions.glReadPixels(x, y, width, height, format, type, data));
}

void glTexSubImage2D(GLenum target,
                     GLint level,
                     GLint xoffset,
                     GLint yoffset,
                     GLsizei width,
                     GLsizei height,
                     GLenum format,
                     GLenum type,
                     const void * data) {
    if(!current_context) return;
    // 检查是否为深度纹理，需要在 swizzle_process_upload 之前检查
    bool is_depth = (format == GL_DEPTH_COMPONENT);
    // MC 1.12 的字形/unicode 纹理用 GL_BGRA + GL_UNSIGNED_INT_8_8_8_8_REV
    // 上传。驱动侧 swizzle 补偿在这台设备上不可靠（查询/写入表现异常），
    // 这里直接在 CPU 上把每个像素的 B/R 字节交换成 RGBA，并把纹理 swizzle
    // 复位为默认值，彻底绕开驱动 swizzle。
    bool cpu_bgra_rev = (data != NULL && format == GL_BGRA_EXT && type == 0x8367);
    void* converted_data = NULL;
    if(cpu_bgra_rev) {
        size_t bytes = (size_t)width * (size_t)height * 4;
        converted_data = malloc(bytes);
        if(converted_data) {
            memcpy(converted_data, data, bytes);
            unsigned char* p = (unsigned char*)converted_data;
            for(size_t i = 0; i < bytes; i += 4) {
                unsigned char t = p[i];
                p[i] = p[i + 2];
                p[i + 2] = t;
            }
            format = GL_RGBA;
            type = GL_UNSIGNED_BYTE;
            swizzle_reset_texture(target);
        } else {
            cpu_bgra_rev = false;
        }
    }
    if(!cpu_bgra_rev) {
        swizzle_process_upload(target, &format, &type);
    }
    if(is_depth) {
        framebuffer_copier_t* copier = &current_context->framebuffer_copier;
        if(width == copier->depthWidth && height == copier->depthHeight && copier->depthData == data) {
            buffer_copier_release(target, level, xoffset, yoffset, width, height);
            free(converted_data);
            return;
        }
    }
    GLTRACE_CALL(glTexSubImage2D, es3_functions.glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type,
                                                                converted_data ? converted_data : data));
    free(converted_data);
}

void texture_blit_framebuffer(GLenum target,
                              GLint level,
                              GLint xoffset,
                              GLint yoffset,
                              GLint x,
                              GLint y,
                              GLsizei width,
                              GLsizei height,
                              bool depth) {
    framebuffer_copier_t* copier = &current_context->framebuffer_copier;
    if(!copier->ready) return;

    GLenum fb_attachment;
    GLbitfield fb_blit_bit;
    if(depth) {
        fb_attachment = GL_DEPTH_ATTACHMENT;
        fb_blit_bit = GL_DEPTH_BUFFER_BIT;
    }else {
        fb_attachment = GL_COLOR_ATTACHMENT0;
        fb_blit_bit = GL_COLOR_BUFFER_BIT;
    }

    GLint texture;
    es3_functions.glGetIntegerv(get_textarget_query_param(target), &texture);
    es3_functions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, copier->destfb);
    es3_functions.glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, fb_attachment, target, texture, level);
    es3_functions.glBlitFramebuffer(x, y, width+x, height+y, xoffset, yoffset, width+xoffset, height+yoffset, fb_blit_bit, GL_NEAREST);
    es3_functions.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, current_context->draw_framebuffer);
}

void glCopyTexSubImage2D(GLenum target,
                         GLint level,
                         GLint xoffset,
                         GLint yoffset,
                         GLint x,
                         GLint y,
                         GLsizei width,
                         GLsizei height) {
    if(current_context->es31) {
        GLint depthtype;
        es3_functions.glGetTexLevelParameteriv(target, level, GL_TEXTURE_DEPTH_TYPE, &depthtype);
        if(depthtype != GL_NONE) {
            texture_blit_framebuffer(target, level, xoffset, yoffset, x, y, width, height, true);
        }else {
            texture_blit_framebuffer(target, level, xoffset, yoffset, x, y, width, height, false);
        }
    } else {
        es3_functions.glGetError();
        es3_functions.glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
        // The QCOM driver is a pathological liar and emits wrong GL errors. Abuse this to decide when we actually need to at least try copying depth.
        if(es3_functions.glGetError() == GL_INVALID_OPERATION) {
            texture_blit_framebuffer(target, level, xoffset, yoffset, x, y, width, height, true);
        }
    }

}
