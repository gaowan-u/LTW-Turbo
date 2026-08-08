/**
 * 固定管线模拟实现，见 fixed_pipeline.h。
 *
 * 设计要点：
 *  - 默认 shader 懒加载（首次绘制时），GLSL 300 es
 *  - 即时模式顶点收集到 CPU 缓冲，glEnd 时一次性上传
 *  - 矩阵列主序（与桌面 GL 一致），MVP = Projection * ModelView
 *  - 客户端数组模式下顶点由应用侧指针提供，绘制时绑定
 *  - 所有状态都是 context 无关的进程级状态（固定管线只有一个）
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include <GLES3/gl3.h>
#include "GL/gl.h"
#include "fixed_pipeline.h"
#include "egl.h"
#include "debug.h"

// ---- 内部常量 ----
#define FP_MAX_STACK_DEPTH 32
#define FP_MAX_VERTICES 65536
#define FP_STRIDE (3 + 4 + 2)          // pos3 + color4 + uv2
#define FP_VERTEX_BYTES (FP_STRIDE * sizeof(GLfloat))

#define FP_ATTR_POS 0
#define FP_ATTR_COLOR 1
#define FP_ATTR_UV 2

// ---- 显示列表（Display List）支持 ----
// Minecraft <=1.12 的生物模型通过 glNewList/glCallList 编译回放，
// GLES 没有显示列表，这里在编译期录制固定管线调用、glCallList 时回放。

// op 类型
#define DL_OP_IMMEDIATE      1   // glBegin/glEnd 顶点快照
#define DL_OP_CLIENT_DRAW    2   // 客户端数组 glDrawArrays/glDrawElements
#define DL_OP_BIND_TEXTURE   3   // 列表内 glBindTexture(GL_TEXTURE_2D)
#define DL_OP_TEXTURE_ENABLE 4   // 列表内 glEnable/glDisable(GL_TEXTURE_2D)
#define DL_OP_CALL_LIST      5   // 列表内 glCallList（嵌套）

#define DL_MAX_LISTS      (1u << 16)            // id 上限（含 0 号占位）
#define DL_MAX_COMPILE    8                     // 嵌套编译深度
#define DL_MAX_EXEC_DEPTH 32                    // 递归执行深度
#define DL_OP_CAP         (8u * 1024u * 1024u)  // 单 op 数据上限
#define DL_LIST_CAP       (16u * 1024u * 1024u) // 单列表数据上限
#define DL_TOTAL_CAP      (64u * 1024u * 1024u) // 全部列表数据上限

// 载荷布局（头 + 可变长数据，数据区 8 字节对齐）
typedef struct {
    GLenum mode;
    uint32_t count;
    uint32_t verts_off;    // 顶点数据偏移（pos3+color4+uv2 交错，FP_STRIDE）
} dl_immediate_payload_t;

// 客户端数组快照（abo == 0：CPU 指针，数据已拷贝，*_off 相对顶点基址；
// abo != 0：VBO 路径，*_off 为 buffer 内字节偏移）
typedef struct {
    bool vertex_enabled;
    bool texcoord_enabled;
    bool color_enabled;
    bool normal_enabled;
    GLint vertex_size;
    GLenum vertex_type;
    GLsizei vertex_stride;
    GLint texcoord_size;
    GLenum texcoord_type;
    GLsizei texcoord_stride;
    GLint color_size;
    GLenum color_type;
    GLsizei color_stride;
    GLenum normal_type;
    GLsizei normal_stride;
    GLint vertex_abo;
    GLint texcoord_abo;
    GLint color_abo;
    GLintptr vertex_off;
    GLintptr texcoord_off;
    GLintptr color_off;
    GLintptr normal_off;
} fp_dl_client_snapshot_t;

typedef struct {
    fp_dl_client_snapshot_t snap;
    GLenum mode;
    GLint first;
    GLsizei count;
    bool indexed;
    GLenum itype;          // indexed 时有效
    GLuint indices_ebo;    // 录制时的源 EBO（0=客户端索引）
    GLintptr indices_src_off; // 客户端索引: 0；EBO: 字节偏移
    uint32_t vertex_len;   // CPU 顶点数据长度（0=VBO 偏移路径）
    uint32_t indices_len;  // CPU 索引数据长度
    uint32_t vertex_off;   // 顶点数据区偏移（相对 payload 起始）
    uint32_t indices_off;  // 索引数据区偏移（相对 payload 起始）
} dl_client_draw_payload_t;

typedef struct {
    GLenum target;
    GLuint texture;
    GLenum unit;           // 录制时的活动纹理单元
} dl_bind_texture_payload_t;

typedef struct {
    GLuint enabled;        // 0/1
} dl_texture_enable_payload_t;

typedef struct {
    GLuint list;
} dl_call_list_payload_t;

// 列表存储
typedef struct {
    uint32_t type;
    uint32_t size;         // 数据字节数
    uint8_t* data;
} dl_op_entry_t;

typedef struct {
    uint32_t id;
    bool allocated;        // 由 glGenLists 保留
    bool compiled;         // glNewList..glEndList 完成
    bool failed;           // 录制内存不足，整表丢弃
    bool pending_delete;   // 执行中被删除，执行结束后释放
    uint32_t exec_count;   // 正在执行的引用计数
    uint32_t op_count;
    uint32_t op_cap;
    dl_op_entry_t* ops;
    size_t bytes;          // 该列表数据总字节
} fp_dl_list_t;

static fp_dl_list_t** dl_table = NULL;   // 下标 = id
static uint32_t dl_table_cap = 0;
static size_t dl_total_bytes = 0;
static uint32_t dl_next_hint = 1;     // dl_gen 扫描起始点

// 编译栈（支持防御性嵌套）
static GLuint dl_compile_stack[DL_MAX_COMPILE];
static GLenum dl_compile_modes[DL_MAX_COMPILE];
static int dl_compile_depth = 0;

// 执行链（防递归）
static GLuint dl_exec_chain[DL_MAX_EXEC_DEPTH];
static int dl_exec_depth = 0;

static GLuint dl_list_base_id = 0;

static void dl_execute_list(GLuint list);

// ---- 默认 shader ----
static const char* fp_vertex_shader_src =
    "#version 300 es\n"
    "layout(location=0) in vec4 aPos;\n"
    "layout(location=1) in vec4 aColor;\n"
    "layout(location=2) in vec2 aUV;\n"
    "uniform mat4 uMVP;\n"
    "out vec4 vColor;\n"
    "out vec2 vUV;\n"
    "void main() {\n"
    "    vColor = aColor;\n"
    "    vUV = aUV;\n"
    "    gl_Position = uMVP * aPos;\n"
    "}\n";

static const char* fp_fragment_shader_src =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec4 vColor;\n"
    "in vec2 vUV;\n"
    "uniform sampler2D uTex;\n"
    "uniform bool uUseTex;\n"
    "uniform bool uUseColor;\n"
    "uniform vec4 uColor;\n"
    "uniform bool uSingle;\n"
    "uniform int uAlphaFunc;\n"
    "uniform float uAlphaRef;\n"
    "out vec4 fragColor;\n"
    "void main() {\n"
    "    vec4 c = vec4(1.0);\n"
    "    if(uUseTex) {\n"
    "        vec4 t = texture(uTex, vUV);\n"
    "        if(uSingle) c = vec4(t.r, t.r, t.r, t.r);\n"
    "        else c = t;\n"
    "    }\n"
    "    vec4 vc = uUseColor ? vColor : uColor;\n"
    "    vec4 fc = c * vc;\n"
    "    bool atpass = true;\n"
    "    if(uAlphaFunc == 0) atpass = false;\n"
    "    else if(uAlphaFunc == 1) atpass = fc.a < uAlphaRef;\n"
    "    else if(uAlphaFunc == 2) atpass = abs(fc.a - uAlphaRef) < 0.0039;\n"
    "    else if(uAlphaFunc == 3) atpass = fc.a <= uAlphaRef;\n"
    "    else if(uAlphaFunc == 4) atpass = fc.a > uAlphaRef;\n"
    "    else if(uAlphaFunc == 5) atpass = abs(fc.a - uAlphaRef) >= 0.0039;\n"
    "    else if(uAlphaFunc == 6) atpass = fc.a >= uAlphaRef;\n"
    "    if(!atpass) discard;\n"
    "    fragColor = fc;\n"
    "}\n";

// ---- 内部状态 ----
static GLuint fp_program = 0;
static GLint fp_mvp_loc = -1;
static GLint fp_tex_loc = -1;
static GLint fp_usetex_loc = -1;
static GLint fp_usecolor_loc = -1;
static GLint fp_color_loc = -1;
static GLint fp_alphafunc_loc = -1;
static GLint fp_alpharef_loc = -1;
static GLint fp_single_loc = -1;
static GLuint fp_vbo = 0;
static GLuint fp_vbo_pos = 0;
static GLuint fp_vbo_color = 0;
static GLuint fp_vbo_uv = 0;static GLuint fp_vao = 0;
// 显示列表回放客户端索引时使用的 scratch EBO（随 context 重建）
static GLuint fp_index_ebo = 0;
static GLsizeiptr fp_index_ebo_cap = 0;
static GLuint fp_saved_vao = 0;
static GLint fp_saved_active_tex = GL_TEXTURE0;
static GLint fp_saved_bound_tex = 0;
static bool fp_saved_texture_valid = false;
static bool fp_init_done = false;

// alpha test 状态（MC 1.12 文字渲染依赖 GL_ALPHA_TEST）
static bool fp_alpha_test = false;
static GLenum fp_alpha_test_func = 0x0207;   // GL_ALWAYS
static GLfloat fp_alpha_ref = 0.0f;
// 客户端颜色数组是否真实启用（决定 shader 用顶点色还是当前色）
static bool fp_client_color_active = false;
// glClientActiveTexture 选中的客户端活动纹理单元。MC 1.12 的
// VertexBuffer.setupBufferState 会用 unit1 设置光照贴图坐标
// （glTexCoordPointer(2, GL_SHORT, 32, 24)），若不去重会覆盖 unit0 的
// 真实 UV（2 floats, offset 16），导致所有方块面采样到错误图集位置。
static GLenum fp_client_active_texture = GL_TEXTURE0;
// glActiveTexture 选中的活动纹理单元。固定管线模拟只消费 unit0 的纹理和
// 即时模式纹理坐标（unit1 是光照贴图，由默认 shader 忽略）。
static GLenum fp_active_texture = GL_TEXTURE0;
// 绑定的纹理是否为单通道（GL_ALPHA 映射 GL_R8），shader 走 uSingle 分支
static bool fp_bound_single_channel = false;

// 矩阵栈
static GLfloat fp_matrix_stack[FP_MATRIX_COUNT][FP_MAX_STACK_DEPTH][FP_MATRIX_SIZE];
static GLint fp_matrix_top[FP_MATRIX_COUNT] = {0, 0, 0};
static fp_matrix_mode_t fp_current_matrix = FP_MATRIX_MODELVIEW;

// 即时模式
static bool fp_immediate_active = false;
static GLenum fp_immediate_mode = 0;
static GLfloat* fp_immediate_vertices = NULL;
static GLsizei fp_immediate_count = 0;
static GLsizei fp_immediate_capacity = 0;
static GLfloat fp_current_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat fp_current_texcoord[4] = {0.0f, 0.0f, 0.0f, 1.0f};

// 客户端数组
static GLint fp_client_vertex_size = 0;
static GLenum fp_client_vertex_type = GL_FLOAT;
static GLsizei fp_client_vertex_stride = 0;
static const void* fp_client_vertex_ptr = NULL;
static GLint fp_client_texcoord_size = 0;
static GLenum fp_client_texcoord_type = GL_FLOAT;
static GLsizei fp_client_texcoord_stride = 0;
static const void* fp_client_texcoord_ptr = NULL;
static GLint fp_client_color_size = 0;
static GLenum fp_client_color_type = GL_FLOAT;
static GLsizei fp_client_color_stride = 0;
static const void* fp_client_color_ptr = NULL;
static GLenum fp_client_normal_type = GL_FLOAT;
static GLsizei fp_client_normal_stride = 0;
static const void* fp_client_normal_ptr = NULL;
static bool fp_client_vertex_enabled = false;
static bool fp_client_texcoord_enabled = false;
static bool fp_client_color_enabled = false;
static bool fp_client_normal_enabled = false;

// 纹理状态
static bool fp_texture_enabled = false;
static GLuint fp_bound_texture = 0;

// 前向声明
static void fp_set_default_uniforms(void);
static void fp_refresh_bound_texture(void);

// ---- 内部工具 ----
static void fp_mat_mul(GLfloat* out, const GLfloat* a, const GLfloat* b) {
    // out = a * b（列主序 4x4）
    GLfloat t[FP_MATRIX_SIZE];
    for(int c = 0; c < 4; c++) {
        for(int r = 0; r < 4; r++) {
            t[c * 4 + r] =
                a[0 * 4 + r] * b[c * 4 + 0] +
                a[1 * 4 + r] * b[c * 4 + 1] +
                a[2 * 4 + r] * b[c * 4 + 2] +
                a[3 * 4 + r] * b[c * 4 + 3];
        }
    }
    memcpy(out, t, sizeof(t));
}

static void fp_mat_identity(GLfloat* m) {
    memset(m, 0, FP_MATRIX_SIZE * sizeof(GLfloat));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

static void fp_mat_ortho(GLfloat* m, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f) {
    fp_mat_identity(m);
    if(r == l || t == b || f == n) return;
    m[0]  = (GLfloat)(2.0 / (r - l));
    m[5]  = (GLfloat)(2.0 / (t - b));
    m[10] = (GLfloat)(-2.0 / (f - n));
    m[12] = (GLfloat)(-(r + l) / (r - l));
    m[13] = (GLfloat)(-(t + b) / (t - b));
    m[14] = (GLfloat)(-(f + n) / (f - n));
}

static void fp_mat_frustum(GLfloat* m, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f) {
    fp_mat_identity(m);
    if(r == l || t == b || f == n) return;
    m[0]  = (GLfloat)(2.0 * n / (r - l));
    m[5]  = (GLfloat)(2.0 * n / (t - b));
    m[8]  = (GLfloat)((r + l) / (r - l));
    m[9]  = (GLfloat)((t + b) / (t - b));
    m[10] = (GLfloat)(-(f + n) / (f - n));
    m[11] = -1.0f;
    m[14] = (GLfloat)(-2.0 * f * n / (f - n));
}

static GLfloat* fp_current_matrix_ptr(void) {
    return fp_matrix_stack[fp_current_matrix][fp_matrix_top[fp_current_matrix]];
}

static void fp_apply_translate(GLfloat x, GLfloat y, GLfloat z) {
    GLfloat t[FP_MATRIX_SIZE];
    fp_mat_identity(t);
    t[12] = x; t[13] = y; t[14] = z;
    GLfloat m[FP_MATRIX_SIZE];
    memcpy(m, fp_current_matrix_ptr(), sizeof(m));
    fp_mat_mul(fp_current_matrix_ptr(), m, t);
}

static void fp_apply_scale(GLfloat x, GLfloat y, GLfloat z) {
    GLfloat t[FP_MATRIX_SIZE];
    fp_mat_identity(t);
    t[0] = x; t[5] = y; t[10] = z;
    GLfloat m[FP_MATRIX_SIZE];
    memcpy(m, fp_current_matrix_ptr(), sizeof(m));
    fp_mat_mul(fp_current_matrix_ptr(), m, t);
}

static void fp_apply_rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) {
    GLfloat rad = angle * (GLfloat)3.14159265358979 / 180.0f;
    GLfloat c = (GLfloat)cos(rad);
    GLfloat s = (GLfloat)sin(rad);
    GLfloat len = (GLfloat)sqrt(x * x + y * y + z * z);
    if(len == 0.0f) return;
    x /= len; y /= len; z /= len;
    GLfloat t[FP_MATRIX_SIZE];
    t[0]  = x * x * (1 - c) + c;     t[4]  = x * y * (1 - c) - z * s; t[8]  = x * z * (1 - c) + y * s; t[12] = 0;
    t[1]  = y * x * (1 - c) + z * s; t[5]  = y * y * (1 - c) + c;     t[9]  = y * z * (1 - c) - x * s; t[13] = 0;
    t[2]  = x * z * (1 - c) - y * s; t[6]  = y * z * (1 - c) + x * s; t[10] = z * z * (1 - c) + c;     t[14] = 0;
    t[3] = 0; t[7] = 0; t[11] = 0; t[15] = 1;
    GLfloat m[FP_MATRIX_SIZE];
    memcpy(m, fp_current_matrix_ptr(), sizeof(m));
    fp_mat_mul(fp_current_matrix_ptr(), m, t);
}

// 即时模式顶点追加
static void fp_immediate_push(GLfloat x, GLfloat y, GLfloat z) {
    if(!fp_immediate_active) return;
    if(fp_immediate_count >= fp_immediate_capacity) {
        GLsizei newcap = fp_immediate_capacity == 0 ? 1024 : fp_immediate_capacity * 2;
        if(newcap > FP_MAX_VERTICES) newcap = FP_MAX_VERTICES;
        if(newcap <= fp_immediate_capacity) return;   // 已到上限，丢弃
        GLfloat* nb = (GLfloat*)realloc(fp_immediate_vertices, (size_t)newcap * FP_VERTEX_BYTES);
        if(!nb) return;
        fp_immediate_vertices = nb;
        fp_immediate_capacity = newcap;
    }
    GLfloat* v = fp_immediate_vertices + (size_t)fp_immediate_count * FP_STRIDE;
    v[0] = x; v[1] = y; v[2] = z;
    v[3] = fp_current_color[0];
    v[4] = fp_current_color[1];
    v[5] = fp_current_color[2];
    v[6] = fp_current_color[3];
    v[7] = fp_current_texcoord[0];
    v[8] = fp_current_texcoord[1];
    fp_immediate_count++;
}

// 编译默认 shader（懒加载）
static void fp_ensure_program(void) {
    if(fp_init_done) return;
    fp_init_done = true;
    if(!current_context) return;

    GLuint vs = es3_functions.glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = es3_functions.glCreateShader(GL_FRAGMENT_SHADER);
    if(!vs || !fs) { es3_functions.glDeleteShader(vs); es3_functions.glDeleteShader(fs); return; }

    es3_functions.glShaderSource(vs, 1, &fp_vertex_shader_src, NULL);
    es3_functions.glCompileShader(vs);
    GLint ok = 0;
    es3_functions.glGetShaderiv(vs, GL_COMPILE_STATUS, &ok);
    if(!ok) {
        GLchar log[512] = {0};
        es3_functions.glGetShaderInfoLog(vs, sizeof(log), NULL, log);
        LTW_ERROR_PRINTF("fp: vertex shader compile failed: %s", log);
        es3_functions.glDeleteShader(vs);
        es3_functions.glDeleteShader(fs);
        return;
    }

    es3_functions.glShaderSource(fs, 1, &fp_fragment_shader_src, NULL);
    es3_functions.glCompileShader(fs);
    es3_functions.glGetShaderiv(fs, GL_COMPILE_STATUS, &ok);
    if(!ok) {
        GLchar log[512] = {0};
        es3_functions.glGetShaderInfoLog(fs, sizeof(log), NULL, log);
        LTW_ERROR_PRINTF("fp: fragment shader compile failed: %s", log);
        es3_functions.glDeleteShader(vs);
        es3_functions.glDeleteShader(fs);
        return;
    }

    GLuint prog = es3_functions.glCreateProgram();
    es3_functions.glAttachShader(prog, vs);
    es3_functions.glAttachShader(prog, fs);
    es3_functions.glLinkProgram(prog);
    es3_functions.glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if(!ok) {
        GLchar log[512] = {0};
        es3_functions.glGetProgramInfoLog(prog, sizeof(log), NULL, log);
        LTW_ERROR_PRINTF("fp: program link failed: %s", log);
        es3_functions.glDeleteProgram(prog);
        es3_functions.glDeleteShader(vs);
        es3_functions.glDeleteShader(fs);
        return;
    }

    fp_program = prog;
    fp_mvp_loc = es3_functions.glGetUniformLocation(prog, "uMVP");
    fp_tex_loc = es3_functions.glGetUniformLocation(prog, "uTex");
    fp_usetex_loc = es3_functions.glGetUniformLocation(prog, "uUseTex");
    fp_usecolor_loc = es3_functions.glGetUniformLocation(prog, "uUseColor");
    fp_alphafunc_loc = es3_functions.glGetUniformLocation(prog, "uAlphaFunc");
    fp_alpharef_loc = es3_functions.glGetUniformLocation(prog, "uAlphaRef");
    fp_single_loc = es3_functions.glGetUniformLocation(prog, "uSingle");
    fp_color_loc = es3_functions.glGetUniformLocation(prog, "uColor");

    es3_functions.glGenBuffers(1, &fp_vbo);
    es3_functions.glGenBuffers(1, &fp_vbo_pos);
    es3_functions.glGenBuffers(1, &fp_vbo_color);
    es3_functions.glGenBuffers(1, &fp_vbo_uv);
    es3_functions.glGenVertexArrays(1, &fp_vao);
    es3_functions.glGenBuffers(1, &fp_index_ebo);

    es3_functions.glDeleteShader(vs);
    es3_functions.glDeleteShader(fs);
}

// 提交即时模式顶点
static void fp_flush_immediate(void) {
    if(!fp_immediate_active || fp_immediate_count == 0) return;
    fp_ensure_program();
    if(!fp_program) { fp_immediate_count = 0; return; }

    // 用画这一刻的真实绑定刷新状态，不吃旧记忆值。
    fp_refresh_bound_texture();

    GLenum mode = fp_immediate_mode;
    GLsizei count = fp_immediate_count;

    // 先保存应用绑定状态，并把 fp_vbo 绑定为当前 ARRAY_BUFFER，
    // glBufferData 操作的是"当前绑定的 buffer"，必须先绑定。
    GLint old_vao = 0;
    GLint old_array_buffer = 0;
    GLint old_program = 0;
    es3_functions.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old_vao);
    es3_functions.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_array_buffer);
    es3_functions.glGetIntegerv(GL_CURRENT_PROGRAM, &old_program);
    es3_functions.glBindBuffer(GL_ARRAY_BUFFER, fp_vbo);

    // QUADS -> TRIANGLES：复制顶点展开
    if(mode == GL_QUADS && count >= 4 && (count & 3) == 0) {
        GLsizei quads = count >> 2;
        GLsizei tri_count = quads * 6;
        if(tri_count > FP_MAX_VERTICES) {
            es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_array_buffer);
            fp_immediate_count = 0;
            return;
        }
        GLfloat* expanded = (GLfloat*)malloc((size_t)tri_count * FP_VERTEX_BYTES);
        if(!expanded) {
            es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_array_buffer);
            fp_immediate_count = 0;
            return;
        }
        for(GLsizei q = 0; q < quads; q++) {
            GLfloat* a = fp_immediate_vertices + (size_t)(q * 4 + 0) * FP_STRIDE;
            GLfloat* b = fp_immediate_vertices + (size_t)(q * 4 + 1) * FP_STRIDE;
            GLfloat* c = fp_immediate_vertices + (size_t)(q * 4 + 2) * FP_STRIDE;
            GLfloat* d = fp_immediate_vertices + (size_t)(q * 4 + 3) * FP_STRIDE;
            GLfloat* t = expanded + (size_t)(q * 6) * FP_STRIDE;
            memcpy(t + 0 * FP_STRIDE, a, FP_VERTEX_BYTES);
            memcpy(t + 1 * FP_STRIDE, b, FP_VERTEX_BYTES);
            memcpy(t + 2 * FP_STRIDE, c, FP_VERTEX_BYTES);
            memcpy(t + 3 * FP_STRIDE, a, FP_VERTEX_BYTES);
            memcpy(t + 4 * FP_STRIDE, c, FP_VERTEX_BYTES);
            memcpy(t + 5 * FP_STRIDE, d, FP_VERTEX_BYTES);
        }
        es3_functions.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)tri_count * FP_VERTEX_BYTES, expanded, GL_STREAM_DRAW);
        free(expanded);
        count = tri_count;
        mode = GL_TRIANGLES;
    } else {
        es3_functions.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)fp_immediate_count * FP_VERTEX_BYTES,
                                   fp_immediate_vertices, GL_STREAM_DRAW);
    }

    // 绑定默认 program + 属性（使用私有 VAO，避免污染应用绑定的 VAO）
    es3_functions.glBindVertexArray(fp_vao);
    es3_functions.glUseProgram(fp_program);
    fp_set_default_uniforms();
    es3_functions.glEnableVertexAttribArray(FP_ATTR_POS);
    es3_functions.glEnableVertexAttribArray(FP_ATTR_COLOR);
    es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
    es3_functions.glVertexAttribPointer(FP_ATTR_POS, 3, GL_FLOAT, GL_FALSE, FP_VERTEX_BYTES, NULL);
    es3_functions.glVertexAttribPointer(FP_ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, FP_VERTEX_BYTES,
                                        (const void*)(3 * sizeof(GLfloat)));
    es3_functions.glVertexAttribPointer(FP_ATTR_UV, 2, GL_FLOAT, GL_FALSE, FP_VERTEX_BYTES,
                                        (const void*)(7 * sizeof(GLfloat)));

    // 纹理绑定到 unit 0
    GLint old_active_tex = 0;
    GLint old_bound_tex = 0;
    if(fp_bound_texture != 0) {
        es3_functions.glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active_tex);
        es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_bound_tex);
        es3_functions.glActiveTexture(GL_TEXTURE0);
        es3_functions.glBindTexture(GL_TEXTURE_2D, fp_bound_texture);
    }

    es3_functions.glDrawArrays(mode, 0, count);

    // 恢复状态
    if(fp_bound_texture != 0) {
        es3_functions.glBindTexture(GL_TEXTURE_2D, (GLuint)old_bound_tex);
        es3_functions.glActiveTexture((GLenum)old_active_tex);
    }
    es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_array_buffer);
    es3_functions.glBindVertexArray((GLuint)old_vao);
    if(old_program != (GLint)fp_program) es3_functions.glUseProgram((GLuint)old_program);

    fp_immediate_count = 0;
}

// ---- 矩阵 API ----
void fp_init(void) {
    // 上下文重建时（FCL 偶尔会重建 EGL context），GL 对象是 context 私有的，
    // 必须重置句柄并让 fp_ensure_program 在新 context 里重建。
    fp_init_done = false;
    fp_program = 0;
    fp_mvp_loc = fp_tex_loc = fp_usetex_loc = fp_usecolor_loc = -1;
    fp_color_loc = fp_alphafunc_loc = fp_alpharef_loc = fp_single_loc = -1;
    fp_vbo = fp_vbo_pos = fp_vbo_color = fp_vbo_uv = fp_vao = 0;
    fp_index_ebo = 0;
    fp_index_ebo_cap = 0;
    fp_saved_vao = 0;
    fp_saved_texture_valid = false;
    for(int m = 0; m < FP_MATRIX_COUNT; m++) {
        fp_matrix_top[m] = 0;
        fp_mat_identity(fp_matrix_stack[m][0]);
    }
    fp_current_matrix = FP_MATRIX_MODELVIEW;
    fp_immediate_active = false;
    fp_immediate_count = 0;
    fp_immediate_capacity = 0;
    free(fp_immediate_vertices); // 上一 context 遗留的缓冲一并释放，避免泄漏
    fp_immediate_vertices = NULL;
    fp_current_color[0] = fp_current_color[1] = fp_current_color[2] = fp_current_color[3] = 1.0f;
    fp_current_texcoord[0] = fp_current_texcoord[1] = 0.0f;
    fp_current_texcoord[2] = 0.0f; fp_current_texcoord[3] = 1.0f;
    fp_client_vertex_enabled = fp_client_texcoord_enabled = fp_client_color_enabled = fp_client_normal_enabled = false;
    fp_client_active_texture = GL_TEXTURE0;
    fp_active_texture = GL_TEXTURE0;
    fp_texture_enabled = false;
    fp_bound_texture = 0;
}

void fp_matrix_mode(GLenum mode) {
    switch(mode) {
        case GL_MODELVIEW:  fp_current_matrix = FP_MATRIX_MODELVIEW;  break;
        case GL_PROJECTION: fp_current_matrix = FP_MATRIX_PROJECTION; break;
        case GL_TEXTURE:    fp_current_matrix = FP_MATRIX_TEXTURE;    break;
        default: break;
    }
}

void fp_load_identity(void) {
    fp_mat_identity(fp_current_matrix_ptr());
}

void fp_load_matrixf(const GLfloat* m) {
    if(m) memcpy(fp_current_matrix_ptr(), m, FP_MATRIX_SIZE * sizeof(GLfloat));
}

void fp_load_matrixd(const GLdouble* m) {
    if(!m) return;
    GLfloat* dst = fp_current_matrix_ptr();
    for(int i = 0; i < FP_MATRIX_SIZE; i++) dst[i] = (GLfloat)m[i];
}

void fp_mult_matrixf(const GLfloat* m) {
    if(!m) return;
    GLfloat cur[FP_MATRIX_SIZE];
    memcpy(cur, fp_current_matrix_ptr(), sizeof(cur));
    fp_mat_mul(fp_current_matrix_ptr(), cur, m);
}

void fp_mult_matrixd(const GLdouble* m) {
    if(!m) return;
    GLfloat mm[FP_MATRIX_SIZE];
    for(int i = 0; i < FP_MATRIX_SIZE; i++) mm[i] = (GLfloat)m[i];
    fp_mult_matrixf(mm);
}

void fp_push_matrix(void) {
    GLint* top = &fp_matrix_top[fp_current_matrix];
    if(*top >= FP_MAX_STACK_DEPTH - 1) return;
    memcpy(fp_matrix_stack[fp_current_matrix][*top + 1], fp_matrix_stack[fp_current_matrix][*top],
           FP_MATRIX_SIZE * sizeof(GLfloat));
    (*top)++;
}

void fp_pop_matrix(void) {
    GLint* top = &fp_matrix_top[fp_current_matrix];
    if(*top <= 0) return;
    (*top)--;
}

void fp_ortho(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f) {
    fp_mat_ortho(fp_current_matrix_ptr(), l, r, b, t, n, f);
}

void fp_frustum(GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f) {
    fp_mat_frustum(fp_current_matrix_ptr(), l, r, b, t, n, f);
}

void fp_translatef(GLfloat x, GLfloat y, GLfloat z) { fp_apply_translate(x, y, z); }
void fp_translated(GLdouble x, GLdouble y, GLdouble z) { fp_apply_translate((GLfloat)x, (GLfloat)y, (GLfloat)z); }
void fp_scalef(GLfloat x, GLfloat y, GLfloat z) { fp_apply_scale(x, y, z); }
void fp_scaled(GLdouble x, GLdouble y, GLdouble z) { fp_apply_scale((GLfloat)x, (GLfloat)y, (GLfloat)z); }
void fp_rotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) { fp_apply_rotate(angle, x, y, z); }
void fp_rotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z) { fp_apply_rotate((GLfloat)angle, (GLfloat)x, (GLfloat)y, (GLfloat)z); }

// ---- 即时模式 ----
void fp_begin(GLenum mode) {
    fp_immediate_mode = mode;
    fp_immediate_active = true;
    fp_immediate_count = 0;
}

void fp_end(void) {
    if(!fp_immediate_active) return;
    if(dl_is_compiling() && fp_immediate_count > 0) {
        // 显示列表编译期间：把顶点快照录进列表，编译期不真正绘制
        if((GLsizei)fp_immediate_count <= FP_MAX_VERTICES) {
            size_t pl_size = sizeof(dl_immediate_payload_t);
            size_t vbytes = (size_t)fp_immediate_count * FP_VERTEX_BYTES;
            size_t total = pl_size + vbytes;
            if(total <= DL_OP_CAP) {
                uint8_t* buf = (uint8_t*)malloc(total);
                if(buf) {
                    dl_immediate_payload_t* pl = (dl_immediate_payload_t*)buf;
                    pl->mode = fp_immediate_mode;
                    pl->count = (uint32_t)fp_immediate_count;
                    pl->verts_off = (uint32_t)((pl_size + 7) & ~(size_t)7);
                    memcpy(buf + pl->verts_off, fp_immediate_vertices, vbytes);
                    dl_capture_op(DL_OP_IMMEDIATE, buf, (uint32_t)total);
                    free(buf);
                }
            }
        }
    } else {
        fp_flush_immediate();
    }
    fp_immediate_active = false;
}

void fp_vertex3fv(const GLfloat* v) { if(v) fp_immediate_push(v[0], v[1], v[2]); }
void fp_vertex3f(GLfloat x, GLfloat y, GLfloat z) { fp_immediate_push(x, y, z); }
void fp_vertex3d(GLdouble x, GLdouble y, GLdouble z) { fp_immediate_push((GLfloat)x, (GLfloat)y, (GLfloat)z); }
void fp_vertex2f(GLfloat x, GLfloat y) { fp_immediate_push(x, y, 0.0f); }
void fp_vertex2d(GLdouble x, GLdouble y) { fp_immediate_push((GLfloat)x, (GLfloat)y, 0.0f); }
void fp_vertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    if(w != 0.0f) fp_immediate_push(x / w, y / w, z / w);
}
void fp_vertex3iv(const GLint* v) { if(v) fp_immediate_push((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2]); }
void fp_vertex3sv(const GLshort* v) { if(v) fp_immediate_push((GLfloat)v[0], (GLfloat)v[1], (GLfloat)v[2]); }
void fp_vertex4fv(const GLfloat* v) { if(v) fp_vertex4f(v[0], v[1], v[2], v[3]); }
void fp_vertex2fv(const GLfloat* v) { if(v) fp_immediate_push(v[0], v[1], 0.0f); }

void fp_color4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    fp_current_color[0] = r; fp_current_color[1] = g; fp_current_color[2] = b; fp_current_color[3] = a;
}
void fp_color3f(GLfloat r, GLfloat g, GLfloat b) { fp_color4f(r, g, b, 1.0f); }
void fp_color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    fp_color4f(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
void fp_color3ub(GLubyte r, GLubyte g, GLubyte b) { fp_color4ub(r, g, b, 255); }
void fp_color4fv(const GLfloat* v) { if(v) fp_color4f(v[0], v[1], v[2], v[3]); }
void fp_color3fv(const GLfloat* v) { if(v) fp_color3f(v[0], v[1], v[2]); }
void fp_color4ubv(const GLubyte* v) { if(v) fp_color4ub(v[0], v[1], v[2], v[3]); }

void fp_texcoord2f_raw(GLfloat s, GLfloat t) {
    fp_current_texcoord[0] = s; fp_current_texcoord[1] = t;
}
void fp_texcoord3f_raw(GLfloat s, GLfloat t, GLfloat r) {
    fp_current_texcoord[0] = s; fp_current_texcoord[1] = t; fp_current_texcoord[2] = r;
}
void fp_texcoord4f_raw(GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
    fp_current_texcoord[0] = s; fp_current_texcoord[1] = t;
    fp_current_texcoord[2] = r; fp_current_texcoord[3] = q;
}
void fp_texcoord2f(GLfloat s, GLfloat t) {
    // unit1 的即时模式纹理坐标（MC 1.12 光照贴图）不覆盖 unit0 的记录。
    if(fp_active_texture != GL_TEXTURE0) return;
    fp_texcoord2f_raw(s, t);
}
void fp_texcoord2fv(const GLfloat* v) { if(v) fp_texcoord2f(v[0], v[1]); }
void fp_texcoord1f(GLfloat s) { fp_texcoord2f(s, 0.0f); }
void fp_texcoord3f(GLfloat s, GLfloat t, GLfloat r) {
    if(fp_active_texture != GL_TEXTURE0) return;
    fp_texcoord3f_raw(s, t, r);
}
void fp_texcoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q) {
    if(fp_active_texture != GL_TEXTURE0) return;
    fp_texcoord4f_raw(s, t, r, q);
}
void fp_texcoord2d(GLdouble s, GLdouble t) { fp_texcoord2f((GLfloat)s, (GLfloat)t); }

void fp_normal3f(GLfloat x, GLfloat y, GLfloat z) { (void)x; (void)y; (void)z; /* 光照模拟未实现 */ }
void fp_normal3fv(const GLfloat* v) { if(v) fp_normal3f(v[0], v[1], v[2]); }

