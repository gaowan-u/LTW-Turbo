/**
 * 文件功能：桌面固定管线（GL 1.x）模拟。
 *
 * 设计要点：
 *  - 默认 shader 懒加载（首次绘制时），GLSL 300 es
 *  - 即时模式顶点收集到 CPU 缓冲，glEnd 时一次性上传
 *  - 矩阵列主序（与桌面 GL 一致），MVP = Projection * ModelView
 *  - 客户端数组模式下顶点由应用侧指针提供，绘制时绑定
 *  - 显示列表录制/回放：批量状态摊分 + 每 op 几何缓存（VAO/VBO/EBO）
 *  - 纹理/程序/缓冲绑定的 CPU 跟踪，减少每帧驱动查询
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
#include "env.h"
#include "ltw_config.h"
#include "debug.h"

// ---- 内部常量 ----
#define FP_MAX_STACK_DEPTH 32
#define FP_MAX_VERTICES 65536
#define FP_TEXENV_UNITS 3
#define FP_STRIDE (3 + 4 + 2)          // pos3 + color4 + uv2
#define FP_VERTEX_BYTES (FP_STRIDE * sizeof(GLfloat))

#define FP_ATTR_POS 0
#define FP_ATTR_COLOR 1
#define FP_ATTR_UV 2
#define FP_ATTR_UV1 3   // unit1（光照贴图）纹理坐标，方块渲染的昼夜亮度

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
    bool texcoord1_enabled;
    bool color_enabled;
    bool normal_enabled;
    GLint vertex_size;
    GLenum vertex_type;
    GLsizei vertex_stride;
    GLint texcoord_size;
    GLenum texcoord_type;
    GLsizei texcoord_stride;
    GLint texcoord1_size;
    GLenum texcoord1_type;
    GLsizei texcoord1_stride;
    GLint color_size;
    GLenum color_type;
    GLsizei color_stride;
    GLenum normal_type;
    GLsizei normal_stride;
    GLint vertex_abo;
    GLint texcoord_abo;
    GLint texcoord1_abo;
    GLint color_abo;
    GLintptr vertex_off;
    GLintptr texcoord_off;
    GLintptr texcoord1_off;
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

// MathCode: 实验性“整表合并”缓存。把同一个显示列表里格式一致、无状态
// 插入的 CLIENT_DRAW op 合并进一个大 VBO/EBO，回放时一次 glDrawElements。
typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLsizei draw_count;    // 合并后的三角形索引总数
    bool use_color;        // 合并 op 的 uUseColor（格式一致才允许合并）
    bool valid;            // 缓存已建立且属于当前 context
    bool attempted;        // 已尝试过合并（失败则回退旧路径，避免每帧重试）
    const void* ctx;       // 建立缓存的 context_t*
} dl_merge_cache_t;

// 列表存储
typedef struct {
    uint32_t type;
    uint32_t size;         // 数据字节数
    uint8_t* data;
    // ---- 回放几何缓存（惰性建立，仅 CPU 快照路径）----
    // 顶点/索引数据在录制后不可变，首次回放时一次性上传到私有 VBO/EBO 并
    // 定型为一个 VAO；此后每次回放只需 bind VAO + draw，避免每帧把同样的
    // 顶点重新 glBufferData（实体密集场景的主要 CPU/驱动开销来源）。
    // EGL context 重建后缓存句柄清零（见 fp_dl_reset_caches），惰性重建。
    GLuint cache_vao;
    GLuint cache_vbo;
    GLuint cache_ebo;
    GLenum cache_mode;     // 缓存后的绘制模式（QUADS 已展开为 TRIANGLES）
    GLsizei cache_count;   // 缓存后的绘制数量
    GLsizei cache_first;   // 非索引路径的 first（数据整体上传后为相对基址偏移）
    GLenum cache_itype;    // 索引类型（展开路径恒为 GL_UNSIGNED_INT）
    uint8_t cache_indexed; // 1=glDrawElements（走 cache_ebo），0=glDrawArrays
    uint8_t cache_valid;   // 缓存是否已建立且属于当前 context
    const void* cache_ctx; // 建立缓存的 context_t*，防止跨 context 误用
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
    dl_merge_cache_t merge;   // 整表合并缓存（实验性，LTW_DL_MERGE）
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

// ---- 显示列表批量回放状态 ----
// 一次 glCallList 执行期间，默认 program / 私有 VAO / unit0 纹理只绑定一次；
// 每个 CLIENT_DRAW op 用各自的缓存 VAO 直接绘制。旧路径每 op 做一次
// 绑定-查询-上传-解绑，动物等实体密集场景（同表被数十个实体每帧复用、
// 每个实体 ~10 个 op）会退化为每帧数千次驱动调用 + 数千次顶点重传，
// 帧率跌到个位数。这里把状态开销摊到每次列表执行上。
static bool dl_replay_active = false;
static bool dl_replay_dirty = true;      // 纹理等状态变化后，绘制前需重设 uniforms
static bool dl_last_uuse_color = false;  // 上一次缓存绘制实际设置的 uUseColor
static GLuint dl_current_vao = 0;        // MathCode: 批量回放期间当前绑定的 VAO，
                                         // 连续缓存 op 之间不再切回 fp_vao
static bool dl_merge_enabled = true;     // MathCode: 实验性整表合并开关（LTW_DL_MERGE）
static GLint dl_saved_vao = 0;
static GLint dl_saved_program = 0;
static GLint dl_saved_abo = 0;
static GLint dl_saved_eab = 0;
static GLint dl_saved_active_tex = GL_TEXTURE0;
static GLint dl_saved_bound_tex = 0;
static bool dl_saved_texture_valid = false;

static void dl_execute_list(GLuint list);
static void fp_dl_reset_caches(void);
static void fp_dl_free_merged(fp_dl_list_t* l);

// ---- 默认 shader ----
// MathCode: 默认 shader 增加受伤红闪模拟（uLightTint/uLightColor，2026-08）
// MathCode: 方块昼夜亮度（2026-08）：MC 1.12 方块渲染在 unit1 提供 lightmap
// 亮度 UV（GL_SHORT，值 0-15），固定管线 unit1 以 GL_MODULATE 把 lightmap
// 纹理乘进片元色。GLES 无固定管线，这里用第二套 UV + lightmap 纹理采样模拟。
static const char* fp_vertex_shader_src =
    "#version 300 es\n"
    "layout(location=0) in vec4 aPos;\n"
    "layout(location=1) in vec4 aColor;\n"
    "layout(location=2) in vec2 aUV;\n"
    "layout(location=3) in vec2 aUV1;\n"
    "uniform mat4 uMVP;\n"
    "out vec4 vColor;\n"
    "out vec2 vUV;\n"
    "out vec2 vUV1;\n"
    "void main() {\n"
    "    vColor = aColor;\n"
    "    vUV = aUV;\n"
    "    vUV1 = aUV1;\n"
    "    gl_Position = uMVP * aPos;\n"
    "}\n";

static const char* fp_fragment_shader_src =
    "#version 300 es\n"
    "precision mediump float;\n"
    "in vec4 vColor;\n"
    "in vec2 vUV;\n"
    "in vec2 vUV1;\n"
    "uniform sampler2D uTex;\n"
    "uniform bool uUseTex;\n"
    "uniform bool uUseColor;\n"
    "uniform vec4 uColor;\n"
    "uniform bool uSingle;\n"
    "uniform int uAlphaFunc;\n"
    "uniform float uAlphaRef;\n"
    "uniform bool uLightTint;\n"
    "uniform vec4 uLightColor;\n"
    "uniform sampler2D uLightMap;\n"
    "uniform int uUseLightMap;\n"
    "uniform vec2 uLightMapUV;\n"
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
    "    if(uLightTint) fc.rgb = mix(fc.rgb, uLightColor.rgb, uLightColor.a);\n"
    "    // 昼夜亮度：uUseLightMap=1 走顶点 lightmap UV（方块，0-15 亮度级 ÷16），\n"
    "    // =2 走常量 UV（实体/掉落物模型无 unit1 坐标，用最近方块的首顶点亮度）\n"
    "    if(uUseLightMap == 1) fc.rgb *= texture(uLightMap, vUV1 / 16.0).rgb;\n"
    "    else if(uUseLightMap == 2) fc.rgb *= texture(uLightMap, uLightMapUV).rgb;\n"
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
static GLint fp_lighttint_loc = -1;
static GLint fp_lightcolor_loc = -1;
static GLint fp_lightmap_loc = -1;
static GLint fp_uselightmap_loc = -1;
static GLint fp_lightmapuv_loc = -1;
static GLuint fp_vbo = 0;
static GLuint fp_vbo_pos = 0;
static GLuint fp_vbo_color = 0;
static GLuint fp_vbo_uv = 0;static GLuint fp_vao = 0;
// 显示列表回放客户端索引时使用的 scratch EBO（随 context 重建）
static GLuint fp_index_ebo = 0;
static GLsizeiptr fp_index_ebo_cap = 0;
static GLuint fp_saved_vao = 0;
// 当前实际绑定的 VAO（CPU 跟踪，替代每次绘制前的 glGetIntegerv 同步查询）。
// 内部所有 VAO 绑定走 fp_gl_bind_vao()，应用侧绑定经 glBindVertexArray
// 包装器通知 fp_set_bound_vao()。
static GLuint fp_app_vao = 0;
// GL_PRIMITIVE_RESTART_FIXED_INDEX 是否已启用（CPU 跟踪，替代 glIsEnabled 查询）
static bool fp_restart_enabled = false;
static bool fp_init_done = false;

// 默认 shader uniform 缓存：值没变就不重传（每次批次提交省约 10 次 glUniform）
static bool fp_uniforms_initialized = false;
static GLfloat fp_last_mvp[FP_MATRIX_SIZE];
static GLint fp_last_tex = -1;
static GLint fp_last_usetex = -1;
static GLint fp_last_usecolor = -1;
static GLfloat fp_last_color[4];
static GLint fp_last_single = -1;
static GLint fp_last_lighttint = -1;
static GLfloat fp_last_lightcolor[4];
static GLint fp_last_alphafunc = -1;
static GLfloat fp_last_alpharef = 0.0f;
static GLint fp_last_uselightmap = -1;
static GLint fp_last_lightmapuv_set = -1;
static GLfloat fp_last_lightmap_uv[2] = {0.5f, 0.5f};

// alpha test 状态（MC 1.12 文字渲染依赖 GL_ALPHA_TEST）
static bool fp_alpha_test = false;
static GLenum fp_alpha_test_func = 0x0207;   // GL_ALWAYS
static GLfloat fp_alpha_ref = 0.0f;
// 混合状态 CPU 跟踪：GL_BLEND 开关 + blend func。桌面 GL 默认 blend func
// 是 (ONE, ZERO)；MC 的 GlStateManager 总在 enableBlend 前显式设置，
// 这里主要用于批次快照的对比/应用/恢复。
static bool fp_blend_enabled = false;
static GLenum fp_blend_sfactor_rgb = GL_ONE;
static GLenum fp_blend_dfactor_rgb = GL_ZERO;
static GLenum fp_blend_sfactor_alpha = GL_ONE;
static GLenum fp_blend_dfactor_alpha = GL_ZERO;
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
// 即时模式批量合并：MC 1.12 FontRenderer 每个字形一个 glBegin/glEnd，如果
// 每个 glEnd 都立刻上传 VBO + glDrawArrays，F3 一开每帧会多出几千次绘制。
// 这里把“状态一致”的连续 glBegin/glEnd 攒进同一个 CPU 缓冲，遇到状态变化、
// 应用侧绘制、清屏、读回或帧切换（eglSwapBuffers）再一次性提交。
static bool fp_batch_active = false;
static GLenum fp_batch_mode = 0;
static GLuint fp_batch_texture = 0;
static bool fp_batch_texture_enabled = false;
static bool fp_batch_single = false;
static bool fp_batch_light_tint = false;
static GLfloat fp_batch_light_color[4];
static bool fp_batch_alpha_test = false;
static GLenum fp_batch_alpha_func = GL_ALWAYS;
static GLfloat fp_batch_alpha_ref = 0.0f;
// 批次快照的混合状态：F3 段内 drawRect 会在行间开关 GL_BLEND/换 blend
// func，提交时（popMatrix）必须用录制时的混合状态绘制，否则文字会不带
// 混合画出（黑块）。
static bool fp_batch_blend_enabled = false;
static GLenum fp_batch_blend_sfactor_rgb = GL_ONE;
static GLenum fp_batch_blend_dfactor_rgb = GL_ZERO;
static GLenum fp_batch_blend_sfactor_alpha = GL_ONE;
static GLenum fp_batch_blend_dfactor_alpha = GL_ZERO;
static GLfloat fp_batch_mvp[FP_MATRIX_SIZE];
// strip 图元批处理：每段 glBegin/glEnd 的起始顶点偏移，提交时用
// GL_PRIMITIVE_RESTART_FIXED_INDEX 断开，避免把两个字形的 strip 连成一条
// 大 strip 画出对角线。QUADS/LINES/TRIANGLES 不需要断开。
static GLsizei* fp_batch_prim_starts = NULL;
static GLsizei fp_batch_prim_count = 0;
static GLsizei fp_batch_prim_cap = 0;
// 批处理索引缓冲（strip 断开用），随 context 重建
static uint32_t* fp_batch_indices = NULL;
static GLsizei fp_batch_indices_cap = 0;
static GLuint fp_batch_ebo = 0;
// 下一次 fp_flush_immediate 是否走索引绘制（仅批处理 strip 时置位）
static bool fp_submit_indexed = false;
static const uint32_t* fp_submit_indices = NULL;
static GLsizei fp_submit_index_count = 0;
// QUADS 展开的可复用 scratch（避免每个 glEnd 一次 malloc/free）
static GLfloat* fp_quad_scratch = NULL;
static GLsizei fp_quad_scratch_cap = 0;
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
// unit1（光照贴图）纹理坐标客户端数组：MC 1.12 方块渲染在 unit1 上提供
// lightmap 亮度 UV（GL_SHORT，值 0-15），固定管线把它乘进片元色实现昼夜
// 明暗。以前被忽略导致夜晚像白天（夜视效果）。
static GLint fp_client_texcoord1_size = 0;
static GLenum fp_client_texcoord1_type = GL_FLOAT;
static GLsizei fp_client_texcoord1_stride = 0;
static const void* fp_client_texcoord1_ptr = NULL;
static GLint fp_client_texcoord1_abo = 0;
static bool fp_client_texcoord1_enabled = false;
// MathCode: 2026-08-11 GUI 变色修复——unit1 指针"本次设置过"标记。
// MC 桌面语义：WorldVertexBufferUploader.draw 每次绘制前设置全部数组指针，
// 带 lightmap 的格式会设 unit1 指针，无 lightmap 的格式（GUI 矩形）不设。
// 仅凭残留指针/off 检查无法区分（Tessellator 复用全局缓冲，残留指针的
// 偏移恰好在本次缓冲范围内），导致 GUI 矩形被 lightmap 误调制（变色）。
// 此标记在 unit1 glTexCoordPointer 时置位，绘制入口消费并清零：
// 只有"本次绘制前设置过 unit1 指针"才视为 lightmap 数据有效。
static bool fp_client_texcoord1_touched = false;
// 当前绘制路径是否真实提供了 unit1 坐标（方块渲染）。与
// fp_client_texcoord1_enabled 分开：MC 的残留状态（GUI/文字段）不会误开
// lightmap 采样，否则文字/矩形会被 lightmap 纹理调制变色。
static bool fp_client_uv1_active = false;
// 实体常量 lightmap：生物/玩家/掉落物模型走显示列表回放，顶点无 unit1
// 坐标（桌面固定管线同样如此）。绘制时用"最近一次方块渲染的首顶点
// lightmap UV"做常量采样，实体整体亮度随场景昼夜变化。
static bool fp_lightmap_const_active = false;
// 最近一次方块渲染首顶点的 lightmap UV（归一化 0-1），由
// fp_upload_client_arrays 在 CPU 路径拷贝（数据在绘制时刻仍有效）。
static bool fp_last_lightmap_uv_valid = false;
static GLfloat fp_last_lightmap_uv_snap[2] = {0.0f, 0.0f};
// 诊断：LTW_LIGHTMAP_TRACE=1 时打印 lightmap 相关状态变化
// （定位掉落物/手持物品昼夜亮度问题的临时探针，定位后移除）
static bool ltw_lightmap_trace = false;
static int ltw_lm_trace_state = -1;
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
// MathCode: GL_TEXTURE_2D 启用状态是每个纹理单元独立的（桌面语义）。
// 原版 disableLightmap() 会在光照贴图单元（unit1）上关纹理，若只用一个
// bool 会把 unit0 的启用状态也带成 false，导致 GUI（准心/装备栏等不显式
// enableTexture2D 的绘制）变成白色方块。
static bool fp_texture_enabled[FP_TEXENV_UNITS];
static GLuint fp_bound_texture = 0;
// unit1（光照贴图）绑定纹理，由 glBindTexture 包装器按活动单元 CPU 维护。
// 方块渲染时 MC 每帧把它绑到 lightmap 纹理（16x16 亮度渐变）。
static GLuint fp_bound_texture1 = 0;
// unit0 绑定是否由 glBindTexture 包装器可靠维护。为真时绘制路径不再向驱动
// 查询 GL_TEXTURE_BINDING_2D（每字形 2 次同步查询，F3 文字路径的主要开销）。
static bool fp_bound_texture_valid = false;

// MathCode: 纹理环境状态模拟（2026-08 新增，修复生物受伤红闪）
// 桌面 GL_TEXTURE_ENV：GLES 3.x 没有 glTexEnv/纹理
// 合并。MC 1.12 用单元1（光照贴图）的 GL_COMBINE/GL_INTERPOLATE +
// GL_TEXTURE_ENV_COLOR 实现受伤红闪（env color = (1,0,0,0.3)）与亮度调整。
// 这里只跟踪固定管线实际用到的单元（0=主纹理 1=光照贴图 2=亮度纹理），
// 默认 shader 在绘制时按需应用。
typedef struct {
    GLint env_mode;      // GL_TEXTURE_ENV_MODE（默认 GL_MODULATE）
    GLint combine_rgb;   // GL_COMBINE_RGB
    GLint src0_rgb;      // GL_SOURCE0_RGB
    GLint src2_rgb;      // GL_SOURCE2_RGB（插值因子来源）
    GLint operand2_rgb;  // GL_OPERAND2_RGB
    GLfloat color[4];    // GL_TEXTURE_ENV_COLOR
} fp_texenv_unit_t;
static fp_texenv_unit_t fp_texenv_state[FP_TEXENV_UNITS];
// unit1 是否处于“常量色插值”合并模式（受伤红闪/亮度色的生效条件）
static bool fp_light_tint = false;

// 单通道纹理格式缓存：GL_TEXTURE_INTERNAL_FORMAT 在纹理分配后不会变化，
// 没必要每次 glBindTexture/绘制都向驱动查询。直接映射槽 + 世代号，
// glTexImage2D/glCopyTexImage2D 等可能改变格式的入口统一使缓存失效。
#define FP_TEXFMT_CACHE_SIZE 64
typedef struct {
    GLuint texture;
    bool single_channel;
    uint32_t epoch;
    const void* ctx;             // 建立缓存的 context_t*，防跨 context 误用
} fp_texfmt_entry_t;
static fp_texfmt_entry_t fp_texfmt_cache[FP_TEXFMT_CACHE_SIZE];
static uint32_t fp_texfmt_epoch = 1;

// 前向声明
static void fp_set_default_uniforms(void);
static void fp_refresh_bound_texture(void);
static void fp_batch_begin(void);
static bool fp_batch_state_matches(GLenum mode);
void fp_flush_immediate_batch(void);

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
    // 批处理攒满后先提交再继续，避免顶点被静默丢弃；QUADS 提交时要展开成
    // 2 倍三角形，阈值按展开后仍不超 FP_MAX_VERTICES 计算。
    GLsizei batch_limit = (fp_immediate_mode == GL_QUADS)
                              ? (GLsizei)((GLsizei)(FP_MAX_VERTICES / 6) * 4)
                              : FP_MAX_VERTICES;
    if(fp_batch_active && fp_immediate_count >= batch_limit) {
        fp_flush_immediate_batch();
        fp_batch_begin();
        // 当前这段 glBegin/glEnd 跨了批次边界，新批次的第一个顶点偏移是 0
        if(fp_batch_prim_cap == 0) {
            GLsizei* nb = (GLsizei*)realloc(NULL, 64 * sizeof(GLsizei));
            if(nb) {
                fp_batch_prim_starts = nb;
                fp_batch_prim_cap = 64;
            }
        }
        if(fp_batch_prim_cap > 0) {
            fp_batch_prim_starts[0] = 0;
            fp_batch_prim_count = 1;
        }
    }
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
    fp_lighttint_loc = es3_functions.glGetUniformLocation(prog, "uLightTint");
    fp_lightcolor_loc = es3_functions.glGetUniformLocation(prog, "uLightColor");
    fp_lightmap_loc = es3_functions.glGetUniformLocation(prog, "uLightMap");
    fp_uselightmap_loc = es3_functions.glGetUniformLocation(prog, "uUseLightMap");
    fp_lightmapuv_loc = es3_functions.glGetUniformLocation(prog, "uLightMapUV");

    es3_functions.glGenBuffers(1, &fp_vbo);
    es3_functions.glGenBuffers(1, &fp_vbo_pos);
    es3_functions.glGenBuffers(1, &fp_vbo_color);
    es3_functions.glGenBuffers(1, &fp_vbo_uv);
    es3_functions.glGenVertexArrays(1, &fp_vao);
    es3_functions.glGenBuffers(1, &fp_index_ebo);
    es3_functions.glGenBuffers(1, &fp_batch_ebo);

    es3_functions.glDeleteShader(vs);
    es3_functions.glDeleteShader(fs);
}

// 绑定 VAO 并同步 CPU 跟踪（替代绘制前的 glGetIntegerv(GL_VERTEX_ARRAY_BINDING)）
static void fp_gl_bind_vao(GLuint vao) {
    es3_functions.glBindVertexArray(vao);
    fp_app_vao = vao;
}

// 提交即时模式顶点
static void fp_flush_immediate(void) {
    if(!fp_immediate_active || fp_immediate_count == 0) return;
    fp_ensure_program();
    if(!fp_program) { fp_immediate_count = 0; return; }

    // unit0 绑定由 glBindTexture 包装器 CPU 维护；只有缓存失效（上下文重建、
    // 防御性入口）才向驱动查询，避免每个字形 2 次同步 glGetIntegerv。
    if(!fp_bound_texture_valid) fp_refresh_bound_texture();

    GLenum mode = fp_immediate_mode;
    GLsizei count = fp_immediate_count;

    // 先保存应用绑定状态，并把 fp_vbo 绑定为当前 ARRAY_BUFFER，
    // glBufferData 操作的是"当前绑定的 buffer"，必须先绑定。
    GLint old_vao = (GLint)fp_app_vao;
    GLint old_array_buffer = current_context ? (GLint)current_context->bound_buffers[0] : 0;
    GLint old_program = current_context ? (GLint)current_context->program : 0;
    glBindBuffer(GL_ARRAY_BUFFER, fp_vbo);

    // QUADS -> TRIANGLES：复制顶点展开
    if(mode == GL_QUADS && count >= 4 && (count & 3) == 0) {
        GLsizei quads = count >> 2;
        GLsizei tri_count = quads * 6;
        if(tri_count > FP_MAX_VERTICES) {
            glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_array_buffer);
            fp_immediate_count = 0;
            return;
        }
        if((size_t)tri_count > (size_t)fp_quad_scratch_cap) {
            GLfloat* nb = (GLfloat*)realloc(fp_quad_scratch, (size_t)tri_count * FP_VERTEX_BYTES);
            if(!nb) {
                glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_array_buffer);
                fp_immediate_count = 0;
                return;
            }
            fp_quad_scratch = nb;
            fp_quad_scratch_cap = tri_count;
        }
        GLfloat* expanded = fp_quad_scratch;
        if(!expanded) {
            glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_array_buffer);
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
        count = tri_count;
        mode = GL_TRIANGLES;
    } else {
        es3_functions.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)fp_immediate_count * FP_VERTEX_BYTES,
                                   fp_immediate_vertices, GL_STREAM_DRAW);
    }

    // 绑定默认 program + 属性（使用私有 VAO，避免污染应用绑定的 VAO）
    fp_gl_bind_vao(fp_vao);
    es3_functions.glUseProgram(fp_program);
    if(current_context) current_context->program = fp_program;
    // 即时模式（GUI 文字/矩形）不带 unit1 坐标：禁用残留的 UV1 属性并
    // 关闭 lightmap 采样（含实体常量分支），防止文字被 lightmap 调制
    fp_client_uv1_active = false;
    fp_lightmap_const_active = false;
    fp_set_default_uniforms();
    {
        es3_functions.glEnableVertexAttribArray(FP_ATTR_POS);
        es3_functions.glEnableVertexAttribArray(FP_ATTR_COLOR);
        es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
        es3_functions.glDisableVertexAttribArray(FP_ATTR_UV1);
        es3_functions.glVertexAttribPointer(FP_ATTR_POS, 3, GL_FLOAT, GL_FALSE, FP_VERTEX_BYTES, NULL);
        es3_functions.glVertexAttribPointer(FP_ATTR_COLOR, 4, GL_FLOAT, GL_FALSE, FP_VERTEX_BYTES,
                                            (const void*)(3 * sizeof(GLfloat)));
        es3_functions.glVertexAttribPointer(FP_ATTR_UV, 2, GL_FLOAT, GL_FALSE, FP_VERTEX_BYTES,
                                            (const void*)(7 * sizeof(GLfloat)));
    }

    if(fp_submit_indexed && fp_submit_indices && fp_submit_index_count > 0) {
        // 批处理 strip：把多段 TRIANGLE_STRIP/LINE_STRIP 用固定重启索引
        // （0xFFFFFFFF）连成一次 glDrawElements，段与段之间不会生成多余三角形。
        es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fp_batch_ebo);
        es3_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                                   (GLsizeiptr)fp_submit_index_count * sizeof(uint32_t),
                                   fp_submit_indices, GL_STREAM_DRAW);
        if(!fp_restart_enabled) {
            es3_functions.glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
            fp_restart_enabled = true;
        }
        es3_functions.glDrawElements(fp_immediate_mode, fp_submit_index_count,
                                     GL_UNSIGNED_INT, NULL);
        if(fp_restart_enabled) {
            es3_functions.glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
            fp_restart_enabled = false;
        }
        // 清掉 fp_vao 上的 EBO，避免影响后续非索引路径
        es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    } else {
        es3_functions.glDrawArrays(mode, 0, count);
    }

    // 恢复状态
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_array_buffer);
    fp_gl_bind_vao((GLuint)old_vao);
    // MathCode: 即时模式路径绑定/解绑了 VAO，必须同步内部跟踪值，
    // 否则显示列表合并路径会误以为自己的 VAO 仍处于绑定状态而跳过绑定，
    // 用应用侧 VAO（可能没有 EBO）执行 glDrawElements → GL_INVALID_OPERATION。
    dl_current_vao = (GLuint)old_vao;
    if(old_program != (GLint)fp_program) {
        es3_functions.glUseProgram((GLuint)old_program);
        if(current_context) current_context->program = (GLuint)old_program;
    }

    // 即时模式路径定制过 uniforms（uUseColor 等），批量回放中的
    // 后续缓存绘制需重设 uniforms，避免沿用即时模式的设置
    if(dl_replay_active) dl_replay_dirty = true;

    fp_immediate_count = 0;
}

// ---- 即时模式批量合并 ----
// 开始一个批次：快照当前绘制状态。同一批内的每个字形共享这份状态，
// 提交时用它还原，而不是用提交那一刻的矩阵/纹理/alpha 状态。
static void fp_batch_begin(void) {
    fp_batch_active = true;
    fp_batch_prim_count = 0;
    fp_batch_mode = fp_immediate_mode;
    fp_batch_texture = fp_bound_texture;
    fp_batch_texture_enabled = fp_texture_enabled[0];
    fp_batch_single = fp_bound_single_channel;
    fp_batch_light_tint = fp_light_tint;
    memcpy(fp_batch_light_color, fp_texenv_state[1].color,
           sizeof(fp_batch_light_color));
    fp_batch_alpha_test = fp_alpha_test;
    fp_batch_alpha_func = fp_alpha_test_func;
    fp_batch_alpha_ref = fp_alpha_ref;
    fp_batch_blend_enabled = fp_blend_enabled;
    fp_batch_blend_sfactor_rgb = fp_blend_sfactor_rgb;
    fp_batch_blend_dfactor_rgb = fp_blend_dfactor_rgb;
    fp_batch_blend_sfactor_alpha = fp_blend_sfactor_alpha;
    fp_batch_blend_dfactor_alpha = fp_blend_dfactor_alpha;
    fp_mat_mul(fp_batch_mvp,
               fp_matrix_stack[FP_MATRIX_PROJECTION][fp_matrix_top[FP_MATRIX_PROJECTION]],
               fp_matrix_stack[FP_MATRIX_MODELVIEW][fp_matrix_top[FP_MATRIX_MODELVIEW]]);
}

// 当前状态与批次快照是否一致（不一致则下一个 glBegin 必须拆分批次）
static bool fp_batch_state_matches(GLenum mode) {
    return fp_batch_mode == mode &&
           fp_batch_texture == fp_bound_texture &&
           fp_batch_texture_enabled == fp_texture_enabled[0] &&
           fp_batch_single == fp_bound_single_channel &&
           fp_batch_light_tint == fp_light_tint &&
           fp_batch_alpha_test == fp_alpha_test &&
           fp_batch_alpha_func == fp_alpha_test_func &&
           fp_batch_alpha_ref == fp_alpha_ref &&
           fp_batch_blend_enabled == fp_blend_enabled &&
           fp_batch_blend_sfactor_rgb == fp_blend_sfactor_rgb &&
           fp_batch_blend_dfactor_rgb == fp_blend_dfactor_rgb &&
           fp_batch_blend_sfactor_alpha == fp_blend_sfactor_alpha &&
           fp_batch_blend_dfactor_alpha == fp_blend_dfactor_alpha &&
           fp_batch_light_color[0] == fp_texenv_state[1].color[0] &&
           fp_batch_light_color[1] == fp_texenv_state[1].color[1] &&
           fp_batch_light_color[2] == fp_texenv_state[1].color[2] &&
           fp_batch_light_color[3] == fp_texenv_state[1].color[3];
}

// 一次性提交当前批次。应用侧任何可能改变绘制顺序/目标/状态的入口
// （glDrawArrays、glClear、glReadPixels、eglSwapBuffers 等）都必须先调用。
void fp_flush_immediate_batch(void) {
    if(!fp_batch_active) return;
    if(fp_immediate_count == 0) {
        // 空批次：直接作废，避免旧状态快照被后续不同状态的图元沿用
        fp_batch_active = false;
        fp_batch_prim_count = 0;
        return;
    }
    fp_ensure_program();
    if(!fp_program) {
        fp_immediate_count = 0;
        fp_batch_active = false;
        return;
    }

    // 保存应用当前状态，用批次快照绘制后恢复
    GLuint saved_tex = fp_bound_texture;
    bool saved_single = fp_bound_single_channel;
    bool saved_tex_enabled = fp_texture_enabled[0];
    bool saved_light_tint = fp_light_tint;
    GLfloat saved_light_color[4];
    memcpy(saved_light_color, fp_texenv_state[1].color,
           sizeof(saved_light_color));
    bool saved_alpha_test = fp_alpha_test;
    GLenum saved_alpha_func = fp_alpha_test_func;
    GLfloat saved_alpha_ref = fp_alpha_ref;
    bool saved_blend_enabled = fp_blend_enabled;
    GLenum saved_blend_sfactor_rgb = fp_blend_sfactor_rgb;
    GLenum saved_blend_dfactor_rgb = fp_blend_dfactor_rgb;
    GLenum saved_blend_sfactor_alpha = fp_blend_sfactor_alpha;
    GLenum saved_blend_dfactor_alpha = fp_blend_dfactor_alpha;
    GLfloat saved_proj[FP_MATRIX_SIZE];
    GLfloat saved_model[FP_MATRIX_SIZE];
    memcpy(saved_proj,
           fp_matrix_stack[FP_MATRIX_PROJECTION][fp_matrix_top[FP_MATRIX_PROJECTION]],
           sizeof(saved_proj));
    memcpy(saved_model,
           fp_matrix_stack[FP_MATRIX_MODELVIEW][fp_matrix_top[FP_MATRIX_MODELVIEW]],
           sizeof(saved_model));

    // 应用批次快照：绑定录制时的纹理，并把矩阵栈临时变成“MVP=快照”
    es3_functions.glBindTexture(GL_TEXTURE_2D, fp_batch_texture);
    fp_bound_texture = fp_batch_texture;
    fp_bound_single_channel = fp_batch_single;
    fp_bound_texture_valid = true;
    fp_texture_enabled[0] = fp_batch_texture_enabled;
    fp_light_tint = fp_batch_light_tint;
    memcpy(fp_texenv_state[1].color, fp_batch_light_color,
           sizeof(fp_batch_light_color));
    fp_alpha_test = fp_batch_alpha_test;
    fp_alpha_test_func = fp_batch_alpha_func;
    fp_alpha_ref = fp_batch_alpha_ref;
    // 混合状态也按批次快照还原（GL 实际状态 + CPU 跟踪同步）：
    // F3 段内录制文字时 GL_BLEND 是关闭的，但提交时刻（popMatrix）可能
    // 已被行间 drawRect 的 enableBlend 打开，若不还原，文字会带着混合画。
    if(fp_batch_blend_enabled) es3_functions.glEnable(GL_BLEND);
    else es3_functions.glDisable(GL_BLEND);
    es3_functions.glBlendFuncSeparate(fp_batch_blend_sfactor_rgb,
                                      fp_batch_blend_dfactor_rgb,
                                      fp_batch_blend_sfactor_alpha,
                                      fp_batch_blend_dfactor_alpha);
    fp_blend_enabled = fp_batch_blend_enabled;
    fp_blend_sfactor_rgb = fp_batch_blend_sfactor_rgb;
    fp_blend_dfactor_rgb = fp_batch_blend_dfactor_rgb;
    fp_blend_sfactor_alpha = fp_batch_blend_sfactor_alpha;
    fp_blend_dfactor_alpha = fp_batch_blend_dfactor_alpha;
    fp_mat_identity(fp_matrix_stack[FP_MATRIX_PROJECTION][fp_matrix_top[FP_MATRIX_PROJECTION]]);
    memcpy(fp_matrix_stack[FP_MATRIX_MODELVIEW][fp_matrix_top[FP_MATRIX_MODELVIEW]],
           fp_batch_mvp, sizeof(fp_batch_mvp));

    // strip 图元：给每段 glBegin/glEnd 生成 段内索引 + 0xFFFFFFFF 重启索引，
    // 一次 glDrawElements 画完整批，段与段之间不会连出多余三角形。
    bool use_restart = (fp_batch_mode == GL_TRIANGLE_STRIP ||
                        fp_batch_mode == GL_LINE_STRIP) &&
                       fp_batch_prim_starts && fp_batch_prim_count > 1 &&
                       fp_batch_ebo != 0;
    if(use_restart) {
        GLsizei total = fp_immediate_count;
        GLsizei need = total + fp_batch_prim_count;
        if(need > fp_batch_indices_cap) {
            GLsizei newcap = need > 4096 ? need : 4096;
            uint32_t* nb = (uint32_t*)realloc(fp_batch_indices,
                                              (size_t)newcap * sizeof(uint32_t));
            if(nb) {
                fp_batch_indices = nb;
                fp_batch_indices_cap = newcap;
            }
        }
        if(fp_batch_indices && fp_batch_indices_cap >= need) {
            size_t o = 0;
            for(GLsizei p = 0; p < fp_batch_prim_count; p++) {
                GLsizei start = fp_batch_prim_starts[p];
                GLsizei end = (p + 1 < fp_batch_prim_count)
                                  ? fp_batch_prim_starts[p + 1]
                                  : total;
                if(start < 0) start = 0;
                if(start > total) start = total;
                if(end > total) end = total;
                if(start > end) start = end;
                for(GLsizei i = start; i < end && o < (size_t)need; i++) {
                    fp_batch_indices[o++] = (uint32_t)i;
                }
                if(o < (size_t)need) fp_batch_indices[o++] = 0xFFFFFFFFu;
            }
            fp_submit_indexed = true;
            fp_submit_indices = fp_batch_indices;
            fp_submit_index_count = (GLsizei)o;
        }
    }

    // 提交。fp_immediate_active 置 true 让默认 shader 使用顶点色
    // （uUseColor=1），字形颜色按录制时的 glColor4f 逐顶点保留。
    GLenum saved_mode = fp_immediate_mode;
    bool saved_active = fp_immediate_active;
    fp_immediate_mode = fp_batch_mode;
    fp_immediate_active = true;
    fp_flush_immediate();
    fp_immediate_mode = saved_mode;
    fp_immediate_active = saved_active;
    fp_submit_indexed = false;
    fp_submit_indices = NULL;
    fp_submit_index_count = 0;

    // 恢复应用状态。GL 实际纹理绑定也要恢复：MC 的 GlStateManager 缓存绑定，
    // 如果这里把字形纹理留在 unit0，下一帧它会以为没换纹理而直接绘制，
    // 导致世界/HUD 采样到字体贴图。
    es3_functions.glBindTexture(GL_TEXTURE_2D, saved_tex);
    fp_bound_texture = saved_tex;
    fp_bound_single_channel = saved_single;
    fp_bound_texture_valid = true;
    fp_texture_enabled[0] = saved_tex_enabled;
    fp_light_tint = saved_light_tint;
    memcpy(fp_texenv_state[1].color, saved_light_color,
           sizeof(saved_light_color));
    fp_alpha_test = saved_alpha_test;
    fp_alpha_test_func = saved_alpha_func;
    fp_alpha_ref = saved_alpha_ref;
    // 恢复应用侧混合状态（GL + CPU）
    if(saved_blend_enabled) es3_functions.glEnable(GL_BLEND);
    else es3_functions.glDisable(GL_BLEND);
    es3_functions.glBlendFuncSeparate(saved_blend_sfactor_rgb,
                                      saved_blend_dfactor_rgb,
                                      saved_blend_sfactor_alpha,
                                      saved_blend_dfactor_alpha);
    fp_blend_enabled = saved_blend_enabled;
    fp_blend_sfactor_rgb = saved_blend_sfactor_rgb;
    fp_blend_dfactor_rgb = saved_blend_dfactor_rgb;
    fp_blend_sfactor_alpha = saved_blend_sfactor_alpha;
    fp_blend_dfactor_alpha = saved_blend_dfactor_alpha;
    memcpy(fp_matrix_stack[FP_MATRIX_PROJECTION][fp_matrix_top[FP_MATRIX_PROJECTION]],
           saved_proj, sizeof(saved_proj));
    memcpy(fp_matrix_stack[FP_MATRIX_MODELVIEW][fp_matrix_top[FP_MATRIX_MODELVIEW]],
           saved_model, sizeof(saved_model));

    fp_batch_active = false;
    fp_batch_prim_count = 0;
}

bool fp_immediate_batch_pending(void) {
    return fp_batch_active && fp_immediate_count > 0;
}

// glBindVertexArray 包装器通知应用侧 VAO 绑定（CPU 跟踪，省驱动查询）
void fp_set_bound_vao(GLuint vao) {
    fp_app_vao = vao;
}

// glEnable/glDisable(GL_PRIMITIVE_RESTART_FIXED_INDEX) 包装器通知，
// 批处理 strip 提交时用 CPU 标志代替 glIsEnabled 查询
void fp_set_restart_enabled(bool enabled) {
    fp_restart_enabled = enabled;
}

// ---- 矩阵 API ----
void fp_init(void) {
    // 上下文重建时（FCL 偶尔会重建 EGL context），GL 对象是 context 私有的，
    // 必须重置句柄并让 fp_ensure_program 在新 context 里重建。
    // MathCode: 实验开关优先级 = LTW_DL_MERGE 环境变量 > 共享配置 dlMerge > 默认开。
    if(getenv("LTW_DL_MERGE")) {
        dl_merge_enabled = env_istrue("LTW_DL_MERGE");
    } else {
        ltw_config_init();
        // MathCode: 2026-08-08 修复合并绘制 1282（Post render）与生物贴图错乱：
        // 合并路径每次绘制前强制绑定自己的 VAO+EBO，并修复内部 VAO 跟踪被
        // 即时模式/默认 program 路径解绑后不同步的问题；默认保持开启。
        dl_merge_enabled = ltw_config_get_bool("dlMerge", true);
    }
    fp_init_done = false;
    fp_program = 0;
    fp_mvp_loc = fp_tex_loc = fp_usetex_loc = fp_usecolor_loc = -1;
    fp_lighttint_loc = fp_lightcolor_loc = -1;
    fp_lightmap_loc = fp_uselightmap_loc = fp_lightmapuv_loc = -1;
    fp_color_loc = fp_alphafunc_loc = fp_alpharef_loc = fp_single_loc = -1;
    fp_vbo = fp_vbo_pos = fp_vbo_color = fp_vbo_uv = fp_vao = 0;
    fp_index_ebo = 0;
    fp_index_ebo_cap = 0;
    fp_batch_ebo = 0;
    fp_saved_vao = 0;
    fp_app_vao = 0;
    fp_restart_enabled = false;
    fp_uniforms_initialized = false;
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
    free(fp_batch_prim_starts);
    fp_batch_prim_starts = NULL;
    fp_batch_prim_count = 0;
    fp_batch_prim_cap = 0;
    free(fp_batch_indices);
    fp_batch_indices = NULL;
    fp_batch_indices_cap = 0;
    fp_submit_indexed = false;
    fp_submit_indices = NULL;
    fp_submit_index_count = 0;
    free(fp_quad_scratch);
    fp_quad_scratch = NULL;
    fp_quad_scratch_cap = 0;
    fp_current_color[0] = fp_current_color[1] = fp_current_color[2] = fp_current_color[3] = 1.0f;
    fp_current_texcoord[0] = fp_current_texcoord[1] = 0.0f;
    fp_current_texcoord[2] = 0.0f; fp_current_texcoord[3] = 1.0f;
    fp_client_vertex_enabled = fp_client_texcoord_enabled = fp_client_color_enabled = fp_client_normal_enabled = false;
    fp_client_texcoord1_enabled = false;
    fp_client_texcoord1_size = 0;
    fp_client_texcoord1_type = GL_FLOAT;
    fp_client_texcoord1_stride = 0;
    fp_client_texcoord1_ptr = NULL;
    fp_client_texcoord1_abo = 0;
    fp_client_texcoord1_touched = false;
    fp_client_uv1_active = false;
    fp_lightmap_const_active = false;
    fp_last_lightmap_uv_valid = false;
    fp_last_lightmap_uv_snap[0] = fp_last_lightmap_uv_snap[1] = 0.0f;
    // MathCode: 探针开关优先级 = LTW_LIGHTMAP_TRACE 环境变量 > 共享配置 lightmapTrace > 默认关。
    if(getenv("LTW_LIGHTMAP_TRACE")) {
        ltw_lightmap_trace = env_istrue("LTW_LIGHTMAP_TRACE");
    } else {
        ltw_config_init();
        ltw_lightmap_trace = ltw_config_get_bool("lightmapTrace", false);
    }
    ltw_lm_trace_state = -1;
    fp_client_active_texture = GL_TEXTURE0;
    fp_active_texture = GL_TEXTURE0;
    fp_texture_enabled[0] = fp_texture_enabled[1] = fp_texture_enabled[2] = false;
    fp_bound_texture = 0;
    fp_bound_texture1 = 0;
    fp_bound_texture_valid = false;
    fp_batch_active = false;
    for(int i = 0; i < FP_TEXENV_UNITS; i++) {
        fp_texenv_state[i].env_mode = GL_MODULATE;
        fp_texenv_state[i].combine_rgb = GL_MODULATE;
        fp_texenv_state[i].src0_rgb = GL_TEXTURE;
        fp_texenv_state[i].src2_rgb = GL_CONSTANT;
        fp_texenv_state[i].operand2_rgb = GL_SRC_ALPHA;
        fp_texenv_state[i].color[0] = fp_texenv_state[i].color[1] = 1.0f;
        fp_texenv_state[i].color[2] = fp_texenv_state[i].color[3] = 1.0f;
    }
    fp_light_tint = false;
    fp_blend_enabled = false;
    fp_blend_sfactor_rgb = GL_ONE;
    fp_blend_dfactor_rgb = GL_ZERO;
    fp_blend_sfactor_alpha = GL_ONE;
    fp_blend_dfactor_alpha = GL_ZERO;

    // 显示列表 CPU 快照保留，但 GL 对象句柄随旧 context 失效，清零后惰性重建
    fp_dl_reset_caches();
    // 纹理格式缓存同样随 context 失效
    fp_texfmt_epoch++;
    if(fp_texfmt_epoch == 0) fp_texfmt_epoch = 1;
    dl_replay_active = false;
    dl_replay_dirty = true;
    dl_current_vao = 0;
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
// 允许合并进同一个批次的图元类型。TRIANGLE_STRIP/LINE_STRIP 提交时用
// GL_PRIMITIVE_RESTART_FIXED_INDEX 断开；TRIANGLE_FAN/POLYGON 等不合并
// （各自独立批次，绘制顺序不变）。
static bool fp_batch_can_append(GLenum mode) {
    return mode == GL_POINTS || mode == GL_LINES || mode == GL_TRIANGLES ||
           mode == GL_QUADS || mode == GL_TRIANGLE_STRIP || mode == GL_LINE_STRIP;
}

void fp_begin(GLenum mode) {
    // 显示列表编译期间不批处理：顶点按原逻辑逐段录制进列表
    if(dl_is_compiling()) {
        fp_immediate_mode = mode;
        fp_immediate_active = true;
        fp_immediate_count = 0;
        return;
    }
    // 绑定缓存失效时先补一次查询，避免用旧记忆值开批次
    if(!fp_bound_texture_valid) fp_refresh_bound_texture();
    // 状态变了就先把上一批提交，再以当前状态开新批
    if(fp_batch_active && (!fp_batch_state_matches(mode) || !fp_batch_can_append(mode))) {
        fp_flush_immediate_batch();
    }
    fp_immediate_mode = mode;
    fp_immediate_active = true;
    if(!fp_batch_active) {
        fp_immediate_count = 0;
        fp_batch_begin();
    }
    // 记录本段图元的起始顶点：strip 批处理用重启索引断开，需要知道段边界
    if(fp_batch_prim_count >= fp_batch_prim_cap) {
        GLsizei newcap = fp_batch_prim_cap == 0 ? 64 : fp_batch_prim_cap * 2;
        GLsizei* nb = (GLsizei*)realloc(fp_batch_prim_starts,
                                        (size_t)newcap * sizeof(GLsizei));
        if(nb) {
            fp_batch_prim_starts = nb;
            fp_batch_prim_cap = newcap;
        }
    }
    if(fp_batch_prim_count < fp_batch_prim_cap) {
        fp_batch_prim_starts[fp_batch_prim_count++] = fp_immediate_count;
    }
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
    }
    // 非编译路径：顶点已经留在 fp_immediate_vertices 里，批次由
    // fp_flush_immediate_batch() 在状态变化/应用绘制/帧切换时提交。
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
    if(fp_client_active_texture == GL_TEXTURE0) {
        fp_client_texcoord_size = size; fp_client_texcoord_type = type;
        fp_client_texcoord_stride = stride; fp_client_texcoord_ptr = pointer;
        fp_client_texcoord_abo = fp_current_abo();
    } else if(fp_client_active_texture == GL_TEXTURE1) {
        // unit1 = 光照贴图坐标（MC 1.12 方块渲染的昼夜亮度），单独记录，
        // 不覆盖 unit0 的图集 UV。
        fp_client_texcoord1_size = size; fp_client_texcoord1_type = type;
        fp_client_texcoord1_stride = stride; fp_client_texcoord1_ptr = pointer;
        fp_client_texcoord1_abo = fp_current_abo();
        fp_client_texcoord1_touched = true;  // MathCode: 本次绘制 lightmap 数据有效标记
    }
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
            else if(fp_client_active_texture == GL_TEXTURE1) fp_client_texcoord1_enabled = true;
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
            else if(fp_client_active_texture == GL_TEXTURE1) fp_client_texcoord1_enabled = false;
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
static int fp_texenv_unit_index(GLenum unit) {
    switch(unit) {
        case GL_TEXTURE0: return 0;
        case GL_TEXTURE1: return 1;
        case GL_TEXTURE2: return 2;
        default: return -1;
    }
}

// MathCode: 按当前活动纹理单元记录 GL_TEXTURE_2D 启用状态
// （GL_TEXTURE_2D 是 per-unit 的桌面状态，unit1 的 disable 不能影响 unit0）。
void fp_set_texture_enabled(bool enabled) {
    int idx = fp_texenv_unit_index(fp_active_texture);
    if(idx >= 0 && idx < FP_TEXENV_UNITS) fp_texture_enabled[idx] = enabled;
}

// 显示列表回放专用：TEXTURE_ENABLE op 录制时来自 unit0 的
// glEnable/glDisable(GL_TEXTURE_2D)，回放时固定写 unit0 槽位。
void fp_set_unit0_texture_enabled(bool enabled) {
    fp_texture_enabled[0] = enabled;
}
void fp_set_active_texture(GLuint unit) {
    fp_active_texture = unit;
    if(unit == GL_TEXTURE0) {
        fp_refresh_bound_texture();
    }
}

// MathCode: 只有 unit1（光照贴图）的“GL_INTERPOLATE + 常量色 + SRC_ALPHA 因子”组合
// 才是受伤红闪/亮度色，推导成 shader 能用的开关。
static void fp_texenv_recompute(void) {
    const fp_texenv_unit_t* u = &fp_texenv_state[1];
    fp_light_tint = (u->env_mode == GL_COMBINE &&
                     u->combine_rgb == GL_INTERPOLATE &&
                     u->src0_rgb == GL_CONSTANT &&
                     u->src2_rgb == GL_CONSTANT &&
                     u->operand2_rgb == GL_SRC_ALPHA);
}

// MathCode: 桌面 glTexEnv* 状态入口（GLES 无对应物，固定管线内部模拟）
void fp_texenv(GLenum target, GLenum pname, const GLfloat* fparams, const GLint* iparams, bool is_int) {
    if(!current_context || target != GL_TEXTURE_ENV) return;
    int idx = fp_texenv_unit_index(fp_active_texture);
    if(idx < 0 || idx >= FP_TEXENV_UNITS) return;
    fp_texenv_unit_t* u = &fp_texenv_state[idx];
    switch(pname) {
        case GL_TEXTURE_ENV_MODE:
            u->env_mode = is_int ? *iparams : (GLint)*fparams;
            break;
        case GL_TEXTURE_ENV_COLOR:
            if(is_int) {
                for(int i = 0; i < 4; i++) u->color[i] = (GLfloat)iparams[i];
            } else {
                for(int i = 0; i < 4; i++) u->color[i] = fparams[i];
            }
            break;
        case GL_COMBINE_RGB:
            u->combine_rgb = is_int ? *iparams : (GLint)*fparams;
            break;
        case GL_SOURCE0_RGB:
            u->src0_rgb = is_int ? *iparams : (GLint)*fparams;
            break;
        case GL_SOURCE2_RGB:
            u->src2_rgb = is_int ? *iparams : (GLint)*fparams;
            break;
        case GL_OPERAND2_RGB:
            u->operand2_rgb = is_int ? *iparams : (GLint)*fparams;
            break;
        default:
            break;
    }
    if(idx == 1) fp_texenv_recompute();
}

void fp_notify_texture_bind(void) {
    if(fp_active_texture == GL_TEXTURE0) fp_refresh_bound_texture();
}

// ---- alpha test ----
void fp_set_alpha_test(bool enabled) { fp_alpha_test = enabled; }
void fp_alpha_func(GLenum func, GLfloat ref) { fp_alpha_test_func = func; fp_alpha_ref = ref; }

void fp_set_blend_enabled(bool enabled) { fp_blend_enabled = enabled; }
void fp_set_blend_func(GLenum sfactor, GLenum dfactor) {
    fp_blend_sfactor_rgb = fp_blend_sfactor_alpha = sfactor;
    fp_blend_dfactor_rgb = fp_blend_dfactor_alpha = dfactor;
}
void fp_set_blend_func_separate(GLenum sfactorRGB, GLenum dfactorRGB,
                                GLenum sfactorAlpha, GLenum dfactorAlpha) {
    fp_blend_sfactor_rgb = sfactorRGB;
    fp_blend_dfactor_rgb = dfactorRGB;
    fp_blend_sfactor_alpha = sfactorAlpha;
    fp_blend_dfactor_alpha = dfactorAlpha;
}

// ---- 绘制挂钩 ----
// 画之前直接查“当前绑定在活动纹理单元上的 2D 纹理”，而不是依赖
// glBindTexture 包装器里的记忆值。MC 的 GlStateManager 会缓存纹理绑定，
// 连续绘制同一字形时可能根本不调用 glBindTexture，记忆值一旦被其他
// 单元/路径覆盖，字就会以“无纹理”的纯色方块画出来。
// 现在 unit0 的绑定由 glBindTexture 包装器用 CPU 维护（fp_notify_texture_bind_tex），
// 这里的查询只作为兜底：活动单元切回 unit0、显示列表批量回放等防御性入口。

// 纹理格式缓存查询：命中返回 true 并写出是否单通道
static bool fp_texfmt_lookup(GLuint tex, bool* single) {
    if(!current_context) return false;
    unsigned idx = (unsigned)(tex * 2654435761u) & (FP_TEXFMT_CACHE_SIZE - 1);
    fp_texfmt_entry_t* e = &fp_texfmt_cache[idx];
    if(e->texture == tex && e->epoch == fp_texfmt_epoch &&
       e->ctx == (const void*)current_context) {
        *single = e->single_channel;
        return true;
    }
    return false;
}

// 纹理格式缓存写入（直接映射槽，冲突时覆盖，最坏只是多一次驱动查询）
static void fp_texfmt_store(GLuint tex, bool single) {
    if(!current_context) return;
    unsigned idx = (unsigned)(tex * 2654435761u) & (FP_TEXFMT_CACHE_SIZE - 1);
    fp_texfmt_entry_t* e = &fp_texfmt_cache[idx];
    e->texture = tex;
    e->single_channel = single;
    e->epoch = fp_texfmt_epoch;
    e->ctx = (const void*)current_context;
}

// 解析纹理是否单通道（R8/RED/R16F/R32F）：缓存未命中才查询驱动
static bool fp_texfmt_resolve(GLuint tex) {
    bool single = false;
    if(fp_texfmt_lookup(tex, &single)) return single;
    GLint fmt = 0;
    es3_functions.glGetTexLevelParameteriv(GL_TEXTURE_2D, 0,
                                           GL_TEXTURE_INTERNAL_FORMAT, &fmt);
    single = (fmt == GL_R8 || fmt == GL_RED || fmt == GL_R16F || fmt == GL_R32F);
    fp_texfmt_store(tex, single);
    return single;
}

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
    fp_bound_single_channel = (tex != 0) && fp_texfmt_resolve((GLuint)tex);
    fp_bound_texture_valid = true;
    if(old_active != GL_TEXTURE0) es3_functions.glActiveTexture((GLenum)old_active);
    // 同步内部活动单元跟踪（refresh 结束后活动单元即 old_active）
    fp_active_texture = (GLenum)old_active;
}

// glBindTexture(GL_TEXTURE_2D) 且活动单元为 unit0 时由包装器直接通知：
// 各单元的绑定用 CPU 维护，免去每次绑定的驱动查询（单通道判断走本地缓存）
void fp_notify_texture_bind_tex(GLuint texture) {
    if(!current_context) return;
    if(fp_active_texture == GL_TEXTURE0) {
        fp_bound_texture = texture;
        fp_bound_single_channel = (texture != 0) && fp_texfmt_resolve(texture);
        fp_bound_texture_valid = true;
    } else if(fp_active_texture == GL_TEXTURE1) {
        // 光照贴图单元（MC 每帧 updateLightmap 绑定 lightmap 纹理）
        fp_bound_texture1 = texture;
    }
}

// 纹理格式可能变化：使格式缓存整体失效（纹理上传不频繁，64 项惰性重建）
void fp_texture_upload_invalidate(void) {
    fp_texfmt_epoch++;
    if(fp_texfmt_epoch == 0) fp_texfmt_epoch = 1;
}

// 设置默认 program 的 uniforms（MVP + 纹理开关）。uUseTex 同时受“绑定纹理”
// 与 GL_TEXTURE_2D 启用状态控制：桌面固定管线里 glDisable(GL_TEXTURE_2D)
// 后即使有绑定纹理也不采样（MC 1.12 物品提示黑框、文字下划线走无纹理
// POSITION_COLOR 矩形，之前忽略启用状态导致采样到上一个绑定的纹理）。
// MathCode: 黑框修复——纹理采样必须同时满足“绑定了纹理”和
// “GL_TEXTURE_2D 已启用”（桌面固定管线语义）。
static void fp_set_default_uniforms(void) {
    GLfloat mvp[FP_MATRIX_SIZE];
    fp_mat_mul(mvp, fp_matrix_stack[FP_MATRIX_PROJECTION][fp_matrix_top[FP_MATRIX_PROJECTION]],
               fp_matrix_stack[FP_MATRIX_MODELVIEW][fp_matrix_top[FP_MATRIX_MODELVIEW]]);
    // uniform 值没变就不重传：F3 一整屏文字共享同一组状态，30 次批次提交
    // 里只有第一次真正需要设 uniform。
    if(!fp_uniforms_initialized ||
       memcmp(fp_last_mvp, mvp, sizeof(fp_last_mvp)) != 0) {
        if(fp_mvp_loc >= 0) es3_functions.glUniformMatrix4fv(fp_mvp_loc, 1, GL_FALSE, mvp);
        memcpy(fp_last_mvp, mvp, sizeof(fp_last_mvp));
    }
    GLint usetex = (fp_bound_texture != 0 && fp_texture_enabled[0]) ? 1 : 0;
    if(!fp_uniforms_initialized || fp_last_tex != 0) {
        if(fp_tex_loc >= 0) es3_functions.glUniform1i(fp_tex_loc, 0);
        fp_last_tex = 0;
    }
    // 有顶点色用顶点色，否则用当前色（glColor4f 状态）——固定管线语义
    GLint usecolor = fp_immediate_active ? 1 : (fp_client_color_active ? 1 : 0);
    if(!fp_uniforms_initialized || fp_last_usetex != usetex) {
        if(fp_usetex_loc >= 0) es3_functions.glUniform1i(fp_usetex_loc, usetex);
        fp_last_usetex = usetex;
    }
    if(!fp_uniforms_initialized || fp_last_usecolor != usecolor) {
        if(fp_usecolor_loc >= 0) es3_functions.glUniform1i(fp_usecolor_loc, usecolor);
        fp_last_usecolor = usecolor;
    }
    if(!fp_uniforms_initialized ||
       memcmp(fp_last_color, fp_current_color, sizeof(fp_last_color)) != 0) {
        if(fp_color_loc >= 0) es3_functions.glUniform4fv(fp_color_loc, 1, fp_current_color);
        memcpy(fp_last_color, fp_current_color, sizeof(fp_last_color));
    }
    GLint single = fp_bound_single_channel ? 1 : 0;
    if(!fp_uniforms_initialized || fp_last_single != single) {
        if(fp_single_loc >= 0) es3_functions.glUniform1i(fp_single_loc, single);
        fp_last_single = single;
    }
    GLint lighttint = fp_light_tint ? 1 : 0;
    if(!fp_uniforms_initialized || fp_last_lighttint != lighttint) {
        if(fp_lighttint_loc >= 0) es3_functions.glUniform1i(fp_lighttint_loc, lighttint);
        fp_last_lighttint = lighttint;
    }
    if(!fp_uniforms_initialized ||
       memcmp(fp_last_lightcolor, fp_texenv_state[1].color, sizeof(fp_last_lightcolor)) != 0) {
        if(fp_lightcolor_loc >= 0) es3_functions.glUniform4fv(fp_lightcolor_loc, 1, fp_texenv_state[1].color);
        memcpy(fp_last_lightcolor, fp_texenv_state[1].color, sizeof(fp_last_lightcolor));
    }
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
    if(!fp_uniforms_initialized || fp_last_alphafunc != alpha_mode) {
        if(fp_alphafunc_loc >= 0) es3_functions.glUniform1i(fp_alphafunc_loc, alpha_mode);
        fp_last_alphafunc = alpha_mode;
    }
    GLfloat alpha_ref = fp_alpha_test ? fp_alpha_ref : 0.0f;
    if(!fp_uniforms_initialized || fp_last_alpharef != alpha_ref) {
        if(fp_alpharef_loc >= 0) es3_functions.glUniform1f(fp_alpharef_loc, alpha_ref);
        fp_last_alpharef = alpha_ref;
    }
    // 昼夜亮度开关：0=关，1=顶点 UV1（方块），2=常量 UV（实体/掉落物）。
    // fp_client_uv1_active 由 fp_prepare_client_arrays 按顶点数据实际布局
    // 设置；fp_lightmap_const_active 由 DL 回放（实体段）按最近方块亮度设置。
    // MathCode: 2026-08-11 掉落物闪烁根因修复——不再依赖 fp_texture_enabled[1]：
    // MC 桌面语义下 unit1（lightmap）的 GL_TEXTURE_2D 启用是持久状态，从不被
    // MC 主动管理（enable 只在 updateLightmap 一次；后续 disable 都发生在
    // unit0）。LTW 按活动单元记录 enable，GUI 段在 unit1 活动时误清
    // fp_texture_enabled[1]，导致掉落物/实体丢失 lightmap（夜晚全亮闪烁）。
    // 现在只要求 unit1 绑定有效纹理（lightmap 绑定后恒不换），
    // 是否启用由数据有效性（uv1act/const_active）决定。
    GLint uselightmap = 0;
    if(fp_client_uv1_active && fp_bound_texture1 != 0) {
        uselightmap = 1;
    } else if(fp_lightmap_const_active && fp_bound_texture1 != 0) {
        uselightmap = 2;
    }
    if(!fp_uniforms_initialized || fp_last_uselightmap != uselightmap) {
        if(fp_uselightmap_loc >= 0) es3_functions.glUniform1i(fp_uselightmap_loc, uselightmap);
        fp_last_uselightmap = uselightmap;
    }
    // 实体常量 lightmap UV（归一化 0-1）
    if(fp_lightmapuv_loc >= 0 && (!fp_uniforms_initialized ||
       fp_last_lightmapuv_set != (fp_lightmap_const_active ? 1 : 0) ||
       memcmp(fp_last_lightmap_uv, fp_last_lightmap_uv_snap, sizeof(fp_last_lightmap_uv)) != 0)) {
        if(fp_lightmap_const_active) {
            es3_functions.glUniform2fv(fp_lightmapuv_loc, 1, fp_last_lightmap_uv_snap);
            memcpy(fp_last_lightmap_uv, fp_last_lightmap_uv_snap, sizeof(fp_last_lightmap_uv));
        }
        fp_last_lightmapuv_set = fp_lightmap_const_active ? 1 : 0;
    }
    // uLightMap 采样器固定绑定 unit1（lightmap 纹理所在单元），只需设一次。
    // 直接走 es3_functions 切换活动单元，避免 glActiveTexture 包装器
    // 触发批次冲刷；fp_active_texture 同步保持 CPU 跟踪一致。
    if(fp_lightmap_loc >= 0 && !fp_uniforms_initialized) {
        GLenum old_active = fp_active_texture;
        if(old_active != GL_TEXTURE1) {
            es3_functions.glActiveTexture(GL_TEXTURE1);
            fp_active_texture = GL_TEXTURE1;
        }
        es3_functions.glUniform1i(fp_lightmap_loc, 1);
        if(old_active != GL_TEXTURE1) {
            es3_functions.glActiveTexture(old_active);
            fp_active_texture = old_active;
        }
    }
    fp_uniforms_initialized = true;
}

// 绑定默认 program。返回 true 表示成功（调用方须配对调用 fp_unbind_default_program）。
// 若应用通过固定管线 API（glVertexPointer 等）提供了客户端数组/VBO 偏移，
// 这里把它们设置成 attribute；否则用即时模式缓冲/默认值。
bool fp_bind_default_program(void) {
    fp_ensure_program();
    if(!fp_program) return false;
    fp_refresh_bound_texture();
    es3_functions.glUseProgram(fp_program);
    if(current_context) current_context->program = fp_program;
    fp_set_default_uniforms();

    // 使用私有 VAO，避免污染应用绑定的 VAO 的 attribute 状态
    fp_saved_vao = fp_app_vao;
    if(fp_vao) fp_gl_bind_vao(fp_vao);
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
static void fp_upload_client_arrays(GLsizei count, bool uv1_touched) {
    if(!fp_client_vertex_enabled || fp_client_vertex_size <= 0 || !fp_client_vertex_ptr) return;
    // ABO 绑定由 glBindBuffer 包装器（含内部绑定）统一维护，无需驱动查询
    GLint old_abo = current_context ? (GLint)current_context->bound_buffers[0] : 0;

    size_t tsize = fp_type_bytes(fp_client_vertex_type);
    size_t vsize = fp_client_vertex_stride ? (size_t)fp_client_vertex_stride
                                           : (size_t)fp_client_vertex_size * tsize;
    if(vsize == 0 || (size_t)count > (size_t)FP_MAX_VERTICES) return;

    // 一次上传整个交错数组
    glBindBuffer(GL_ARRAY_BUFFER, fp_vbo);
    es3_functions.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vsize * count,
                               fp_client_vertex_ptr, GL_STREAM_DRAW);

    // 位置 attribute：offset 0
    {
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
        // unit1（光照贴图）坐标 attribute：偏移 = 指针差（交错缓冲内）。
        // MathCode: 2026-08-11 掉落物闪烁/GUI 变色根因修复——条件改为
        // "本次绘制前设置过 unit1 指针"（touched）：MC 桌面语义下 unit1
        // 的 TEXTURE_COORD_ARRAY 启用状态持久且从不被管理，enable 状态
        // 不可靠；但残留指针+缓冲复用（Tessellator 全局缓冲）会让 off
        // 越界检查漏放 GUI 矩形，必须用 touched 区分"本次真实设置"。
        if(uv1_touched && fp_client_texcoord1_size > 0 && fp_client_texcoord1_ptr) {
            ptrdiff_t off = (const uint8_t*)fp_client_texcoord1_ptr - (const uint8_t*)fp_client_vertex_ptr;
            if(off >= 0 && (size_t)off < vsize) {
                es3_functions.glEnableVertexAttribArray(FP_ATTR_UV1);
                es3_functions.glVertexAttribPointer(FP_ATTR_UV1, fp_client_texcoord1_size,
                                                    fp_client_texcoord1_type, GL_FALSE,
                                                    fp_client_vertex_stride, (const void*)off);
                fp_client_uv1_active = true;
                // 快照首顶点的 lightmap UV（GL_SHORT 亮度级 → ÷16 归一化）。
                // 实体模型无 unit1 坐标，绘制时用这个常量亮度近似场景明暗；
                // 数据此刻仍指向 MC 正在使用的缓冲，拷贝后才安全。
                {
                    const uint8_t* p = (const uint8_t*)fp_client_texcoord1_ptr;
                    if(fp_client_texcoord1_type == GL_SHORT && fp_client_texcoord1_size >= 2) {
                        int16_t u0 = *(const int16_t*)p;
                        int16_t v0 = *(const int16_t*)(p + 2);
                        if(u0 >= 0 && u0 <= 255 && v0 >= 0 && v0 <= 255) {
                            fp_last_lightmap_uv_snap[0] = (GLfloat)u0 / 16.0f;
                            fp_last_lightmap_uv_snap[1] = (GLfloat)v0 / 16.0f;
                            fp_last_lightmap_uv_valid = true;
                        }
                    }
                }
            } else {
                es3_functions.glDisableVertexAttribArray(FP_ATTR_UV1);
                fp_client_uv1_active = false;
            }
        } else {
            es3_functions.glDisableVertexAttribArray(FP_ATTR_UV1);
            fp_client_uv1_active = false;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
}

bool fp_prepare_client_arrays(GLsizei count) {
    if(!fp_program) fp_ensure_program();
    if(!fp_program) return false;

    // [LMT] 诊断探针：掉落物/手持物品模型顶点少（<512），每次都打印；
    // 区块等大绘制只在状态变化时打印。定位掉落物亮度不稳定的状态竞态。
    if(ltw_lightmap_trace) {
        int st = (fp_client_texcoord1_enabled ? 1 : 0)
               | ((fp_client_texcoord1_size > 0 ? 1 : 0) << 1)
               | ((fp_client_texcoord1_ptr ? 1 : 0) << 2)
               | ((fp_client_texcoord1_abo != 0 ? 1 : 0) << 3)
               | ((fp_bound_texture1 != 0 ? 1 : 0) << 4)
               | ((fp_texture_enabled[1] ? 1 : 0) << 5)
               | ((fp_client_uv1_active ? 1 : 0) << 6)
               | ((fp_lightmap_const_active ? 1 : 0) << 7)
               | ((fp_client_texcoord1_touched ? 1 : 0) << 8);
        bool small_draw = (count < 512);
        if(small_draw || st != ltw_lm_trace_state) {
            ltw_lm_trace_state = st;
            LTW_ERROR_PRINTF("[LMT] draw count=%d uv1_en=%d size=%d ptr=%p abo=%d "
                             "tex1=%u texen1=%d uv1act=%d const=%d tch=%d",
                             count, fp_client_texcoord1_enabled, fp_client_texcoord1_size,
                             fp_client_texcoord1_ptr, fp_client_texcoord1_abo,
                             fp_bound_texture1, fp_texture_enabled[1],
                             fp_client_uv1_active ? 1 : 0,
                             fp_lightmap_const_active ? 1 : 0,
                             fp_client_texcoord1_touched ? 1 : 0);
        }
    }
    // MathCode: 2026-08-11 GUI 变色修复——消费 unit1 touched 标记：
    // 只有本次绘制前 MC 设置过 unit1 指针（lightmap 数据真实存在）才有效，
    // 残留指针（Tessellator 缓冲复用）不再能触发 lightmap。
    bool uv1_touched = fp_client_texcoord1_touched;
    fp_client_texcoord1_touched = false;

    // GLES 3.x 禁止客户端数组指针。用数组"设置时"的 ARRAY_BUFFER 绑定判断：
    // 设置时未绑定（pointer 是 CPU 地址）→ 拷贝到 fp_vbo；设置时已绑定
    // （pointer 是 VBO 偏移）→ 直通。不能用绘制时的当前绑定判断（应用
    // 可能在 glVertexPointer 之后绑定/解绑了 ARRAY_BUFFER 做别的事）。
    if(fp_client_vertex_abo == 0) {
        fp_upload_client_arrays(count, uv1_touched);
    } else {
        // VBO 路径：pointer 是偏移，直通（必须先绑定对应的 VBO，偏移才有效）。
        // 应用在 glVertexPointer 时绑定了 VBO（fp_client_*_abo），绘制时当前
        // ARRAY_BUFFER 可能已换成别的 buffer，这里按各自 abo 重新绑定。
        GLint old_abo = current_context ? (GLint)current_context->bound_buffers[0] : 0;
        GLint vbo = fp_client_vertex_abo ? fp_client_vertex_abo : old_abo;
        if(vbo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)vbo);
        if(fp_client_vertex_enabled && fp_client_vertex_size > 0) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_POS);
            es3_functions.glVertexAttribPointer(FP_ATTR_POS, fp_client_vertex_size, fp_client_vertex_type,
                                                GL_FALSE, fp_client_vertex_stride, fp_client_vertex_ptr);
        }
        if(vbo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
        GLint cbo = fp_client_color_abo ? fp_client_color_abo : old_abo;
        if(cbo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)cbo);
        if(fp_client_color_enabled && fp_client_color_size > 0) {
            fp_client_color_active = true;
            es3_functions.glEnableVertexAttribArray(FP_ATTR_COLOR);
            es3_functions.glVertexAttribPointer(FP_ATTR_COLOR, fp_client_color_size, fp_client_color_type,
                                                GL_TRUE, fp_client_color_stride, fp_client_color_ptr);
        } else {
            fp_client_color_active = false;
            es3_functions.glDisableVertexAttribArray(FP_ATTR_COLOR);
        }
        if(cbo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
        GLint ubo = fp_client_texcoord_abo ? fp_client_texcoord_abo : old_abo;
        if(ubo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)ubo);
        if(fp_client_texcoord_enabled && fp_client_texcoord_size > 0) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
            es3_functions.glVertexAttribPointer(FP_ATTR_UV, fp_client_texcoord_size, fp_client_texcoord_type,
                                                GL_FALSE, fp_client_texcoord_stride, fp_client_texcoord_ptr);
        } else {
            es3_functions.glDisableVertexAttribArray(FP_ATTR_UV);
        }
        if(ubo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
        // unit1（光照贴图）坐标：与 unit0 UV 同缓冲（交错布局），各自绑定
        GLint v1bo = fp_client_texcoord1_abo ? fp_client_texcoord1_abo : old_abo;
        if(v1bo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)v1bo);
        // 防御：unit1 指针是 VBO 偏移，绘制前必须落在本次顶点数据范围内
        // （GUI 矩形等无 unit1 的格式会残留旧偏移，越界则禁用 lightmap）
        // MathCode: 2026-08-11 掉落物闪烁/GUI 变色根因修复——条件 = 本次
        // 绘制前 MC 设置过 unit1 指针（touched）+ 数据有效性 + 越界防御；
        // 不依赖 enabled（桌面语义 unit1 数组启用状态持久，MC 从不管理）。
        GLintptr v1off = (GLintptr)(intptr_t)fp_client_texcoord1_ptr;
        size_t v1stride = fp_client_texcoord1_stride
                              ? (size_t)fp_client_texcoord1_stride
                              : (size_t)fp_client_texcoord1_size * fp_type_bytes(fp_client_texcoord1_type);
        if(uv1_touched && fp_client_texcoord1_size > 0 &&
           fp_client_texcoord1_ptr != NULL &&
           v1off > 0 && v1stride > 0 &&
           (size_t)v1off + v1stride <= (size_t)count * v1stride) {
            es3_functions.glEnableVertexAttribArray(FP_ATTR_UV1);
            es3_functions.glVertexAttribPointer(FP_ATTR_UV1, fp_client_texcoord1_size,
                                                fp_client_texcoord1_type, GL_FALSE,
                                                fp_client_texcoord1_stride, fp_client_texcoord1_ptr);
            fp_client_uv1_active = true;
        } else {
            es3_functions.glDisableVertexAttribArray(FP_ATTR_UV1);
            fp_client_uv1_active = false;
        }
        if(v1bo != old_abo) glBindBuffer(GL_ARRAY_BUFFER, (GLuint)old_abo);
    }
    // attribute 启用情况影响 uUseColor，这里重设 uniforms（bind 先于 prepare）
    fp_set_default_uniforms();
    return true;
}

void fp_unbind_default_program(void) {
    if(!fp_program) return;
    // unit0 绑定在绑定阶段从未被改写（fp_bound_texture 即 unit0 当前绑定，
    // 由 glBindTexture 包装器 CPU 维护），无需恢复纹理状态
    if(fp_saved_vao != 0) fp_gl_bind_vao(fp_saved_vao);
    // 与 fp_flush_immediate 同理：恢复应用 VAO 后同步内部跟踪值，
    // 避免显示列表合并路径基于过期值跳过自己的 VAO 绑定。
    dl_current_vao = (GLuint)fp_saved_vao;
    es3_functions.glUseProgram(0);
    if(current_context) current_context->program = 0;
}

// 无 program 时用默认 shader 绘制。应用已设置好 VAO attribute（MC 1.12
// 的 Tessellator 用 glVertexAttribPointer + VBO），这里只切换 program。
bool fp_try_draw_arrays(GLenum mode, GLint first, GLsizei count) {
    if(!current_context) return false;
    // 当前 program 由 glUseProgram 包装器 + 固定管线自身绑定路径统一维护，
    // 无需向驱动查询（应用有 program 时说明走的现代管线，固定管线不介入）
    if(current_context->program != 0) return false;
    if(!fp_bind_default_program()) return false;
    fp_prepare_client_arrays(count);
    es3_functions.glDrawArrays(mode, first, count);
    fp_unbind_default_program();
    return true;
}

bool fp_try_draw_elements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    if(!current_context) return false;
    if(current_context->program != 0) return false;
    // GL_ELEMENT_ARRAY_BUFFER 绑定属于 VAO：必须在切到私有 fp_vao 之前
    // 记录应用的 EBO（可能为 0=客户端索引），切过去后重新绑定，否则
    // indices 会被当作 fp_vao 里残留 EBO 的偏移，画出垃圾几何。
    GLint eab = 0;
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eab);
    if(!fp_bind_default_program()) return false;
    if(eab == 0) {
        // GLES 禁止客户端索引指针（桌面 GL 1.x 允许）：无 EBO 时直接把
        // CPU 索引上传到内部 scratch EBO，否则 glDrawElements 会产生
        // GL_INVALID_OPERATION（MC 帧末 “Post render 1282” 的来源之一）。
        if(indices && count > 0 &&
           (type == GL_UNSIGNED_BYTE || type == GL_UNSIGNED_SHORT || type == GL_UNSIGNED_INT)) {
            GLsizeiptr isize = (GLsizeiptr)count * (GLsizeiptr)fp_type_bytes(type);
            if(fp_index_ebo != 0) {
                es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fp_index_ebo);
                if(isize > fp_index_ebo_cap) {
                    es3_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices, GL_STREAM_DRAW);
                    fp_index_ebo_cap = isize;
                } else {
                    es3_functions.glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, isize, indices);
                }
                indices = NULL;
            }
        }
    } else {
        es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)eab);
    }
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
    out->texcoord1_enabled = fp_client_texcoord1_enabled;
    out->texcoord1_size = fp_client_texcoord1_size;
    out->texcoord1_type = fp_client_texcoord1_type;
    out->texcoord1_stride = fp_client_texcoord1_stride;
    out->color_enabled = fp_client_color_enabled;
    out->color_size = fp_client_color_size;
    out->color_type = fp_client_color_type;
    out->color_stride = fp_client_color_stride;
    out->normal_enabled = fp_client_normal_enabled;
    out->normal_type = fp_client_normal_type;
    out->normal_stride = fp_client_normal_stride;
    out->vertex_abo = fp_client_vertex_abo;
    out->texcoord_abo = fp_client_texcoord_abo;
    out->texcoord1_abo = fp_client_texcoord1_abo;
    out->color_abo = fp_client_color_abo;
    out->vertex_off = (GLintptr)(intptr_t)fp_client_vertex_ptr;
    out->texcoord_off = (GLintptr)(intptr_t)fp_client_texcoord_ptr;
    out->texcoord1_off = (GLintptr)(intptr_t)fp_client_texcoord1_ptr;
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
    fp_client_texcoord1_enabled = s->texcoord1_enabled;
    fp_client_texcoord1_size = s->texcoord1_size;
    fp_client_texcoord1_type = s->texcoord1_type;
    fp_client_texcoord1_stride = s->texcoord1_stride;
    fp_client_color_enabled = s->color_enabled;
    fp_client_color_size = s->color_size;
    fp_client_color_type = s->color_type;
    fp_client_color_stride = s->color_stride;
    fp_client_normal_enabled = s->normal_enabled;
    fp_client_normal_type = s->normal_type;
    fp_client_normal_stride = s->normal_stride;
    fp_client_vertex_abo = s->vertex_abo;
    fp_client_texcoord_abo = s->texcoord_abo;
    fp_client_texcoord1_abo = s->texcoord1_abo;
    fp_client_color_abo = s->color_abo;
    fp_client_vertex_ptr = (const void*)(intptr_t)s->vertex_off;
    fp_client_texcoord_ptr = (const void*)(intptr_t)s->texcoord_off;
    fp_client_texcoord1_ptr = (const void*)(intptr_t)s->texcoord1_off;
    fp_client_color_ptr = (const void*)(intptr_t)s->color_off;
    fp_client_normal_ptr = (const void*)(intptr_t)s->normal_off;
}

bool fp_dl_capture_client_draw(GLenum mode, GLint first, GLsizei count,
                               bool indexed, GLenum itype, const void* indices) {
    if(!dl_is_compiling()) return false;
    // MathCode: 2026-08-11 GUI 变色修复——绘制被吞入 DL，消费 unit1 touched
    // 标记（该绘制的 lightmap 数据已进快照，touched 语义由快照替代）。
    fp_client_texcoord1_touched = false;
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
    snap.texcoord1_enabled = fp_client_texcoord1_enabled;
    snap.texcoord1_size = fp_client_texcoord1_size;
    snap.texcoord1_type = fp_client_texcoord1_type;
    snap.texcoord1_stride = fp_client_texcoord1_stride;
    snap.color_enabled = fp_client_color_enabled;
    snap.color_size = fp_client_color_size;
    snap.color_type = fp_client_color_type;
    snap.color_stride = fp_client_color_stride;
    snap.normal_enabled = fp_client_normal_enabled;
    snap.normal_type = fp_client_normal_type;
    snap.normal_stride = fp_client_normal_stride;
    snap.vertex_abo = fp_client_vertex_abo;
    snap.texcoord_abo = fp_client_texcoord_abo;
    snap.texcoord1_abo = fp_client_texcoord1_abo;
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
                // 交错块内偏移：相对原始基址计算（即 stride 内偏移），
                // 拷贝起点右移 first 后这些偏移仍然适用
                const uint8_t* base = (const uint8_t*)fp_client_vertex_ptr;
                snap.vertex_off = 0;
                snap.texcoord_off = fp_dl_ptr_off(base, fp_client_texcoord_ptr, bytes);
                snap.texcoord1_off = fp_dl_ptr_off(base, fp_client_texcoord1_ptr, bytes);
                snap.color_off = fp_dl_ptr_off(base, fp_client_color_ptr, bytes);
                snap.normal_off = fp_dl_ptr_off(base, fp_client_normal_ptr, bytes);
                // 桌面语义：glDrawArrays(mode, first, count) 从第 first 个
                // 顶点开始。快照从 first 处拷贝并把回放 first 归零，
                // 否则回放会越界读到缓冲区之后。
                vertex_data = (first > 0) ? (const void*)(base + (size_t)first * vsize)
                                          : (const void*)base;
                if(first > 0) first = 0;
                vertex_len = (uint32_t)bytes;
            }
        }
    } else {
        // VBO 路径：记录 buffer 与字节偏移，不拷贝 GPU 数据。
        // 注意：MC 的 glVertexAttribPointer 直通 GLES、不更新 fp_client_* 状态，
        // 因此 VBO 开启时显示列表内实际录不到几何（回放为空操作）。
        // FCL/Pojav 对 <=1.12 强制关闭 VBO（客户端数组路径），MC 1.12 不受影响。
        snap.vertex_off = (GLintptr)(intptr_t)fp_client_vertex_ptr;
        snap.texcoord_off = (GLintptr)(intptr_t)fp_client_texcoord_ptr;
        snap.texcoord1_off = (GLintptr)(intptr_t)fp_client_texcoord1_ptr;
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
    // MathCode: 2026-08-11 GUI 变色修复——回放绘制期间 touched 由快照数据
    // 决定（录制时若设置了 unit1 指针则 lightmap 有效），结束后恢复进入值。
    bool save_touched = fp_client_texcoord1_touched;
    fp_client_texcoord1_touched = (snap->texcoord1_size > 0 && snap->texcoord1_off >= 0);

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
    fp_client_texcoord1_touched = save_touched;
}

// ---- 显示列表批量回放：几何缓存与状态摊分 ----

// 读取客户端索引元素（与录制端允许的类型一致）
static uint32_t fp_dl_read_idx(const void* src, GLenum type, GLsizei i) {
    if(!src) return 0;
    switch(type) {
        case GL_UNSIGNED_BYTE:  return ((const uint8_t*)src)[i];
        case GL_UNSIGNED_SHORT: return ((const uint16_t*)src)[i];
        case GL_UNSIGNED_INT:   return ((const uint32_t*)src)[i];
        default:                return 0;
    }
}

// 进入显示列表批量回放：保存应用 GL 状态，绑定默认 program / 私有 VAO /
// unit0 纹理。调用方必须配对调用 fp_end_dl_replay。
static bool fp_begin_dl_replay(void) {
    fp_ensure_program();
    if(!fp_program || !fp_vao) return false;
    es3_functions.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &dl_saved_vao);
    es3_functions.glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &dl_saved_abo);
    es3_functions.glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &dl_saved_eab);
    dl_saved_program = current_context ? (GLint)current_context->program : 0;

    // 固定管线只消费 unit0 的纹理；先记录应用的活动单元与 unit0 绑定
    fp_refresh_bound_texture();
    dl_saved_active_tex = (GLint)fp_active_texture;
    dl_saved_bound_tex = (GLint)fp_bound_texture;
    dl_saved_texture_valid = (fp_bound_texture != 0);
    // 实体显示列表段无 per-vertex unit1 坐标（桌面固定管线同样如此）：
    // 用最近一次方块渲染的首顶点 lightmap UV 做常量采样，实体整体亮度
    // 随场景昼夜变化（生物/玩家/掉落物模型；受伤红闪走 uLightTint 模拟，
    // 常量 lightmap 乘在其后，与原版 unit2 MODULATE 顺序一致）。
    // MathCode: 2026-08-11 掉落物闪烁根因修复——不依赖 fp_texture_enabled[1]
    // （桌面语义 unit1 纹理启用持久，MC 从不管理；见 fp_set_default_uniforms）。
    fp_client_uv1_active = false;
    fp_lightmap_const_active = (fp_last_lightmap_uv_valid &&
                                fp_bound_texture1 != 0);

    es3_functions.glUseProgram(fp_program);
    if(current_context) current_context->program = fp_program;
    fp_gl_bind_vao(fp_vao);
    dl_current_vao = fp_vao;
    dl_replay_dirty = true;
    dl_last_uuse_color = !fp_client_color_active; // 强制首次缓存绘制重设 uUseColor
    return true;
}

// 退出批量回放：一次性恢复应用状态（纹理 / VAO / EAB / ABO / program）。
static void fp_end_dl_replay(void) {
    if(dl_saved_texture_valid) {
        es3_functions.glActiveTexture(GL_TEXTURE0);
        es3_functions.glBindTexture(GL_TEXTURE_2D, (GLuint)dl_saved_bound_tex);
        fp_bound_texture = (GLuint)dl_saved_bound_tex; // unit0 绑定已还原，同步跟踪
    }
    es3_functions.glActiveTexture((GLenum)dl_saved_active_tex);
    // 同步内部活动单元跟踪，避免后续 glBindTexture 的刷新判断用旧值
    fp_active_texture = (GLenum)dl_saved_active_tex;
    // EAB 属于 VAO：先切回应用 VAO（含默认 VAO 0）再恢复 EAB，
    // 避免把 EAB 写进私有 fp_vao
    if(dl_saved_vao != 0) {
        fp_gl_bind_vao((GLuint)dl_saved_vao);
    } else {
        fp_gl_bind_vao(0);
    }
    dl_current_vao = (GLuint)dl_saved_vao;
    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLuint)dl_saved_eab);
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)dl_saved_abo);
    if((GLint)fp_program != dl_saved_program) {
        es3_functions.glUseProgram((GLuint)dl_saved_program);
        if(current_context) current_context->program = (GLuint)dl_saved_program;
    }
    // MathCode: 2026-08-11 GUI 灰色修复——实体段结束必须清除常量 lightmap，
    // 否则 const_active 残留到 GUI 矩形（客户端数组绘制，touched=0 无 uv1），
    // uselightmap 落到 const 分支（2）→ GUI 整体被 lightmap 调制变灰。
    // 实体段内（begin..end 之间）的 fallback/缓存绘制仍正常使用 const。
    fp_lightmap_const_active = false;
}

// 为 CLIENT_DRAW op 建立回放缓存（仅 CPU 快照路径）。录制后的顶点/索引
// 不可变，这里一次性上传到私有 VBO/EBO 并定型 VAO；GL_QUADS 在缓存期
// 展开为三角形索引，回放直接 glDrawElements。
// 返回 true 表示缓存可用；false 表示该 op 需走既有回放路径。
static bool fp_dl_build_client_cache(dl_op_entry_t* op, const dl_client_draw_payload_t* p) {
    if(!op || !p || !current_context) return false;
    // VBO 直通 / 源 EBO 路径不缓存：数据可能在应用侧更新，保持既有语义
    if(p->snap.vertex_abo != 0 || p->indices_ebo != 0) return false;
    if(p->vertex_len == 0 || p->count <= 0) return false;
    if((size_t)p->vertex_off + (size_t)p->vertex_len > (size_t)op->size) return false;

    const fp_dl_client_snapshot_t* snap = &p->snap;
    const uint8_t* base = op->data;
    const void* vdata = base + p->vertex_off;
    size_t tsize = fp_type_bytes(snap->vertex_type);
    size_t vsize = snap->vertex_stride ? (size_t)snap->vertex_stride
                                       : (size_t)snap->vertex_size * tsize;
    if(vsize == 0) return false;

    GLuint vao = 0, vbo = 0, ebo = 0;
    es3_functions.glGenVertexArrays(1, &vao);
    if(vao == 0) return false;
    es3_functions.glGenBuffers(1, &vbo);
    if(vbo == 0) { es3_functions.glDeleteVertexArrays(1, &vao); return false; }

    GLenum draw_mode = p->mode;
    GLsizei draw_count = p->count;
    GLsizei draw_first = p->first;
    GLenum draw_itype = GL_UNSIGNED_INT;
    uint32_t* expanded = NULL;
    bool ok = false;

    fp_gl_bind_vao(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    es3_functions.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)p->vertex_len, vdata, GL_STATIC_DRAW);

    es3_functions.glEnableVertexAttribArray(FP_ATTR_POS);
    es3_functions.glVertexAttribPointer(FP_ATTR_POS, snap->vertex_size, snap->vertex_type,
                                        GL_FALSE, snap->vertex_stride, NULL);
    if(snap->texcoord_enabled && snap->texcoord_size > 0 && snap->texcoord_off >= 0 &&
       (size_t)snap->texcoord_off < vsize) {
        es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
        es3_functions.glVertexAttribPointer(FP_ATTR_UV, snap->texcoord_size, snap->texcoord_type,
                                            GL_FALSE, snap->vertex_stride,
                                            (const void*)(intptr_t)snap->texcoord_off);
    } else {
        es3_functions.glDisableVertexAttribArray(FP_ATTR_UV);
    }
    if(snap->color_enabled && snap->color_size > 0 && snap->color_off >= 0 &&
       (size_t)snap->color_off < vsize) {
        es3_functions.glEnableVertexAttribArray(FP_ATTR_COLOR);
        es3_functions.glVertexAttribPointer(FP_ATTR_COLOR, snap->color_size, snap->color_type,
                                            GL_TRUE, snap->vertex_stride,
                                            (const void*)(intptr_t)snap->color_off);
    } else {
        es3_functions.glDisableVertexAttribArray(FP_ATTR_COLOR);
    }

    if(p->indexed) {
        if(p->indices_len == 0 || (size_t)p->indices_off + (size_t)p->indices_len > (size_t)op->size) {
            goto fail;
        }
        const void* idata = base + p->indices_off;
        if(fp_type_bytes(p->itype) == 0) goto fail;
        if(p->mode == GL_QUADS && p->count >= 4 && (p->count & 3) == 0) {
            // 索引 QUADS：读取源索引并展开为三角形
            GLsizei quads = p->count >> 2;
            GLsizei tri = quads * 6;
            expanded = (uint32_t*)malloc((size_t)tri * sizeof(uint32_t));
            if(!expanded) goto fail;
            for(GLsizei q = 0; q < quads; q++) {
                uint32_t a = fp_dl_read_idx(idata, p->itype, q * 4 + 0);
                uint32_t b = fp_dl_read_idx(idata, p->itype, q * 4 + 1);
                uint32_t c = fp_dl_read_idx(idata, p->itype, q * 4 + 2);
                uint32_t d = fp_dl_read_idx(idata, p->itype, q * 4 + 3);
                uint32_t* t = expanded + q * 6;
                t[0] = a; t[1] = b; t[2] = c;
                t[3] = a; t[4] = c; t[5] = d;
            }
            draw_mode = GL_TRIANGLES;
            draw_count = tri;
            draw_itype = GL_UNSIGNED_INT;
        } else {
            draw_mode = p->mode;
            draw_count = p->count;
            draw_itype = p->itype;
        }
    } else if(p->mode == GL_QUADS && p->count >= 4 && (p->count & 3) == 0) {
        // 非索引 QUADS：first..first+count-1 展开为三角形索引
        GLsizei quads = p->count >> 2;
        GLsizei tri = quads * 6;
        expanded = (uint32_t*)malloc((size_t)tri * sizeof(uint32_t));
        if(!expanded) goto fail;
        for(GLsizei q = 0; q < quads; q++) {
            uint32_t a = (uint32_t)(p->first + q * 4 + 0);
            uint32_t b = (uint32_t)(p->first + q * 4 + 1);
            uint32_t c = (uint32_t)(p->first + q * 4 + 2);
            uint32_t d = (uint32_t)(p->first + q * 4 + 3);
            uint32_t* t = expanded + q * 6;
            t[0] = a; t[1] = b; t[2] = c;
            t[3] = a; t[4] = c; t[5] = d;
        }
        draw_mode = GL_TRIANGLES;
        draw_count = tri;
        draw_itype = GL_UNSIGNED_INT;
    } else {
        draw_mode = p->mode;
        draw_count = p->count;
        draw_first = p->first;
    }

    // 上传索引（展开路径用 expanded，其余用录制时的原数据）
    if(p->indexed || expanded) {
        const void* idata = expanded;
        GLsizeiptr isize = (GLsizeiptr)draw_count * (GLsizeiptr)sizeof(uint32_t);
        if(!expanded) {
            idata = base + p->indices_off;
            isize = (GLsizeiptr)p->indices_len;
        }
        es3_functions.glGenBuffers(1, &ebo);
        if(ebo == 0) goto fail;
        es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        es3_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, idata, GL_STATIC_DRAW);
    }

    ok = true;

fail:
    // ok=true 时是正常收尾（无 goto 到此），ok=false 时是错误清理路径
    if(expanded) free(expanded);
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)dl_saved_abo);
    fp_gl_bind_vao(0);
    if(!ok) {
        if(ebo) es3_functions.glDeleteBuffers(1, &ebo);
        es3_functions.glDeleteBuffers(1, &vbo);
        es3_functions.glDeleteVertexArrays(1, &vao);
        return false;
    }
    op->cache_vao = vao;
    op->cache_vbo = vbo;
    op->cache_ebo = ebo;
    op->cache_mode = draw_mode;
    op->cache_count = draw_count;
    op->cache_first = draw_first;
    op->cache_itype = draw_itype;
    op->cache_indexed = (ebo != 0) ? 1 : 0;
    op->cache_valid = 1;
    op->cache_ctx = current_context;
    return true;
}

// 批量回放 CLIENT_DRAW op：命中缓存时只 bind VAO + draw；首次回放建立缓存；
// 无法缓存（VBO 直通 / OOM）时挂起批量状态，走既有完整绑定路径。
// data/idx 为执行端已做越界校验的数据指针，回退路径直接复用。
static void fp_dl_play_client_cached(dl_op_entry_t* op, const dl_client_draw_payload_t* p,
                                     const void* data, const void* idx) {
    if(!current_context || !op || !p) return;
    if(!op->cache_valid || op->cache_ctx != (const void*)current_context) {
        if(!fp_dl_build_client_cache(op, p)) {
            // 挂起批量状态：旧路径自行绑定/绘制/解绑，再重新进入批量
            dl_replay_active = false;
            fp_end_dl_replay();
            fp_dl_play_client_draw(&p->snap, p->mode, p->first, p->count, p->indexed, p->itype,
                                   p->indices_ebo, p->indices_src_off, idx, p->indices_len, data);
            dl_replay_active = fp_begin_dl_replay();
            return;
        }
        // MathCode: 缓存构建内部绑定过临时 VAO，结束后实际绑定为 0，
        // 清掉跟踪值，下面按需重新绑定 cache_vao。
        dl_current_vao = 0;
    }

    // uUseColor 由客户端颜色数组是否启用决定；判断条件必须与缓存构建端
    // （fp_dl_build_client_cache）完全一致，否则颜色偏移非法时 uUseColor=1
    // 但颜色 attribute 已被禁用，shader 会读到 (0,0,0,1) 的脏值
    size_t tsize = fp_type_bytes(p->snap.vertex_type);
    size_t vsize = p->snap.vertex_stride ? (size_t)p->snap.vertex_stride
                                         : (size_t)p->snap.vertex_size * tsize;
    bool use_color = p->snap.color_enabled && p->snap.color_size > 0 && p->snap.color_off >= 0 &&
                     vsize != 0 && (size_t)p->snap.color_off < vsize;
    if(dl_replay_dirty || use_color != dl_last_uuse_color) {
        fp_client_color_active = use_color;
        fp_set_default_uniforms();
        dl_last_uuse_color = use_color;
        dl_replay_dirty = false;
    }

    if(dl_current_vao != op->cache_vao) {
        fp_gl_bind_vao(op->cache_vao);
        dl_current_vao = op->cache_vao;
    }
    if(op->cache_indexed) {
        es3_functions.glDrawElements(op->cache_mode, op->cache_count, op->cache_itype, NULL);
    } else {
        es3_functions.glDrawArrays(op->cache_mode, op->cache_first, op->cache_count);
    }
    // MathCode: 不再每 op 切回 fp_vao；列表结束/遇到非缓存路径时再恢复，
    // 生物密集场景每个缓存 op 省一次 VAO 绑定。
}

// ---- 实验性整表合并（LTW_DL_MERGE）----
// 只在“整个列表全部是可缓存的 CLIENT_DRAW op、且顶点格式完全一致、中间
// 没有任何纹理/开关/嵌套/即时模式 op”时启用。满足条件就把所有顶点拼进
// 一个大 VBO、索引拼进一个大 EBO，回放一次 glDrawElements；不满足就完全
// 走原来的每 op 缓存路径。
static bool fp_dl_merge_mode_supported(GLenum mode) {
    return mode == GL_QUADS || mode == GL_TRIANGLES;
}

static bool fp_dl_merge_op_cacheable(const dl_op_entry_t* op, const dl_client_draw_payload_t* p) {
    if(!op || !p || op->type != DL_OP_CLIENT_DRAW || op->size < sizeof(*p)) return false;
    if(p->snap.vertex_abo != 0 || p->snap.texcoord_abo != 0 || p->snap.color_abo != 0) return false;
    if(p->indices_ebo != 0 || p->vertex_len == 0 || p->count <= 0 || p->first != 0) return false;
    if((size_t)p->vertex_off + (size_t)p->vertex_len > (size_t)op->size) return false;
    // 合并 VBO 按 vertex_stride 交错拼接，UV/COLOR 必须与顶点同 stride，
    // 否则合并后的 attribute 偏移全部错位（生物贴图错乱）。
    if(p->snap.texcoord_stride != 0 && p->snap.texcoord_stride != p->snap.vertex_stride) return false;
    if(p->snap.color_stride != 0 && p->snap.color_stride != p->snap.vertex_stride) return false;
    if(p->indexed) {
        if(p->indices_len == 0 || (size_t)p->indices_off + (size_t)p->indices_len > (size_t)op->size) return false;
        if(p->itype != GL_UNSIGNED_BYTE && p->itype != GL_UNSIGNED_SHORT && p->itype != GL_UNSIGNED_INT) return false;
        if((size_t)p->count * fp_type_bytes(p->itype) > (size_t)p->indices_len) return false;
    }
    return true;
}

static bool fp_dl_merge_snapshots_compatible(const fp_dl_client_snapshot_t* a,
                                             const fp_dl_client_snapshot_t* b) {
    return a->vertex_enabled == b->vertex_enabled &&
           a->vertex_size == b->vertex_size &&
           a->vertex_type == b->vertex_type &&
           a->vertex_stride == b->vertex_stride &&
           a->texcoord_enabled == b->texcoord_enabled &&
           a->texcoord_size == b->texcoord_size &&
           a->texcoord_type == b->texcoord_type &&
           a->texcoord_stride == b->texcoord_stride &&
           a->texcoord_off == b->texcoord_off &&
           a->color_enabled == b->color_enabled &&
           a->color_size == b->color_size &&
           a->color_type == b->color_type &&
           a->color_stride == b->color_stride &&
           a->color_off == b->color_off &&
           a->normal_enabled == b->normal_enabled &&
           a->normal_type == b->normal_type &&
           a->normal_stride == b->normal_stride &&
           a->normal_off == b->normal_off;
}

static bool fp_dl_merge_use_color(const fp_dl_client_snapshot_t* s, size_t vsize) {
    return s->color_enabled && s->color_size > 0 && s->color_off >= 0 &&
           vsize != 0 && (size_t)s->color_off < vsize;
}

static bool fp_dl_build_merged_cache(fp_dl_list_t* l) {
    if(!l || !current_context) return false;
    l->merge.attempted = true;
    if(l->op_count < 2) return false;

    const fp_dl_client_snapshot_t* first_snap = NULL;
    size_t vsize = 0;
    size_t total_vertices = 0;
    size_t total_vertex_bytes = 0;
    size_t total_indices = 0;

    // 第一遍：校验整表可合并
    for(uint32_t i = 0; i < l->op_count; i++) {
        const dl_op_entry_t* op = &l->ops[i];
        const dl_client_draw_payload_t* p = (const dl_client_draw_payload_t*)op->data;
        if(!fp_dl_merge_op_cacheable(op, p)) return false;
        if(!fp_dl_merge_mode_supported(p->mode)) return false;
        size_t tsize = fp_type_bytes(p->snap.vertex_type);
        size_t ovsize = p->snap.vertex_stride ? (size_t)p->snap.vertex_stride
                                              : (size_t)p->snap.vertex_size * tsize;
        if(ovsize == 0) return false;
        if(i == 0) {
            first_snap = &p->snap;
            vsize = ovsize;
        } else if(!fp_dl_merge_snapshots_compatible(first_snap, &p->snap)) {
            return false;
        }
        size_t verts = (size_t)p->count;
        if(verts > FP_MAX_VERTICES || total_vertices + verts > FP_MAX_VERTICES) return false;
        if((size_t)p->vertex_len != verts * ovsize) return false;
        if(p->mode == GL_QUADS) {
            if(p->count % 4 != 0) return false;
            total_indices += (verts / 4) * 6;
        } else {
            // MathCode: 三角形列表必须整组对齐，否则跨 op 拼接会把上一条
            // 多余的顶点和下一条顶点凑成错误三角形。
            if(p->count % 3 != 0) return false;
            total_indices += verts;
        }
        if(p->indexed) {
            const uint8_t* src = op->data + p->indices_off;
            for(GLsizei k = 0; k < p->count; k++) {
                if(fp_dl_read_idx(src, p->itype, k) >= (uint32_t)p->count) return false;
            }
        }
        total_vertices += verts;
        total_vertex_bytes += p->vertex_len;
    }
    if(total_vertices == 0 || total_indices == 0 || total_indices > 1024u * 1024u) return false;

    uint8_t* vdata = (uint8_t*)malloc(total_vertex_bytes);
    uint32_t* idata = (uint32_t*)malloc(total_indices * sizeof(uint32_t));
    if(!vdata || !idata) {
        free(vdata);
        free(idata);
        return false;
    }

    size_t v_out = 0;
    size_t i_out = 0;
    GLsizei vbase = 0;
    for(uint32_t i = 0; i < l->op_count; i++) {
        const dl_op_entry_t* op = &l->ops[i];
        const dl_client_draw_payload_t* p = (const dl_client_draw_payload_t*)op->data;
        memcpy(vdata + v_out, op->data + p->vertex_off, p->vertex_len);
        if(p->indexed) {
            const uint8_t* src = op->data + p->indices_off;
            if(p->mode == GL_QUADS) {
                for(GLsizei q = 0; q < p->count; q += 4) {
                    uint32_t a = fp_dl_read_idx(src, p->itype, q + 0);
                    uint32_t b = fp_dl_read_idx(src, p->itype, q + 1);
                    uint32_t c = fp_dl_read_idx(src, p->itype, q + 2);
                    uint32_t d = fp_dl_read_idx(src, p->itype, q + 3);
                    idata[i_out++] = (uint32_t)vbase + a;
                    idata[i_out++] = (uint32_t)vbase + b;
                    idata[i_out++] = (uint32_t)vbase + c;
                    idata[i_out++] = (uint32_t)vbase + a;
                    idata[i_out++] = (uint32_t)vbase + c;
                    idata[i_out++] = (uint32_t)vbase + d;
                }
            } else {
                for(GLsizei k = 0; k < p->count; k++) {
                    idata[i_out++] = (uint32_t)vbase + fp_dl_read_idx(src, p->itype, k);
                }
            }
        } else if(p->mode == GL_QUADS) {
            for(GLsizei q = 0; q < p->count; q += 4) {
                uint32_t a = (uint32_t)vbase + q + 0;
                uint32_t b = (uint32_t)vbase + q + 1;
                uint32_t c = (uint32_t)vbase + q + 2;
                uint32_t d = (uint32_t)vbase + q + 3;
                idata[i_out++] = a;
                idata[i_out++] = b;
                idata[i_out++] = c;
                idata[i_out++] = a;
                idata[i_out++] = c;
                idata[i_out++] = d;
            }
        } else {
            for(GLsizei k = 0; k < p->count; k++) {
                idata[i_out++] = (uint32_t)vbase + (uint32_t)k;
            }
        }
        v_out += p->vertex_len;
        vbase += p->count;
    }
    if(i_out != total_indices) {
        free(vdata);
        free(idata);
        return false;
    }

    GLuint vao = 0, vbo = 0, ebo = 0;
    es3_functions.glGenVertexArrays(1, &vao);
    if(vao == 0) goto fail;
    es3_functions.glGenBuffers(1, &vbo);
    if(vbo == 0) goto fail;
    es3_functions.glGenBuffers(1, &ebo);
    if(ebo == 0) goto fail;

    fp_gl_bind_vao(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    es3_functions.glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)total_vertex_bytes, vdata, GL_STATIC_DRAW);

    es3_functions.glEnableVertexAttribArray(FP_ATTR_POS);
    es3_functions.glVertexAttribPointer(FP_ATTR_POS, first_snap->vertex_size, first_snap->vertex_type,
                                        GL_FALSE, first_snap->vertex_stride, NULL);
    if(first_snap->texcoord_enabled && first_snap->texcoord_size > 0 &&
       first_snap->texcoord_off >= 0 && (size_t)first_snap->texcoord_off < vsize) {
        es3_functions.glEnableVertexAttribArray(FP_ATTR_UV);
        es3_functions.glVertexAttribPointer(FP_ATTR_UV, first_snap->texcoord_size, first_snap->texcoord_type,
                                            GL_FALSE, first_snap->vertex_stride,
                                            (const void*)(intptr_t)first_snap->texcoord_off);
    } else {
        es3_functions.glDisableVertexAttribArray(FP_ATTR_UV);
    }
    if(first_snap->color_enabled && first_snap->color_size > 0 &&
       first_snap->color_off >= 0 && (size_t)first_snap->color_off < vsize) {
        es3_functions.glEnableVertexAttribArray(FP_ATTR_COLOR);
        es3_functions.glVertexAttribPointer(FP_ATTR_COLOR, first_snap->color_size, first_snap->color_type,
                                            GL_TRUE, first_snap->vertex_stride,
                                            (const void*)(intptr_t)first_snap->color_off);
    } else {
        es3_functions.glDisableVertexAttribArray(FP_ATTR_COLOR);
    }

    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    es3_functions.glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(total_indices * sizeof(uint32_t)),
                               idata, GL_STATIC_DRAW);

    free(vdata);
    free(idata);
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)dl_saved_abo);
    fp_gl_bind_vao(0);
    dl_current_vao = 0;

    l->merge.vao = vao;
    l->merge.vbo = vbo;
    l->merge.ebo = ebo;
    l->merge.draw_count = (GLsizei)total_indices;
    l->merge.use_color = fp_dl_merge_use_color(first_snap, vsize);
    l->merge.valid = true;
    l->merge.ctx = current_context;
    return true;

fail:
    if(ebo) es3_functions.glDeleteBuffers(1, &ebo);
    if(vbo) es3_functions.glDeleteBuffers(1, &vbo);
    if(vao) es3_functions.glDeleteVertexArrays(1, &vao);
    free(vdata);
    free(idata);
    glBindBuffer(GL_ARRAY_BUFFER, (GLuint)dl_saved_abo);
    fp_gl_bind_vao(0);
    dl_current_vao = 0;
    return false;
}

static bool fp_dl_try_play_merged(fp_dl_list_t* l) {
    if(!l || !current_context || !dl_merge_enabled) return false;
    if(!l->merge.valid || l->merge.ctx != current_context) {
        if(l->merge.attempted) return false;
        if(!fp_dl_build_merged_cache(l)) return false;
    }
    if(l->merge.draw_count <= 0 || !l->merge.vao || !l->merge.ebo) return false;

    fp_client_color_active = l->merge.use_color;
    if(dl_replay_dirty || l->merge.use_color != dl_last_uuse_color) {
        fp_set_default_uniforms();
        dl_replay_dirty = false;
        dl_last_uuse_color = l->merge.use_color;
    }

    // 每次绘制前强制绑定自己的 VAO + EBO（不信任可能过期的跟踪值），
    // 避免用应用侧 VAO（可能没有 EBO）执行 glDrawElements。
    if(dl_current_vao != l->merge.vao) {
        fp_gl_bind_vao(l->merge.vao);
        dl_current_vao = l->merge.vao;
    }
    es3_functions.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, l->merge.ebo);
    es3_functions.glDrawElements(GL_TRIANGLES, l->merge.draw_count, GL_UNSIGNED_INT, NULL);
    return true;
}

static void fp_dl_free_merged(fp_dl_list_t* l) {
    if(!l) return;
    if(l->merge.vao) {
        GLint bound_vao = 0;
        es3_functions.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound_vao);
        if((GLuint)bound_vao == l->merge.vao) {
            fp_gl_bind_vao(0);
            dl_current_vao = 0;
        }
        if(l->merge.ebo) es3_functions.glDeleteBuffers(1, &l->merge.ebo);
        if(l->merge.vbo) es3_functions.glDeleteBuffers(1, &l->merge.vbo);
        es3_functions.glDeleteVertexArrays(1, &l->merge.vao);
    }
    l->merge.vao = l->merge.vbo = l->merge.ebo = 0;
    l->merge.valid = false;
    l->merge.attempted = false;
    l->merge.ctx = NULL;
}

// 释放 op 的回放缓存 GL 对象。缓存句柄非 0 时一定属于当前 context
// （context 重建时 fp_dl_reset_caches 已清零），且调用方保证存在当前
// context（glDeleteLists 等入口均检查 current_context）。
static void fp_dl_free_cache(dl_op_entry_t* op) {
    if(!op || !op->cache_vao) return;
    GLint bound_vao = 0;
    es3_functions.glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &bound_vao);
    if((GLuint)bound_vao == op->cache_vao) {
        fp_gl_bind_vao(0);
        dl_current_vao = 0;
    }
    if(op->cache_ebo) es3_functions.glDeleteBuffers(1, &op->cache_ebo);
    if(op->cache_vbo) es3_functions.glDeleteBuffers(1, &op->cache_vbo);
    es3_functions.glDeleteVertexArrays(1, &op->cache_vao);
    op->cache_vao = 0;
    op->cache_vbo = 0;
    op->cache_ebo = 0;
    op->cache_valid = 0;
    op->cache_ctx = NULL;
}

// EGL context 重建后调用：显示列表 CPU 快照保留，但所有 GL 对象句柄失效，
// 清零后首次回放会在新 context 里重建缓存。
static void fp_dl_reset_caches(void) {
    for(uint32_t i = 0; i < dl_table_cap; i++) {
        fp_dl_list_t* l = dl_table[i];
        if(!l) continue;
        for(uint32_t j = 0; j < l->op_count; j++) {
            dl_op_entry_t* op = &l->ops[j];
            op->cache_vao = 0;
            op->cache_vbo = 0;
            op->cache_ebo = 0;
            op->cache_valid = 0;
            op->cache_ctx = NULL;
        }
        l->merge.vao = l->merge.vbo = l->merge.ebo = 0;
        l->merge.draw_count = 0;
        l->merge.valid = false;
        l->merge.attempted = false;
        l->merge.ctx = NULL;
    }
}

static void fp_dl_execute_op(dl_op_entry_t* op) {
    if(!op || !op->data || op->size == 0) return;
    const uint8_t* payload = op->data;
    switch(op->type) {
        case DL_OP_IMMEDIATE: {
            if(op->size < sizeof(dl_immediate_payload_t)) return;
            const dl_immediate_payload_t* p = (const dl_immediate_payload_t*)payload;
            if((size_t)p->verts_off + (size_t)p->count * FP_VERTEX_BYTES > (size_t)op->size) return;
            fp_dl_play_immediate(p->mode,
                                 (const GLfloat*)((const uint8_t*)payload + p->verts_off),
                                 (GLsizei)p->count);
            break;
        }
        case DL_OP_CLIENT_DRAW: {
            if(op->size < sizeof(dl_client_draw_payload_t)) return;
            const dl_client_draw_payload_t* p = (const dl_client_draw_payload_t*)payload;
            const void* data = NULL;
            const void* idx = NULL;
            if(p->vertex_len > 0 &&
               (size_t)p->vertex_off + (size_t)p->vertex_len <= (size_t)op->size) {
                data = payload + p->vertex_off;
            }
            if(p->indexed && p->indices_len > 0 &&
               (size_t)p->indices_off + (size_t)p->indices_len <= (size_t)op->size) {
                idx = payload + p->indices_off;
            }
            if(dl_replay_active) {
                fp_dl_play_client_cached(op, p, data, idx);
            } else {
                fp_dl_play_client_draw(&p->snap, p->mode, p->first, p->count, p->indexed, p->itype,
                                       p->indices_ebo, p->indices_src_off, idx, p->indices_len, data);
            }
            break;
        }
        case DL_OP_BIND_TEXTURE: {
            if(op->size < sizeof(dl_bind_texture_payload_t)) return;
            const dl_bind_texture_payload_t* p = (const dl_bind_texture_payload_t*)payload;
            if(p->target == GL_TEXTURE_2D) {
                glActiveTexture(p->unit);
                glBindTexture(GL_TEXTURE_2D, p->texture);
                dl_replay_dirty = true; // 纹理绑定已变，绘制前刷新 uniforms
            }
            break;
        }
        case DL_OP_TEXTURE_ENABLE: {
            if(op->size < sizeof(dl_texture_enable_payload_t)) return;
            const dl_texture_enable_payload_t* p = (const dl_texture_enable_payload_t*)payload;
            fp_set_unit0_texture_enabled(p->enabled != 0);
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
    fp_dl_free_merged(l);
    for(uint32_t i = 0; i < l->op_count; i++) {
        fp_dl_free_cache(&l->ops[i]);
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
        uint32_t old_count = l->op_count;
        dl_op_entry_t* no = (dl_op_entry_t*)realloc(l->ops, (size_t)ncap * sizeof(dl_op_entry_t));
        if(!no) {
            l->failed = true;
            return false;
        }
        l->ops = no;
        l->op_cap = ncap;
        // MathCode: 扩容后必须把整个新增区域清零，不能只清第一个条目。
        // cache_vao/cache_vbo/cache_ebo 等回放缓存字段未初始化时，
        // dl_clear_ops 清理列表会把垃圾句柄当有效 VAO 删除（诊断日志确认
        // 曾以 vao=1 误删内部私有 fp_vao），导致此后每次绑定 fp_vao 都生成
        // GL_INVALID_OPERATION(0x502)，并连锁刷出数万行 "stale 0x502"
        // （Post render 1282 的根因）。
        memset(&l->ops[old_count], 0, (size_t)(ncap - old_count) * sizeof(dl_op_entry_t));
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

    // 最外层 glCallList 进入批量回放：默认 program/VAO/纹理只绑定一次，
    // 嵌套列表（DL_OP_CALL_LIST）在同一批内继续执行
    bool outer = !dl_replay_active;
    if(outer) {
        if(!fp_begin_dl_replay()) return;
        dl_replay_active = true;
    }

    dl_exec_chain[dl_exec_depth++] = list;
    l->exec_count++;
    // MathCode: 实验性整表合并。整表可合并时一次 glDrawElements 画完，
    // 跳过逐 op 回放；不可合并/未开启时走原有路径。
    if(dl_replay_active && fp_dl_try_play_merged(l)) {
        // 合并绘制已覆盖整个列表
    } else {
        for(uint32_t i = 0; i < l->op_count; i++) {
            dl_op_entry_t* op = &l->ops[i];
            if(op->type == DL_OP_CALL_LIST) {
                GLuint sub = 0;
                if(op->size >= sizeof(sub)) memcpy(&sub, op->data, sizeof(sub));
                dl_execute_list(sub);
            } else {
                fp_dl_execute_op(op);
            }
        }
    }
    l->exec_count--;
    dl_exec_depth--;
    if(l->pending_delete && l->exec_count == 0) dl_free_list(l);

    if(outer) {
        // fallback 路径可能临时退出并重新进入批量状态；若重新进入失败
        // （理论上仅在 program/VAO 初始化失败时发生），这里不再重复恢复
        if(dl_replay_active) {
            dl_replay_active = false;
            fp_end_dl_replay();
        }
    }
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
