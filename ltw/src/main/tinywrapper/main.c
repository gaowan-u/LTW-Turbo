/**
 * Created by: artDev, DuyKhanhTran
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */
#include <stdio.h>
#include <dlfcn.h>

#include <stdbool.h>
#include "GL/gl.h"
#include <GLES3/gl3.h>
#include "string_utils.h"
#include <stdlib.h>
#include <string.h>
#include "proc.h"
#include "egl.h"
#include "glformats.h"
#include "main.h"
#include "swizzle.h"
#include "libraryinternal.h"
#include "env.h"
#include "mempool.h"
#include "debug.h"
#include "quads.h"
#include "fixed_pipeline.h"

//GL清空深度缓存使用glClearDepth这个GL的api
void glClearDepth(GLdouble depth) {
    if(!current_context) return;    //判断是否为context_t结构体中成员，否则直接返回
    es3_functions.glClearDepthf((GLfloat) depth);   //对应ltw\src\main\tinywrapper\es3_functions.h中的GLESFUNC(glClearDepthf,PFNGLCLEARDEPTHFPROC)
}

//GL映射缓冲区
void *glMapBuffer(GLenum target, GLenum access) {
    if(!current_context) return NULL;

    GLenum access_range = GL_MAP_READ_BIT;
    GLint length = 0;

    switch (target) {
        case GL_ATOMIC_COUNTER_BUFFER:
        case GL_DISPATCH_INDIRECT_BUFFER:
        case GL_SHADER_STORAGE_BUFFER:
        case GL_QUERY_BUFFER:
            LTW_ERROR_PRINTF("glMapBuffer unsupported target=0x%x", target);
            return NULL;
        case GL_DRAW_INDIRECT_BUFFER:
        case GL_TEXTURE_BUFFER:
            LTW_ERROR_PRINTF("glMapBuffer unimplemented target=0x%x", target);
            return NULL;
    }

    switch (access) {
        case GL_READ_ONLY:
            access_range = GL_MAP_READ_BIT;
            break;
        case GL_WRITE_ONLY:
            access_range = GL_MAP_WRITE_BIT;
            break;
        case GL_READ_WRITE:
            access_range = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
            break;
        default:
            LTW_ERROR_PRINTF("glMapBuffer unknown access=0x%x", access);
            return NULL;
    }

    es3_functions.glGetBufferParameteriv(target, GL_BUFFER_SIZE, &length);
    return es3_functions.glMapBufferRange(target, 0, length, access_range);
}

//判断是否为代理纹理
INTERNAL int isProxyTexture(GLenum target) {
    switch (target) {
        case GL_PROXY_TEXTURE_1D:
        case GL_PROXY_TEXTURE_2D:
        case GL_PROXY_TEXTURE_3D:
        case GL_PROXY_TEXTURE_RECTANGLE_ARB:    //矩形代理纹理（ARB扩展）
            return 1;   //返回真
    }
    return 0;   //返回假
}

//查询纹理在glext.h中的映射
INTERNAL GLenum get_textarget_query_param(GLenum target) {
    switch (target) {
        case GL_TEXTURE_2D:
            return GL_TEXTURE_BINDING_2D;
        case GL_TEXTURE_2D_MULTISAMPLE:
            return GL_TEXTURE_BINDING_2D_MULTISAMPLE;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
            return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
        case GL_TEXTURE_3D:
            return GL_TEXTURE_BINDING_3D;
        case GL_TEXTURE_2D_ARRAY:
            return GL_TEXTURE_BINDING_2D_ARRAY;
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
        case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
        case GL_TEXTURE_CUBE_MAP:
            return GL_TEXTURE_BINDING_CUBE_MAP;
        case GL_TEXTURE_CUBE_MAP_ARRAY:
            return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY;
        case GL_TEXTURE_BUFFER:
            return GL_TEXTURE_BUFFER_BINDING;
        default:
            return 0;
    }
}

//此处为一个内联函数，计算纹理级别的尺寸，可以插入到调用它的地方
static int inline nlevel(int size, int level) {
    if(size) {
        size>>=level;   //右移赋值运算符，将size右移level位后赋值给size
        if(!size) size=1;
    }
    return size;
}

static bool trigger_texlevelparameter = false;  //纹理级别参数触发器，默认关闭

//纹理级别参数验证
static bool check_texlevelparameter() {
    if(current_context->es31) return true;  //如果支持OpenGL ES 3.1则返回真
    if(trigger_texlevelparameter) return false; //如果触发器为真则返回假
    LTW_ERROR_PRINTF("glGetTexLevelParameter* functions are not supported below OpenGL ES 3.1");
    trigger_texlevelparameter = true;   //开启触发器
    return false;   //返回假
}

static void proxy_getlevelparameter(GLenum target, GLint level, GLenum pname, GLint *params) {
    switch (pname) {
        case GL_TEXTURE_WIDTH:
            (*params) = nlevel(current_context->proxy_width, level);
            break;
        case GL_TEXTURE_HEIGHT:
            (*params) = nlevel(current_context->proxy_height, level);
            break;
        case GL_TEXTURE_INTERNAL_FORMAT:
            (*params) = current_context->proxy_intformat;
            break;
    }
}

// GLES 3.x 中 glGetTexLevelParameter 可查询的 pname（纹理状态 pname 如
// GL_TEXTURE_WRAP_S/T 不是 level 参数，透传会让驱动报 GL_INVALID_ENUM）。
static bool is_gles_texlevelparam_pname(GLenum pname) {
    switch (pname) {
        case GL_TEXTURE_WIDTH:
        case GL_TEXTURE_HEIGHT:
        case GL_TEXTURE_DEPTH:
        case GL_TEXTURE_INTERNAL_FORMAT:
        case GL_TEXTURE_RED_SIZE:
        case GL_TEXTURE_GREEN_SIZE:
        case GL_TEXTURE_BLUE_SIZE:
        case GL_TEXTURE_ALPHA_SIZE:
        case GL_TEXTURE_DEPTH_SIZE:
        case GL_TEXTURE_STENCIL_SIZE:
        case GL_TEXTURE_COMPRESSED:
        case 0x86A0: /* GL_TEXTURE_COMPRESSED_IMAGE_SIZE */
        case 0x8C3F: /* GL_TEXTURE_SHARED_SIZE */
        case 0x8C10: /* GL_TEXTURE_RED_TYPE */   case 0x8C11: /* GREEN */
        case 0x8C12: /* GL_TEXTURE_BLUE_TYPE */  case 0x8C13: /* ALPHA */
        case 0x8C14: /* GL_TEXTURE_DEPTH_TYPE */
        case 0x9106: /* GL_TEXTURE_SAMPLES */
        case 0x9107: /* GL_TEXTURE_FIXED_SAMPLE_LOCATIONS */
        case 0x919D: /* GL_TEXTURE_BUFFER_SIZE (GLES 3.2) */
        case 0x919E: /* GL_TEXTURE_BUFFER_OFFSET */
            return true;
        default:
            return false;
    }
}

