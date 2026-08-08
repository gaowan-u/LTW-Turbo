# LTW-Turbo

**让经典，重获新生。 (Let the classic be reborn.)**

> ### **⚠️ 项目状态 (Project Status)**
>
> 请注意，LTW-Turbo 目前仍处于**早期积极开发阶段**。
>
> 当前代码库主要作为项目重启的起点，核心功能尚在重写和开发中。因此，当前版本并非稳定的正式版，主要用于开发和测试，可能存在功能不全或不稳定的情况。
>
> 感谢您的关注与耐心等待！

## 关于项目 (About the Project)

LTW 是一个开源项目，致力于为 Minecraft Java 版（1.17 及更高版本）的玩家提供一个高性能、持续维护的 OpenGL ES 渲染兼容层。

随着社区的发展，新的渲染技术层出不穷，但它们大多专注于最新的 Minecraft 版本。这使得许多热爱经典版本（1.17-）的玩家，无法享受到持续的性能优化和问题修复。

LTW-Turbo 的诞生，正是为了回应这份需求。我们继承了 LTW 项目的精神，并为其注入新的活力。我们的目标不是追赶最新版本，而是守护经典，让那些依旧在老版本中探索的玩家们，能拥有更流畅、更稳定的游戏体验。

我们相信，每一个版本都有其独特的魅力和忠实的玩家群体。LTW-Turbo 愿成为连接经典与现代的桥梁。

---

## 技术原理及版本限制

### 核心原理 (Core Principle)

LTW (Large Thin Wrapper) 的本质是一个实时的 **OpenGL 到 OpenGL ES 的指令翻译层**。Minecraft Java 版原生使用桌面平台的 OpenGL API，而安卓设备则支持 OpenGL ES (GLES)。LTW 的工作就是在这两者之间架起一座桥梁，其关键技术包括：

1.  **API 拦截 (Hooking)**：通过 `eglGetProcAddress` 拦截游戏对 OpenGL 函数的调用请求。
2.  **版本伪装 (Spoofing)**：在 `glGetString` 等函数中返回虚假的桌面级 OpenGL 版本号（如 3.0+），以“欺骗”游戏，使其认为自己运行在兼容的环境中。
3.  **着色器转换 (Shader Translation)**：这是最核心的部分。LTW 在 `glShaderSource` 中拦截游戏发送的 GLSL（桌面版着色器语言）代码，并利用内置的 `glsl_optimizer` 工具，将其动态编译转换为移动端 GLES 支持的 GLSL ES 代码。
4.  **函数模拟 (Emulation)**：对 GLES 中缺失或行为不一致的桌面级 OpenGL 函数进行模拟和“填充”(Polyfill)，例如处理双精度浮点数(`double`)参数、模拟代理纹理(Proxy Textures)等。

### 版本定位 (Version Positioning)

原版 LTW 主要面向 Minecraft 1.17 及更高版本。LTW-Turbo 在继承其能力的同时，重点补齐 1.17 及更早经典版本的渲染支持（如固定管线文字、旧版 GL 状态模拟等），让经典版本也能持续获得性能优化与问题修复。

---

## 核心目标 (Core Goals)

*   **性能优化**: 持续挖掘渲染性能，为低版本带来更流畅的体验。
*   **专注经典**: 核心服务于 Minecraft 1.17 及以下版本。
*   **问题修复**: 积极修复已知的 Bug，提升兼容性和稳定性。
*   **社区驱动**: 欢迎所有热爱经典版本的开发者加入我们，共同建设！

## 开发者构建 (For Developers)

以下是本项目的标准构建流程。请注意，在开发完成前，直接构建可能无法获得功能完整的版本。此部分主要面向希望参与开发的贡献者。

```bash
./gradlew :ltw:assembleRelease
```
构建完成后，你可以在 `ltw/build/outputs/aar/ltw-release.aar` 路径下找到生成的 AAR 文件。

构建 APK：

```bash
cd capacitor-android
./gradlew assembleDebug --parallel --build-cache --configuration-cache
```

APK 输出位于 `capacitor-android/app/build/outputs/apk/debug/app-debug.apk`。

## 贡献 (Contributing)

我们欢迎任何形式的贡献！无论是代码提交、问题报告还是功能建议，都对项目至关重要。

## 致谢 (Acknowledgements)

LTW-Turbo 的重生离不开原 LTW 项目的奠基者们。在此，我们向 artDev, SerpentSpirale, CADIndie 以及所有曾为 LTW 做出贡献的开发者们致以诚挚的感谢。

## 许可证 (License)

本项目基于 LGPL-3.0 协议开源，详情请见 `LICENSE` 文件