// ---- 客户端数组 ----
// 记录各数组设置时的 ARRAY_BUFFER 绑定状态：设置时未绑定 buffer（abo==0）
// 说明 pointer 是客户端 CPU 指针（GLES 禁止，绘制时需拷贝到 VBO）；
// 已绑定（abo!=0）说明 pointer 是 VBO 偏移（可直通）。
static GLint fp_client_vertex_abo = 0;
static GLint fp_client_texcoord_abo = 0;
static GLint fp_client_color_abo = 0;

static GLint fp_current_abo(void) {
    GLint abo = 0;
    es3_functions.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &abo);
    return abo;
}

void fp_vertex_pointer(GLint size, GLenum type, GLsizei stride, const void* pointer) {
    fp_client_vertex_size = size; fp_client_vertex_type = type;
    fp_client_vertex_stride = stride; fp_client_vertex_ptr = pointer;
    fp_client_vertex_abo = fp_current_abo();
}
void fp_texcoord_pointer(GLint size, GLenum type, GLsizei stride, const void* pointer) {
    // 固定管线模拟只消费 unit0 的纹理坐标；unit1（MC 1.12 光照贴图）忽略，
    // 且不能让它覆盖 unit0 的记录。
    if(fp_client_active_texture != GL_TEXTURE0) return;
    fp_client_texcoord_size = size; fp_client_texcoord_type = type;
    fp_client_texcoord_stride = stride; fp_client_texcoord_ptr = pointer;
    fp_client_texcoord_abo = fp_current_abo();
}
void fp_set_client_active_texture(GLenum unit) {
    fp_client_active_texture = unit;
}
void fp_color_pointer(GLint size, GLenum type, GLsizei stride, const void* pointer) {
    fp_client_color_size = size; fp_client_color_type = type;
    fp_client_color_stride = stride; fp_client_color_ptr = pointer;
    fp_client_color_abo = fp_current_abo();
}
void fp_normal_pointer(GLenum type, GLsizei stride, const void* pointer) {
    fp_client_normal_type = type; fp_client_normal_stride = stride; fp_client_normal_ptr = pointer;
}
void fp_enable_client_state(GLenum cap) {
    switch(cap) {
        case GL_VERTEX_ARRAY:  fp_client_vertex_enabled = true;   break;
        case GL_TEXTURE_COORD_ARRAY:
            if(fp_client_active_texture == GL_TEXTURE0) fp_client_texcoord_enabled = true;
            break;
        case GL_COLOR_ARRAY:   fp_client_color_enabled = true;    break;
        case GL_NORMAL_ARRAY:  fp_client_normal_enabled = true;   break;
        default: break;
    }
}
void fp_disable_client_state(GLenum cap) {
    switch(cap) {
        case GL_VERTEX_ARRAY:  fp_client_vertex_enabled = false;  break;
        case GL_TEXTURE_COORD_ARRAY:
            if(fp_client_active_texture == GL_TEXTURE0) fp_client_texcoord_enabled = false;
            break;
        case GL_COLOR_ARRAY:   fp_client_color_enabled = false;   break;
        case GL_NORMAL_ARRAY:  fp_client_normal_enabled = false;  break;
        default: break;
    }
}
void fp_array_element(GLint i) {
    // 从客户端数组取第 i 个顶点（未实现，客户端数组路径在 MC 1.12 中很少用）
    (void)i;
}

