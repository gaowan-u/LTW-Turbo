/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */
#include "proc.h"
#include "egl.h"
#include <stdbool.h>
#include <stdlib.h>
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
    if(format != GL_RGBA && format != GL_RGBA_INTEGER && type != GL_UNSIGNED_BYTE && type != GL_UNSIGNED_INT && type != GL_INT && type != GL_FLOAT) goto unsupported;
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
    // 诊断放最前：即使 current_context 为 NULL（Pre startup 等）也能看到调用
    {
        // 诊断：纹理数据填充格式（前 64 次 + 每 128 次），定位字体纹理上传
        static unsigned int sub_n = 0;
        sub_n++;
        if(sub_n <= 64 || (sub_n & 0x7F) == 1) {
            GLint tex = 0;
            if(current_context) {
                es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
            }
            printf("[LTW DIAG] glTexSubImage2D #%u tex=%d %dx%d+%d+%d fmt=0x%x type=0x%x data=%s ctx=%p\n",
                   sub_n, tex, width, height, xoffset, yoffset, format, type,
                   data ? "yes" : "null", (void*)current_context);
            fflush(stdout);
        }
    }
    if(!current_context) return;
    // 检查是否为深度纹理，需要在 swizzle_process_upload 之前检查
    bool is_depth = (format == GL_DEPTH_COMPONENT);
    swizzle_process_upload(target, &format, &type);
    if(is_depth) {
        framebuffer_copier_t* copier = &current_context->framebuffer_copier;
        if(width == copier->depthWidth && height == copier->depthHeight && copier->depthData == data) {
            buffer_copier_release(target, level, xoffset, yoffset, width, height);
            return;
        }
    }
    {
        // 字形/unicode 页纹理上传的精确诊断：tex>=24 时无论计数采样都打印
        GLint tex = 0;
        es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
        if(tex >= 24 && tex <= 64) {
            printf("[LTW DIAG] glTexSubImage2D tex=%d %dx%d+%d+%d fmt=0x%x type=0x%x (converted)\n",
                   tex, width, height, xoffset, yoffset, (unsigned)format, (unsigned)type);
            fflush(stdout);
        }
    }
    GLTRACE_CALL(glTexSubImage2D, es3_functions.glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, data));
    {
        GLint tex = 0;
        es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
        if(tex >= 24 && tex <= 64) {
            GLenum e = es3_functions.glGetError();
            printf("[LTW DIAG] glTexSubImage2D tex=%d err=0x%x\n", tex, (unsigned)e);
            fflush(stdout);
        }
        if(tex == 26) {
            GLint sw[4] = {0};
            GLint sw_rgba[4] = {0};
            es3_functions.glGetTexParameteriv(GL_TEXTURE_2D, 0x8B42 /* R */, &sw[0]);
            es3_functions.glGetTexParameteriv(GL_TEXTURE_2D, 0x8B43 /* G */, &sw[1]);
            es3_functions.glGetTexParameteriv(GL_TEXTURE_2D, 0x8B44 /* B */, &sw[2]);
            es3_functions.glGetTexParameteriv(GL_TEXTURE_2D, 0x8B45 /* A */, &sw[3]);
            es3_functions.glGetTexParameteriv(GL_TEXTURE_2D, 0x8E46 /* RGBA */, sw_rgba);
            printf("[LTW DIAG] tex26 swizzle_rgba=0x%x,0x%x,0x%x,0x%x rgba_query=0x%x,0x%x,0x%x,0x%x\n",
                   (unsigned)sw[0], (unsigned)sw[1], (unsigned)sw[2], (unsigned)sw[3],
                   (unsigned)sw_rgba[0], (unsigned)sw_rgba[1], (unsigned)sw_rgba[2], (unsigned)sw_rgba[3]);
            // 读回字形纹理的原始像素，确认 alpha 是否真的进了纹理
            GLint old_rb = 0;
            es3_functions.glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &old_rb);
            unsigned char* buf = (unsigned char*)malloc((size_t)width * height * 4);
            if(buf) {
                glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
                const int pts[][2] = {{0,0},{1,0},{64,64},{128,128},{200,200},{255,255}};
                printf("[LTW DIAG] tex26 pixels:");
                for(unsigned int i = 0; i < sizeof(pts)/sizeof(pts[0]); i++) {
                    int px = pts[i][0], py = pts[i][1];
                    unsigned char* p = buf + ((size_t)py * width + px) * 4;
                    printf(" (%d,%d)=%u,%u,%u,%u", px, py, p[0], p[1], p[2], p[3]);
                }
                printf("\n");
                fflush(stdout);
                free(buf);
            }
            es3_functions.glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint)old_rb);
        }
    }
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
