/**
 * 固定管线（桌面 GL 1.x 兼容层）模拟。
 *
 * GLES 3.x 移除了全部固定管线：glBegin/glEnd 即时模式、矩阵栈、
 * 客户端顶点数组、GL_TEXTURE_2D 开关等。MC <=1.12（经 lwjglx）的
 * 界面/加载屏/部分绘制走固定管线：glBegin(GL_QUADS) +
 * glTexCoord2fv + glVertex3fv。这些调用在 GLES 上原本全部被
 * functionMissingAbort 丢弃导致黑屏。这里在 LTW 内部用一个默认
 * shader（MVP 矩阵 + 顶点色 + 纹理）模拟固定管线：
 *
 *  - 即时模式：glBegin 开始收集顶点，glVertex3fv/glTexCoord2fv/
 *    glColor4f 等追加，glEnd 时上传 VBO 并 glDrawArrays
 *  - 矩阵栈：MODELVIEW/PROJECTION/TEXTURE 矩阵跟踪，MVP = P * M
 *  - 绘制挂钩：main.c 的 glDrawArrays/glDrawElements 在无 program
 *    绑定时自动切换到默认 shader
 */
#ifndef LTW_FIXED_PIPELINE_H
#define LTW_FIXED_PIPELINE_H

#include <GLES3/gl3.h>
#include <stdint.h>
#include <stdbool.h>

// GLES3/gl3.h 不含 GLdouble（桌面 GL 类型）。仓库 GL/gl.h 定义了它；
// 若两者都包含会重复 typedef，这里用宏守卫保证只定义一次。
#ifndef GLdouble
typedef double GLdouble;
#endif

#define FP_MATRIX_SIZE 16

typedef enum {
    FP_MATRIX_MODELVIEW = 0,
    FP_MATRIX_PROJECTION = 1,
    FP_MATRIX_TEXTURE = 2,
    FP_MATRIX_COUNT = 3
} fp_matrix_mode_t;

// 初始化固定管线状态（context 创建时调用一次）
void fp_init(void);

// 矩阵栈
void fp_matrix_mode(GLenum mode);
void fp_load_identity(void);
void fp_load_matrixf(const GLfloat* m);
void fp_load_matrixd(const GLdouble* m);
void fp_mult_matrixf(const GLfloat* m);
void fp_mult_matrixd(const GLdouble* m);
void fp_push_matrix(void);
void fp_pop_matrix(void);
void fp_ortho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void fp_frustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void fp_translatef(GLfloat x, GLfloat y, GLfloat z);
void fp_translated(GLdouble x, GLdouble y, GLdouble z);
void fp_scalef(GLfloat x, GLfloat y, GLfloat z);
void fp_scaled(GLdouble x, GLdouble y, GLdouble z);
void fp_rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void fp_rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);