// ---- 纹理状态 ----
void fp_set_texture_enabled(bool enabled) { fp_texture_enabled = enabled; }
void fp_set_active_texture(GLuint unit) {
    fp_active_texture = unit;
    if(unit == GL_TEXTURE0) {
        fp_refresh_bound_texture();
    }
}

void fp_notify_texture_bind(void) {
    if(fp_active_texture == GL_TEXTURE0) fp_refresh_bound_texture();
}

// ---- alpha test ----
void fp_set_alpha_test(bool enabled) { fp_alpha_test = enabled; }
void fp_alpha_func(GLenum func, GLfloat ref) { fp_alpha_test_func = func; fp_alpha_ref = ref; }

// ---- 绘制挂钩 ----
// 画之前直接查“当前绑定在活动纹理单元上的 2D 纹理”，而不是依赖
// glBindTexture 包装器里的记忆值。MC 的 GlStateManager 会缓存纹理绑定，
// 连续绘制同一字形时可能根本不调用 glBindTexture，记忆值一旦被其他
// 单元/路径覆盖，字就会以“无纹理”的纯色方块画出来。
static void fp_refresh_bound_texture(void) {
    if(!current_context) return;
    // GL_TEXTURE_BINDING_2D 查询的是“当前活动单元”的绑定；固定管线始终用
    // unit0 绘制，这里临时切到 unit0 再查询，避免活动单元是 unit1 时
    // 把光照贴图当成方块纹理。
    GLint old_active = 0;
    es3_functions.glGetIntegerv(GL_ACTIVE_TEXTURE, &old_active);
    if(old_active != GL_TEXTURE0) es3_functions.glActiveTexture(GL_TEXTURE0);
    GLint tex = 0;
    es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
    fp_bound_texture = (GLuint)tex;
    fp_bound_single_channel = false;
    if(tex != 0) {
        GLint fmt = 0;
        es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);
        if(fmt == GL_R8 || fmt == GL_RED || fmt == GL_R16F || fmt == GL_R32F) {
            fp_bound_single_channel = true;
        }
    }
    if(old_active != GL_TEXTURE0) es3_functions.glActiveTexture((GLenum)old_active);
}