void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params) {
    if(!current_context) return;
    if(isProxyTexture(target)) {
        GLint param = 0;
        proxy_getlevelparameter(target, level, pname, &param);
        *params = (GLfloat) param;
        return;
    }
    if(!is_gles_texlevelparam_pname(pname)) {
        *params = 0.0f;
        return;
    }
    if(!check_texlevelparameter()) return;
    es3_functions.glGetTexLevelParameterfv(target, level, pname, params);
}

void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params) {
    if(!current_context) return;
    if (isProxyTexture(target)) {
        proxy_getlevelparameter(target, level, pname, params);
        return;
    }
    if(!is_gles_texlevelparam_pname(pname)) {
        *params = 0;
        return;
    }
    if(!check_texlevelparameter()) return;
    es3_functions.glGetTexLevelParameteriv(target, level, pname, params);

}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data) {
    if(!current_context) return;
    if (isProxyTexture(target)) {
        current_context->proxy_width = ((width<<level)>current_context->maxTextureSize)?0:width;
        current_context->proxy_height = ((height<<level)>current_context->maxTextureSize)?0:height;
        current_context->proxy_intformat = internalformat;
    } else {
        if(data != NULL) swizzle_process_upload(target, &format, &type);
        pick_internalformat(&internalformat, &type, &format, &data);
        {
            // 低频诊断：纹理上传格式（前 8 次 + 每 256 次）
            static unsigned int ti_n = 0;
            ti_n++;
            if(ti_n <= 8 || (ti_n & 0xFF) == 1) {
                GLint tex = 0;
                es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
                printf("[LTW DIAG] glTexImage2D #%u tex=%d %dx%d level=%d intfmt=0x%x fmt=0x%x type=0x%x data=%s\n",
                       ti_n, tex, width, height, level, internalformat, format, type,
                       data ? "yes" : "null");
                fflush(stdout);
            }
        }
        GLTRACE_CALL(glTexImage2D, es3_functions.glTexImage2D(target, level, internalformat, width, height, border, format, type, data));
    }
}

INTERNAL bool filter_params_integer(GLenum target, GLenum pname, GLint param) {
    return true;
}
// GLES 没有 GL_CLAMP(0x2900)（桌面 GL 1.x 遗留），老游戏（MC <=1.16）常给
// WRAP_S/T/R 设 GL_CLAMP，GLES 驱动会报 GL_INVALID_ENUM 且状态不生效。
// 统一映射为 GL_CLAMP_TO_EDGE，语义最接近，也消除了 0x500 错误的来源。
INTERNAL GLint sanitize_texparam_value(GLenum pname, GLint param) {
    if(param == GL_CLAMP &&
       (pname == GL_TEXTURE_WRAP_S || pname == GL_TEXTURE_WRAP_T || pname == GL_TEXTURE_WRAP_R)) {
        return GL_CLAMP_TO_EDGE;
    }
    return param;
}
INTERNAL bool filter_params_float(GLenum target, GLenum pname, GLfloat param) {
    if(pname == GL_TEXTURE_LOD_BIAS) {
        if(param != 0.0f) {
            static bool lodbias_trigger = false;
            if(!lodbias_trigger) {
                LTW_ERROR_PRINTF("LTW: setting GL_TEXTURE_LOD_BIAS to nondefault value not supported");
            }
        }
        return false;
    }
    return true;
}
void glTexParameterf( 	GLenum target,
                         GLenum pname,
                         GLfloat param) {
    if(!current_context) return;
    if(!filter_params_integer(target, pname, (GLint) param)) return;
    if(!filter_params_float(target, pname, param)) return;
    param = (GLfloat)sanitize_texparam_value(pname, (GLint)param);
    es3_functions.glTexParameterf(target, pname, param);
}
void glTexParameteri( 	GLenum target,
                         GLenum pname,
                         GLint param) {
    if(!current_context) return;
    if(!filter_params_integer(target, pname, param)) return;
    if(!filter_params_float(target, pname, (GLfloat)param)) return;
    param = sanitize_texparam_value(pname, param);
    swizzle_process_swizzle_param(target, pname, &param);
    GLTRACE_CALL(glTexParameteri, es3_functions.glTexParameteri(target, pname, param));
}

