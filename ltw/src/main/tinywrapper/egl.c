/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */
#include "egl.h"
#include "unordered_map/int_hash.h"
#include "string_utils.h"
#include "env.h"
#include <string.h>

thread_local context_t *current_context;
unordered_map* context_map;

// 保存当前的 EGL 状态，用于防止重复绑定
static EGLDisplay current_display = EGL_NO_DISPLAY;
static EGLSurface current_draw_surface = EGL_NO_SURFACE;
static EGLSurface current_read_surface = EGL_NO_SURFACE;
static EGLContext current_egl_context = EGL_NO_CONTEXT;

EGLContext (*host_eglCreateContext)(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
EGLBoolean (*host_eglDestroyContext)(EGLDisplay dpy, EGLContext ctx);
EGLBoolean (*host_eglMakeCurrent) (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);

void init_egl() {
    context_map = alloc_intmap();
    host_eglCreateContext = (EGLContext (*)(EGLDisplay, EGLConfig, EGLContext,
                                            const EGLint *)) host_eglGetProcAddress("eglCreateContext");
    host_eglDestroyContext = (EGLBoolean (*)(EGLDisplay, EGLContext)) host_eglGetProcAddress(
            "eglDestroyContext");
    host_eglMakeCurrent = (EGLBoolean (*)(EGLDisplay, EGLSurface, EGLSurface,
                                          EGLContext)) host_eglGetProcAddress("eglMakeCurrent");
}

static bool init_context(context_t* tw_context) {
    tw_context->shader_map = alloc_intmap_safe();
    if(!tw_context->shader_map) goto fail;
    tw_context->framebuffer_map = alloc_intmap_safe();
    if(!tw_context->framebuffer_map) goto fail_dealloc;
    tw_context->program_map = alloc_intmap_safe();
    if(!tw_context->program_map) goto fail_dealloc;
    tw_context->texture_swztrack_map = alloc_intmap_safe();
    if(!tw_context->texture_swztrack_map) goto fail_dealloc;
    for(int i = 0; i < MAX_BOUND_BASEBUFFERS; i++) {
        unordered_map *map = alloc_intmap_safe();
        if(!map) goto fail_dealloc;
        tw_context->bound_basebuffers[i] = map;
    }
    return true;

    fail_dealloc:
    for(int i = 0; i < MAX_BOUND_BASEBUFFERS; i++) {
        unordered_map *map = tw_context->bound_basebuffers[i];
        if(map) unordered_map_free(map);
    }
    if(tw_context->shader_map)
        unordered_map_free(tw_context->shader_map);
    if(tw_context->framebuffer_map)
        unordered_map_free(tw_context->framebuffer_map);
    if(tw_context->program_map)
        unordered_map_free(tw_context->program_map);
    if(tw_context->texture_swztrack_map)
        unordered_map_free(tw_context->texture_swztrack_map);
    fail:
    return false;
}

static void free_context(context_t* tw_context) {
    unordered_map_free(tw_context->shader_map);
    unordered_map_free(tw_context->program_map);
    unordered_map_free(tw_context->framebuffer_map);
    unordered_map_free(tw_context->texture_swztrack_map);
    if(tw_context->extensions_string != NULL) free(tw_context->extensions_string);
    if(tw_context->nextras != 0 && tw_context->extra_extensions_array != NULL) {
        for(int i = 0; i < tw_context->nextras; i++) {
            free((tw_context->extra_extensions_array[i]));
        }
        free(tw_context->extra_extensions_array);
    }
}

void init_extra_extensions(context_t* context, int* length) {
    const char* es_extensions = (const char*)es3_functions.glGetString(GL_EXTENSIONS);
    *length = (int)strlen(es_extensions);
    // 预分配额外512字节的空间，减少后续realloc次数
    context->extensions_string = malloc(*length + 512 + 1);
    memcpy(context->extensions_string, es_extensions, *length+1);
}

void add_extra_extension(context_t* context, int* length, const char* extension)  {
    size_t extension_len = strlen(extension);

    char str_append_extension[extension_len + 2];
    memcpy(str_append_extension, extension, extension_len);
    str_append_extension[extension_len] = ' ';
    str_append_extension[extension_len + 1] = 0;

    // 检查当前分配的内存是否足够
    size_t current_capacity = malloc_usable_size(context->extensions_string);
    size_t new_length = *length + extension_len + 1;
    if(new_length >= current_capacity) {
        // 需要扩容，一次性分配更大的空间
        size_t new_capacity = new_length + 256;  // 额外预留256字节
        context->extensions_string = realloc(context->extensions_string, new_capacity);
    }
    // 直接追加字符串
    memcpy(context->extensions_string + *length, str_append_extension, extension_len + 2);
    *length += extension_len + 1;

    int extension_idx = context->nextras++;
    context->extra_extensions_array = realloc(context->extra_extensions_array, sizeof(char*)*context->nextras);
    char* extra_extension = malloc(extension_len + 1);
    strncpy(extra_extension, extension, extension_len + 1);
    context->extra_extensions_array[extension_idx] = extra_extension;
}

void fin_extra_extensions(context_t* context, int length) {
    if(context->extensions_string[length-2] != ' ') return;
    char* orig_string = context->extensions_string;
    context->extensions_string = realloc(context->extensions_string, length - 1);
    if(context->extensions_string == NULL) {
        free(orig_string);
        return;
    }
    context->extensions_string[length-2] = 0;
}

void build_extension_string(context_t* context) {
    int length;
    init_extra_extensions(context, &length);
    if(context->buffer_storage) {
        if(!env_istrue("LTW_HIDE_BUFFER_STORAGE"))
            add_extra_extension(context, &length, "GL_ARB_buffer_storage");
        else printf("LTW: The buffer storage extension is hidden.\n");
    }
    if(context->buffer_texture_ext || context->es32) {
        add_extra_extension(context, &length, "GL_ARB_texture_buffer_object");
    }
    add_extra_extension(context, &length, "GL_ARB_draw_elements_base_vertex");

    // More extensions are possible, but will need way more wraps and tracking.
    fin_extra_extensions(context, length);
}

static void find_esversion(context_t* context) {
    const char* version = (const char*) es3_functions.glGetString(GL_VERSION);
    const char* shader_version = (const char*) es3_functions.glGetString(GL_SHADING_LANGUAGE_VERSION);

    int esmajor = 0, esminor = 0, shadermajor = 3, shaderminor = 0;
    sscanf(version, " OpenGL ES %i.%i", &esmajor, &esminor);
    sscanf(shader_version, " OpenGL ES GLSL ES %i.%i", &shadermajor, &shaderminor);
    context->shader_version = shadermajor * 100 + shaderminor;
    printf("LTW: Running on OpenGL ES %i.%i with ESSL %i\n", esmajor, esminor, context->shader_version);
    if(esmajor == 0 && esminor == 0) goto fail;
    if(esmajor < 3 || context->shader_version < 300) {
        printf("Unsupported OpenGL ES version. This will cause you problems down the line.\n");
        return;
    }
    if(esmajor == 3) {
        context->es31 = esminor >= 1;
        context->es32 = esminor >= 2;
    }else if(esmajor > 3) {
        context->es32 = context->es31 = true;
    }

    const char* extensions = (const char*) es3_functions.glGetString(GL_EXTENSIONS);
    if(strstr(extensions, "GL_EXT_buffer_storage")) context->buffer_storage = true;
    if(strstr(extensions, "GL_EXT_texture_buffer")) context->buffer_texture_ext = true;
    if(strstr(extensions, "GL_EXT_multi_draw_indirect")) context->multidraw_indirect = true;

    bool basevertex_oes = strstr(extensions, "GL_OES_draw_elements_base_vertex");
    bool basevertex_ext = strstr(extensions, "GL_EXT_draw_elements_base_vertex");
    if(context->es32) context->drawelementsbasevertex = es3_functions.glDrawElementsBaseVertex;
    else if(basevertex_oes) context->drawelementsbasevertex = es3_functions.glDrawElementsBaseVertexOES;
    else if(basevertex_ext) context->drawelementsbasevertex = es3_functions.glDrawElementsBaseVertexEXT;
    else context->drawelementsbasevertex = NULL;

    build_extension_string(context);

    return;
    fail:
    printf("LTW: Failed to detect OpenGL ES version");
}

void basevertex_init(context_t* context);
void buffer_copier_init(context_t* context);
static void init_incontext(context_t* tw_context) {
    es3_functions.glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tw_context->maxTextureSize);
    es3_functions.glGetIntegerv(GL_MAX_DRAW_BUFFERS, &tw_context->max_drawbuffers);
    es3_functions.glGetIntegerv(GL_NUM_EXTENSIONS, &tw_context->nextensions_es);
    if(tw_context->max_drawbuffers > MAX_DRAWBUFFERS) {
        tw_context->max_drawbuffers = MAX_DRAWBUFFERS;
    }

    find_esversion(tw_context);

    basevertex_init(tw_context);
    buffer_copier_init(tw_context);
    es3_functions.glGenBuffers(1, &tw_context->multidraw_element_buffer);

    // 初始化格式缓存
    memset(tw_context->format_cache, 0, sizeof(tw_context->format_cache));
    tw_context->format_cache_index = 0;

    // 初始化multidraw缓冲区，预分配256KB
    tw_context->multidraw_buffer_size = 256 * 1024;
    es3_functions.glBindBuffer(GL_COPY_WRITE_BUFFER, tw_context->multidraw_element_buffer);
    es3_functions.glBufferData(GL_COPY_WRITE_BUFFER, tw_context->multidraw_buffer_size, NULL, GL_STREAM_DRAW);
    es3_functions.glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

EGLContext eglCreateContext(EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list) {
    EGLContext phys_context = host_eglCreateContext(dpy, config, share_context, attrib_list);
    if(phys_context == EGL_NO_CONTEXT) return phys_context;
    context_t* tw_context = calloc(1, sizeof(context_t));
    if(tw_context == NULL || !init_context(tw_context)) {
        if(tw_context) free(tw_context);
        host_eglDestroyContext(dpy, phys_context);
        return EGL_NO_CONTEXT;
    }
    unordered_map_put(context_map, phys_context, tw_context);
    return phys_context;
}

EGLBoolean eglDestroyContext (EGLDisplay dpy, EGLContext ctx) {
    if(!host_eglDestroyContext(dpy, ctx)) return EGL_FALSE;
    context_t* old_ctx = unordered_map_remove(context_map, ctx);
    free_context(old_ctx);
    free(old_ctx);

    // 如果销毁的是当前上下文，清除保存的 EGL 状态
    if (current_egl_context == ctx) {
        current_display = EGL_NO_DISPLAY;
        current_draw_surface = EGL_NO_SURFACE;
        current_read_surface = EGL_NO_SURFACE;
        current_egl_context = EGL_NO_CONTEXT;
        current_context = NULL;
    }

    return EGL_TRUE;
}

EGLBoolean eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) {
    // 清除上下文的情况
    if (ctx == EGL_NO_CONTEXT) {
        if (current_display != EGL_NO_DISPLAY || current_draw_surface != EGL_NO_SURFACE ||
            current_read_surface != EGL_NO_SURFACE || current_egl_context != EGL_NO_CONTEXT) {
            current_display = EGL_NO_DISPLAY;
            current_draw_surface = EGL_NO_SURFACE;
            current_read_surface = EGL_NO_SURFACE;
            current_egl_context = EGL_NO_CONTEXT;
            current_context = NULL;
        }
        return host_eglMakeCurrent(dpy, draw, read, ctx);
    }

    // 检查是否是重复绑定相同的上下文
    if (current_display == dpy && current_draw_surface == draw &&
        current_read_surface == read && current_egl_context == ctx) {
        // 上下文已经绑定，直接返回成功
        return EGL_TRUE;
    }

    // 新的上下文绑定
    if(!host_eglMakeCurrent(dpy, draw, read, ctx)) return EGL_FALSE;

    // 保存新的 EGL 状态
    current_display = dpy;
    current_draw_surface = draw;
    current_read_surface = read;
    current_egl_context = ctx;

    context_t* tw_context = unordered_map_get(context_map, ctx);
    if(tw_context == NULL) {
        printf("TinywrapperEGL: Failed to find context %p\n", ctx);
        abort();
    }
    if(!tw_context->context_rdy) {
        init_incontext(tw_context);
        tw_context->context_rdy = true;
    }
    current_context = tw_context;
    return EGL_TRUE;
}