// 设置默认 program 的 uniforms（MVP + 纹理开关）。uUseTex 由绑定纹理决定：
// GLES 纹理始终启用，绑定过纹理就采样，否则用顶点色。
static void fp_set_default_uniforms(void) {
    GLfloat mvp[FP_MATRIX_SIZE];
    fp_mat_mul(mvp, fp_matrix_stack[FP_MATRIX_PROJECTION][fp_matrix_top[FP_MATRIX_PROJECTION]],
               fp_matrix_stack[FP_MATRIX_MODELVIEW][fp_matrix_top[FP_MATRIX_MODELVIEW]]);
    if(fp_mvp_loc >= 0) es3_functions.glUniformMatrix4fv(fp_mvp_loc, 1, GL_FALSE, mvp);
    if(fp_tex_loc >= 0) es3_functions.glUniform1i(fp_tex_loc, 0);
    if(fp_usetex_loc >= 0) es3_functions.glUniform1i(fp_usetex_loc, fp_bound_texture ? 1 : 0);
    // 有顶点色用顶点色，否则用当前色（glColor4f 状态）——固定管线语义
    if(fp_usecolor_loc >= 0) es3_functions.glUniform1i(fp_usecolor_loc, fp_immediate_active ? 1 : (fp_client_color_active ? 1 : 0));
    if(fp_color_loc >= 0) es3_functions.glUniform4fv(fp_color_loc, 1, fp_current_color);
    if(fp_single_loc >= 0) es3_functions.glUniform1i(fp_single_loc, fp_bound_single_channel ? 1 : 0);
    // shader 里用 0..7 表示 GL_NEVER..GL_ALWAYS，不能直接传原始 GLenum
    // （例如 GL_GREATER=516），否则所有分支都匹配不上、alpha test 失效。
    GLint alpha_mode = 7;
    if(fp_alpha_test) {
        switch(fp_alpha_test_func) {
            case GL_NEVER:    alpha_mode = 0; break;
            case GL_LESS:     alpha_mode = 1; break;
            case GL_EQUAL:    alpha_mode = 2; break;
            case GL_LEQUAL:   alpha_mode = 3; break;
            case GL_GREATER:  alpha_mode = 4; break;
            case GL_NOTEQUAL: alpha_mode = 5; break;
            case GL_GEQUAL:   alpha_mode = 6; break;
            case GL_ALWAYS:   alpha_mode = 7; break;
            default:          alpha_mode = 7; break;
        }
    }
    if(fp_alphafunc_loc >= 0) es3_functions.glUniform1i(fp_alphafunc_loc, alpha_mode);
    if(fp_alpharef_loc >= 0) es3_functions.glUniform1f(fp_alpharef_loc, fp_alpha_test ? fp_alpha_ref : 0.0f);
}

