/**
 * 文件功能：固定管线桌面 GL API 入口包装。
 *
 * 把 glBegin/glEnd/glVertex 系列/glTexCoord 系列/glColor 系列、矩阵栈函数、
 * 显示列表（glGenLists/glNewList/glCallList 等）等桌面 API 全部转发到
 * fixed_pipeline.c 的模拟实现。这些函数在 GLES 上不存在，此前被
 * functionMissingAbort 静默丢弃。
 */
#include <stdio.h>
#include <GLES3/gl3.h>
#include "GL/gl.h"
#include "fixed_pipeline.h"
#include "egl.h"
#include "debug.h"

// ---- 矩阵栈 ----
// 任何矩阵变化都先冲刷批次：批次按录制时的 MVP 快照提交，矩阵一变
// 快照即失效。F3 段内矩阵不变，文字攒到段末 popMatrix 一次性提交
// （docs/f3-overlay-single-submit-plan.md 阶段 A）。
void glMatrixMode(GLenum mode) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_matrix_mode(mode);
}
void glLoadIdentity(void) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_load_identity();
}
void glLoadMatrixf(const GLfloat* m) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_load_matrixf(m);
}
void glLoadMatrixd(const GLdouble* m) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_load_matrixd(m);
}
void glMultMatrixf(const GLfloat* m) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_mult_matrixf(m);
}
void glMultMatrixd(const GLdouble* m) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_mult_matrixd(m);
}
void glPushMatrix(void) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_push_matrix();
}
void glPopMatrix(void) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_pop_matrix();
}
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_ortho(left, right, bottom, top, zNear, zFar);
}
void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_frustum(left, right, bottom, top, zNear, zFar);
}
void glTranslatef(GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_translatef(x, y, z);
}
void glTranslated(GLdouble x, GLdouble y, GLdouble z) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_translated(x, y, z);
}
void glScalef(GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_scalef(x, y, z);
}
void glScaled(GLdouble x, GLdouble y, GLdouble z) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_scaled(x, y, z);
}
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_rotatef(angle, x, y, z);
}
void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z) {
    if(!current_context) return;
    fp_flush_immediate_batch();
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
// MathCode: 1.12.2 实体渲染前 MC 调 setLightmapCoordinates → 多纹理坐标
// (GL_TEXTURE1, x, y) ——这是每个实体的真实光照，截获记录供 DL 回放使用。
float fp_multi_lm_uv[2] = {0.f, 0.f};
bool fp_multi_lm_valid = false;
// MathCode 2026-09-28 手持/掉落物亮度根因修复(#2)——"当前 lightmap 坐标"的
// 持久镜像（桌面固定管线语义：glMultiTexCoord2f(unit1) 是粘性状态，无 unit1
// 坐标的 ITEM 格式绘制沿用它）。与 fp_multi_lm_uv 的区别：本镜像不做一次性
// 消费（DL 回放不会清空它），仅供 Tessellator 物品路径读取，避免影响
// 天空/云的 mc2f 一次性消费修复（19c3d99）。
float fp_cur_lm_uv[2] = {0.f, 0.f};
bool fp_cur_lm_valid = false;
// MathCode 2026-10-04【GUI 满亮独立镜像】——根因修复：GuiInventory 的玩家
// 模型渲染会 setLightmap(玩家位置真实光照，夜间 (0,240))，把持久镜像里的
// GUI 满亮 (240,240) 覆盖掉 → 玩家模型之后的图标全部采样夜空暗行（图标
// 随天黑变暗，白天恰好看不出）。现把两者分存：GUI 镜像只收"unit1 disabled
// 窗口的 (240,240)"（mc2f_gui），世界光照永远不碰它；looks_item 消费时按
// fp_lightmap_enabled() 选镜像（GUI 段 disabled → 用 GUI 镜像）。
float fp_gui_lm_uv[2] = {240.f, 240.f};
bool fp_gui_lm_valid = true;
void glMultiTexCoord2f(GLenum texture, GLfloat s, GLfloat t) {
    if(!current_context) return;
    if(texture == GL_TEXTURE0) fp_texcoord2f_raw(s, t);
    else if(texture == GL_TEXTURE1) {
        // 值域过滤：光照坐标是 0-240 像素域（格×16）；OptiFine 在窗口内也会
        // 用 unit1 传非光照数据（如 61680=0xF0F0 的纹理动画坐标），取到会
        // UV 错位采样 lightmap 亮区=物品发亮。只接受合法像素域。
        bool valid_domain = (s >= 0.f && s <= 240.f && t >= 0.f && t <= 240.f);
        // MathCode 2026-09-28：GUIContainer.drawScreen 显式调用
        // setLightmapTextureCoords(unit1, 240, 240) 让 GUI 物品满亮度，此时
        // unit1 的 GL_TEXTURE_2D 处于 disabled（fp_lightmap_enabled()=false）。
        // 桌面语义：当前 lightmap 坐标是粘性状态，ITEM 绘制沿用它——
        // 所以 GUI 的 (240,240) 必须写进"持久镜像"，否则背包/物品栏物品会
        // 沿用世界的昼夜值而随白天/黑夜变暗（game5 实锤）。
        if(valid_domain && (fp_lightmap_enabled() || (s == 240.f && t == 240.f))) {
            // 持久镜像：供 Tessellator 物品路径（ITEM 格式）读取（含 GUI 满亮）
            fp_cur_lm_uv[0] = s; fp_cur_lm_uv[1] = t;
            fp_cur_lm_valid = true;
            // MathCode 2026-10-04【GUI 镜像】：unit1 disabled 窗口的 (240,240)
            // 是 GUI 满亮信号 → 写独立镜像。世界光照（enabled 窗口）永远
            // 不碰它——玩家模型 setLightmap(0,240) 不再污染 GUI 图标。
            if(s == 240.f && t == 240.f) {
                fp_gui_lm_uv[0] = s; fp_gui_lm_uv[1] = t;
                fp_gui_lm_valid = true;
            }
            // 一次性值（实体 DL 段用）：仍在 lightmap 启用窗口内取，且排除
            // (240,240)——那是 GUI 满亮复位信号，不是世界实体光照（沿用
            // a813652 的结论，避免影响实体昼夜亮度）。
            if(fp_lightmap_enabled() && !(s == 240.f && t == 240.f)) {
                fp_multi_lm_uv[0] = s; fp_multi_lm_uv[1] = t;
                fp_multi_lm_valid = true;
                if(ltw_lightmap_trace) {
                    static int lm_mc_cnt = 0;
                    if(lm_mc_cnt++ < 40 || (lm_mc_cnt & 1023) == 0)
                        LTW_ERROR_PRINTF("[LMT] mc2f s=%.3f t=%.3f", s, t);
                }
            } else if(ltw_lightmap_trace) {
                static int lm_mc_gui_cnt = 0;
                if(lm_mc_gui_cnt++ < 20)
                    LTW_ERROR_PRINTF("[LMT] mc2f_gui s=%.3f t=%.3f", s, t);
            }
        } else if(ltw_lightmap_trace) {
            static int lm_mc_junk_cnt = 0;
            if(lm_mc_junk_cnt++ < 20)
                LTW_ERROR_PRINTF("[LMT] mc2f_junk s=%.3f t=%.3f en=%d",
                                 s, t, fp_lightmap_enabled() ? 1 : 0);
        }
    }
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
    fp_flush_immediate_batch();
    fp_set_client_active_texture(texture);
}
void glClientActiveTextureARB(GLenum texture) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_set_client_active_texture(texture);
}
void glEnableClientState(GLenum cap) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_enable_client_state(cap);
}
void glDisableClientState(GLenum cap) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_disable_client_state(cap);
}
void glArrayElement(GLint i) {
    if(!current_context) return;
    fp_array_element(i);
}