// 即时模式
void fp_begin(GLenum mode);
void fp_end(void);
void fp_vertex3fv(const GLfloat* v);
void fp_vertex3f(GLfloat x, GLfloat y, GLfloat z);
void fp_vertex3d(GLdouble x, GLdouble y, GLdouble z);
void fp_vertex2f(GLfloat x, GLfloat y);
void fp_vertex2d(GLdouble x, GLdouble y);
void fp_vertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void fp_vertex3iv(const GLint* v);
void fp_vertex3sv(const GLshort* v);
void fp_vertex4fv(const GLfloat* v);
void fp_vertex2fv(const GLfloat* v);
void fp_color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
void fp_color3f(GLfloat r, GLfloat g, GLfloat b);
void fp_color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
void fp_color3ub(GLubyte r, GLubyte g, GLubyte b);
void fp_color4fv(const GLfloat* v);
void fp_color3fv(const GLfloat* v);
void fp_color4ubv(const GLubyte* v);
void fp_texcoord2f(GLfloat s, GLfloat t);
void fp_texcoord2fv(const GLfloat* v);
void fp_texcoord1f(GLfloat s);
void fp_texcoord3f(GLfloat s, GLfloat t, GLfloat r);
void fp_texcoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void fp_texcoord2d(GLdouble s, GLdouble t);
// 显式写 unit0 的即时模式纹理坐标（glMultiTexCoord2f(GL_TEXTURE0,...) 用，
// 不受当前活动纹理单元影响）。
void fp_texcoord2f_raw(GLfloat s, GLfloat t);
void fp_texcoord3f_raw(GLfloat s, GLfloat t, GLfloat r);
void fp_texcoord4f_raw(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
void fp_normal3f(GLfloat x, GLfloat y, GLfloat z);
void fp_normal3fv(const GLfloat* v);

// 客户端顶点数组
void fp_vertex_pointer(GLint size, GLenum type, GLsizei stride, const void* pointer);
void fp_texcoord_pointer(GLint size, GLenum type, GLsizei stride, const void* pointer);
void fp_color_pointer(GLint size, GLenum type, GLsizei stride, const void* pointer);
void fp_normal_pointer(GLenum type, GLsizei stride, const void* pointer);
void fp_enable_client_state(GLenum cap);
void fp_disable_client_state(GLenum cap);
void fp_array_element(GLint i);
// glClientActiveTexture：记录客户端活动纹理单元，决定 glTexCoordPointer
// 写入的是哪个单元的纹理坐标（MC 1.12 区块用 unit1 存光照贴图坐标）。
void fp_set_client_active_texture(GLenum unit);

// 纹理状态
void fp_set_texture_enabled(bool enabled);
void fp_set_active_texture(GLuint unit);
// glBindTexture(GL_TEXTURE_2D) 后调用：若当前活动单元是 unit0，刷新固定管线
// 缓存的绑定纹理（避免 glBindTexture 后、绘制前被其他单元/路径覆盖）。
void fp_notify_texture_bind(void);

// alpha test 状态（MC 1.12 文字/透明渲染依赖）
void fp_set_alpha_test(bool enabled);
void fp_alpha_func(GLenum func, GLfloat ref);

// 无 program 时的绘制挂钩：返回 true 表示已用固定管线绘制
// 这些函数假定当前绑定的 VAO 已设置好 attribute 数组（应用自己的
// glVertexAttribPointer/glVertexPointer 已被 LTW 转换），只切换 program。
bool fp_try_draw_arrays(GLenum mode, GLint first, GLsizei count);
bool fp_try_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices);

// 绑定/解绑固定管线默认 program（供 quads 等内部绘制路径在无 program 时使用）
bool fp_bind_default_program(void);
void fp_unbind_default_program(void);

// 上传客户端数组到内部 VBO 并设置 attribute（在绘制前调用，count 为顶点数）。
// GLES 3.x 禁止客户端数组指针，MC 1.12 的 glVertexPointer 传 CPU 指针，
// 必须在绘制时把数据拷贝到 VBO。返回 true 表示已设置。
bool fp_prepare_client_arrays(GLsizei count);

// 查询默认 shader 当前是否可用
bool fp_default_program_ready(void);

// 查询固定管线矩阵（GL_MODELVIEW_MATRIX/GL_PROJECTION_MATRIX/GL_TEXTURE_MATRIX），
// 返回 true 表示 pname 是固定管线矩阵且已写入 out（16 floats）
bool fp_get_matrix(GLenum pname, GLfloat* out);

// ---- Display list support ----
// Minecraft <=1.12 renders entity models through glNewList/glCallList display
// lists, which do not exist on GLES. The fixed pipeline records the
// fixed-function calls at compile time (dl_capture_*) and replays them on
// glCallList (dl_call), reusing the current matrix/texture state.
bool fp_dl_capture_client_draw(GLenum mode, GLint first, GLsizei count,
                               bool indexed, GLenum itype, const void* indices);
void fp_dl_capture_bind_texture(GLenum target, GLuint texture);
void fp_dl_capture_texture_enable(bool enabled);
bool dl_is_compiling(void);
bool dl_capture_op(uint32_t type, const void* data, uint32_t size);
GLuint dl_gen(GLsizei range);
void dl_new(GLuint list, GLenum mode);
void dl_end(void);
void dl_delete(GLuint list, GLsizei range);
bool dl_is_list(GLuint list);
void dl_list_base(GLuint base);
void dl_call(GLuint list);
void dl_calls(GLsizei n, GLenum type, const void* lists);

#endif //LTW_FIXED_PIPELINE_H
