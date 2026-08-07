# Minecraft 1.12.2 字体白块问题排查与修复记录

## 1. 问题描述

LTW-Turbo 在 Android 上运行 Minecraft 1.12.2 时，主菜单左下角与右下角的小字（版本号、版权信息等）渲染为白色矩形块，按钮上的文字正常。

白块并非整体缺失，而是字形轮廓完全不可见，表现为若干等宽的白色方块。该问题仅出现在 1.12.2 及更早版本的固定管线文字路径中。

## 2. 环境信息

| 项目 | 值 |
| --- | --- |
| Minecraft | 1.12.2 |
| 渲染层 | LTW-Turbo（OpenGL 到 OpenGL ES 翻译层） |
| 设备 | Android 14，ARM64 |
| GL 驱动 | Turnip/Zink（Vulkan 后端） |
| 分辨率 | 2400 × 1080 |

## 3. 渲染链路确认

1.12.2 的 `FontRenderer` 使用桌面 OpenGL 固定管线绘制文字：

```java
GlStateManager.glBegin(GL11.GL_TRIANGLE_STRIP);
GlStateManager.glTexCoord2f(u, v);
GlStateManager.glVertex3f(x, y, 0.0F);
GlStateManager.glEnd();
```

LTW-Turbo 通过 `fixed_pipeline.c` 模拟该路径：

- `glBegin/glEnd` 收集顶点；
- `glTexCoord2f/glVertex3f/glColor4f` 维护顶点状态；
- 默认 shader 完成 MVP 变换、纹理采样与 alpha test；
- `glEnd` 时上传 VBO 并执行 `glDrawArrays`。

字形纹理由 `TextureUtil` 上传，格式为：

```text
internalformat = GL_RGBA
format         = GL_BGRA
type           = GL_UNSIGNED_INT_8_8_8_8_REV
```

## 4. 排查过程与证据

### 4.1 纹理上传

在 `glTexSubImage2D` 入口与出口加入日志，确认字形纹理上传调用是否到达 LTW，以及上传后是否产生 GL 错误。

结果：

| 检查项 | 结果 |
| --- | --- |
| `glTexSubImage2D` 是否被拦截 | 是 |
| 上传格式转换 | `GL_BGRA + UNSIGNED_INT_8_8_8_8_REV` → `GL_RGBA + UNSIGNED_BYTE` |
| 上传后 GL 错误 | `GL_NO_ERROR` |

结论：纹理上传链路本身没有报错。

### 4.2 纹理内容

通过 framebuffer 读回字形纹理原始像素，结果如下：

```text
(0,0)   = 255,255,255,255   // 字形不透明
(1,0)   = 255,255,255,0     // 背景透明
(64,64) = 255,255,255,0
(128,128) = 255,255,255,255
```

字形纹理内容正确：白色字形 alpha 为 255，透明背景 alpha 为 0。

### 4.3 alpha test 状态

日志确认 Minecraft 设置了：

```text
glAlphaFunc(GL_GREATER, 0.1F)
glEnable(GL_ALPHA_TEST)
```

LTW 的默认 shader 应执行 `fc.a > 0.1` 的 discard。该状态在绘制时已正确记录。

### 4.4 swizzle 状态排除

设备驱动对 `GL_TEXTURE_SWIZZLE_*` 的查询结果异常：

```text
tex26 swizzle = 0x0, 0x0, 0x0, 0x0
```

为此加入了两项防御性处理：

1. 在 `glTexSubImage2D` 中对 `GL_BGRA + UNSIGNED_INT_8_8_8_8_REV` 执行 CPU 侧 B/R 字节交换，直接生成 RGBA；
2. 上传前将纹理 swizzle 复位为默认 `R/G/B/A`。

这两项处理排除了驱动 swizzle 状态对字形采样的影响，但问题仍然存在，说明 swizzle 不是根因。

### 4.5 屏幕像素取证

根据截图确认白块位于屏幕左下角与右下角。将截图坐标转换为 `glReadPixels` 坐标后，在所有绘制路径后轮询固定像素点。

结果：

```text
[LTW WHITE] (13,25) = 255,255,255,0
[LTW WHITE] (60,25) = 255,255,255,0
```

该颜色出现在字形 `glEnd` 绘制之后，说明白块正是由字形绘制写入的，且写入内容为“白色、alpha=0”的像素。

## 5. 根因

默认 shader 的 alpha test 逻辑如下：

```glsl
if (uAlphaFunc == 0)      atpass = false;
else if (uAlphaFunc == 1) atpass = fc.a < uAlphaRef;
else if (uAlphaFunc == 2) atpass = abs(fc.a - uAlphaRef) < 0.0039;
else if (uAlphaFunc == 3) atpass = fc.a <= uAlphaRef;
else if (uAlphaFunc == 4) atpass = fc.a > uAlphaRef;
else if (uAlphaFunc == 5) atpass = abs(fc.a - uAlphaRef) >= 0.0039;
else if (uAlphaFunc == 6) atpass = fc.a >= uAlphaRef;
```

shader 使用 `0..7` 表示 `GL_NEVER..GL_ALWAYS`。但 `fp_set_default_uniforms` 之前直接传递原始 GL 枚举：

```c
glUniform1i(fp_alphafunc_loc, fp_alpha_test_func);
```

`GL_GREATER` 的原始值为 `0x0204`，即十进制 `516`。shader 中没有任何分支匹配 `516`，因此 `atpass` 保持为 `true`，alpha test 实际从未 discard。

字形纹理的透明背景为 `(255,255,255,0)`。alpha test 失效后，这些透明像素未被丢弃，而是以白色写入 framebuffer，最终形成白块。

## 6. 修复方案

在 `fp_set_default_uniforms` 中将 GL 枚举映射为 shader 使用的 `0..7`：

```c
GLint alpha_mode = 7;
if (fp_alpha_test) {
    switch (fp_alpha_test_func) {
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
glUniform1i(fp_alphafunc_loc, alpha_mode);
```

`GL_GREATER` 现在映射为 `4`，shader 执行 `fc.a > uAlphaRef`，透明背景被 discard，字形正常显示。

## 7. 验证

修复后：

- 左下角/右下角小字正常显示；
- 按钮文字保持正常；
- 字形纹理透明背景不再写入白色像素；
- 屏幕像素探针在该区域不再检测到 `(255,255,255,0)`。

## 8. 后续清理

排查过程中加入的日志与探针已全部移除：

- `[LTW DIAG]` / `[LTW DUMP]` / `[LTW CNT]` / `[LTW PIX]` / `[LTW WHITE]` 输出；
- 纹理上传、顶点数据、绘制路径诊断；
- 屏幕像素读回探针；
- stub 函数与 override 注册的启动日志。

保留的修复逻辑：

- alpha test 枚举映射；
- CPU 侧 BGRA→RGBA 转换；
- 纹理 swizzle 默认值复位；
- 固定管线绘制前后的纹理状态恢复。

## 9. 涉及文件

| 文件 | 说明 |
| --- | --- |
| `ltw/src/main/tinywrapper/fixed_pipeline.c` | 默认 shader uniform 设置、alpha test 映射、固定管线绘制 |
| `ltw/src/main/tinywrapper/of_buffer_copier.c` | `glTexSubImage2D` 的 CPU 侧 BGRA 转换 |
| `ltw/src/main/tinywrapper/swizzle.c` | swizzle 状态复位与默认值处理 |
| `ltw/src/main/tinywrapper/fixed_pipeline_gl.c` | 固定管线 GL 入口 |

该技术文档由MathCode提供。此bug目前已修复。