void glTexParameterfv( 	GLenum target,
                          GLenum pname,
                          const GLfloat * params) {
    if(!current_context) return;
    if(!filter_params_integer(target, pname, (GLint)*params)) return;
    if(!filter_params_float(target, pname, *params)) return;
    GLfloat sanitized[4];
    memcpy(sanitized, params, 4 * sizeof(GLfloat));
    if(pname == GL_TEXTURE_WRAP_S || pname == GL_TEXTURE_WRAP_T || pname == GL_TEXTURE_WRAP_R) {
        for(int i = 0; i < 4; i++) sanitized[i] = (GLfloat)sanitize_texparam_value(pname, (GLint)sanitized[i]);
    }
    es3_functions.glTexParameterfv(target, pname, sanitized);
}
void glTexParameteriv( 	GLenum target,
                          GLenum pname,
                          const GLint * params) {
    if(!current_context) return;
    if(!filter_params_integer(target, pname, *params)) return;
    if(!filter_params_float(target, pname, (GLfloat)*params)) return;
    GLint sanitized[4];
    memcpy(sanitized, params, 4 * sizeof(GLint));
    if(pname == GL_TEXTURE_WRAP_S || pname == GL_TEXTURE_WRAP_T || pname == GL_TEXTURE_WRAP_R) {
        for(int i = 0; i < 4; i++) sanitized[i] = sanitize_texparam_value(pname, sanitized[i]);
    }
    swizzle_process_swizzle_param(target, pname, (const GLenum*)sanitized);
    es3_functions.glTexParameteriv(target, pname, sanitized);
}
static bool trigger_gltexparameteri = false;
void glTexParameterIiv( 	GLenum target,
                           GLenum pname,
                           const GLint * params) {
    if(!current_context) return;
    if(pname != GL_TEXTURE_SWIZZLE_RGBA) {
        if(!trigger_gltexparameteri) {
            LTW_ERROR_PRINTF("LTW: glTexParameterIiv for parameters other than GL_TEXTURE_SWIZZLE_RGBA is not supported");
            trigger_gltexparameteri = true;
        }
        return;
    }
    swizzle_process_swizzle_param(target, pname, params);
}

void glTexParameterIuiv( 	GLenum target,
                            GLenum pname,
                            const GLuint * params) {
    if(!current_context) return;
    if(pname != GL_TEXTURE_SWIZZLE_RGBA) {
        if(!trigger_gltexparameteri) {
            LTW_ERROR_PRINTF("LTW: glTexParameterIuiv for parameters other than GL_TEXTURE_SWIZZLE_RGBA is not supported");
            trigger_gltexparameteri = true;
        }
        return;
    }
    swizzle_process_swizzle_param(target, pname, params);
}

void glRenderbufferStorage(	GLenum target,
                               GLenum internalformat,
                               GLsizei width,
                               GLsizei height) {
    if(!current_context) return;
    if(internalformat == GL_DEPTH_COMPONENT) internalformat = GL_DEPTH_COMPONENT16;
    es3_functions.glRenderbufferStorage(target, internalformat, width, height);
}

static bool never_flush_buffers;
static bool coherent_dynamic_storage;

