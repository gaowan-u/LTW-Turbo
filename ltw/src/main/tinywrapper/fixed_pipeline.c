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
static GLuint fp_saved_vao = 0;
static GLint fp_saved_active_tex = GL_TEXTURE0;
static GLint fp_saved_bound_tex = 0;
static bool fp_saved_texture_valid = false;
static bool fp_init_done = false;

// alpha test 状态（MC 1.12 文字渲染依赖 GL_ALPHA_TEST）
static bool fp_alpha_test = false;
static GLenum fp_alpha_test_func = 0x0207;   // GL_ALWAYS
static GLfloat fp_alpha_ref = 0.0f;
// blend 状态
static bool fp_blend_enabled = false;
// 客户端颜色数组是否真实启用（决定 shader 用顶点色还是当前色）
static bool fp_client_color_active = false;
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
static bool fp_texture_bound = false;
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

    es3_functions.glDeleteShader(vs);
    es3_functions.glDeleteShader(fs);

    static bool fp_ready_printed = false;
    if(!fp_ready_printed) {
        fp_ready_printed = true;
        printf("[LTW FP] default program ready (prog=%u)\n", fp_program);
        fflush(stdout);
    }
}

// 提交即时模式顶点
static void fp_flush_immediate(void) {
    static bool fp_flush_printed = false;
    if(!fp_flush_printed) {
        fp_flush_printed = true;
        printf("[LTW FP] glEnd flush count=%d mode=0x%x\n", fp_immediate_count, (unsigned)fp_immediate_mode);
        fflush(stdout);
    }
    if(!fp_immediate_active || fp_immediate_count == 0) return;
    fp_ensure_program();
    if(!fp_program) { fp_immediate_count = 0; return; }

    // 用画这一刻的真实绑定刷新状态，诊断/绘制都不吃旧记忆值。
    fp_refresh_bound_texture();

    // 诊断：即时模式顶点数据（字形候选路径）——前 2 次 + 每 256 次
    {
        static unsigned int im_n = 0;
        im_n++;
        if(im_n <= 16 || (im_n & 0x3F) == 1) {
            printf("[LTW DUMP] imm n=%u count=%d mode=0x%x tex=%u blend=%d atest=%d single=%d afunc=0x%x aref=%f\n",
                   im_n, fp_immediate_count, (unsigned)fp_immediate_mode, fp_bound_texture,
                   fp_blend_enabled ? 1 : 0, fp_alpha_test ? 1 : 0, fp_bound_single_channel ? 1 : 0,
                   fp_alpha_test ? (unsigned)fp_alpha_test_func : 0, fp_alpha_test ? fp_alpha_ref : 0.0f);
            if(fp_bound_texture != 0) {
                GLint tw = 0, th = 0, tf = 0;
                es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
                es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
                es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &tf);
                printf("[LTW DUMP]   tex %ux%u fmt=0x%x\n", tw, th, tf);
            }
            int vn = fp_immediate_count < 4 ? fp_immediate_count : 4;
            for(int vi = 0; vi < vn; vi++) {
                const GLfloat* v = fp_immediate_vertices + (size_t)vi * FP_STRIDE;
                printf("[LTW DUMP]   v%d pos=(%f,%f,%f) col=(%f,%f,%f,%f) uv=(%f,%f)\n",
                       vi, v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7], v[8]);
            }
            fflush(stdout);
        }
    }

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

    // 诊断：第一次画字形后直接读回 quad 实际渲染像素（区分全白/全黑/透明）
    {
        static unsigned int fp_read_n = 0;
        if(fp_bound_texture >= 24 && fp_read_n < 2 && fp_immediate_count >= 4) {
            fp_read_n++;
            GLfloat mvp[FP_MATRIX_SIZE];
            fp_mat_mul(mvp, fp_matrix_stack[FP_MATRIX_PROJECTION][fp_matrix_top[FP_MATRIX_PROJECTION]],
                       fp_matrix_stack[FP_MATRIX_MODELVIEW][fp_matrix_top[FP_MATRIX_MODELVIEW]]);
            GLint vp[4] = {0};
            es3_functions.glGetIntegerv(GL_VIEWPORT, vp);
            GLint draw_fb = 0, read_fb = 0;
            es3_functions.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_fb);
            es3_functions.glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_fb);
            GLint samples = 0;
            es3_functions.glGetIntegerv(GL_SAMPLES, &samples);
            if(read_fb != draw_fb) es3_functions.glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint)draw_fb);
            // quad 中心 + 四角（取前 4 个顶点）
            float pts[5][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
            for(int vi = 0; vi < 4; vi++) {
                const GLfloat* v = fp_immediate_vertices + (size_t)vi * FP_STRIDE;
                pts[vi][0] = v[0]; pts[vi][1] = v[1]; pts[vi][2] = v[2];
                pts[4][0] += v[0] / 4.0f; pts[4][1] += v[1] / 4.0f; pts[4][2] += v[2] / 4.0f;
            }
            printf("[LTW DIAG] fp_read tex=%u vp=%d,%d,%dx%d drawfb=%d readfb=%d samples=%d:",
                   fp_bound_texture, vp[0], vp[1], vp[2], vp[3], draw_fb, read_fb, samples);
            int last_sx = 0, last_sy = 0;
            for(int pi = 0; pi < 5; pi++) {
                float in[4] = {pts[pi][0], pts[pi][1], pts[pi][2], 1.0f};
                float out[4] = {0,0,0,0};
                for(int c = 0; c < 4; c++) {
                    out[c] = mvp[c*4+0]*in[0] + mvp[c*4+1]*in[1] +
                             mvp[c*4+2]*in[2] + mvp[c*4+3]*in[3];
                }
                if(out[3] == 0.0f) continue;
                float ndcx = out[0]/out[3], ndcy = out[1]/out[3];
                int sx = vp[0] + (int)((ndcx * 0.5f + 0.5f) * vp[2]);
                int sy = vp[1] + (int)((ndcy * 0.5f + 0.5f) * vp[3]);
                unsigned char px[4] = {0};
                es3_functions.glReadPixels(sx, sy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
                printf(" p%d=(%u,%u,%u,%u)@(%d,%d)", pi, px[0], px[1], px[2], px[3], sx, sy);
                last_sx = sx; last_sy = sy;
            }
            // 中心周围 5x5：确认这个微小 quad 附近是否有字形像素
            printf(" grid5x5:");
            for(int dy = -2; dy <= 2; dy++) {
                for(int dx = -2; dx <= 2; dx++) {
                    unsigned char px[4] = {0};
                    es3_functions.glReadPixels(last_sx + dx, last_sy + dy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
                    printf(" %u,%u,%u,%u", px[0], px[1], px[2], px[3]);
                }
            }
            printf("\n");
            fflush(stdout);
            if(read_fb != draw_fb) es3_functions.glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint)read_fb);
        }
    }

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
    for(int m = 0; m < FP_MATRIX_COUNT; m++) {
        fp_matrix_top[m] = 0;
        fp_mat_identity(fp_matrix_stack[m][0]);
    }
    fp_current_matrix = FP_MATRIX_MODELVIEW;
    fp_immediate_active = false;
    fp_immediate_count = 0;
    fp_immediate_capacity = 0;
    fp_immediate_vertices = NULL;
    fp_current_color[0] = fp_current_color[1] = fp_current_color[2] = fp_current_color[3] = 1.0f;
    fp_current_texcoord[0] = fp_current_texcoord[1] = 0.0f;
    fp_current_texcoord[2] = 0.0f; fp_current_texcoord[3] = 1.0f;
    fp_client_vertex_enabled = fp_client_texcoord_enabled = fp_client_color_enabled = fp_client_normal_enabled = false;
    fp_texture_enabled = false;
    fp_texture_bound = false;
    fp_bound_texture = 0;
}