// 绑定默认 program。返回 true 表示成功（调用方须配对调用 fp_unbind_default_program）。
// 若应用通过固定管线 API（glVertexPointer 等）提供了客户端数组/VBO 偏移，
// 这里把它们设置成 attribute；否则用即时模式缓冲/默认值。
bool fp_bind_default_program(void) {
    fp_ensure_program();
    if(!fp_program) return false;
    fp_refresh_bound_texture();
    es3_functions.glUseProgram(fp_program);
    fp_set_default_uniforms();

    // 使用私有 VAO，避免污染应用绑定的 VAO 的 attribute 状态
    es3_functions.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&fp_saved_vao);
    if(fp_vao) es3_functions.glBindVertexArray(fp_vao);

    fp_saved_texture_valid = false;
    if(fp_bound_texture != 0) {
        fp_saved_texture_valid = true;
        es3_functions.glGetIntegerv(GL_ACTIVE_TEXTURE, &fp_saved_active_tex);
        // 先切到 unit0 再查询绑定，否则保存的是其他单元的纹理，恢复时会写错单元。
        es3_functions.glActiveTexture(GL_TEXTURE0);
        es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &fp_saved_bound_tex);
        es3_functions.glBindTexture(GL_TEXTURE_2D, fp_bound_texture);
    }
    return true;
}

// GLES 类型字节数
static size_t fp_type_bytes(GLenum type) {
    switch(type) {
        case GL_FLOAT: return 4;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT: return 2;
        case GL_BYTE:
        case GL_UNSIGNED_BYTE: return 1;
        case GL_INT:
        case GL_UNSIGNED_INT: return 4;
        case GL_DOUBLE: return 8;
        default: return 4;
    }
}

// 把客户端交错数组上传到单个 VBO，并按各数组指针相对顶点指针的偏移
// 设置 attribute（MC 1.12 的 Tessellator 用交错布局：stride 内依次是
// 位置+纹理+颜色，glColorPointer/glTexCoordPointer 的指针指向块内偏移）。
static void fp_upload_client_arrays(GLsizei count) {
    if(!fp_client_vertex_enabled || fp_client_vertex_size <= 0 || !fp_client_vertex_ptr) return;
    GLint old_abo = 0;
    es3_functions.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_abo);

    size_t tsize = fp_type_bytes(fp_client_vertex_type);
    size_t vsize = fp_client_vertex_stride ? (size_t)fp_client_vertex_stride
                                           : (size_t)fp_client_vertex_size * tsize;
    if(vsize == 0 || (size_t)count > (size_t)FP_MAX_VERTICES) return;

    // 一次上传整个交错数组
    es3_functions.glBindBuffer(GL_ARRAY_BUFFER, fp_vbo);
    es3_functions.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vsize * count, fp_client_vertex_ptr, GL_STREAM_DRAW);

    // 位置 attribute：offset 0
    es3_functions.glEnableVertexAttribArray(FP_ATTR_POS);
    es3_functions.glVertexAttribPointer(FP_ATTR_POS, fp_client_vertex_size, fp_client_vertex_type,
                                        GL_FALSE, fp_client_vertex_stride, NULL);

    // 纹理 attribute：偏移 = texcoord 指针 - 顶点指针（交错布局内）
    if(fp_client_texcoord_enabled && fp_client_texcoord_size > 0 && fp_client_texcoord_ptr) {
        ptrdiff_t off = (const uint8_t*)fp_client_texcoord_ptr - (const uint8_t*)fp_client_vertex_ptr;
        if(off >= 0 && (size_t)off < vsize) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
            es3_functions.glVertexAttribPointer(FP_ATTR_UV, fp_client_texcoord_size, fp_client_texcoord_type,
                                                GL_FALSE, fp_client_vertex_stride, (const void*)off);
        } else {
            es3_functions.glDisableVertexAttribArray(FP_ATTR_UV);
        }
    } else {
        es3_functions.glDisableVertexAttribArray(FP_ATTR_UV);
    }
    // 颜色 attribute：偏移 = color 指针 - 顶点指针
    fp_client_color_active = (fp_client_color_enabled && fp_client_color_size > 0 && fp_client_color_ptr);
    if(fp_client_color_active) {
        ptrdiff_t off = (const uint8_t*)fp_client_color_ptr - (const uint8_t*)fp_client_vertex_ptr;
        if(off >= 0 && (size_t)off < vsize) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_COLOR);
            es3_functions.glVertexAttribPointer(FP_ATTR_COLOR, fp_client_color_size, fp_client_color_type,
                                                GL_TRUE, fp_client_vertex_stride, (const void*)off);
        } else {
            fp_client_color_active = false;
            es3_functions.glDisableVertexAttribArray(FP_ATTR_COLOR);
        }
    } else {
        es3_functions.glDisableVertexAttribArray(FP_ATTR_COLOR);
    }

    es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
}

bool fp_prepare_client_arrays(GLsizei count) {
    if(!fp_program) fp_ensure_program();
    if(!fp_program) return false;

    // GLES 3.x 禁止客户端数组指针。用数组"设置时"的 ARRAY_BUFFER 绑定判断：
    // 设置时未绑定（pointer 是 CPU 地址）→ 拷贝到 fp_vbo；设置时已绑定
    // （pointer 是 VBO 偏移）→ 直通。不能用绘制时的当前绑定判断（应用
    // 可能在 glVertexPointer 之后绑定/解绑了 ARRAY_BUFFER 做别的事）。
    if(fp_client_vertex_abo == 0) {
        fp_upload_client_arrays(count);
    } else {
        // VBO 路径：pointer 是偏移，直通（必须先绑定对应的 VBO，偏移才有效）。
        // 应用在 glVertexPointer 时绑定了 VBO（fp_client_*_abo），绘制时当前
        // ARRAY_BUFFER 可能已换成别的 buffer，这里按各自 abo 重新绑定。
        GLint old_abo = 0;
        es3_functions.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old_abo);
        GLint vbo = fp_client_vertex_abo ? fp_client_vertex_abo : old_abo;
        if(vbo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)vbo);
        if(fp_client_vertex_enabled && fp_client_vertex_size > 0) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_POS);
            es3_functions.glVertexAttribPointer(FP_ATTR_POS, fp_client_vertex_size, fp_client_vertex_type,
                                                GL_FALSE, fp_client_vertex_stride, fp_client_vertex_ptr);
        }
        if(vbo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
        GLint cbo = fp_client_color_abo ? fp_client_color_abo : old_abo;
        if(cbo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)cbo);
        if(fp_client_color_enabled && fp_client_color_size > 0) {
            fp_client_color_active = true;
            es3_functions.glEnableVertexAttribArray(FP_ATTR_COLOR);
            es3_functions.glVertexAttribPointer(FP_ATTR_COLOR, fp_client_color_size, fp_client_color_type,
                                                GL_TRUE, fp_client_color_stride, fp_client_color_ptr);
        } else {
            fp_client_color_active = false;
            es3_functions.glDisableVertexAttribArray(FP_ATTR_COLOR);
        }
        if(cbo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
        GLint ubo = fp_client_texcoord_abo ? fp_client_texcoord_abo : old_abo;
        if(ubo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)ubo);
        if(fp_client_texcoord_enabled && fp_client_texcoord_size > 0) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
            es3_functions.glVertexAttribPointer(FP_ATTR_UV, fp_client_texcoord_size, fp_client_texcoord_type,
                                                GL_FALSE, fp_client_texcoord_stride, fp_client_texcoord_ptr);
        } else {
            es3_functions.glDisableVertexAttribArray(FP_ATTR_UV);
        }
        if(ubo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
    }
    // attribute 启用情况影响 uUseColor，这里重设 uniforms（bind 先于 prepare）
    fp_set_default_uniforms();
    return true;
}

void fp_unbind_default_program(void) {
    if(!fp_program) return;
    if(fp_saved_texture_valid) {
        // 恢复必须先把活动单元切回 unit0 再绑回原纹理，最后恢复活动单元，
        // 否则会把 unit0 的绑定写到别的单元上。
        es3_functions.glActiveTexture(GL_TEXTURE0);
        es3_functions.glBindTexture(GL_TEXTURE_2D, (GLuint)fp_saved_bound_tex);
        es3_functions.glActiveTexture((GLenum)fp_saved_active_tex);
        fp_saved_texture_valid = false;
    }
    if(fp_saved_vao != 0) es3_functions.glBindVertexArray(fp_saved_vao);
    es3_functions.glUseProgram(0);
}

