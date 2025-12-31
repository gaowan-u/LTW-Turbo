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
                es3_functions.glDrawArrays(mode, first[i], count[i]);
                return;
            }
        }
    }

    // 多个绘制调用：逐个执行（保持原有逻辑）
    for (int i = 0; i < primcount; i++) {
        if (count[i] > 0) {
            es3_functions.glDrawArrays(mode, first[i], count[i]);
        }
    }
}

void glMultiDrawElements( GLenum mode, GLsizei *count, GLenum type, const void * const *indices, GLsizei primcount )
{
    if(!current_context) return;
    if(primcount <= 0) return;

    GLint elementbuffer;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementbuffer);
    es3_functions.glBindBuffer(GL_COPY_WRITE_BUFFER, current_context->multidraw_element_buffer);

    GLsizei total = 0, typebytes = type_bytes(type);
    if(typebytes <= 0) {
        LTW_ERROR_PRINTF("LTW: unsupported type for multidraw");
        return;
    }

    // 第一遍：计算总大小并检查是否需要扩容
    for (GLsizei i = 0; i < primcount; i++) {
        total += count[i];
    }
    // 检查整数溢出
    if(total > INT_MAX / typebytes) {
        LTW_ERROR_PRINTF("LTW: multidraw size overflow");
        return;
    }
    GLsizei needed_size = total * typebytes;

    // 检查是否需要扩容（使用指数增长策略，但限制最大大小）
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
        es3_functions.glBufferData(GL_COPY_WRITE_BUFFER, new_size, NULL, GL_STREAM_DRAW);
        current_context->multidraw_buffer_size = new_size;
    }

    // 优化：根据数据源选择最佳填充策略
    if(elementbuffer != 0) {
        // 使用 glCopyBufferSubData 进行批量复制（GPU间传输，更快）
        GLsizei offset = 0;
        for (GLsizei i = 0; i < primcount; i++) {
            GLsizei icount = count[i];
            if(icount == 0) continue;
            icount *= typebytes;
            es3_functions.glCopyBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, (GLintptr)indices[i], offset, icount);
            offset += icount;
        }
    } else {
        // CPU数据：使用 glBufferSubData（一次性分配后批量填充）
        GLsizei offset = 0;
        for (GLsizei i = 0; i < primcount; i++) {
            GLsizei icount = count[i];
            if(icount == 0) continue;
            icount *= typebytes;
            es3_functions.glBufferSubData(GL_COPY_WRITE_BUFFER, offset, icount, indices[i]);
            offset += icount;
        }
    }

    // 绑定并绘制
    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_context->multidraw_element_buffer);
    es3_functions.glDrawElements(mode, total, type, 0);

    // 恢复原始绑定
    if(elementbuffer != 0) {
        es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    }
}