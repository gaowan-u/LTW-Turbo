# 渲染性能优化记录

## 1. 背景

LTW-Turbo 在 MC 1.12.1（FCL + 骁龙 835 设备）的动物场景出现个位数帧率。
日志无 GL 报错、无崩溃，定位为渲染路径的 CPU/驱动调用开销问题而非错误：

- 1.12 的生物模型通过显示列表（`glNewList/glCallList`）渲染，每帧回放；
- 同一模型表被同类型数十个实体每帧复用，每个列表约 10 个几何 op；
- 旧回放路径每个 op 都做绑定-查询-上传-解绑，导致每帧数千次驱动调用
  + 数千次顶点重传 + 每次 `malloc`，在旧驱动上退化为个位数帧率。

本文记录三轮针对性优化：显示列表批量回放与几何缓存、纹理/程序/缓冲
绑定的 CPU 跟踪、QUADS 展开免分配与索引缓存。

## 2. 第一轮：显示列表回放（修复动物场景掉帧）

改动集中在 `ltw/src/main/tinywrapper/fixed_pipeline.c`。

### 2.1 每 op 几何缓存（VAO + VBO + EBO）

显示列表录制时顶点/索引是 CPU 快照，之后不可变。旧路径每帧把同一份
快照重新 `glBufferData`（`GL_STREAM_DRAW` 会触发驱动分配/同步开销），
每次现算 QUADS→TRIANGLES 展开并重新上传。

现在 `dl_op_entry_t` 携带 `cache_vao/cache_vbo/cache_ebo`：

- 首次回放时一次性上传顶点（`GL_STATIC_DRAW`）、把 QUADS 展开为三角形
  索引存 EBO、定型为私有 VAO；
- 之后每次回放只需 `bind VAO + glDrawElements`；
- 缓存带 `cache_ctx`（context_t*）防跨 context 误用；EGL context 重建时
  `fp_dl_reset_caches()` 清零句柄，惰性重建；列表删除时 `fp_dl_free_cache()`
  同步释放 GL 对象。

### 2.2 批量回放状态

一次 `glCallList` 执行期间（`fp_begin_dl_replay`/`fp_end_dl_replay`）：

- 默认 program / 私有 VAO / unit0 纹理只绑定一次，op 之间只切换各自缓存 VAO；
- 应用状态（VAO/EAB/ABO/program/活动纹理单元）进入时保存、退出统一恢复，
  内部活动单元跟踪同步；
- 纹理绑定变化、即时模式绘制后通过 `dl_replay_dirty` 按需刷新 uniforms；
- 无法缓存的路径（VBO 直通、源 EBO）挂起批量状态走旧逻辑，再重新进入。

### 2.3 QUADS 即时模式 scratch

即时模式的 QUADS 顶点展开从每次 `malloc/free` 改为可复用 scratch
（`fp_quad_scratch`），随 context 重建释放。

## 3. 第二轮：绑定状态 CPU 跟踪

### 3.1 纹理绑定零查询（fixed_pipeline.c / main.c）

原来每次 `glBindTexture(GL_TEXTURE_2D)` 都向驱动做
`glGetIntegerv(GL_ACTIVE_TEXTURE)` + `glGetIntegerv(GL_TEXTURE_BINDING_2D)`
+ `glGetTexLevelParameteriv(GL_TEXTURE_INTERNAL_FORMAT)`（判断单通道，
字形等 alpha 纹理依赖），开销明显。

现在：

- `glBindTexture` 包装器直接调用 `fp_notify_texture_bind_tex`，unit0 绑定
  用 CPU 维护（`fp_bound_texture`），零驱动查询；
- 单通道格式判断走 64 槽直接映射缓存（`fp_texfmt_cache`），带世代号与
  context 隔离；`glTexImage2D`/`glCopyTexImage2D`（可能改格式）使缓存失效；
- `fp_refresh_bound_texture` 保留为兜底（活动单元切回 unit0、显示列表批量
  回放时），并顺带校正内部活动单元跟踪；
- 删除了 `fp_bind_default_program`/`fp_flush_immediate` 里每次绘制的
  纹理保存/绑定/恢复（unit0 绑定从未被改写，恢复是多余操作）。

### 3.2 当前 program 跟踪

`glUseProgram` 包装器 + 固定管线自身 5 处绑定路径统一维护
`current_context->program`；`fp_try_draw_arrays/elements` 不再每次
`glGetIntegerv(GL_CURRENT_PROGRAM)`。

### 3.3 第三轮补充：ARRAY_BUFFER 绑定跟踪

`glBindBuffer` 包装器本就在维护 `current_context->bound_buffers[]`，
但固定管线内部一直用 `es3_functions.glBindBuffer` 直连，导致跟踪失效。
现将 fixed_pipeline.c 内部全部 ARRAY_BUFFER 绑定点改走包装器，
`fp_upload_client_arrays` / `fp_prepare_client_arrays`（VBO 路径）/
`fp_flush_immediate` 的 ABO 查询全部改为读 `bound_buffers[0]`。

## 4. QUADS 展开优化（quads.c / egl.h）

