/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#ifndef POJAVLAUNCHER_EGL_H
#define POJAVLAUNCHER_EGL_H

#include <stdbool.h>
#include <EGL/egl.h>
#include "proc.h"
#include "unordered_map/unordered_map.h"

#define MAX_BOUND_BUFFERS 9
#define MAX_BOUND_BASEBUFFERS 4
#define MAX_DRAWBUFFERS 8
#define MAX_FBTARGETS 8
#define MAX_TMUS 8
#define MAX_TEXTARGETS 8

typedef struct {
    bool ready;
    GLuint indirectRenderBuffer;
} basevertex_renderer_t;

typedef struct {
    GLuint index;
    GLuint buffer;
    bool ranged;
    GLintptr  offset;
    GLintptr  size;
} basebuffer_binding_t;

typedef struct {
    GLuint color_targets[MAX_FBTARGETS];
    GLuint color_objects[MAX_FBTARGETS];
    GLuint color_levels[MAX_FBTARGETS];
    GLuint color_layers[MAX_FBTARGETS];
    GLenum virt_drawbuffers[MAX_DRAWBUFFERS];
    GLenum phys_drawbuffers[MAX_DRAWBUFFERS];
    GLsizei nbuffers;
} framebuffer_t;

typedef struct {
    bool ready;
    GLuint temp_texture;
    GLuint tempfb;
    GLuint destfb;
    void* depthData;
    GLsizei depthWidth, depthHeight;
} framebuffer_copier_t;

typedef struct {
    GLenum original_swizzle[4];  // 原始swizzle
    GLenum applied_swizzle[4];   // 已应用的swizzle（缓存）
    GLboolean goofy_byte_order;
    GLboolean upload_bgra;
} texture_swizzle_track_t;

typedef struct {
    GLint internalformat;
    GLenum type;
    GLenum format;
    bool valid;
} format_cache_entry_t;

#define FORMAT_CACHE_SIZE 64

typedef struct {
    EGLContext phys_context;    //实际的EGL上下文句柄
    bool context_rdy;   //标记上下文是否已准备就绪
    bool es31, es32, buffer_storage, buffer_texture_ext, multidraw_indirect;    //支持OpenGL ES 3.1/3.2版本
    PFNGLDRAWELEMENTSBASEVERTEXPROC drawelementsbasevertex; //函数指针，指向绘制元素基顶点的函数
    GLint shader_version;   //着色器版本
    basevertex_renderer_t basevertex;   //基顶点渲染器
    GLuint multidraw_element_buffer;    //多重绘制元素缓冲区对象
    framebuffer_copier_t framebuffer_copier;    //帧缓冲复制器
    unordered_map* shader_map;  //着色器映射表
    unordered_map* program_map; //程序映射表
    unordered_map* framebuffer_map; //帧缓冲映射表
    unordered_map* texture_swztrack_map;    //纹理重组跟踪映射表
    unordered_map* bound_basebuffers[MAX_BOUND_BASEBUFFERS];    //绑定的基本缓冲区映射表数组
    int proxy_width, proxy_height, proxy_intformat, maxTextureSize; //代理纹理参数和最大纹理尺寸
    GLint max_drawbuffers;  //最大绘制缓冲区数
    GLuint bound_buffers[MAX_BOUND_BUFFERS];       //绑定的缓冲区对象数组
    GLuint program;     //当前使用的程序对象
    GLuint draw_framebuffer;    //绘制帧缓冲对象
    GLuint read_framebuffer;    //读取帧缓冲对象
    framebuffer_t* cached_draw_framebuffer;   //缓存的绘制帧缓冲对象
    framebuffer_t* cached_read_framebuffer;   //缓存的读取帧缓冲对象
    char* extensions_string;    //扩展字符串
    size_t nextras;         //额外扩展数量
    int nextensions_es;     //ES扩展数量
    char** extra_extensions_array;      //额外扩展字符串数组
    format_cache_entry_t format_cache[FORMAT_CACHE_SIZE];   //纹理格式缓存
    int format_cache_index;    //格式缓存索引
    GLsizei multidraw_buffer_size;  //多重绘制缓冲区大小
} context_t;        //表示OpenGL ES的上下文状态信息

extern thread_local context_t *current_context;
extern void init_egl();
extern GLenum get_textarget_query_param(GLenum target);

#endif //POJAVLAUNCHER_EGL_H
