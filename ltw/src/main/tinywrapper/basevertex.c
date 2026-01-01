/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */
#include <GLES3/gl31.h>
#include "proc.h"
#include "egl.h"
#include "main.h"
#include "debug.h"

typedef struct {
    GLuint count;
    GLuint instanceCount;
    GLuint firstIndex;
    GLint baseVertex;
    GLuint reservedMustBeZero;
} indirect_pass_t;

void basevertex_init(context_t* context) {
    basevertex_renderer_t *renderer = &context->basevertex;
    if(context->drawelementsbasevertex != NULL) {
        LTW_ERROR_PRINTF("LTW: BaseVertex render calls will use the host driver implementation");
        return;
    }
    if(!context->es31) {
        LTW_ERROR_PRINTF("LTW: BaseVertex render calls not available: requires OpenGL ES 3.1");
        return;
    }
    es3_functions.glGenBuffers(1, &renderer->indirectRenderBuffer);
    GLenum error = es3_functions.glGetError();
    if(error != GL_NO_ERROR) {
        LTW_ERROR_PRINTF("LTW: Failed to initialize indirect buffers: %x", error);
        return;
    }
    renderer->ready = true;
}

GLint type_bytes(GLenum type) {
    switch (type) {
        case GL_UNSIGNED_BYTE: return 1;
        case GL_UNSIGNED_SHORT: return 2;
        case GL_UNSIGNED_INT: return 4;
        default: return -1;
    }
}

static void restore_state(GLuint element_buffer) {
    es3_functions.glBindBuffer(GL_DRAW_INDIRECT_BUFFER, current_context->bound_buffers[get_buffer_index(GL_DRAW_INDIRECT_BUFFER)]);
}

void glDrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex) {
    if(!current_context) return;
    if(current_context->drawelementsbasevertex != NULL) {
        current_context->drawelementsbasevertex(mode, count, type, indices, basevertex);
        return;
    }
    basevertex_renderer_t *renderer = &current_context->basevertex;
    if(!renderer->ready) return;
    GLint elementbuffer;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementbuffer);
    if(elementbuffer == 0) {
        // I am not bothered enough to implement this.
        LTW_ERROR_PRINTF("LTW: Base vertex draws without element buffer are not supported");
        return;
    }
    GLint typeBytes = type_bytes(type);
    if(typeBytes <= 0) {
        LTW_ERROR_PRINTF("LTW: unsupported type for base vertex draw");
        return;
    }
    uintptr_t indicesPointer = (uintptr_t)indices;
    if(indicesPointer % typeBytes != 0) {
        LTW_ERROR_PRINTF("LTW: misaligned base vertex draw not supported");
        return;
    }
    indirect_pass_t indirect_pass;
    indirect_pass.count = count;
    indirect_pass.firstIndex = indicesPointer / typeBytes;
    indirect_pass.baseVertex = basevertex;
    indirect_pass.instanceCount = 1;
    indirect_pass.reservedMustBeZero = 0;
    es3_functions.glBindBuffer(GL_DRAW_INDIRECT_BUFFER, renderer->indirectRenderBuffer);
    es3_functions.glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(indirect_pass_t), &indirect_pass, GL_STREAM_DRAW);
    es3_functions.glDrawElementsIndirect(mode, type, 0);
    restore_state(elementbuffer);
}

// 注释

void glMultiDrawElementsBaseVertex(GLenum mode,
                                   const GLsizei *count,
                                   GLenum type,
                                   const void * const *indices,
                                   GLsizei drawcount,
                                   const GLint *basevertex) {
    if(!current_context) return;
    // 添加参数验证
    if(!count || !indices || !basevertex) {
        LTW_ERROR_PRINTF("LTW: NULL pointer passed to glMultiDrawElementsBaseVertex");
        return;
    }
    if(current_context->drawelementsbasevertex != NULL) {
        for(GLsizei i = 0; i < drawcount; i++) {
            current_context->drawelementsbasevertex(mode, count[i], type, indices[i], basevertex[i]);
        }
        return;
    }
    basevertex_renderer_t *renderer = &current_context->basevertex;
    if(!renderer->ready) return;
    GLint elementbuffer;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementbuffer);
    if(elementbuffer == 0) {
        // I am not bothered enough to implement this.
        LTW_ERROR_PRINTF("LTW: Base vertex draws without element buffer are not supported");
        return;
    }
    GLint typeBytes = type_bytes(type);
    if(typeBytes <= 0) {
        LTW_ERROR_PRINTF("LTW: unsupported type for multi base vertex draw");
        return;
    }
    // 检查 drawcount 大小，防止栈溢出和负数
    if(drawcount <= 0 || drawcount > 10000) {
        LTW_ERROR_PRINTF("LTW: invalid drawcount: %d", drawcount);
        return;
    }
    // 检查整数溢出
    if(drawcount > SIZE_MAX / sizeof(indirect_pass_t)) {
        LTW_ERROR_PRINTF("LTW: drawcount overflow: %d", drawcount);
        return;
    }
    size_t alloc_size = sizeof(indirect_pass_t) * drawcount;
    indirect_pass_t* indirect_passes = malloc(alloc_size);
    if(!indirect_passes) {
        LTW_ERROR_PRINTF("LTW: Failed to allocate memory for indirect passes");
        return;
    }
    for(GLsizei i = 0; i < drawcount; i++) {
        // 添加对单个 indices 数组的检查
        if(!indices[i]) {
            LTW_ERROR_PRINTF("LTW: NULL indices pointer at draw %i", i);
            free(indirect_passes);
            return;
        }
        uintptr_t indicesPointer = (uintptr_t)indices[i];
        if(indicesPointer % typeBytes != 0) {
            LTW_ERROR_PRINTF("LTW: misaligned base vertex draw not supported (draw %i)", i);
            free(indirect_passes);
            return;
        }
        indirect_pass_t* pass = &indirect_passes[i];
        pass->count = count[i];
        pass->firstIndex = indicesPointer / typeBytes;
        pass->baseVertex = basevertex[i];
        pass->instanceCount = 1;
        pass->reservedMustBeZero = 0;
    }
    es3_functions.glBindBuffer(GL_DRAW_INDIRECT_BUFFER, renderer->indirectRenderBuffer);
    es3_functions.glBufferData(GL_DRAW_INDIRECT_BUFFER, alloc_size, indirect_passes, GL_STREAM_DRAW);
    if(current_context->multidraw_indirect) {
        es3_functions.glMultiDrawElementsIndirectEXT(mode, type, 0, drawcount, 0);
    } else for(GLsizei i = 0; i < drawcount; i++) {
        es3_functions.glDrawElementsIndirect(mode, type, (void*)(sizeof(indirect_pass_t) * i));
    }
    free(indirect_passes);
    restore_state(elementbuffer);
}