void fp_matrix_mode(GLenum mode) {
    {
        static bool diag = false;
        if(!diag) {
            diag = true;
            printf("[LTW DIAG] glMatrixMode 0x%x\n", (unsigned)mode);
            fflush(stdout);
        }
    }
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
    {
        static bool diag = false;
        if(!diag) {
            diag = true;
            printf("[LTW DIAG] glOrtho l=%f r=%f b=%f t=%f n=%f f=%f\n", l, r, b, t, n, f);
            fflush(stdout);
        }
    }
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
    static bool fp_begin_printed = false;
    if(!fp_begin_printed) {
        fp_begin_printed = true;
        printf("[LTW FP] glBegin mode=0x%x\n", (unsigned)mode);
        fflush(stdout);
    }
    fp_immediate_mode = mode;
    fp_immediate_active = true;
    fp_immediate_count = 0;
}

void fp_end(void) {
    if(!fp_immediate_active) return;
    fp_flush_immediate();
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

void fp_texcoord2f(GLfloat s, GLfloat t) {
    fp_current_texcoord[0] = s; fp_current_texcoord[1] = t;
}
void fp_texcoord2fv(const GLfloat* v) { if(v) fp_texcoord2f(v[0], v[1]); }
void fp_texcoord1f(GLfloat s) { fp_texcoord2f(s, 0.0f); }
void fp_texcoord3f(GLfloat s, GLfloat t, GLfloat r) { fp_texcoord2f(s, t); fp_current_texcoord[2] = r; }
void fp_texcoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q) { fp_texcoord2f(s, t); fp_current_texcoord[2] = r; fp_current_texcoord[3] = q; }
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
    fp_client_texcoord_size = size; fp_client_texcoord_type = type;
    fp_client_texcoord_stride = stride; fp_client_texcoord_ptr = pointer;
    fp_client_texcoord_abo = fp_current_abo();
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
        case GL_TEXTURE_COORD_ARRAY: fp_client_texcoord_enabled = true; break;
        case GL_COLOR_ARRAY:   fp_client_color_enabled = true;    break;
        case GL_NORMAL_ARRAY:  fp_client_normal_enabled = true;   break;
        default: break;
    }
}
void fp_disable_client_state(GLenum cap) {
    switch(cap) {
        case GL_VERTEX_ARRAY:  fp_client_vertex_enabled = false;  break;
        case GL_TEXTURE_COORD_ARRAY: fp_client_texcoord_enabled = false; break;
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
    // 只跟踪 unit 0 的绑定纹理（固定管线场景下 MC 1.12 只用 unit 0）
    if(unit == 0) {
        fp_texture_bound = true;
        GLint tex = 0;
        es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex);
        fp_bound_texture = (GLuint)tex;
        // 查询纹理格式：GL_R8/GL_RED 等单通道纹理（GL_ALPHA 映射而来）
        // shader 采样时把 R 通道当 alpha/灰度用
        fp_bound_single_channel = false;
        if(tex != 0) {
            GLint fmt = 0;
            es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);
            if(fmt == GL_R8 || fmt == GL_RED || fmt == GL_R16F || fmt == GL_R32F) {
                fp_bound_single_channel = true;
            }
        }
    }
}