void glBufferStorage(GLenum target,
                     GLsizeiptr size,
                     const void * data,
                     GLbitfield flags) {
    if(!current_context || !current_context->buffer_storage) return;
    // Enable coherence to make sure the buffers are synced without flushing.
    if(never_flush_buffers && ((flags & GL_MAP_PERSISTENT_BIT) != 0)) {
        flags |= GL_MAP_COHERENT_BIT;
    }
    // Force dynamic storage buffers to be coherent (for working around driver bugs)
    if(coherent_dynamic_storage && (flags & GL_DYNAMIC_STORAGE_BIT) != 0) {
        flags |= (GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    }
    es3_functions.glBufferStorageEXT(target, size, data, flags);
}

void *glMapBufferRange( 	GLenum target,
                           GLintptr offset,
                           GLsizeiptr length,
                           GLbitfield access) {
    if(never_flush_buffers) access &= ~GL_MAP_FLUSH_EXPLICIT_BIT;
    return es3_functions.glMapBufferRange(target, offset, length, access);
}

void glFlushMappedBufferRange( 	GLenum target,
                                  GLintptr offset,
                                  GLsizeiptr length) {
    if(!never_flush_buffers) es3_functions.glFlushMappedBufferRange(target, offset, length);
}

const GLubyte* glGetStringi(GLenum name, GLuint index) {
    if(!current_context || name != GL_EXTENSIONS) return NULL;
    static int dbg_count = 0;
    if(dbg_count < 8) {
        const GLubyte* s;
        if(index < current_context->nextras && current_context->extra_extensions_array != NULL)
            s = (const GLubyte*)current_context->extra_extensions_array[index];
        else
            s = es3_functions.glGetStringi(name, index - current_context->nextras);
        printf("[LTW DBG] glGetStringi[%u] = %s\n", index, s ? (const char*)s : "(null)");
        dbg_count++;
        return s;
    }
    if(index < current_context->nextras && current_context->extra_extensions_array != NULL) {
        return (const GLubyte*)current_context->extra_extensions_array[index];
    } else {
        return es3_functions.glGetStringi(name, index - current_context->nextras);
    }
}

const GLubyte* glGetString(GLenum name) {
    if(!current_context) return NULL;
    switch(name) {
        case GL_VERSION:
            const GLubyte* realVersion = es3_functions.glGetString(GL_VERSION);
            if (!realVersion) return NULL;
            static char buf[128];
            snprintf(buf, sizeof(buf), "%s LTW-Turbo", realVersion);
            return (const GLubyte*)buf;
        case GL_SHADING_LANGUAGE_VERSION:
            const GLubyte* realShadingVer = es3_functions.glGetString(GL_SHADING_LANGUAGE_VERSION);
            if (!realShadingVer) return NULL;
            static char buf2[128];
            snprintf(buf2, sizeof(buf2), "%s LTW-Turbo", realShadingVer);
            return (const GLubyte*)buf2;
        case GL_VENDOR:
            return (const GLubyte*)"MathCode";
        case GL_EXTENSIONS:
            if(current_context->extensions_string != NULL) return (const GLubyte*)current_context->extensions_string;
            return (const GLubyte*)es3_functions.glGetString(GL_EXTENSIONS);
        default:
            return es3_functions.glGetString(name);
    }
}

bool debug = false;

// Fixed-function pipeline state that GLES 3.x does not support. LWJGL2-era
// apps (Minecraft <=1.16) toggle these caps every frame, e.g.
// glEnable(GL_ALPHA_TEST). Forwarding them makes the driver raise
// GL_INVALID_ENUM; on ES these states are no-ops (fully programmable
// pipeline), so swallow them. GL_CLIP_PLANE* is intentionally absent:
// it shares values with the valid GL_CLIP_DISTANCE* caps.
static bool is_fixed_function_cap(GLenum cap) {
    switch (cap) {
        case GL_ALPHA_TEST:
        case GL_LIGHTING:
        case GL_LIGHT0: case GL_LIGHT1: case GL_LIGHT2: case GL_LIGHT3:
        case GL_LIGHT4: case GL_LIGHT5: case GL_LIGHT6: case GL_LIGHT7:
        case GL_FOG:
        case GL_COLOR_MATERIAL:
        case GL_NORMALIZE:
        case GL_RESCALE_NORMAL:
        case GL_TEXTURE_1D: case GL_TEXTURE_2D: case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
        case GL_TEXTURE_GEN_S: case GL_TEXTURE_GEN_T:
        case GL_TEXTURE_GEN_R: case GL_TEXTURE_GEN_Q:
        case GL_POINT_SMOOTH: case GL_LINE_SMOOTH: case GL_POLYGON_SMOOTH:
        case GL_POLYGON_OFFSET_POINT: case GL_POLYGON_OFFSET_LINE:
        case GL_VERTEX_ARRAY: case GL_NORMAL_ARRAY: case GL_COLOR_ARRAY:
        case GL_TEXTURE_COORD_ARRAY: case GL_EDGE_FLAG_ARRAY:
        case GL_FOG_COORD_ARRAY: case GL_SECONDARY_COLOR_ARRAY:
        case GL_INDEX_ARRAY:
            return true;
        default:
            return false;
    }
}

void glEnable(GLenum cap) {
    if(!current_context) return;
    if(cap == GL_DEBUG_OUTPUT && !debug) return;
    if(is_fixed_function_cap(cap)) {
        if(cap == GL_TEXTURE_2D) fp_set_texture_enabled(true);
        if(cap == GL_ALPHA_TEST) fp_set_alpha_test(true);
        return;
    }
    if(cap == GL_BLEND) {
        fp_set_blend(true);
        static bool blend_diag = false;
        if(!blend_diag) {
            blend_diag = true;
            printf("[LTW DIAG] glEnable(GL_BLEND)\n");
            fflush(stdout);
        }
    }
    es3_functions.glEnable(cap);
}

void glDisable(GLenum cap) {
    if(!current_context) return;
    if(is_fixed_function_cap(cap)) {
        if(cap == GL_TEXTURE_2D) fp_set_texture_enabled(false);
        if(cap == GL_ALPHA_TEST) fp_set_alpha_test(false);
        return;
    }
    if(cap == GL_BLEND) {
        static bool blend_diag = false;
        if(!blend_diag) {
            blend_diag = true;
            printf("[LTW DIAG] glDisable(GL_BLEND)\n");
            fflush(stdout);
        }
    }
    es3_functions.glDisable(cap);
}

// Pass-through wrappers with double-drain error tracing (LTW_DEBUG trace
// hunt for the recurring 0x500 INVALID_ENUM).
void glBindTexture(GLenum target, GLuint texture) {
    if(!current_context) return;
    {
        // 低频诊断：确认纹理绑定（每 512 次打印一次）
        static unsigned int bt_n = 0;
        if((++bt_n & 0x1FF) == 1) {
            GLint act = 0;
            es3_functions.glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
            printf("[LTW DIAG] glBindTexture #%u target=0x%x tex=%u unit=0x%x\n", bt_n, target, texture, act);
            fflush(stdout);
        }
    }
    GLTRACE_CALL(glBindTexture, es3_functions.glBindTexture(target, texture));
    if(target == GL_TEXTURE_2D) fp_set_active_texture(0);
}
void glActiveTexture(GLenum texture) {
    if(!current_context) return;
    GLTRACE_CALL(glActiveTexture, es3_functions.glActiveTexture(texture));
}
void glPixelStorei(GLenum pname, GLint param) {
    if(!current_context) return;
    GLTRACE_CALL(glPixelStorei, es3_functions.glPixelStorei(pname, param));
}
void glGenerateMipmap(GLenum target) {
    if(!current_context) return;
    GLTRACE_CALL(glGenerateMipmap, es3_functions.glGenerateMipmap(target));
}
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {
    if(!current_context) return;
    GLTRACE_CALL(glViewport, es3_functions.glViewport(x, y, width, height));
}
void glBlendFunc(GLenum sfactor, GLenum dfactor) {
    if(!current_context) return;
    GLTRACE_CALL(glBlendFunc, es3_functions.glBlendFunc(sfactor, dfactor));
}
void glBlendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha) {
    if(!current_context) return;
    GLTRACE_CALL(glBlendFuncSeparate, es3_functions.glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha));
}
void glDepthFunc(GLenum func) {
    if(!current_context) return;
    GLTRACE_CALL(glDepthFunc, es3_functions.glDepthFunc(func));
}
void glDepthMask(GLboolean flag) {
    if(!current_context) return;
    GLTRACE_CALL(glDepthMask, es3_functions.glDepthMask(flag));
}
void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
    if(!current_context) return;
    GLTRACE_CALL(glColorMask, es3_functions.glColorMask(red, green, blue, alpha));
}
void glCullFace(GLenum mode) {
    if(!current_context) return;
    GLTRACE_CALL(glCullFace, es3_functions.glCullFace(mode));
}
void glStencilFunc(GLenum func, GLint ref, GLuint mask) {
    if(!current_context) return;
    GLTRACE_CALL(glStencilFunc, es3_functions.glStencilFunc(func, ref, mask));
}
void glStencilMask(GLuint mask) {
    if(!current_context) return;
    GLTRACE_CALL(glStencilMask, es3_functions.glStencilMask(mask));
}
void glLineWidth(GLfloat width) {
    if(!current_context) return;
    GLTRACE_CALL(glLineWidth, es3_functions.glLineWidth(width));
}
void glHint(GLenum target, GLenum mode) {
    if(!current_context) return;
    GLTRACE_CALL(glHint, es3_functions.glHint(target, mode));
}
void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
    if(!current_context) return;
    GLTRACE_CALL(glBufferData, es3_functions.glBufferData(target, size, data, usage));
}
void glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data) {
    if(!current_context) return;
    GLTRACE_CALL(glBufferSubData, es3_functions.glBufferSubData(target, offset, size, data));
}
void glCompileShader(GLuint shader) {
    if(!current_context) return;
    GLTRACE_CALL(glCompileShader, es3_functions.glCompileShader(shader));
}
void glUniform1i(GLint location, GLint v0) {
    if(!current_context) return;
    GLTRACE_CALL(glUniform1i, es3_functions.glUniform1i(location, v0));
}
void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    if(!current_context) return;
    GLTRACE_CALL(glUniform4f, es3_functions.glUniform4f(location, v0, v1, v2, v3));
}
void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
    if(!current_context) return;
    GLTRACE_CALL(glUniformMatrix4fv, es3_functions.glUniformMatrix4fv(location, count, transpose, value));
}
void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {
    if(!current_context) return;
    GLTRACE_CALL(glStencilOp, es3_functions.glStencilOp(sfail, dpfail, dppass));
}
void glStencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask) {
    if(!current_context) return;
    GLTRACE_CALL(glStencilFuncSeparate, es3_functions.glStencilFuncSeparate(face, func, ref, mask));
}
void glStencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass) {
    if(!current_context) return;
    GLTRACE_CALL(glStencilOpSeparate, es3_functions.glStencilOpSeparate(face, sfail, dpfail, dppass));
}
void glStencilMaskSeparate(GLenum face, GLuint mask) {
    if(!current_context) return;
    GLTRACE_CALL(glStencilMaskSeparate, es3_functions.glStencilMaskSeparate(face, mask));
}
void glPolygonOffset(GLfloat factor, GLfloat units) {
    if(!current_context) return;
    GLTRACE_CALL(glPolygonOffset, es3_functions.glPolygonOffset(factor, units));
}
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {
    if(!current_context) return;
    GLTRACE_CALL(glScissor, es3_functions.glScissor(x, y, width, height));
}
void glClearDepthf(GLclampf d) {
    if(!current_context) return;
    GLTRACE_CALL(glClearDepthf, es3_functions.glClearDepthf(d));
}
void glClearStencil(GLint s) {
    if(!current_context) return;
    GLTRACE_CALL(glClearStencil, es3_functions.glClearStencil(s));
}
void glDrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount) {
    if(!current_context) return;
    GLTRACE_CALL(glDrawArraysInstanced, es3_functions.glDrawArraysInstanced(mode, first, count, primcount));
}
void glDrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount) {
    if(!current_context) return;
    GLTRACE_CALL(glDrawElementsInstanced, es3_functions.glDrawElementsInstanced(mode, count, type, indices, primcount));
}
void glVertexAttribDivisor(GLuint index, GLuint divisor) {
    if(!current_context) return;
    GLTRACE_CALL(glVertexAttribDivisor, es3_functions.glVertexAttribDivisor(index, divisor));
}
void glEnableVertexAttribArray(GLuint index) {
    if(!current_context) return;
    GLTRACE_CALL(glEnableVertexAttribArray, es3_functions.glEnableVertexAttribArray(index));
}
void glDisableVertexAttribArray(GLuint index) {
    if(!current_context) return;
    GLTRACE_CALL(glDisableVertexAttribArray, es3_functions.glDisableVertexAttribArray(index));
}
void glUniform2f(GLint location, GLfloat v0, GLfloat v1) {
    if(!current_context) return;
    GLTRACE_CALL(glUniform2f, es3_functions.glUniform2f(location, v0, v1));
}
void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {
    if(!current_context) return;
    GLTRACE_CALL(glUniform3f, es3_functions.glUniform3f(location, v0, v1, v2));
}
void glUniform2fv(GLint location, GLsizei count, const GLfloat* value) {
    if(!current_context) return;
    GLTRACE_CALL(glUniform2fv, es3_functions.glUniform2fv(location, count, value));
}
void glUniform3fv(GLint location, GLsizei count, const GLfloat* value) {
    if(!current_context) return;
    GLTRACE_CALL(glUniform3fv, es3_functions.glUniform3fv(location, count, value));
}
GLint glGetUniformLocation(GLuint program, const GLchar* name) {
    GLint ret = -1;
    if(!current_context) return -1;
    static int dbg_n = 0;
    if(dbg_n < 10) {
        printf("[LTW DBG] glGetUniformLocation(prog=%u, name=%s)\n", program, name ? name : "(null)");
        dbg_n++;
    }
    GLTRACE_CALL(glGetUniformLocation, ret = es3_functions.glGetUniformLocation(program, name));
    return ret;
}
void glTexImage3D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels) {
    if(!current_context) return;
    GLTRACE_CALL(glTexImage3D, es3_functions.glTexImage3D(target, level, internalformat, width, height, depth, border, format, type, pixels));
}
void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels) {
    if(!current_context) return;
    GLTRACE_CALL(glTexSubImage3D, es3_functions.glTexSubImage3D(target, level, xoffset, yoffset, zoffset, width, height, depth, format, type, pixels));
}
void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border) {
    if(!current_context) return;
    GLTRACE_CALL(glCopyTexImage2D, es3_functions.glCopyTexImage2D(target, level, internalformat, x, y, width, height, border));
}
void glBlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter) {
    if(!current_context) return;
    GLTRACE_CALL(glBlitFramebuffer, es3_functions.glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter));
}
void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices) {
    if(!current_context) return;
    GLTRACE_CALL(glDrawRangeElements, es3_functions.glDrawRangeElements(mode, start, end, count, type, indices));
}
void glSampleCoverage(GLfloat value, GLboolean invert) {
    if(!current_context) return;
    GLTRACE_CALL(glSampleCoverage, es3_functions.glSampleCoverage(value, invert));
}
void glFlush(void) {
    if(!current_context) return;
    GLTRACE_CALL(glFlush, es3_functions.glFlush());
}
void glFinish(void) {
    if(!current_context) return;
    GLTRACE_CALL(glFinish, es3_functions.glFinish());
}

