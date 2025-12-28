/**
 * Created by: artDev
 * Copyright (c) 2025 artDev, SerpentSpirale, CADIndie.
 * For use under LGPL-3.0
 */

#include <proc.h>
#include <egl.h>
#include "basevertex.h"
void glMultiDrawArrays( GLenum mode, GLint *first, GLsizei *count, GLsizei primcount )
{
    // We'd need to merge each buffer attached to the VBO to properly achieve this. Nuh-uh. Aint no way im doin allat
    if(!current_context) return;
    for (int i = 0; i < primcount; i++) {
        if (count[i] > 0)
            es3_functions.glDrawArrays(mode, first[i], count[i]);
    }
}

void glMultiDrawElements( GLenum mode, GLsizei *count, GLenum type, const void * const *indices, GLsizei primcount )
{
    if(!current_context) return;
    GLint elementbuffer;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elementbuffer);
    es3_functions.glBindBuffer(GL_COPY_WRITE_BUFFER, current_context->multidraw_element_buffer);
    GLsizei total = 0, offset = 0, typebytes = type_bytes(type);
    for (GLsizei i = 0; i < primcount; i++) {
        total += count[i];
    }
    GLsizei needed_size = total * typebytes;

    // 检查是否需要扩容
    if(needed_size > current_context->multidraw_buffer_size) {
        // 扩容到2倍或足够大小
        GLsizei new_size = needed_size * 2;
        es3_functions.glBufferData(GL_COPY_WRITE_BUFFER, new_size, NULL, GL_STREAM_DRAW);
        current_context->multidraw_buffer_size = new_size;
    }

    // 使用 glBufferSubData 填充数据
    for (GLsizei i = 0; i < primcount; i++) {
        GLsizei icount = count[i];
        if(icount == 0) continue;
        icount *= typebytes;
        if(elementbuffer != 0) {
            es3_functions.glCopyBufferSubData(GL_ELEMENT_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, (GLintptr)indices[i], offset, icount);
        }else {
            es3_functions.glBufferSubData(GL_COPY_WRITE_BUFFER, offset, icount, indices[i]);
        }
        offset += icount;
    }
    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, current_context->multidraw_element_buffer);
    es3_functions.glDrawElements(mode, total, type, 0);
    if(elementbuffer != 0) es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

}