// 无 program 时用默认 shader 绘制。应用已设置好 VAO attribute（MC 1.12
// 的 Tessellator 用 glVertexAttribPointer + VBO），这里只切换 program。
bool fp_try_draw_arrays(GLenum mode, GLint first, GLsizei count) {
    if(!current_context) return false;
    GLint prog = 0;
    es3_functions.glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    if(prog != 0) return false;
    if(!fp_bind_default_program()) return false;
    fp_prepare_client_arrays(count);
    es3_functions.glDrawArrays(mode, first, count);
    fp_unbind_default_program();
    return true;
}

bool fp_try_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if(!current_context) return false;
    GLint prog = 0;
    es3_functions.glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
    if(prog != 0) return false;
    // GL_ELEMENT_ARRAY_BUFFER 绑定属于 VAO：必须在切到私有 fp_vao 之前
    // 记录应用的 EBO（可能为 0=客户端索引），切过去后重新绑定，否则
    // indices 会被当作 fp_vao 里残留 EBO 的偏移，画出垃圾几何。
    GLint eab = 0;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);
    if(!fp_bind_default_program()) return false;
    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)eab);
    fp_prepare_client_arrays(count);
    es3_functions.glDrawElements(mode, count, type, indices);
    fp_unbind_default_program();
    return true;
}

bool fp_default_program_ready(void) {
    return fp_program != 0;
}

bool fp_get_matrix(GLenum pname, GLfloat* out) {
    if(!out) return false;
    fp_matrix_mode_t mode;
    switch(pname) {
        case GL_MODELVIEW_MATRIX:  mode = FP_MATRIX_MODELVIEW;  break;
        case GL_PROJECTION_MATRIX: mode = FP_MATRIX_PROJECTION; break;
        case GL_TEXTURE_MATRIX:    mode = FP_MATRIX_TEXTURE;    break;
        default: return false;
    }
    memcpy(out, fp_matrix_stack[mode][fp_matrix_top[mode]], FP_MATRIX_SIZE * sizeof(GLfloat));
    return true;
}

// ---- Display list capture / replay ----

// 计算子指针相对顶点基址的偏移；越界或无效返回 -1
static GLintptr fp_dl_ptr_off(const void* base, const void* p, size_t bytes) {
    if(!base || !p) return -1;
    ptrdiff_t off = (const uint8_t*)p - (const uint8_t*)base;
    if(off < 0 || (size_t)off >= bytes) return -1;
    return (GLintptr)off;
}

// 保存当前客户端数组状态（vertex_off 等字段存原始指针值，供恢复）
static void fp_dl_save_client_state(fp_dl_client_snapshot_t* out) {
    if(!out) return;
    out->vertex_enabled = fp_client_vertex_enabled;
    out->vertex_size = fp_client_vertex_size;
    out->vertex_type = fp_client_vertex_type;
    out->vertex_stride = fp_client_vertex_stride;
    out->texcoord_enabled = fp_client_texcoord_enabled;
    out->texcoord_size = fp_client_texcoord_size;
    out->texcoord_type = fp_client_texcoord_type;
    out->texcoord_stride = fp_client_texcoord_stride;
    out->color_enabled = fp_client_color_enabled;
    out->color_size = fp_client_color_size;
    out->color_type = fp_client_color_type;
    out->color_stride = fp_client_color_stride;
    out->normal_enabled = fp_client_normal_enabled;
    out->normal_type = fp_client_normal_type;
    out->normal_stride = fp_client_normal_stride;
    out->vertex_abo = fp_client_vertex_abo;
    out->texcoord_abo = fp_client_texcoord_abo;
    out->color_abo = fp_client_color_abo;
    out->vertex_off = (GLintptr)(intptr_t)fp_client_vertex_ptr;
    out->texcoord_off = (GLintptr)(intptr_t)fp_client_texcoord_ptr;
    out->color_off = (GLintptr)(intptr_t)fp_client_color_ptr;
    out->normal_off = (GLintptr)(intptr_t)fp_client_normal_ptr;
}

// 恢复客户端数组状态（指针直接取 off 字段值）
static void fp_dl_restore_client_state(const fp_dl_client_snapshot_t* s) {
    if(!s) return;
    fp_client_vertex_enabled = s->vertex_enabled;
    fp_client_vertex_size = s->vertex_size;
    fp_client_vertex_type = s->vertex_type;
    fp_client_vertex_stride = s->vertex_stride;
    fp_client_texcoord_enabled = s->texcoord_enabled;
    fp_client_texcoord_size = s->texcoord_size;
    fp_client_texcoord_type = s->texcoord_type;
    fp_client_texcoord_stride = s->texcoord_stride;
    fp_client_color_enabled = s->color_enabled;
    fp_client_color_size = s->color_size;
    fp_client_color_type = s->color_type;
    fp_client_color_stride = s->color_stride;
    fp_client_normal_enabled = s->normal_enabled;
    fp_client_normal_type = s->normal_type;
    fp_client_normal_stride = s->normal_stride;
    fp_client_vertex_abo = s->vertex_abo;
    fp_client_texcoord_abo = s->texcoord_abo;
    fp_client_color_abo = s->color_abo;
    fp_client_vertex_ptr = (const void*)(intptr_t)s->vertex_off;
    fp_client_texcoord_ptr = (const void*)(intptr_t)s->texcoord_off;
    fp_client_color_ptr = (const void*)(intptr_t)s->color_off;
    fp_client_normal_ptr = (const void*)(intptr_t)s->normal_off;
}

bool fp_dl_capture_client_draw(GLenum mode, GLint first, GLsizei count,
                               bool indexed, GLenum itype, const void* indices) {
    if(!dl_is_compiling()) return false;
    // 编译期间一律吞掉绘制（不真正画），无论录制是否成功；桌面语义是
    // "编译即收录"，GL_COMPILE_AND_EXECUTE 由 dl_end 在结束时整体执行一次。
    if(first < 0 || count < 0) return true;

    fp_dl_client_snapshot_t snap;
    memset(&snap, 0, sizeof(snap));
    snap.vertex_enabled = fp_client_vertex_enabled;
    snap.vertex_size = fp_client_vertex_size;
    snap.vertex_type = fp_client_vertex_type;
    snap.vertex_stride = fp_client_vertex_stride;
    snap.texcoord_enabled = fp_client_texcoord_enabled;
    snap.texcoord_size = fp_client_texcoord_size;
    snap.texcoord_type = fp_client_texcoord_type;
    snap.texcoord_stride = fp_client_texcoord_stride;
    snap.color_enabled = fp_client_color_enabled;
    snap.color_size = fp_client_color_size;
    snap.color_type = fp_client_color_type;
    snap.color_stride = fp_client_color_stride;
    snap.normal_enabled = fp_client_normal_enabled;
    snap.normal_type = fp_client_normal_type;
    snap.normal_stride = fp_client_normal_stride;
    snap.vertex_abo = fp_client_vertex_abo;
    snap.texcoord_abo = fp_client_texcoord_abo;
    snap.color_abo = fp_client_color_abo;

    const void* vertex_data = NULL;
    uint32_t vertex_len = 0;
    if(fp_client_vertex_abo == 0) {
        // CPU 指针路径：拷贝交错数据（MC 1.12 关闭 VBO 时走这里）
        if(fp_client_vertex_enabled && fp_client_vertex_size > 0 && fp_client_vertex_ptr) {
            size_t vsize = fp_client_vertex_stride
                               ? (size_t)fp_client_vertex_stride
                               : (size_t)fp_client_vertex_size * fp_type_bytes(fp_client_vertex_type);
            size_t bytes = vsize * (size_t)count;
            if(vsize > 0 && count <= FP_MAX_VERTICES && bytes <= DL_OP_CAP) {
                vertex_data = fp_client_vertex_ptr;
                vertex_len = (uint32_t)bytes;
                snap.vertex_off = 0;
                snap.texcoord_off = fp_dl_ptr_off(fp_client_vertex_ptr, fp_client_texcoord_ptr, bytes);
                snap.color_off = fp_dl_ptr_off(fp_client_vertex_ptr, fp_client_color_ptr, bytes);
                snap.normal_off = fp_dl_ptr_off(fp_client_vertex_ptr, fp_client_normal_ptr, bytes);
            }
        }
    } else {
        // VBO 路径：记录 buffer 与字节偏移，不拷贝 GPU 数据。
        // 注意：MC 的 glVertexAttribPointer 直通 GLES、不更新 fp_client_* 状态，
        // 因此 VBO 开启时显示列表内实际录不到几何（回放为空操作）。
        // FCL/Pojav 对 <=1.12 强制关闭 VBO（客户端数组路径），MC 1.12 不受影响。
        snap.vertex_off = (GLintptr)(intptr_t)fp_client_vertex_ptr;
        snap.texcoord_off = (GLintptr)(intptr_t)fp_client_texcoord_ptr;
        snap.color_off = (GLintptr)(intptr_t)fp_client_color_ptr;
        snap.normal_off = (GLintptr)(intptr_t)fp_client_normal_ptr;
    }

    const void* indices_data = NULL;
    uint32_t indices_len = 0;
    GLuint indices_ebo = 0;
    GLintptr indices_off = 0;
    if(indexed) {
        GLint eab = 0;
        es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);
        if(eab != 0) {
            indices_ebo = (GLuint)eab;
            indices_off = (GLintptr)(intptr_t)indices;
        } else if(indices && count > 0 &&
                  (itype == GL_UNSIGNED_BYTE || itype == GL_UNSIGNED_SHORT ||
                   itype == GL_UNSIGNED_INT)) {
            size_t isize = (size_t)count * fp_type_bytes(itype);
            if(isize <= DL_OP_CAP) {
                indices_data = indices;
                indices_len = (uint32_t)isize;
            }
        }
    }

    size_t pl_size = sizeof(dl_client_draw_payload_t);
    uint32_t v_off = (uint32_t)((pl_size + 7) & ~(size_t)7);
    uint32_t i_off = v_off + vertex_len;
    size_t total = pl_size + vertex_len + indices_len;
    if(total > DL_OP_CAP) return true; // 超限：丢弃该 op，但仍不执行
    uint8_t* buf = (uint8_t*)malloc(total);
    if(!buf) return true; // OOM：丢弃该 op，但仍不执行
    dl_client_draw_payload_t* pl = (dl_client_draw_payload_t*)buf;
    memset(pl, 0, pl_size);
    pl->snap = snap;
    pl->mode = mode;
    pl->first = first;
    pl->count = count;
    pl->indexed = indexed;
    pl->itype = itype;
    pl->indices_ebo = indices_ebo;
    pl->indices_src_off = indices_off;
    pl->vertex_len = vertex_len;
    pl->indices_len = indices_len;
    pl->vertex_off = v_off;
    pl->indices_off = i_off;
    if(vertex_len && vertex_data) memcpy(buf + v_off, vertex_data, vertex_len);
    if(indices_len && indices_data) memcpy(buf + i_off, indices_data, indices_len);

    bool ok = dl_capture_op(DL_OP_CLIENT_DRAW, buf, (uint32_t)total);
    free(buf);
    (void)ok;
    return true;
}

