/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#include <proc.h>
#include <egl.h>
#include "basevertex.h"
#include "debug.h"
void glMultiDrawArrays( GLenum mode, GLint *first, GLsizei *count, GLsizei primcount )
{
    // 优化：跳过空绘制调用
    if(!current_context || primcount <= 0) return;

    // 统计非空绘制调用数量
    GLsizei valid_count = 0;
    for (int i = 0; i < primcount; i++) {
        if (count[i] > 0) valid_count++;
    }

    // 如果所有绘制都是空的，直接返回
    if (valid_count == 0) return;

    // 优化：如果只有一个有效绘制，直接调用 glDrawArrays
    if (valid_count == 1) {
        for (int i = 0; i < primcount; i++) {
            if (count[i] > 0) {
                current_context->fast_gl.glDrawArrays(mode, first[i], count[i]);
                return;
            }
        }
    }

    // 多个绘制调用：逐个执行（保持原有逻辑）
    for (int i = 0; i < primcount; i++) {
        if (count[i] > 0) {
            current_context->fast_gl.glDrawArrays(mode, first[i], count[i]);
        }
    }
}

void glMultiDrawElements( GLenum mode, GLsizei *count, GLenum type, const void * const *indices, GLsizei primcount )
{
    if(!current_context) return;
    if(primcount <= 0) return;

    GLint elementbuffer;
    current_context->fast_gl.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementbuffer);
    current_context->fast_gl.glBindBuffer(GL_COPY_WRITE_BUFFER, current_context->multidraw_element_buffer);

    GLsizei total = 0, typebytes = type_bytes(type);
    if(typebytes <= 0) {
        LTW_ERROR_PRINTF("LTW: unsupported type for multidraw");
        return;
    }

    // 第一遍：计算总大小
    for (GLsizei i = 0; i < primcount; i++) {
        total += count[i];
    }
    // 检查整数溢出
    if(total > INT_MAX / typebytes) {
        LTW_ERROR_PRINTF("LTW: multidraw size overflow");
        return;
    }
    GLsizei needed_size = total * typebytes;

    // 环形缓冲区逻辑：检查是否需要扩容
    if(needed_size > current_context->multidraw_buffer_size) {
        // 扩容到2倍或足够大小，但不超过 16MB
        GLsizei new_size = needed_size * 2;
        const GLsizei MAX_MULTIDRAW_BUFFER_SIZE = 16 * 1024 * 1024; // 16MB
        if(new_size > MAX_MULTIDRAW_BUFFER_SIZE) {
            new_size = needed_size;
            if(new_size > MAX_MULTIDRAW_BUFFER_SIZE) {
                LTW_ERROR_PRINTF("LTW: multidraw buffer size too large: %d", new_size);
                return;
            }
        }
        current_context->fast_gl.glBufferData(GL_COPY_WRITE_BUFFER, new_size, NULL, GL_STREAM_DRAW);
        current_context->multidraw_buffer_size = new_size;
        // 重置环形缓冲区
        current_context->multidraw_ring_head = 0;
        current_context->multidraw_ring_tail = 0;
        current_context->multidraw_ring_wrapped = false;
    } else {
        // 检查环形缓冲区是否有足够空间
        size_t available_space;
        if(!current_context->multidraw_ring_wrapped) {
            available_space = current_context->multidraw_buffer_size - current_context->multidraw_ring_head;
        } else {
            available_space = current_context->multidraw_ring_tail - current_context->multidraw_ring_head;
        }

        if(needed_size > available_space) {
            // 空间不足，重置到缓冲区开头
            current_context->multidraw_ring_head = 0;
            current_context->multidraw_ring_wrapped = false;
        }
    }

    // 计算写入偏移量
    size_t write_offset = current_context->multidraw_ring_head;

    // 优化：根据数据源选择最佳填充策略
    if(elementbuffer != 0) {
        // 使用 glCopyBufferSubData 进行批量复制（GPU间传输，更快）
        GLsizei offset = 0;
        for (GLsizei i = 0; i < primcount; i++) {
            GLsizei icount = count[i];
            if(icount == 0) continue;
            icount *= typebytes;
            current_context->fast_gl.glCopyBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, (GLintptr)indices[i], write_offset + offset, icount);
            offset += icount;
        }
    } else {
        // CPU数据：使用 glBufferSubData（一次性分配后批量填充）
        GLsizei offset = 0;
        for (GLsizei i = 0; i < primcount; i++) {
            GLsizei icount = count[i];
            if(icount == 0) continue;
            icount *= typebytes;
            current_context->fast_gl.glBufferSubData(GL_COPY_WRITE_BUFFER, write_offset + offset, icount, indices[i]);
            offset += icount;
        }
    }

    // 更新环形缓冲区头部
    current_context->multidraw_ring_head += needed_size;
    if(current_context->multidraw_ring_head >= current_context->multidraw_buffer_size) {
        current_context->multidraw_ring_head = 0;
        current_context->multidraw_ring_wrapped = true;
    }
    if(!current_context->multidraw_ring_wrapped) {
        current_context->multidraw_ring_tail = current_context->multidraw_ring_head;
    }

    // 绑定并绘制
    current_context->fast_gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_context->multidraw_element_buffer);
    current_context->fast_gl.glDrawElements(mode, total, type, (const void*)write_offset);

    // 恢复原始绑定
    if(elementbuffer != 0) {
        current_context->fast_gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    }
}