// ---- alpha test ----
void fp_set_alpha_test(bool enabled) { fp_alpha_test = enabled; }
void fp_alpha_func(GLenum func, GLfloat ref) { fp_alpha_test_func = func; fp_alpha_ref = ref; }
void fp_set_blend(bool enabled) { fp_blend_enabled = enabled; }

// ---- 绘制挂钩 ----
// 画之前直接查“当前绑定在活动纹理单元上的 2D 纹理”，而不是依赖
// glBindTexture 包装器里的记忆值。MC 的 GlStateManager 会缓存纹理绑定，
// 连续绘制同一字形时可能根本不调用 glBindTexture，记忆值一旦被其他
// 单元/路径覆盖，字就会以“无纹理”的纯色方块画出来。
static void fp_refresh_bound_texture(void) {
    if(!current_context) return;
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
    if(fp_alphafunc_loc >= 0) es3_functions.glUniform1i(fp_alphafunc_loc, fp_alpha_test ? (GLint)fp_alpha_test_func : 7);
    if(fp_alpharef_loc >= 0) es3_functions.glUniform1f(fp_alpharef_loc, fp_alpha_test ? fp_alpha_ref : 0.0f);
}

// 低频屏幕像素探针：确认渲染结果（每 4096 次 fp 绘制读取一次屏幕
// 几个固定点的像素颜色，直接验证画面内容）。
void fp_pixel_probe(void) {
    static unsigned int probe_n = 0;
    if((++probe_n & 0xFFF) != 1) return;
    GLint vp[4] = {0};
    es3_functions.glGetIntegerv(GL_VIEWPORT, vp);
    if(vp[2] <= 0 || vp[3] <= 0) return;
    // 中心 + 四角 + 中心偏下（按钮/文字区域）
    struct { int x, y; } pts[9] = {
        {vp[2] / 2,         vp[3] / 2},                    // 0 中心
        {vp[2] * 1 / 4,     vp[3] * 1 / 4},                // 1 左上
        {vp[2] * 3 / 4,     vp[3] * 1 / 4},                // 2 右上
        {vp[2] * 1 / 4,     vp[3] * 3 / 4},                // 3 左下
        {vp[2] * 3 / 4,     vp[3] * 3 / 4},                // 4 右下
        {vp[2] / 2,         vp[3] * 3 / 5},                // 5 中心偏下（按钮）
        {vp[2] / 2,         vp[3] * 7 / 10},               // 6 更下（按钮）
        {vp[2] / 2,         vp[3] * 1 / 5},                // 7 中心偏上（标题）
        {vp[2] / 2,         vp[3] * 2 / 10},               // 8 上（logo）
    };
    GLint old_rb = 0;
    es3_functions.glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &old_rb);
    printf("[LTW PIX] vp=%d,%d %dx%d fb=%d", vp[0], vp[1], vp[2], vp[3], old_rb);
    for(int i = 0; i < 9; i++) {
        uint8_t px[4] = {0};
        es3_functions.glReadPixels(pts[i].x, pts[i].y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, px);
        printf(" p%d=(%u,%u,%u,%u)", i, px[0], px[1], px[2], px[3]);
    }
    printf("\n");
    fflush(stdout);
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

    {
        // 一次性诊断：纹理状态（unit/绑定/格式）
        static bool diag = false;
        if(!diag) {
            diag = true;
            GLint act = 0, tex0 = 0, tex1 = 0, w = 0, h = 0, fmt = 0;
            es3_functions.glGetIntegerv(GL_ACTIVE_TEXTURE, &act);
            es3_functions.glActiveTexture(GL_TEXTURE0);
            es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex0);
            es3_functions.glActiveTexture(GL_TEXTURE1);
            es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &tex1);
            if(tex0) {
                es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
                es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
                es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &fmt);
            }
            printf("[LTW DIAG] texstate: active=0x%x tex0=%d(%dx%d fmt=0x%x) tex1=%d fp_bound=%u\n",
                   act, tex0, w, h, fmt, tex1, fp_bound_texture);
            fflush(stdout);
            es3_functions.glActiveTexture((GLenum)act);
        }
    }

    fp_saved_texture_valid = false;
    if(fp_bound_texture != 0) {
        fp_saved_texture_valid = true;
        es3_functions.glGetIntegerv(GL_ACTIVE_TEXTURE, &fp_saved_active_tex);
        es3_functions.glGetIntegerv(GL_TEXTURE_BINDING_2D, &fp_saved_bound_tex);
        es3_functions.glActiveTexture(GL_TEXTURE0);
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
        }
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
        }
    }

    {
        // 低频诊断：确认上传时的关键参数（每 256 次打印一次）
        static unsigned int up_n = 0;
        if((++up_n & 0xFF) == 1) {
            ptrdiff_t uv_off = 0, col_off = 0;
            if(fp_client_texcoord_enabled && fp_client_texcoord_ptr)
                uv_off = (const uint8_t*)fp_client_texcoord_ptr - (const uint8_t*)fp_client_vertex_ptr;
            if(fp_client_color_enabled && fp_client_color_ptr)
                col_off = (const uint8_t*)fp_client_color_ptr - (const uint8_t*)fp_client_vertex_ptr;
            printf("[LTW DIAG] upload n=%u count=%d stride=%d vsize=%zu uv_off=%td col_off=%td tex=%u usetex=%d\n",
                   up_n, count, fp_client_vertex_stride, vsize, uv_off, col_off,
                   fp_bound_texture, fp_bound_texture ? 1 : 0);
            fflush(stdout);
        }
        // 每 64 次上传 dump 一个 quad：覆盖按钮/文字等元素
        if((up_n & 0x3F) == 1 && fp_client_vertex_ptr && count >= 4) {
            const uint8_t* p = (const uint8_t*)fp_client_vertex_ptr;
            size_t st = fp_client_vertex_stride ? (size_t)fp_client_vertex_stride : (size_t)fp_client_vertex_size * 4;
            ptrdiff_t uvo = fp_client_texcoord_ptr ? (const uint8_t*)fp_client_texcoord_ptr - (const uint8_t*)fp_client_vertex_ptr : -1;
            ptrdiff_t co = fp_client_color_ptr ? (const uint8_t*)fp_client_color_ptr - (const uint8_t*)fp_client_vertex_ptr : -1;
            printf("[LTW DUMP] n=%u stride=%zu uv_off=%td col_off=%td tex=%u blend=%d atest=%d single=%d\n",
                   up_n, st, uvo, co, fp_bound_texture, fp_blend_enabled ? 1 : 0,
                   fp_alpha_test ? 1 : 0, fp_bound_single_channel ? 1 : 0);
            for(int vi = 0; vi < 4; vi++) {
                const uint8_t* v = p + vi * st;
                float pos[3]; memcpy(pos, v, 12);
                float uv[2] = {0, 0};
                if(uvo >= 0) memcpy(uv, v + uvo, 8);
                uint8_t col[4] = {255, 255, 255, 255};
                if(co >= 0) memcpy(col, v + co, 4);
                printf("[LTW DUMP]   v%d pos=(%f,%f,%f) uv=(%f,%f) col=(%u,%u,%u,%u)\n",
                       vi, pos[0], pos[1], pos[2], uv[0], uv[1], col[0], col[1], col[2], col[3]);
                fflush(stdout);
            }
        }
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
        }
        if(cbo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
        GLint ubo = fp_client_texcoord_abo ? fp_client_texcoord_abo : old_abo;
        if(ubo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)ubo);
        if(fp_client_texcoord_enabled && fp_client_texcoord_size > 0) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
            es3_functions.glVertexAttribPointer(FP_ATTR_UV, fp_client_texcoord_size, fp_client_texcoord_type,
                                                GL_FALSE, fp_client_texcoord_stride, fp_client_texcoord_ptr);
        }
        if(ubo != old_abo) es3_functions.glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
        {
            // 诊断：VBO 路径（字形等 1.12 Tessellator 元素）参数
            static unsigned int vb_n = 0;
            if((++vb_n & 0x3F) == 1) {
                printf("[LTW DUMP] vbo n=%u count=%d pos:abo=%d ptr=%p stride=%d size=%d col:en=%d abo=%d tex:en=%d abo=%d tex=%u blend=%d atest=%d single=%d\n",
                       vb_n, count, fp_client_vertex_abo, fp_client_vertex_ptr, fp_client_vertex_stride, fp_client_vertex_size,
                       fp_client_color_enabled && fp_client_color_size > 0, fp_client_color_abo,
                       fp_client_texcoord_enabled && fp_client_texcoord_size > 0, fp_client_texcoord_abo,
                       fp_bound_texture, fp_blend_enabled ? 1 : 0,
                       fp_alpha_test ? 1 : 0, fp_bound_single_channel ? 1 : 0);
                fflush(stdout);
            }
        }
    }
    // attribute 启用情况影响 uUseColor，这里重设 uniforms（bind 先于 prepare）
    fp_set_default_uniforms();
    return true;
}

void fp_unbind_default_program(void) {
    if(!fp_program) return;
    if(fp_saved_texture_valid) {
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
    static bool fp_draw_printed = false;
    if(!fp_draw_printed) {
        fp_draw_printed = true;
        printf("[LTW FP] draw arrays prog=0 mode=0x%x first=%d count=%d\n", (unsigned)mode, first, count);
        fflush(stdout);
    }
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
    static bool fp_drawe_printed = false;
    if(!fp_drawe_printed) {
        fp_drawe_printed = true;
        printf("[LTW FP] draw elements prog=0 mode=0x%x count=%d\n", (unsigned)mode, count);
        fflush(stdout);
    }
    if(!fp_bind_default_program()) return false;
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
