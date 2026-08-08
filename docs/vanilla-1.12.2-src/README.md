# Minecraft 1.12.2 原版源码（调试参考）

这里保存的是 Minecraft 1.12.2 客户端的关键类源码，仅供 LTW 渲染兼容层
调试时对照“原版到底发了哪些 GL 调用”使用。

> 作者：MathCode（LTW-Turbo）

## 来源

仓库 `KealJones/mc-1.12.2-source_files`（MCP 反混淆源码，MCP 命名）。
如需其他类，可到该仓库拉取：
`https://github.com/KealJones/mc-1.12.2-source_files`（`src/minecraft/...`）。

## 包含文件与用途

| 文件 | 调试用途 |
| --- | --- |
| `RenderLivingBase.java` | 生物实体渲染：受伤红闪、亮度、`setBrightness`/`unsetBrightness` |
| `Gui.java` | `drawGradientRect` 等无纹理矩形绘制（物品提示黑框） |
| `GuiScreen.java` | `drawHoveringText` 物品提示框的完整调用序列 |
| `FontRenderer.java` | 文字字形、下划线/删除线（无纹理 POSITION 矩形） |
| `ScreenShotHelper.java` | 世界缩略图/F2 截图：从 FBO 颜色纹理 `glGetTexImage(GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV)` 读回 |
| `GuiListWorldSelectionEntry.java` | 世界列表：加载 `icon.png` 为 `DynamicTexture` 并绘制 32x32 缩略图 |

## 已确认的关键调用序列（2026-08 调试记录）

### 1. 物品提示黑框消失

`Gui.drawGradientRect`（`Gui.java:90`）：

1. `GlStateManager.disableTexture2D()`；
2. `enableBlend` + `tryBlendFuncSeparate(SRC_ALPHA, ONE_MINUS_SRC_ALPHA, ONE, ZERO)`；
3. `bufferbuilder.begin(7, DefaultVertexFormats.POSITION_COLOR)` → GL_QUADS，
   只有位置 + 顶点色，**没有纹理坐标**；
4. `tessellator.draw()` → `glDrawArrays(GL_QUADS)`。

LTW 之前的 bug：默认 shader 的 `uUseTex` 只看“是否绑定了纹理”，忽略了
`GL_TEXTURE_2D` 启用状态，于是这些无纹理矩形采样了上一个绑定的纹理
（物品图标）在 UV(0,0) 处的透明像素 → 黑框不可见。

修复：`fp_set_default_uniforms` 中 `uUseTex = fp_bound_texture && fp_texture_enabled`。

### 2. 生物受伤红闪缺失

`RenderLivingBase.setBrightness`（`RenderLivingBase.java:270`，受伤分支
`flag1 = hurtTime > 0 || deathTime > 0`）：

1. 活动纹理单元切到 `OpenGlHelper.lightmapTexUnit`（unit1）；
2. `glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE)`，
   `GL_SOURCE0_RGB = GL_CONSTANT`、`GL_SOURCE1_RGB = GL_PREVIOUS`、
   `GL_SOURCE2_RGB = GL_CONSTANT`、`GL_OPERAND2_RGB = GL_SRC_ALPHA`；
3. `glTexEnv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, (1,0,0,0.3))`；
4. 模型照常绘制一次，靠纹理环境插值把模型染红：`result = prev*(1-a) + red*a`。

LTW 之前的 bug：`glTexEnv*` 没进 override 表，GLES 也没有纹理环境，调用被
丢掉 → 红闪完全消失。

修复：`fixed_pipeline_gl.c` 新增 `glTexEnv/glTexEnvi/glTexEnvf/glTexEnvfv/
glTexEnviv` 包装，`fixed_pipeline.c` 记录 unit1 的合并状态（`fp_texenv_state`），
默认 shader 用 `uLightTint/uLightColor` 模拟：
`fc.rgb = mix(fc.rgb, envColor.rgb, envColor.a)`。

### 3. 世界缩略图变黑

`EntityRenderer.createWorldIcon`（`EntityRenderer.java:1230`，世界渲染稳定后每
秒检测一次）：

1. `ScreenShotHelper.createScreenshot(displayWidth, displayHeight, mc.getFramebuffer())`；
2. `GlStateManager.bindTexture(framebufferTexture)`；
3. `GlStateManager.glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, pixelBuffer)`，
   即 `glGetTexImage(3553, 0, 32993, 33639, ...)`；
4. `ImageIO.write(64x64, "png", saves/<world>/icon.png)`。

世界列表读取同一文件（`GuiListWorldSelectionEntry.loadServerIcon`），所以生成
的全黑 PNG 会直接显示为黑色缩略图。

LTW 之前的 bug：`glGetTexImage` 白名单只允许 RGBA 系格式/类型，
`GL_BGRA + GL_UNSIGNED_INT_8_8_8_8_REV` 被判为 unsupported，函数直接返回、
不写 `pixelBuffer` → 像素数组全 0 → `icon.png` 全黑。

修复：`of_buffer_copier.c` 的 `glGetTexImage` 对该组合先按
`GL_RGBA + GL_UNSIGNED_BYTE` 读回，再把每个像素的 R/B 字节交换成
BGRA+REV 的内存布局（即 Java ARGB 整数）。F2 截图与世界缩略图共用此路径，
一并修复。

## 注意事项

- 这些是 1.12.2 **原版**逻辑；OptiFine/Forge 可能改动部分方法，但受伤红闪
  与提示框机制仍是同一套 GL 状态。
- `FontRenderer.java:531` 附近：下划线/删除线也是
  `disableTexture2D + POSITION` 无纹理矩形，属于同一类 bug 的姊妹路径。
