/**
 * 固定管线 GL 入口包装：glBegin/glEnd/glVertex 系列/glTexCoord 系列/
 * glColor 系列、矩阵栈函数等，全部转发到 fixed_pipeline.c 的模拟实现。
 * 这些函数在 GLES 上不存在，此前被 functionMissingAbort 静默丢弃。
 */
#include <stdio.h>
#include <GLES3/gl3.h>
#include "GL/gl.h"
#include "fixed_pipeline.h"
#include "egl.h"
#include "debug.h"

// ---- 矩阵栈 ----
void glMatrixMode(GLenum mode) {
    if(!current_context) return;
    fp_matrix_mode(mode);
}
void glLoadIdentity(void) {
    if(!current_context) return;
    fp_load_identity();
}
void glLoadMatrixf(const GLfloat* m) {
    if(!current_context) return;
    fp_load_matrixf(m);
}
void glLoadMatrixd(const GLdouble* m) {
    if(!current_context) return;
    fp_load_matrixd(m);
}
void glMultMatrixf(const GLfloat* m) {
    if(!current_context) return;
    fp_mult_matrixf(m);
}
void glMultMatrixd(const GLdouble* m) {
    if(!current_context) return;
    fp_mult_matrixd(m);
}
void glPushMatrix(void) {
    if(!current_context) return;
    fp_push_matrix();
}
void glPopMatrix(void) {
    if(!current_context) return;
    fp_pop_matrix();
}
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
    if(!current_context) return;
    fp_ortho(left, right, bottom, top, zNear, zFar);
}
void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
    if(!current_context) return;
    fp_frustum(left, right, bottom, top, zNear, zFar);
}
void glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_translatef(x, y, z);
}
void glTranslated(GLdouble x, GLdouble y, GLdouble z) {
    if(!current_context) return;
    fp_translated(x, y, z);
}
void glScalef(GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_scalef(x, y, z);
}
void glScaled(GLdouble x, GLdouble y, GLdouble z) {
    if(!current_context) return;
    fp_scaled(x, y, z);
}
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_rotatef(angle, x, y, z);
}
void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z) {
    if(!current_context) return;
    fp_rotated(angle, x, y, z);
}

// ---- 即时模式 ----
void glBegin(GLenum mode) {
    if(!current_context) return;
    fp_begin(mode);
}
void glEnd(void) {
    if(!current_context) return;
    fp_end();
}
void glVertex3fv(const GLfloat* v) {
    if(!current_context) return;
    fp_vertex3fv(v);
}
void glVertex3f(GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_vertex3f(x, y, z);
}
void glVertex3d(GLdouble x, GLdouble y, GLdouble z) {
    if(!current_context) return;
    fp_vertex3d(x, y, z);
}
void glVertex2f(GLfloat x, GLfloat y) {
    if(!current_context) return;
    fp_vertex2f(x, y);
}
void glVertex2d(GLdouble x, GLdouble y) {
    if(!current_context) return;
    fp_vertex2d(x, y);
}
void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if(!current_context) return;
    fp_vertex4f(x, y, z, w);
}
void glVertex3iv(const GLint* v) {
    if(!current_context) return;
    fp_vertex3iv(v);
}
void glVertex3sv(const GLshort* v) {
    if(!current_context) return;
    fp_vertex3sv(v);
}
void glVertex4fv(const GLfloat* v) {
    if(!current_context) return;
    fp_vertex4fv(v);
}
void glVertex2fv(const GLfloat* v) {
    if(!current_context) return;
    fp_vertex2fv(v);
}

// ---- 颜色 ----
void glColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    if(!current_context) return;
    fp_color4f(r, g, b, a);
}
void glColor3f(GLfloat r, GLfloat g, GLfloat b) {
    if(!current_context) return;
    fp_color3f(r, g, b);
}
void glColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    if(!current_context) return;
    fp_color4ub(r, g, b, a);
}
void glColor3ub(GLubyte r, GLubyte g, GLubyte b) {
    if(!current_context) return;
    fp_color3ub(r, g, b);
}
void glColor4fv(const GLfloat* v) {
    if(!current_context) return;
    fp_color4fv(v);
}
void glColor3fv(const GLfloat* v) {
    if(!current_context) return;
    fp_color3fv(v);
}
void glColor4ubv(const GLubyte* v) {
    if(!current_context) return;
    fp_color4ubv(v);
}

// ---- 纹理坐标 ----
void glTexCoord2f(GLfloat s, GLfloat t) {
    if(!current_context) return;
    fp_texcoord2f(s, t);
}
void glTexCoord2fv(const GLfloat* v) {
    if(!current_context) return;
    fp_texcoord2fv(v);
}
void glTexCoord1f(GLfloat s) {
    if(!current_context) return;
    fp_texcoord1f(s);
}
void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r) {
    if(!current_context) return;
    fp_texcoord3f(s, t, r);
}
void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
    if(!current_context) return;
    fp_texcoord4f(s, t, r, q);
}
void glTexCoord2d(GLdouble s, GLdouble t) {
    if(!current_context) return;
    fp_texcoord2d(s, t);
}

