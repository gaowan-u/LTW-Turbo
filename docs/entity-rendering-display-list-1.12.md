# Minecraft 1.12.1 生物不可见（有影子）排查与修复记录

## 1. 问题描述

LTW-Turbo 在 Android（Fold Craft Launcher + LTW 渲染器）运行 Minecraft
1.12.1 Forge 时，生物（村民、僵尸等）身体完全不渲染，但生物脚下阴影正常。
掉落物、方块、字体等渲染正常。日志中无 GL 报错、无 Java 异常、无贴图缺失。

## 2. 根因

1.12.x 的 `ModelRenderer` 把模型几何编译进 **显示列表**：

```java
this.displayList = GLAllocation.generateDisplayLists(1); // glGenLists
GlStateManager.glNewList(this.displayList, GL_COMPILE);  // glNewList
for (Box box : this.cubeList) box.render(buffer, scale); // Tessellator draw
GlStateManager.glEndList();                              // glEndList
```

每帧通过 `glCallList` 回放。而生物阴影由 `RenderManager.renderShadow`
直接走 Tessellator 绘制，**不经过显示列表**。

LTW 此前把所有显示列表入口（`glGenLists/glNewList/glEndList/glCallList/
glDeleteLists`）实现为空 stub，因此身体绘制被静默丢弃、影子正常，
与"生物不可见但有影子"的现象完全吻合；无 GL 报错也正因调用"成功"但无事发生。

## 3. 修复方案

在 `ltw/src/main/tinywrapper/fixed_pipeline.c` 内实现显示列表录制/回放：

- `glNewList(GL_COMPILE) .. glEndList` 期间，把固定管线调用录制成 op 序列：
  - `DL_OP_IMMEDIATE`：`glBegin/glEnd` 顶点快照（pos+color+uv）
  - `DL_OP_CLIENT_DRAW`：客户端数组 `glDrawArrays/glDrawElements` 快照
    （MC 关闭 VBO 时 Tessellator 走 `glVertexPointer` 路径）
  - `DL_OP_BIND_TEXTURE` / `DL_OP_TEXTURE_ENABLE`：列表内纹理状态
  - `DL_OP_CALL_LIST`：嵌套列表调用
- `glCallList` 时按 op 顺序回放，复用既有绘制链
  （QUADS→TRIANGLES 展开、默认 shader、纹理刷新），
  矩阵/纹理采用**执行时**状态（桌面 GL 语义：显示列表内顶点在执行时变换）。

## 4. 健壮性设计

- **编译期快照**：顶点/客户端数组数据在录制时拷贝，不依赖录制时的临时缓冲
  （MC 的 Tessellator ByteBuffer 会被下一个 box 复用）
- **内存上限**：单 op 8MB / 单列表 16MB / 全局 64MB，超限整表丢弃并打日志，
  避免半成品几何和失控内存
- **嵌套编译**：防御性支持，内层列表作为 `CALL_LIST` 记入外层
- **递归保护**：执行链查重 + 深度上限，防止 `glCallList` 自递归死循环
- **执行中删除**：`glDeleteLists` 命中正在执行的列表时延迟到执行结束释放，
  避免 use-after-free
- **上下文重建**：列表数据为 CPU 拷贝，跨 EGL context 重建仍有效；
  `fp_init` 顺带修复了即时模式缓冲未释放的小泄漏
- **GL 错误纪律**：所有路径防御性校验（空指针、越界偏移、负计数），
  编译期绘制一律吞掉，不产生半成品绘制

## 5. 涉及文件

| 文件 | 改动 |
| --- | --- |
| `ltw/src/main/tinywrapper/fixed_pipeline.h` | 显示列表 API 声明 |
| `ltw/src/main/tinywrapper/fixed_pipeline.c` | 录制/回放/注册表/生命周期实现 |
| `ltw/src/main/tinywrapper/fixed_pipeline_gl.c` | `glGenLists/glNewList/...` 入口 |
| `ltw/src/main/tinywrapper/main.c` | `glDrawArrays/glDrawElements/glBindTexture/glEnable` 录制钩子 |
| `ltw/src/main/tinywrapper/es3_overrides.h` | 注册 8 个显示列表 GL 入口 |

## 6. 验证清单

- 生物/玩家/盔甲架/箱子（ModelRenderer 路径）可见，阴影正常
- 关闭/开启 VBO（`useVbo`）两种模式均正常
- 长时间游玩内存不增长（列表删除与全局上限生效）
- `latest.log` 无 `display list ... cap exceeded` 异常日志
- 快速进出生存/退档重进，无崩溃、无残留几何

本 bug 修复后 MC <=1.12 的实体渲染与桌面端一致。

## 7. 已知限制

- **VBO 路径**：MC 的 `glVertexAttribPointer` 目前直通 GLES、不更新
  `fixed_pipeline` 的客户端数组状态，因此开启 VBO 时显示列表内录不到几何。
  FCL/Pojav 对 <=1.12 强制使用客户端数组，MC 1.12 实际不受影响；若未来要
  支持 VBO 路径，需要额外拦截 `glVertexAttribPointer` 并快照属性状态。
- **列表内矩阵调用**：只快照顶点数据，`glTranslate/glRotate` 等写在列表内的
  矩阵调用不会录制；顶点按 `glCallList` 执行时的矩阵变换（桌面语义），
  MC 1.12 的模型变换都在列表外，行为一致。
- **`glCallLists` 的 `GL_FLOAT` 索引**：按截断取整处理（MC 不使用该路径）。
- **嵌套 `glNewList`**：桌面 GL 属未定义/错误用法，这里防御性支持
  （内层作为 `CALL_LIST` 记入外层），语义与桌面存在细微差异。