// GLES 3.x 中 glGetTexParameter 支持查询的 pname。桌面 GL 独有的 pname
// （GL_TEXTURE_PRIORITY/RESIDENT/GENERATE_MIPMAP/DEPTH_TEXTURE_MODE 等）一旦
// 透传，GLES 驱动就会报 GL_INVALID_ENUM(0x500)，污染错误队列并触发
// "stale 0x500" 连锁。不支持的 pname 由调用方返回规范默认值，不透传。
static bool is_gles_texparam_pname(GLenum pname) {
    switch (pname) {
        case GL_TEXTURE_MAG_FILTER:
        case GL_TEXTURE_MIN_FILTER:
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
        case GL_TEXTURE_MIN_LOD:
        case GL_TEXTURE_MAX_LOD:
        case GL_TEXTURE_BASE_LEVEL:
        case GL_TEXTURE_MAX_LEVEL:
        case GL_TEXTURE_BORDER_COLOR:
            return true;
        // 数值常量：GL_TEXTURE_COMPARE_MODE 0x884C / FUNC 0x884D,
        // GL_TEXTURE_SWIZZLE_R/G/B/A 0x8B42..0x8B45,
        // GL_TEXTURE_IMMUTABLE_FORMAT 0x912F / IMMUTABLE_LEVELS 0x82DF
        case 0x884C:
        case 0x884D:
        case 0x8B42:
        case 0x8B43:
        case 0x8B44:
        case 0x8B45:
        case 0x912F:
        case 0x82DF:
            return true;
        default:
            return false;
    }
}