MC 1.12 的影子、方块裂纹等非索引 QUADS 绘制，原来每次调用都
`malloc` 临时索引数组、展开一趟、再上传 scratch EBO。现在：

- 非索引路径的源索引就是 `first..first+count-1`，直接在可复用缓冲
  `ctx->quads_expanded` 生成展开结果，免去 malloc 与中间数组；
- 展开结果只取决于 `(first, count)`，与顶点内容无关；新增
  `quads_last_first/quads_last_count` 缓存键，参数不变时跳过 scratch EBO
  重传；
- 修复共用 scratch EBO 的交叉失效隐患：客户端数组、源 EBO、客户端索引
  三条路径共用一个 scratch EBO，原代码缺少交叉失效可能画出过期索引；
  现在用 `quads_last_ebo` 取值区分生产者（0=客户端数组、-1=客户端索引、
  其它=源 EBO），任一路径上传都会使其它路径缓存键失效。

## 5. 效果

| 路径 | 优化前 | 优化后 |
| --- | --- | --- |
| 显示列表 CLIENT_DRAW op 回放 | ~25 驱动调用 + 2 次缓冲重传 + malloc | 3 次调用（bind VAO + draw + bind fp_vao），0 重传 |
| 每次 glBindTexture(GL_TEXTURE_2D) | 3-4 个驱动查询/调用 | 1 次 CPU 写入 + 缓存命中 |
| 每次固定管线绘制 | ~20 调用（含 4-6 次查询） | ~12 调用（查询基本清零） |
| 每次非索引 QUADS 展开 | malloc + 两趟 + EBO 重传 | 免分配，参数未变时零上传 |

## 6. 涉及文件

| 文件 | 改动 |
| --- | --- |
| `ltw/src/main/tinywrapper/fixed_pipeline.c` | 显示列表几何缓存/批量回放、纹理与程序绑定跟踪、ARRAY_BUFFER 内部绑定改走包装器 |
| `ltw/src/main/tinywrapper/fixed_pipeline.h` | 新增 `fp_notify_texture_bind_tex` / `fp_texture_upload_invalidate` |
| `ltw/src/main/tinywrapper/main.c` | `glBindTexture` 走 CPU 跟踪；纹理上传入口使格式缓存失效 |
| `ltw/src/main/tinywrapper/quads.c` | QUADS 免分配展开、客户端路径索引缓存、交叉失效 |
| `ltw/src/main/tinywrapper/egl.h` | context 新增客户端 QUADS 缓存键 |

## 7. 验证清单

- 动物群场景（牛/羊/猪各数十只同屏）帧率恢复流畅，无 GL 报错；
- 关闭/开启 VBO 两种模式均正常；生物可见、阴影正常、字体正常；
- 长时间游玩内存不增长（显示列表缓存随列表删除释放、格式缓存 64 槽固定）；
- 快速进出存档/重建 EGL context 无崩溃、无残留几何（缓存随 context 重建
  自动失效）；
- 日志无 `display list ... cap exceeded`、无 GL 错误刷屏。

## 8. 已知限制

- 显示列表 VBO 直通路径（`vertex_abo != 0`）不建几何缓存，回退旧路径
  （FCL/Pojav 对 <=1.12 强制客户端数组，实际不影响）；
- 纹理格式缓存只在 `glTexImage2D`/`glCopyTexImage2D` 失效，`glTexStorage2D`
  未被拦截（未走 LTW 包装器），极端情况下单通道判断可能多一次查询，
  不影响正确性；
- `glBindBuffer` 跟踪在应用绑定非法 buffer 时可能与真实状态不一致
  （应用侧错误，固定管线读取前会先绑定自己的缓冲，不产生错误绘制）。

## 9. 实验性：显示列表整表合并（LTW_DL_MERGE）

在显示列表批量回放之上再进一步：如果整个列表全部是“可缓存 CLIENT_DRAW
op、顶点格式完全一致、中间没有任何纹理/开关/嵌套/即时模式 op”，就把所有
op 的顶点拼进一个大 VBO、索引拼进一个大 EBO，回放时一次
`glDrawElements(GL_TRIANGLES)` 画完整个列表；不满足条件自动回退原每 op
缓存路径。

- 开关：`LTW_DL_MERGE`（默认 `1`，设为 `0` 关闭）；
- 缓存首次建立时一次性 CPU 拼接 + GPU 上传，之后每帧只有一次绘制；
- 合并缓存随列表删除/EGL context 重建释放或失效，与原有每 op 缓存一致。

### 9.1 应用侧桥接（参考 MobileGlues 思路，实现独立）

设置 App 会把配置写到共享文件 `/sdcard/LTW-Turbo/config.json`
（目录可用环境变量 `LTW_CONFIG_DIR` 覆盖），LTW 渲染库启动时自己读取：

- `dlMerge`：整表合并开关（`true`/`false`）；
- 优先级：`LTW_DL_MERGE` 环境变量 > 共享配置 > 内置默认值；
- 不依赖启动器注入环境变量；`ltw_config.c` 是轻量扁平 JSON 读取器，
  只解析本应用生成的 int/bool/string 字段。
