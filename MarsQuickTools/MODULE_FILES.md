# MarsQuickTools 文件说明

本文档用于快速说明 `MarsQuickTools` 模块内各文件的职责，方便维护与二次开发。

## 根目录

- `CMakeLists.txt`：模块构建脚本，定义 QML 模块、C++ 源码、输出目录和 Qt 依赖。

## src（C++ 底层能力）

- `src/appcontroller.h`：`AppController` 声明，QML 单例入口（当前提供 `appName`）。
- `src/appcontroller.cpp`：`AppController` 实现。
- `src/framelesswindowagent.h`：`FramelessWindowAgent` 声明，封装无边框窗口行为代理接口。
- `src/framelesswindowagent.cpp`：`FramelessWindowAgent` 实现，包含 Windows 原生命中测试、拖拽/缩放、窗口控制。
- `src/themerevealoverlay.h`：`ThemeRevealOverlay` 声明，主题切换揭幕覆盖层接口。
- `src/themerevealoverlay.cpp`：`ThemeRevealOverlay` 实现，抓图、缓存、圆形扩散揭幕动画绘制。

## qml/components（可直接复用的组件）

- `qml/components/AppButton.qml`：基础主按钮组件。
- `qml/components/SystemTitleButton.qml`：标题栏系统按钮基类（最小化/最大化/关闭/主题切换复用）。
- `qml/components/WindowTitleBar.qml`：统一标题栏组件，处理拖拽区、系统按钮与主题切换事件。
- `qml/components/WindowResizeHandles.qml`：非 Windows 平台的窗口边缘缩放热区。
- `qml/components/MarsApplicationWindow.qml`：封装后的主窗口基类（标题栏 + 无边框代理 + 主题揭幕）。
- `qml/components/qmldir`：components 子目录的 QML 类型导出描述。

## qml/theme（主题系统）

- `qml/theme/Theme.qml`：主题单例，集中定义亮暗模式和设计令牌（颜色、圆角、间距、时长）。
- `qml/theme/qmldir`：theme 子目录的 QML 类型导出描述。

## 说明

- `bin_win64/`、`bin_linux_x64/`、`bin_linux_arm64/` 为构建产物目录，不属于源码。
- `CMakeLists.txt.user` 是 Qt Creator 本地配置文件，不建议纳入仓库版本管理。