// GLES 3.x 中 glGetTexParameter 接受的目标。桌面遗留目标（GL_TEXTURE_1D
// 等）透传同样会报 INVALID_ENUM。
static bool is_gles_texparam_target(GLenum target) {
    switch (target) {
        case GL_TEXTURE_2D:
        case GL_TEXTURE_3D:
        case GL_TEXTURE_CUBE_MAP:
        case 0x8C1A: /* GL_TEXTURE_2D_ARRAY */
        case 0x9009: /* GL_TEXTURE_CUBE_MAP_ARRAY */
        case 0x9100: /* GL_TEXTURE_2D_MULTISAMPLE */
        case 0x9102: /* GL_TEXTURE_2D_MULTISAMPLE_ARRAY */
            return true;
        default:
            return false;
    }
}

static void texparam_defaults(GLenum pname, GLint* params) {
    switch (pname) {
        case GL_TEXTURE_WRAP_S:
        case GL_TEXTURE_WRAP_T:
        case GL_TEXTURE_WRAP_R:
            params[0] = GL_REPEAT;
            break;
        case GL_TEXTURE_MIN_FILTER:
            params[0] = GL_NEAREST_MIPMAP_LINEAR;
            break;
        case GL_TEXTURE_MAG_FILTER:
            params[0] = GL_LINEAR;
            break;
        case GL_TEXTURE_MIN_LOD:
            params[0] = -1000;
            break;
        case GL_TEXTURE_MAX_LOD:
        case GL_TEXTURE_MAX_LEVEL:
            params[0] = 1000;
            break;
        case GL_TEXTURE_BASE_LEVEL:
        case 0x82DF: /* GL_TEXTURE_IMMUTABLE_LEVELS */
        case 0x912F: /* GL_TEXTURE_IMMUTABLE_FORMAT */
            params[0] = 0;
            break;
        case 0x884C: /* GL_TEXTURE_COMPARE_MODE */
            params[0] = GL_NONE;
            break;
        case 0x884D: /* GL_TEXTURE_COMPARE_FUNC */
            params[0] = GL_LEQUAL;
            break;
        case 0x8B42: params[0] = GL_RED;   break;
        case 0x8B43: params[0] = GL_GREEN; break;
        case 0x8B44: params[0] = GL_BLUE;  break;
        case 0x8B45: params[0] = GL_ALPHA; break;
        case GL_TEXTURE_BORDER_COLOR:
            params[0] = params[1] = params[2] = params[3] = 0;
            break;
        default:
            params[0] = 0;
    }
}

void glGetTexParameteriv(GLenum target, GLenum pname, GLint* params) {
    LTW_ENTER("glGetTexParameteriv");
    if(!current_context) { LTW_EXIT(); return; }
    if(!is_gles_texparam_target(target) || !is_gles_texparam_pname(pname)) {
        // 不透传：驱动不会收到非法枚举，错误队列保持干净
        texparam_defaults(pname, params);
        LTW_EXIT();
        return;
    }
    GLTRACE_CALL(glGetTexParameteriv, es3_functions.glGetTexParameteriv(target, pname, params));
    LTW_EXIT();
}

void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat* params) {
    LTW_ENTER("glGetTexParameterfv");
    if(!current_context) { LTW_EXIT(); return; }
    if(!is_gles_texparam_target(target) || !is_gles_texparam_pname(pname)) {
        GLint def[4] = {0};
        texparam_defaults(pname, def);
        params[0] = (GLfloat)def[0];
        params[1] = (GLfloat)def[1];
        params[2] = (GLfloat)def[2];
        params[3] = (GLfloat)def[3];
        LTW_EXIT();
        return;
    }
    GLTRACE_CALL(glGetTexParameterfv, es3_functions.glGetTexParameterfv(target, pname, params));
    LTW_EXIT();
}

INTERNAL int get_buffer_index(GLenum buffer) {
    switch (buffer) {
        case GL_ARRAY_BUFFER: return 0;
        case GL_COPY_READ_BUFFER: return 1;
        case GL_COPY_WRITE_BUFFER: return 2;
        case GL_PIXEL_PACK_BUFFER: return 3;
        case GL_PIXEL_UNPACK_BUFFER: return 4;
        case GL_TRANSFORM_FEEDBACK_BUFFER: return 5;
        case GL_UNIFORM_BUFFER: return 6;
        case GL_SHADER_STORAGE_BUFFER: return 7;
        case GL_DRAW_INDIRECT_BUFFER: return 8;
        default: return -1;
    }
}

INTERNAL int get_base_buffer_index(GLenum buffer) {
    switch (buffer) {
        case GL_ATOMIC_COUNTER_BUFFER: return 0;
        case GL_SHADER_STORAGE_BUFFER: return 1;
        case GL_TRANSFORM_FEEDBACK_BUFFER: return 2;
        case GL_UNIFORM_BUFFER: return 3;
        default: return -1;
    }
}

