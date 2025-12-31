/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */
#include "egl.h"
#include "unordered_map/int_hash.h"
#include "string_utils.h"
#include "env.h"
#include "mempool.h"
#include "debug.h"
#include <string.h>
#include <pthread.h>

thread_local context_t *current_context;
unordered_map* context_map;

// 保存当前的 EGL 状态，用于防止重复绑定
static EGLDisplay current_display = EGL_NO_DISPLAY;
static EGLSurface current_draw_surface = EGL_NO_SURFACE;
static EGLSurface current_read_surface = EGL_NO_SURFACE;
static EGLContext current_egl_context = EGL_NO_CONTEXT;

// 互斥锁，用于保护全局 EGL 状态
static pthread_mutex_t egl_state_mutex = PTHREAD_MUTEX_INITIALIZER;

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

    // 初始化内存池
    tw_context->shader_info_pool = mempool_create(sizeof(shader_info_t), 64);
    if(!tw_context->shader_info_pool) goto fail_dealloc;
    tw_context->program_info_pool = mempool_create(sizeof(program_info_t), 32);
    if(!tw_context->program_info_pool) goto fail_dealloc;
    tw_context->framebuffer_pool = mempool_create(sizeof(framebuffer_t), 32);
    if(!tw_context->framebuffer_pool) goto fail_dealloc;
    tw_context->swizzle_track_pool = mempool_create(sizeof(texture_swizzle_track_t), 128);
    if(!tw_context->swizzle_track_pool) goto fail_dealloc;

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
    
    // 清理内存池
    if(tw_context->shader_info_pool) mempool_destroy(tw_context->shader_info_pool);
    if(tw_context->program_info_pool) mempool_destroy(tw_context->program_info_pool);
    if(tw_context->framebuffer_pool) mempool_destroy(tw_context->framebuffer_pool);
    if(tw_context->swizzle_track_pool) mempool_destroy(tw_context->swizzle_track_pool);
    
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

    // 销毁内存池
    if(tw_context->shader_info_pool) mempool_destroy(tw_context->shader_info_pool);
    if(tw_context->program_info_pool) mempool_destroy(tw_context->program_info_pool);
    if(tw_context->framebuffer_pool) mempool_destroy(tw_context->framebuffer_pool);
    if(tw_context->swizzle_track_pool) mempool_destroy(tw_context->swizzle_track_pool);
}

void init_extra_extensions(context_t* context, int* length) {
    const char* es_extensions = (const char*)es3_functions.glGetString(GL_EXTENSIONS);
    *length = (int)strlen(es_extensions);
    // 预分配额外512字节的空间，减少后续realloc次数
    size_t capacity = *length + 512 + 1;
    context->extensions_string = malloc(capacity);
    if(!context->extensions_string) {
        LTW_ERROR_PRINTF("LTW: Failed to allocate memory for extensions string");
        *length = 0;
        return;
    }
    context->extensions_capacity = capacity;
    memcpy(context->extensions_string, es_extensions, *length+1);
}