// ---- 显示列表（display list）----
// GLES 无显示列表，由 fixed_pipeline.c 录制/回放（MC <=1.12 生物模型依赖）。
GLuint glGenLists(GLsizei range) {
    if(!current_context) return 0;
    return dl_gen(range);
}
void glNewList(GLuint list, GLenum mode) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    dl_new(list, mode);
}
void glEndList(void) {
    if(!current_context) return;
    dl_end();
}
void glCallList(GLuint list) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    dl_call(list);
}
void glCallLists(GLsizei n, GLenum type, const void* lists) {
    if(!current_context || n <= 0 || !lists) return;
    fp_flush_immediate_batch();
    dl_calls(n, type, lists);
}
void glDeleteLists(GLuint list, GLsizei range) {
    if(!current_context) return;
    dl_delete(list, range);
}
GLboolean glIsList(GLuint list) {
    if(!current_context) return GL_FALSE;
    return dl_is_list(list) ? GL_TRUE : GL_FALSE;
}
void glListBase(GLuint base) {
    if(!current_context) return;
    dl_list_base(base);
}

// ---- 其他固定管线函数（no-op 兜底，避免 functionMissingAbort）----
void glShadeModel(GLenum mode) {
    if(!current_context) return;
    (void)mode;
}

// MathCode: 纹理环境（桌面 GL_TEXTURE_ENV；GLES 无对应物，2026-08 新增）
// MC 1.12 生物受伤红闪依赖 RenderLivingBase.setBrightness 在光照贴图单元
// 上设置 GL_COMBINE/GL_INTERPOLATE + GL_TEXTURE_ENV_COLOR=(1,0,0,0.3)。
// 状态由固定管线记录，默认 shader 在绘制时模拟，不透传宿主机。
void glTexEnvi(GLenum target, GLenum pname, GLint param) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_texenv(target, pname, NULL, &param, true);
}
void glTexEnvf(GLenum target, GLenum pname, GLfloat param) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_texenv(target, pname, &param, NULL, false);
}
void glTexEnvfv(GLenum target, GLenum pname, const GLfloat* params) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_texenv(target, pname, params, NULL, false);
}
void glTexEnviv(GLenum target, GLenum pname, const GLint* params) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_texenv(target, pname, NULL, params, true);
}
void glTexEnv(GLenum target, GLenum pname, const GLfloat* params) {
    if(!current_context) return;
    fp_flush_immediate_batch();
    fp_texenv(target, pname, params, NULL, false);
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
    fp_flush_immediate_batch();
    fp_alpha_func(func, ref);
}