INTERNAL GLenum get_base_buffer_enum(int buffer_index) {
    switch (buffer_index) {
        case 0: return GL_ATOMIC_COUNTER_BUFFER;
        case 1: return GL_SHADER_STORAGE_BUFFER;
        case 2: return GL_TRANSFORM_FEEDBACK_BUFFER;
        case 3: return GL_UNIFORM_BUFFER;
        default: return -1;
    }
}

void glBindBuffer(GLenum buffer, GLuint name) {
    if(!current_context) return;
    {
        // 一次性诊断：确认 MC 的 buffer 使用模式
        static bool diag = false;
        if(!diag) {
            diag = true;
            printf("[LTW DIAG] glBindBuffer target=0x%x name=%u\n", buffer, name);
            fflush(stdout);
        }
    }
    es3_functions.glBindBuffer(buffer, name);
    int buffer_index = get_buffer_index(buffer);
    if(buffer_index == -1) return;
    current_context->bound_buffers[buffer_index] = name;
}

static basebuffer_binding_t* set_basebuffer(GLenum target, GLuint index, GLuint buffer) {
    int buffer_mapindex = get_base_buffer_index(target);
    if(buffer_mapindex == -1) return NULL;
    if(!buffer) {
        basebuffer_binding_t *old_binding = unordered_map_remove(current_context->bound_basebuffers[buffer_mapindex], (void*)index);
        free(old_binding);
        return NULL;
    }else {
        basebuffer_binding_t *binding = unordered_map_get(current_context->bound_basebuffers[buffer_mapindex], (void*)index);
        if(binding == NULL) {
            binding = calloc(1, sizeof(basebuffer_binding_t));
            unordered_map_put(current_context->bound_basebuffers[buffer_mapindex], (void*)index, binding);
        }
        binding->index = index;
        binding->buffer = buffer;
        return binding;
    }
}

void glBindBufferBase(GLenum target, GLuint index, GLuint buffer) {
    if(!current_context) return;
    es3_functions.glBindBufferBase(target, index, buffer);
    basebuffer_binding_t * binding = set_basebuffer(target, index, buffer);
    if(!binding) return;
    binding->ranged = false;
}

void glBindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    if(!current_context) return;
    es3_functions.glBindBufferRange(target, index, buffer, offset, size);
    basebuffer_binding_t * binding = set_basebuffer(target, index, buffer);
    if(!binding) return;
    binding->ranged = true;
    binding->size = size;
    binding->offset = offset;
}

void glUseProgram(GLuint program) {
    if(!current_context) return;
    GLTRACE_CALL(glUseProgram, es3_functions.glUseProgram(program));
    current_context->program = program;
    {
        static unsigned int un = 0;
        if((++un & 0xFF) == 0)
            printf("[LTW ERROR] useProgram: %u\n", program);
        if(un <= 10)
            printf("[LTW DBG] useProgram(prog=%u)\n", program);
    }
}

void glGetIntegerv(GLenum pname, GLint* data) {
    LTW_ENTER("glGetIntegerv");
    if(!current_context) { LTW_EXIT(); return; }
    switch (pname) {
        case GL_NUM_EXTENSIONS:
            es3_functions.glGetIntegerv(pname, data);
            (*data) += current_context->nextras;
            LTW_DEBUG_PRINTF("GL_NUM_EXTENSIONS: %i", (*data));
            break;
        case GL_MAX_COLOR_ATTACHMENTS:
            *data = MAX_FBTARGETS;
            LTW_EXIT();
            return;
        case GL_MAX_DRAW_BUFFERS:
            *data = current_context->max_drawbuffers;
            break;
        default:
            if(fp_get_matrix(pname, (GLfloat*)data)) break;
            es3_functions.glGetIntegerv(pname, data);
    }
    LTW_EXIT();
}

void glGetQueryObjectiv( 	GLuint id,
                            GLenum pname,
                            GLint * params) {
    if(!current_context) return;
    // This is not recommended but i don't care
    GLuint temp;
    es3_functions.glGetQueryObjectuiv(id, pname, &temp);
    *params = (GLint)temp;
}

void glDepthRange(GLdouble nearVal,
                  GLdouble farVal) {
    if(!current_context) return;
    es3_functions.glDepthRangef((GLfloat)nearVal, (GLfloat)farVal);
}

void glDeleteTextures(GLsizei n, const GLuint *textures) {
    if(!current_context) return;
    if(!textures) return;
    es3_functions.glDeleteTextures(n, textures);
    for(int i = 0; i < n; i++) {
        void* tracker = unordered_map_remove(current_context->texture_swztrack_map, (void*)textures[i]);
        if(tracker) mempool_free(current_context->swizzle_track_pool, tracker);
    }
}

static bool buf_tex_trigger = false;

void glTexBuffer(GLenum target, GLenum internalFormat, GLuint buffer) {
    if(!current_context) return;
    if(current_context->es32 && es3_functions.glTexBuffer) {
        es3_functions.glTexBuffer(target, internalFormat, buffer);
    } else if(current_context->buffer_texture_ext && es3_functions.glTexBufferEXT) {
        es3_functions.glTexBufferEXT(target, internalFormat, buffer);
    } else if(!buf_tex_trigger) {
        buf_tex_trigger = true;
        LTW_ERROR_PRINTF("LTW: Buffer textures aren't supported on your device");
    }
}

void glTexBufferARB(GLenum target, GLenum internalFormat, GLuint buffer) {
    glTexBuffer(target, internalFormat, buffer);
}

void glTexBufferRange(GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    if(!current_context) return;
    if(current_context->es32 && es3_functions.glTexBufferRange) {
        es3_functions.glTexBufferRange(target, internalFormat, buffer, offset, size);
    } else if(current_context->buffer_texture_ext && es3_functions.glTexBufferRangeEXT) {
        es3_functions.glTexBufferRangeEXT(target, internalFormat, buffer, offset, size);
    } else if(!buf_tex_trigger) {
        buf_tex_trigger = true;
        LTW_ERROR_PRINTF("LTW: Buffer textures aren't supported on your device");
    }
}

void glTexBufferRangeARB(GLenum target, GLenum internalFormat, GLuint buffer, GLintptr offset, GLsizeiptr size) {
    glTexBufferRange(target, internalFormat, buffer, offset, size);
}