void add_extra_extension(context_t* context, int* length, const char* extension)  {
    size_t extension_len = strlen(extension);

    // 检查扩展名长度，防止栈溢出或过大的内存分配
    if(extension_len > 1024) {
        LTW_ERROR_PRINTF("LTW: Extension name too long: %zu bytes", extension_len);
        return;
    }

    // 使用动态分配替代可变长度数组
    char* str_append_extension = malloc(extension_len + 2);
    if(!str_append_extension) {
        LTW_ERROR_PRINTF("LTW: Failed to allocate memory for extension string");
        return;
    }
    memcpy(str_append_extension, extension, extension_len);
    str_append_extension[extension_len] = ' ';
    str_append_extension[extension_len + 1] = 0;

    // 检查当前分配的内存是否足够
    size_t current_capacity = context->extensions_capacity;
    size_t new_length = *length + extension_len + 1;
    if(new_length >= current_capacity) {
        // 需要扩容，一次性分配更大的空间
        size_t new_capacity = new_length + 256;  // 额外预留256字节
        char* new_ptr = realloc(context->extensions_string, new_capacity);
        if(!new_ptr) {
            LTW_ERROR_PRINTF("LTW: Failed to reallocate extensions string");
            free(str_append_extension);
            return;
        }
        context->extensions_string = new_ptr;
        context->extensions_capacity = new_capacity;
    }

    // 先分配扩展数组，再追加字符串，确保原子性
    int extension_idx = context->nextras;
    char** new_array = realloc(context->extra_extensions_array, sizeof(char*)*(context->nextras + 1));
    if(!new_array) {
        LTW_ERROR_PRINTF("LTW: Failed to reallocate extra extensions array");
        free(str_append_extension);
        return;
    }
    context->extra_extensions_array = new_array;
    char* extra_extension = malloc(extension_len + 1);
    if(!extra_extension) {
        LTW_ERROR_PRINTF("LTW: Failed to allocate memory for extra extension");
        free(str_append_extension);
        return;
    }
    strncpy(extra_extension, extension, extension_len + 1);
    context->extra_extensions_array[extension_idx] = extra_extension;

    // 所有分配都成功后，再更新状态
    memcpy(context->extensions_string + *length, str_append_extension, extension_len + 2);
    *length += extension_len + 1;
    context->nextras++;
    free(str_append_extension);
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
        else LTW_ERROR_PRINTF("LTW: The buffer storage extension is hidden.");
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
    LTW_ERROR_PRINTF("LTW: Running on OpenGL ES %i.%i with ESSL %i", esmajor, esminor, context->shader_version);
    if(esmajor == 0 && esminor == 0) goto fail;
    if(esmajor < 3 || context->shader_version < 300) {
        LTW_ERROR_PRINTF("Unsupported OpenGL ES version. This will cause you problems down the line.");
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

    // 初始化 swizzle 批量更新相关字段
    tw_context->pending_swizzle_count = 0;
    tw_context->swizzle_batch_mode = false;
    memset(tw_context->pending_swizzle_textures, 0, sizeof(tw_context->pending_swizzle_textures));
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
    if(old_ctx) {
        free_context(old_ctx);
        free(old_ctx);
    }

    // 使用互斥锁保护全局 EGL 状态
    pthread_mutex_lock(&egl_state_mutex);

    // 如果销毁的是当前上下文，清除保存的 EGL 状态
    if (current_egl_context == ctx) {
        current_display = EGL_NO_DISPLAY;
        current_draw_surface = EGL_NO_SURFACE;
        current_read_surface = EGL_NO_SURFACE;
        current_egl_context = EGL_NO_CONTEXT;
        current_context = NULL;
    }

    pthread_mutex_unlock(&egl_state_mutex);

    return EGL_TRUE;
}

EGLBoolean eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) {
    // 使用互斥锁保护全局 EGL 状态
    pthread_mutex_lock(&egl_state_mutex);

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
        pthread_mutex_unlock(&egl_state_mutex);
        return host_eglMakeCurrent(dpy, draw, read, ctx);
    }

    // 检查是否是重复绑定相同的上下文
    if (current_display == dpy && current_draw_surface == draw &&
        current_read_surface == read && current_egl_context == ctx) {
        // 上下文已经绑定，直接返回成功
        pthread_mutex_unlock(&egl_state_mutex);
        return EGL_TRUE;
    }

    // 保存当前状态用于调用后验证
    EGLDisplay saved_display = current_display;
    EGLSurface saved_draw_surface = current_draw_surface;
    EGLSurface saved_read_surface = current_read_surface;
    EGLContext saved_egl_context = current_egl_context;

    // 释放锁，调用 host 函数
    pthread_mutex_unlock(&egl_state_mutex);

    // 新的上下文绑定
    if(!host_eglMakeCurrent(dpy, draw, read, ctx)) return EGL_FALSE;

    // 获取上下文信息（在锁外进行，避免死锁）
    pthread_mutex_lock(&egl_state_mutex);
    context_t* tw_context = unordered_map_get(context_map, ctx);
    pthread_mutex_unlock(&egl_state_mutex);

    if(tw_context == NULL) {
        LTW_ERROR_PRINTF("TinywrapperEGL: Failed to find context %p", ctx);
        abort();
    }
    if(!tw_context->context_rdy) {
        init_incontext(tw_context);
        tw_context->context_rdy = true;
    }

    // 重新获取锁，验证状态一致性并更新
    pthread_mutex_lock(&egl_state_mutex);

    // 验证状态是否被其他线程修改
    // 如果状态被修改，说明有并发操作，此时我们仍然继续更新为新的上下文
    // 因为 host_eglMakeCurrent 已经成功，说明底层 EGL 状态已经更新
    if (current_display != saved_display || current_draw_surface != saved_draw_surface ||
        current_read_surface != saved_read_surface || current_egl_context != saved_egl_context) {
        // 状态被修改，记录警告但继续执行
        LTW_ERROR_PRINTF("TinywrapperEGL: State changed during MakeCurrent, continuing with new context");
    }

    // 更新全局状态
    current_display = dpy;
    current_draw_surface = draw;
    current_read_surface = read;
    current_egl_context = ctx;
    current_context = tw_context;
    pthread_mutex_unlock(&egl_state_mutex);

    return EGL_TRUE;
}