// 回放即时模式顶点快照：按当前矩阵/纹理绘制（桌面语义：执行时变换）
static void fp_dl_play_immediate(GLenum mode, const GLfloat* verts, GLsizei count) {
    if(!current_context || !verts || count <= 0) return;
    if((size_t)count > (size_t)fp_immediate_capacity) {
        GLsizei newcap = fp_immediate_capacity ? fp_immediate_capacity : 1024;
        while(newcap < count) {
            if(newcap > FP_MAX_VERTICES / 2) { newcap = FP_MAX_VERTICES; break; }
            newcap *= 2;
        }
        if(newcap < count) return;
        GLfloat* nb = (GLfloat*)realloc(fp_immediate_vertices, (size_t)newcap * FP_VERTEX_BYTES);
        if(!nb) return;
        fp_immediate_vertices = nb;
        fp_immediate_capacity = newcap;
    }
    memcpy(fp_immediate_vertices, verts, (size_t)count * FP_VERTEX_BYTES);
    fp_immediate_mode = mode;
    fp_immediate_count = count;
    fp_immediate_active = true;
    fp_flush_immediate();
    fp_immediate_active = false;
}

// 回放客户端数组绘制：恢复录制时的数组状态，走既有绘制链
// （QUADS 展开 / 默认 shader / 纹理刷新全部复用，与编译期路径一致）
static void fp_dl_play_client_draw(const fp_dl_client_snapshot_t* snap, GLenum mode, GLint first,
                                   GLsizei count, bool indexed, GLenum itype,
                                   GLuint indices_ebo, GLintptr indices_off,
                                   const void* indices_cpu, uint32_t indices_len,
                                   const void* data) {
    if(!current_context || !snap) return;

    fp_dl_client_snapshot_t save;
    fp_dl_save_client_state(&save);
    bool save_color_active = fp_client_color_active;

    // 应用录制快照
    if(snap->vertex_abo == 0) {
        fp_client_vertex_ptr = data;
        fp_client_vertex_abo = 0;
        fp_client_texcoord_ptr = (snap->texcoord_enabled && snap->texcoord_off >= 0 && data)
                                     ? (const uint8_t*)data + snap->texcoord_off : NULL;
        fp_client_texcoord_abo = 0;
        fp_client_color_ptr = (snap->color_enabled && snap->color_off >= 0 && data)
                                  ? (const uint8_t*)data + snap->color_off : NULL;
        fp_client_color_abo = 0;
        fp_client_normal_ptr = (snap->normal_enabled && snap->normal_off >= 0 && data)
                                   ? (const uint8_t*)data + snap->normal_off : NULL;
    } else {
        fp_client_vertex_ptr = (const void*)(intptr_t)snap->vertex_off;
        fp_client_vertex_abo = snap->vertex_abo;
        fp_client_texcoord_ptr = (const void*)(intptr_t)snap->texcoord_off;
        fp_client_texcoord_abo = snap->texcoord_abo;
        fp_client_color_ptr = (const void*)(intptr_t)snap->color_off;
        fp_client_color_abo = snap->color_abo;
        fp_client_normal_ptr = (const void*)(intptr_t)snap->normal_off;
    }
    fp_client_vertex_enabled = snap->vertex_enabled;
    fp_client_vertex_size = snap->vertex_size;
    fp_client_vertex_type = snap->vertex_type;
    fp_client_vertex_stride = snap->vertex_stride;
    fp_client_texcoord_enabled = snap->texcoord_enabled;
    fp_client_texcoord_size = snap->texcoord_size;
    fp_client_texcoord_type = snap->texcoord_type;
    fp_client_texcoord_stride = snap->texcoord_stride;
    fp_client_color_enabled = snap->color_enabled;
    fp_client_color_size = snap->color_size;
    fp_client_color_type = snap->color_type;
    fp_client_color_stride = snap->color_stride;
    fp_client_normal_enabled = snap->normal_enabled;
    fp_client_normal_type = snap->normal_type;
    fp_client_normal_stride = snap->normal_stride;

    GLint old_eab = 0;
    bool restore_eab = false;
    if(indexed) {
        if(indices_ebo != 0) {
            // 源 EBO 直通
            es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_eab);
            es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_ebo);
            restore_eab = true;
            glDrawElements(mode, count, itype, (const void*)(intptr_t)indices_off);
        } else if(indices_cpu && indices_len > 0) {
            // GLES 禁止客户端索引指针：上传到内部 scratch EBO
            es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &old_eab);
            if(fp_index_ebo != 0) {
                es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fp_index_ebo);
                if((GLsizeiptr)indices_len > fp_index_ebo_cap) {
                    es3_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)indices_len,
                                               indices_cpu, GL_STREAM_DRAW);
                    fp_index_ebo_cap = (GLsizeiptr)indices_len;
                } else {
                    es3_functions.glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                                                  (GLsizeiptr)indices_len, indices_cpu);
                }
                restore_eab = true;
                glDrawElements(mode, count, itype, NULL);
            } else {
                glDrawElements(mode, count, itype, indices_cpu);
            }
        } else {
            glDrawElements(mode, count, itype, NULL);
        }
    } else {
        glDrawArrays(mode, first, count);
    }
    if(restore_eab) {
        es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)old_eab);
    }

    // 恢复调用前的客户端数组状态
    fp_dl_restore_client_state(&save);
    fp_client_color_active = save_color_active;
}

static void fp_dl_execute_op(uint32_t type, const void* payload, uint32_t size) {
    if(!payload) return;
    switch(type) {
        case DL_OP_IMMEDIATE: {
            if(size < sizeof(dl_immediate_payload_t)) return;
            const dl_immediate_payload_t* p = (const dl_immediate_payload_t*)payload;
            if((size_t)p->verts_off + (size_t)p->count * FP_VERTEX_BYTES > (size_t)size) return;
            fp_dl_play_immediate(p->mode,
                                 (const GLfloat*)((const uint8_t*)payload + p->verts_off),
                                 (GLsizei)p->count);
            break;
        }
        case DL_OP_CLIENT_DRAW: {
            if(size < sizeof(dl_client_draw_payload_t)) return;
            const dl_client_draw_payload_t* p = (const dl_client_draw_payload_t*)payload;
            const void* data = NULL;
            const void* idx = NULL;
            if(p->vertex_len > 0 &&
               (size_t)p->vertex_off + (size_t)p->vertex_len <= (size_t)size) {
                data = (const uint8_t*)payload + p->vertex_off;
            }
            if(p->indexed && p->indices_len > 0 &&
               (size_t)p->indices_off + (size_t)p->indices_len <= (size_t)size) {
                idx = (const uint8_t*)payload + p->indices_off;
            }
            fp_dl_play_client_draw(&p->snap, p->mode, p->first, p->count, p->indexed, p->itype,
                                   p->indices_ebo, p->indices_src_off, idx, p->indices_len, data);
            break;
        }
        case DL_OP_BIND_TEXTURE: {
            if(size < sizeof(dl_bind_texture_payload_t)) return;
            const dl_bind_texture_payload_t* p = (const dl_bind_texture_payload_t*)payload;
            if(p->target == GL_TEXTURE_2D) {
                glActiveTexture(p->unit);
                glBindTexture(GL_TEXTURE_2D, p->texture);
            }
            break;
        }
        case DL_OP_TEXTURE_ENABLE: {
            if(size < sizeof(dl_texture_enable_payload_t)) return;
            const dl_texture_enable_payload_t* p = (const dl_texture_enable_payload_t*)payload;
            fp_set_texture_enabled(p->enabled != 0);
            break;
        }
        default:
            break;
    }
}

// ---- 显示列表：注册表 / 生命周期 / 执行 ----
// 列表注册表为进程级（与 fixed_pipeline 一致），id 从 1 开始；顶点与客户端
// 数组数据在编译期拷贝，回放不依赖录制时的临时缓冲（MC 的 Tessellator
// ByteBuffer 会被下一个 box 复用，必须快照）。GL 调用均为渲染线程单线程
// 执行，与 fixed_pipeline 其余部分一致，不加锁。

static fp_dl_list_t* dl_lookup(uint32_t id) {
    if(id == 0 || id >= dl_table_cap) return NULL;
    return dl_table[id];
}

static bool dl_table_ensure(uint32_t id) {
    if(id >= DL_MAX_LISTS) return false;
    if(id < dl_table_cap) return true;
    uint32_t ncap = dl_table_cap ? dl_table_cap : 64;
    while(ncap <= id) ncap <<= 1;
    if(ncap > DL_MAX_LISTS) ncap = DL_MAX_LISTS;
    fp_dl_list_t** nt = (fp_dl_list_t**)realloc(dl_table, (size_t)ncap * sizeof(*nt));
    if(!nt) return false;
    memset(nt + dl_table_cap, 0, (size_t)(ncap - dl_table_cap) * sizeof(*nt));
    dl_table = nt;
    dl_table_cap = ncap;
    return true;
}

static fp_dl_list_t* dl_get_or_create(uint32_t id) {
    if(id == 0 || !dl_table_ensure(id)) return NULL;
    if(!dl_table[id]) {
        fp_dl_list_t* l = (fp_dl_list_t*)calloc(1, sizeof(fp_dl_list_t));
        if(!l) return NULL;
        l->id = id;
        dl_table[id] = l;
    }
    dl_table[id]->allocated = true;
    return dl_table[id];
}

static void dl_clear_ops(fp_dl_list_t* l) {
    if(!l) return;
    for(uint32_t i = 0; i < l->op_count; i++) {
        if(l->ops[i].data) free(l->ops[i].data);
    }
    free(l->ops);
    l->ops = NULL;
    l->op_count = l->op_cap = 0;
    if(dl_total_bytes >= l->bytes) dl_total_bytes -= l->bytes;
    else dl_total_bytes = 0;
    l->bytes = 0;
}