static bool noerror;
// GL error queue tracing. Was ON by default during the 1280 INVALID_ENUM
// hunt; the hunt is over, so default OFF (the trace consumed the game's
// own glGetError results and spammed stale-error messages). Enable with
// the LTW_GLERR_TRACE env var when hunting regressions again.
bool glerr_trace = false;
_Thread_local const char* ltw_last_glfn = NULL;

__attribute((constructor)) void init_noerror() {
    noerror = env_istrue("LIBGL_NOERROR");
    debug = env_istrue("LTW_DEBUG");
    glerr_trace = env_istrue("LTW_GLERR_TRACE");
    never_flush_buffers = env_istrue_d("LTW_NEVER_FLUSH_BUFFERS", true);
    coherent_dynamic_storage = env_istrue_d("LTW_COHERENT_DYNAMIC_STORAGE", true);
    if(!noerror) LTW_ERROR_PRINTF("LTW will NOT ignore GL errors. This may break mods, consider yourself warned.");
    if(coherent_dynamic_storage) LTW_ERROR_PRINTF("LTW will force dynamic storage buffers to be coherent.");
    if(debug) LTW_ERROR_PRINTF("LTW will allow GL_DEBUG_OUTPUT to be enabled. Expect massive logs.");
    if(never_flush_buffers) LTW_ERROR_PRINTF("LTW will prevent all explicit buffer flushes.");
}

GLenum glGetError() {
    if(noerror) return 0;
    GLenum e = es3_functions.glGetError();
    if(glerr_trace && e != GL_NO_ERROR) {
        static unsigned int n = 0;
        if((n++ & 0x3F) == 0) LTW_ERROR_PRINTF("LTW: glGetError -> 0x%x", (unsigned)e);
    }
    return e;
}

void glDebugMessageControl( 	GLenum source,
                               GLenum type,
                               GLenum severity,
                               GLsizei count,
                               const GLuint *ids,
                               GLboolean enabled) {
    //STUB
}

// 测试拦截函数 - 用于验证LTW拦截功能
void glTestIntercept(void) {
    LTW_ERROR_PRINTF("LTW INTERCEPT SUCCESS: glTestIntercept called - LTW is actively intercepting OpenGL calls!");
    LTW_ERROR_PRINTF("LTW STATUS: Interception layer is working correctly");
    LTW_ERROR_PRINTF("LTW DEBUG: This proves LTW can intercept and map OpenGL functions");
}

// 增强关键函数的日志输出
void glClear(GLbitfield mask) {
    if(!current_context) return;
    {
        // 无条件低频计数：确认渲染循环是否在跑
        static unsigned int clear_n = 0;
        if((++clear_n & 0xFF) == 1) {
            printf("[LTW CNT] glClear %u mask=0x%x\n", clear_n, mask);
            fflush(stdout);
        }
    }
    if(debug) {
        LTW_DEBUG_PRINTF("LTW INTERCEPT: glClear called with mask=0x%x", mask);
        LTW_DEBUG_PRINTF("LTW MAPPING: Mapping to es3_functions.glClear");
    }
    GLTRACE_CALL(glClear, es3_functions.glClear(mask));
    if(debug) {
        LTW_DEBUG_PRINTF("LTW SUCCESS: glClear completed successfully");
    }
}

void glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
    if(!current_context) return;
    GLTRACE_CALL(glClearColor, es3_functions.glClearColor(red, green, blue, alpha));
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
    LTW_ENTER("glDrawArrays");
    if(!current_context) { LTW_EXIT(); return; }
    {
        // 无条件低频计数：确认绘制调用是否到达 LTW
        static unsigned int draw_n = 0;
        if((++draw_n & 0xFF) == 1) {
            printf("[LTW CNT] glDrawArrays %u mode=0x%x first=%d count=%d\n", draw_n, mode, first, count);
            fflush(stdout);
        }
    }
    if(ltw_quads_draw_arrays(mode, first, count)) { LTW_EXIT(); return; }
    if(fp_try_draw_arrays(mode, first, count)) { LTW_EXIT(); return; }
    {
        // 透传诊断：prog != 0 的绘制（字形/文字候选路径）
        static bool pt_diag = false;
        if(!pt_diag) {
            GLint prog = 0;
            es3_functions.glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
            printf("[LTW DIAG] passthrough glDrawArrays mode=0x%x first=%d count=%d prog=%d\n",
                   (unsigned)mode, first, count, prog);
            fflush(stdout);
        }
    }
    GLTRACE_CALL(glDrawArrays, current_context->fast_gl.glDrawArrays(mode, first, count));
    LTW_EXIT();
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    LTW_ENTER("glDrawElements");
    if(!current_context) { LTW_EXIT(); return; }
    {
        // 无条件低频计数：确认绘制调用是否到达 LTW
        static unsigned int drawe_n = 0;
        if((++drawe_n & 0xFF) == 1) {
            printf("[LTW CNT] glDrawElements %u mode=0x%x count=%d\n", drawe_n, mode, count);
            fflush(stdout);
        }
    }
    if(ltw_quads_draw_elements(mode, count, type, indices)) { LTW_EXIT(); return; }
    if(fp_try_draw_elements(mode, count, type, indices)) { LTW_EXIT(); return; }
    {
        // 透传诊断：prog != 0 的绘制（字形/文字候选路径）
        static bool pte_diag = false;
        if(!pte_diag) {
            GLint prog = 0;
            es3_functions.glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
            printf("[LTW DIAG] passthrough glDrawElements mode=0x%x count=%d type=0x%x prog=%d\n",
                   (unsigned)mode, count, type, prog);
            fflush(stdout);
        }
    }
    GLTRACE_CALL(glDrawElements, current_context->fast_gl.glDrawElements(mode, count, type, indices));
    LTW_EXIT();
}

void glLTWBeginBatchUpdate(void) {
    if(!current_context) return;
    swizzle_begin_batch_update();
}

void glLTWEndBatchUpdate(void) {
    if(!current_context) return;
    swizzle_end_batch_update();
}