// ---- 多纹理坐标（直接指定单元）----
// LWJGL2 时代代码/老版本 MC 用 glMultiTexCoord2f(GL_TEXTURE1, ...) 设光照
// 贴图坐标。固定管线模拟只消费 unit0，unit1 忽略；实现这些入口同时避免
// LWJGL 因解析不到函数而刷 "No context is current"。
void glMultiTexCoord2f(GLenum texture, GLfloat s, GLfloat t) {
    if(!current_context) return;
    if(texture == GL_TEXTURE0) fp_texcoord2f_raw(s, t);
}
void glMultiTexCoord2fv(GLenum texture, const GLfloat* v) {
    if(!current_context || !v) return;
    if(texture == GL_TEXTURE0) fp_texcoord2f_raw(v[0], v[1]);
}
void glMultiTexCoord3f(GLenum texture, GLfloat s, GLfloat t, GLfloat r) {
    if(!current_context) return;
    if(texture == GL_TEXTURE0) fp_texcoord3f_raw(s, t, r);
}
void glMultiTexCoord4f(GLenum texture, GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
    if(!current_context) return;
    if(texture == GL_TEXTURE0) fp_texcoord4f_raw(s, t, r, q);
}
void glMultiTexCoord2fARB(GLenum texture, GLfloat s, GLfloat t) { glMultiTexCoord2f(texture, s, t); }
void glMultiTexCoord2fvARB(GLenum texture, const GLfloat* v) { glMultiTexCoord2fv(texture, v); }
void glMultiTexCoord3fARB(GLenum texture, GLfloat s, GLfloat t, GLfloat r) { glMultiTexCoord3f(texture, s, t, r); }
void glMultiTexCoord4fARB(GLenum texture, GLfloat s, GLfloat t, GLfloat r, GLfloat q) { glMultiTexCoord4f(texture, s, t, r, q); }

// ---- 法线 ----
void glNormal3f(GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_normal3f(x, y, z);
}
void glNormal3fv(const GLfloat* v) {
    if(!current_context) return;
    fp_normal3fv(v);
}

// ---- 客户端数组 ----
void glVertexPointer(GLint size, GLenum type, GLsizei stride, const void* pointer) {
    if(!current_context) return;
    fp_vertex_pointer(size, type, stride, pointer);
}
void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const void* pointer) {
    if(!current_context) return;
    fp_texcoord_pointer(size, type, stride, pointer);
}
void glColorPointer(GLint size, GLenum type, GLsizei stride, const void* pointer) {
    if(!current_context) return;
    fp_color_pointer(size, type, stride, pointer);
}
void glNormalPointer(GLenum type, GLsizei stride, const void* pointer) {
    if(!current_context) return;
    fp_normal_pointer(type, stride, pointer);
}
// glClientActiveTexture 决定 glTexCoordPointer / glEnableClientState(GL_TEXTURE_COORD_ARRAY)
// 作用于哪个纹理单元。GLES 没有此入口，桌面 MC 1.12 用它在 unit1 上设置
// 光照贴图坐标；这里记录单元选择，fixed_pipeline.c 只消费 unit0 的坐标。
void glClientActiveTexture(GLenum texture) {
    if(!current_context) return;
    fp_set_client_active_texture(texture);
}
void glClientActiveTextureARB(GLenum texture) {
    if(!current_context) return;
    fp_set_client_active_texture(texture);
}
void glEnableClientState(GLenum cap) {
    if(!current_context) return;
    fp_enable_client_state(cap);
}
void glDisableClientState(GLenum cap) {
    if(!current_context) return;
    fp_disable_client_state(cap);
}
void glArrayElement(GLint i) {
    if(!current_context) return;
    fp_array_element(i);
}

// ---- 其他固定管线函数（no-op 兜底，避免 functionMissingAbort）----
void glShadeModel(GLenum mode) {
    if(!current_context) return;
    (void)mode;
}
void glPushAttrib(GLbitfield mask) {
    if(!current_context) return;
    (void)mask;
}
void glPopAttrib(void) {
    if(!current_context) return;
}

// alpha test（MC 1.12 文字渲染依赖；GLES 无此功能，由默认 shader discard 模拟）
void glAlphaFunc(GLenum func, GLfloat ref) {
    if(!current_context) return;
    fp_alpha_func(func, ref);
}

// ---- 状态查询：固定管线矩阵走内部栈，其余透传 GLES ----
void glGetFloatv(GLenum pname, GLfloat* params) {
    if(!current_context) return;
    if(!fp_get_matrix(pname, params)) {
        es3_functions.glGetFloatv(pname, params);
    }
}
void glGetDoublev(GLenum pname, GLdouble* params) {
    if(!current_context || !params) return;
    GLfloat tmp[FP_MATRIX_SIZE];
    if(!fp_get_matrix(pname, tmp)) {
        es3_functions.glGetFloatv(pname, tmp);
    }
    // 之前直接把 float 位模式写进 double 缓冲区，值全是垃圾；必须逐元素转换。
    for(int i = 0; i < FP_MATRIX_SIZE; i++) params[i] = (GLdouble)tmp[i];
}
void glGetBooleanv(GLenum pname, GLboolean* params) {
    if(!current_context) return;
    GLint tmp;
    es3_functions.glGetIntegerv(pname, &tmp);
    *params = tmp ? GL_TRUE : GL_FALSE;
}
