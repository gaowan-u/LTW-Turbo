/**
 * 文件功能：QUADS → TRIANGLES 转换与 EBO 影子副本接口声明。
 * GLES 3.x 不支持 GL_QUADS（mode=7），MC 1.12（lwjglx 兼容层）却大量使用。
 * 在基础版 glDrawArrays/glDrawElements 中拦截，把每 4 个顶点展开为 2 个三角形。
 */
#ifndef LTW_QUADS_H
#define LTW_QUADS_H

#include <GLES3/gl3.h>
#include <stdbool.h>
#include "egl.h"

// 尝试把 QUADS 绘制转换为 TRIANGLES。返回 true 表示已处理，false 表示不是 QUADS 模式。
bool ltw_quads_draw_arrays(GLenum mode, GLint first, GLsizei count);
bool ltw_quads_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices);

// EBO CPU 影子副本（QUADS 展开零同步）：glBufferData/glBufferSubData 时保留
// 索引副本，展开时直接读 CPU 内存，避免每帧 glMapBufferRange 造成 GPU 同步停顿。
void ltw_ebo_shadow_upload(GLenum target, GLsizeiptr size, const void* data, GLintptr offset, bool full);
void ltw_ebo_shadow_invalidate(GLuint buffer);
void ltw_ebo_shadow_destroy(context_t* ctx);

#endif //LTW_QUADS_H