static void dl_free_list(fp_dl_list_t* l) {
    if(!l) return;
    dl_clear_ops(l);
    dl_table[l->id] = NULL;
    free(l);
}

bool dl_is_compiling(void) {
    return dl_compile_depth > 0;
}

bool dl_capture_op(uint32_t type, const void* data, uint32_t size) {
    if(dl_compile_depth <= 0) return false;
    fp_dl_list_t* l = dl_lookup(dl_compile_stack[dl_compile_depth - 1]);
    if(!l || l->failed) return false;
    if(size > DL_OP_CAP || l->bytes + size > DL_LIST_CAP ||
       dl_total_bytes + size > DL_TOTAL_CAP) {
        l->failed = true;
        LTW_ERROR_PRINTF("display list %u: op too large (cap exceeded), list discarded",
                         (unsigned)l->id);
        return false;
    }
    if(l->op_count >= l->op_cap) {
        uint32_t ncap = l->op_cap ? l->op_cap * 2 : 8;
        if(ncap > 4096) ncap = 4096;
        if(ncap <= l->op_cap) {
            l->failed = true;
            return false;
        }
        dl_op_entry_t* no = (dl_op_entry_t*)realloc(l->ops, (size_t)ncap * sizeof(dl_op_entry_t));
        if(!no) {
            l->failed = true;
            return false;
        }
        l->ops = no;
        l->op_cap = ncap;
    }
    uint8_t* buf = NULL;
    if(size > 0) {
        buf = (uint8_t*)malloc(size);
        if(!buf) {
            l->failed = true;
            return false;
        }
        if(data) memcpy(buf, data, size);
        else memset(buf, 0, size); // 防御：无数据时零初始化，避免未定义内容
    }
    l->ops[l->op_count].type = type;
    l->ops[l->op_count].size = size;
    l->ops[l->op_count].data = buf;
    l->op_count++;
    l->bytes += size;
    dl_total_bytes += size;
    return true;
}

// 编译期 glBindTexture / GL_TEXTURE_2D 开关：只记录，不立即生效
void fp_dl_capture_bind_texture(GLenum target, GLuint texture) {
    if(!dl_is_compiling() || target != GL_TEXTURE_2D) return;
    GLint unit = GL_TEXTURE0;
    es3_functions.glGetIntegerv(GL_ACTIVE_TEXTURE, &unit);
    dl_bind_texture_payload_t p;
    p.target = target;
    p.texture = texture;
    p.unit = (GLenum)unit;
    dl_capture_op(DL_OP_BIND_TEXTURE, &p, sizeof(p));
}

void fp_dl_capture_texture_enable(bool enabled) {
    if(!dl_is_compiling()) return;
    dl_texture_enable_payload_t p;
    p.enabled = enabled ? 1 : 0;
    dl_capture_op(DL_OP_TEXTURE_ENABLE, &p, sizeof(p));
}

GLuint dl_gen(GLsizei range) {
    if(range <= 0) return 0;
    uint32_t need = (uint32_t)range;
    if(need >= DL_MAX_LISTS) return 0;
    uint32_t start = dl_next_hint;
    if(start == 0 || start + need > DL_MAX_LISTS) start = 1;
    uint32_t scanned = 0;
    while(scanned < DL_MAX_LISTS) {
        bool free_run = true;
        for(uint32_t i = 0; i < need; i++) {
            fp_dl_list_t* l = dl_lookup(start + i);
            if(l && l->allocated) {
                free_run = false;
                break;
            }
        }
        if(free_run) {
            uint32_t created = 0;
            for(uint32_t i = 0; i < need; i++) {
                if(!dl_get_or_create(start + i)) break;
                created++;
            }
            if(created == need) {
                dl_next_hint = start + need;
                return start;
            }
            // 创建中途失败（OOM）：回滚已保留的槽位，避免泄漏
            for(uint32_t i = 0; i < created; i++) {
                fp_dl_list_t* l = dl_lookup(start + i);
                if(l) {
                    l->allocated = false;
                    if(!l->compiled && !l->pending_delete && l->exec_count == 0) {
                        dl_free_list(l);
                    }
                }
            }
            return 0;
        }
        start++;
        if(start + need > DL_MAX_LISTS) start = 1;
        scanned++;
    }
    return 0;
}

void dl_new(GLuint list, GLenum mode) {
    if(list == 0) return;
    if(mode != GL_COMPILE && mode != GL_COMPILE_AND_EXECUTE) {
        LTW_ERROR_PRINTF("display list: glNewList(%u) with unsupported mode 0x%x",
                         (unsigned)list, (unsigned)mode);
        return;
    }
    if(dl_compile_depth >= DL_MAX_COMPILE) {
        LTW_ERROR_PRINTF("display list: nested compile depth exceeded");
        return;
    }
    // 防止自嵌套 / 与外层同 id
    for(int i = 0; i < dl_compile_depth; i++) {
        if(dl_compile_stack[i] == list) return;
    }
    fp_dl_list_t* l = dl_get_or_create(list);
    if(!l) return;
    if(l->exec_count > 0) return; // 正在执行时重编译：跳过（防御）
    dl_clear_ops(l);
    l->compiled = false;
    l->failed = false;
    dl_compile_stack[dl_compile_depth] = list;
    dl_compile_modes[dl_compile_depth] = mode;
    dl_compile_depth++;
}

void dl_end(void) {
    if(dl_compile_depth <= 0) return;
    dl_compile_depth--;
    GLuint list = dl_compile_stack[dl_compile_depth];
    GLenum mode = dl_compile_modes[dl_compile_depth];
    fp_dl_list_t* l = dl_lookup(list);
    if(l) {
        if(l->failed) {
            // 录制不完整：丢弃，避免回放半成品几何
            dl_clear_ops(l);
            l->compiled = false;
        } else {
            l->compiled = true;
        }
    }
    // 嵌套编译：把内部列表作为 CALL_LIST 记录到外层
    if(dl_compile_depth > 0) {
        dl_call_list_payload_t p;
        p.list = list;
        dl_capture_op(DL_OP_CALL_LIST, &p, sizeof(p));
    }
    // GL_COMPILE_AND_EXECUTE：结束立即执行一次（桌面语义）
    if(mode == GL_COMPILE_AND_EXECUTE) {
        dl_execute_list(list);
    }
}

void dl_delete(GLuint list, GLsizei range) {
    if(range <= 0) return;
    for(GLsizei i = 0; i < range; i++) {
        // 用 64 位累加，避免 list 接近 UINT32_MAX 时溢出回绕删错列表
        uint64_t id64 = (uint64_t)list + (uint64_t)i;
        if(id64 >= DL_MAX_LISTS) break;
        uint32_t id = (uint32_t)id64;
        fp_dl_list_t* l = dl_lookup(id);
        if(!l) continue;
        l->allocated = false;
        if(l->exec_count > 0) {
            l->pending_delete = true; // 执行结束后释放
        } else {
            dl_free_list(l);
        }
    }
}

bool dl_is_list(GLuint list) {
    fp_dl_list_t* l = dl_lookup(list);
    return l && l->compiled && !l->pending_delete;
}

void dl_list_base(GLuint base) {
    dl_list_base_id = base;
}

// 执行：回放 op 序列（调用前已验证非递归）
static void dl_execute_list(GLuint list) {
    if(list == 0 || dl_exec_depth >= DL_MAX_EXEC_DEPTH) return;
    for(int i = 0; i < dl_exec_depth; i++) {
        if(dl_exec_chain[i] == list) return; // 递归保护
    }
    fp_dl_list_t* l = dl_lookup(list);
    if(!l || !l->compiled || l->pending_delete) return;
    dl_exec_chain[dl_exec_depth++] = list;
    l->exec_count++;
    for(uint32_t i = 0; i < l->op_count; i++) {
        const dl_op_entry_t* op = &l->ops[i];
        if(op->type == DL_OP_CALL_LIST) {
            GLuint sub = 0;
            if(op->size >= sizeof(sub)) memcpy(&sub, op->data, sizeof(sub));
            dl_execute_list(sub);
        } else {
            fp_dl_execute_op(op->type, op->data, op->size);
        }
    }
    l->exec_count--;
    dl_exec_depth--;
    if(l->pending_delete && l->exec_count == 0) dl_free_list(l);
}

void dl_call(GLuint list) {
    if(dl_compile_depth > 0) {
        // 编译期间调用：记录嵌套调用
        dl_call_list_payload_t p;
        p.list = list;
        dl_capture_op(DL_OP_CALL_LIST, &p, sizeof(p));
        return;
    }
    dl_execute_list(list);
}

void dl_calls(GLsizei n, GLenum type, const void* lists) {
    if(n <= 0 || !lists) return;
    const uint8_t* p = (const uint8_t*)lists;
    for(GLsizei i = 0; i < n; i++) {
        GLuint idx = 0;
        switch(type) {
            case GL_BYTE: {
                GLbyte v;
                memcpy(&v, p, 1);
                idx = (GLuint)v;
                p += 1;
                break;
            }
            case GL_UNSIGNED_BYTE:
                idx = (GLuint)p[0];
                p += 1;
                break;
            case GL_SHORT: {
                GLshort v;
                memcpy(&v, p, 2);
                idx = (GLuint)v;
                p += 2;
                break;
            }
            case GL_UNSIGNED_SHORT: {
                GLushort v;
                memcpy(&v, p, 2);
                idx = (GLuint)v;
                p += 2;
                break;
            }
            case GL_INT: {
                GLint v;
                memcpy(&v, p, 4);
                idx = (GLuint)v;
                p += 4;
                break;
            }
            case GL_UNSIGNED_INT: {
                GLuint v;
                memcpy(&v, p, 4);
                idx = v;
                p += 4;
                break;
            }
            case GL_FLOAT: {
                GLfloat v;
                memcpy(&v, p, 4);
                idx = (GLuint)(GLint)v; // 截断（desktop 语义允许取整，此处保守）
                p += 4;
                break;
            }
            case GL_2_BYTES:
                idx = ((GLuint)p[0] << 8) | (GLuint)p[1];
                p += 2;
                break;
            case GL_3_BYTES:
                idx = ((GLuint)p[0] << 16) | ((GLuint)p[1] << 8) | (GLuint)p[2];
                p += 3;
                break;
            case GL_4_BYTES:
                idx = ((GLuint)p[0] << 24) | ((GLuint)p[1] << 16) |
                      ((GLuint)p[2] << 8) | (GLuint)p[3];
                p += 4;
                break;
            default:
                return; // 非法类型：停止
        }
        dl_call(dl_list_base_id + idx);
    }
}