// ---- 状态查询：固定管线矩阵走内部栈，其余透传 GLES ----
void glGetFloatv(GLenum pname, GLfloat* params) {
    if(!current_context) return;
    if(!fp_get_matrix(pname, params)) {
        es3_functions.glGetFloatv(pname, params);
    }
}
// glGetDoublev 每次写入的元素个数：矩阵 16，范围类 2，向量类 4，其余按标量。
// 不能对非矩阵查询固定写 16 个 double，会越界写坏调用方缓冲区。
static int fp_doublev_count(GLenum pname) {
    switch(pname) {
        case GL_DEPTH_RANGE:
        case GL_ALIASED_POINT_SIZE_RANGE:
        case GL_ALIASED_LINE_WIDTH_RANGE:
        // 注意：GL_POINT_SIZE_RANGE 与 GL_SMOOTH_POINT_SIZE_RANGE、GL_LINE_WIDTH_RANGE
        // 与 GL_SMOOTH_LINE_WIDTH_RANGE 值相同（0x0B12 / 0x0B22），不能同时出现。
        case GL_LINE_WIDTH_RANGE:
        case GL_POINT_SIZE_RANGE:
            return 2;
        case GL_BLEND_COLOR:
        case GL_COLOR_CLEAR_VALUE:
        case GL_VIEWPORT:
        case GL_SCISSOR_BOX:
            return 4;
        default:
            return 1;
    }
}
void glGetDoublev(GLenum pname, GLdouble* params) {
    if(!current_context || !params) return;
    GLfloat tmp[FP_MATRIX_SIZE];
    bool is_matrix = fp_get_matrix(pname, tmp);
    if(!is_matrix) {
        es3_functions.glGetFloatv(pname, tmp);
    }
    // 之前直接把 float 位模式写进 double 缓冲区，值全是垃圾；必须逐元素转换。
    // 同时按查询类型写正确数量的元素，避免固定写 16 个 double 越界。
    int count = is_matrix ? FP_MATRIX_SIZE : fp_doublev_count(pname);
    for(int i = 0; i < count; i++) params[i] = (GLdouble)tmp[i];
}
void glGetBooleanv(GLenum pname, GLboolean* params) {
    if(!current_context) return;
    GLint tmp;
    es3_functions.glGetIntegerv(pname, &tmp);
    *params = tmp ? GL_TRUE : GL_FALSE;
}
