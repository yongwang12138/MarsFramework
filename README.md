# MarsFramework

MarsFramework 当前包含两个独立项目：

- `MarsQuickTools`：Qt6 的 QML/C++ 组件库（含无边框窗口代理、主题系统、标题栏组件等）
- `MarsQuickToolsExample`：示例应用（模拟第三方项目接入）

## 目录结构

```text
MarsFramework/
├─ MarsQuickTools/
│  ├─ CMakeLists.txt
│  ├─ src/
│  ├─ qml/
│  └─ shaders/
├─ MarsQuickToolsExample/
│  ├─ CMakeLists.txt
│  ├─ app/
│  ├─ qml/
│  └─ modules/                # 手动放置 MarsQuickTools 模块
├─ CODING_RULES.md
└─ AGENTS.md
```

## 环境要求

- Qt `6.8.3`
- CMake `3.21+`
- C++17
- Qt 组件：`Quick`、`Qml`

## 0. 顶层聚合工程（可选）

仓库根目录新增了顶层 `CMakeLists.txt`，用途是：

- 在一个 CMake 工程中同时打开 `MarsQuickTools` 与 `MarsQuickToolsExample`
- 构建时分别构建两个子工程
- 运行时默认目标为 `MarsQuickToolsExample`（可用 `run_example` 便捷目标）

示例命令：

```powershell
cmake -S . -B build-MarsFramework
cmake --build build-MarsFramework --config Debug --target MarsFrameworkAll
cmake --build build-MarsFramework --config Debug --target run_example
```

说明：两个子工程原有的独立构建方式保持不变，仍可按下文步骤单独构建。

## 1. 构建 MarsQuickTools（库）

```powershell
cmake -S MarsQuickTools -B build-MarsQuickTools
cmake --build build-MarsQuickTools --config Debug
```

输出目录（按平台）：

- Windows: `MarsQuickTools/bin_win64`
- Linux x64: `MarsQuickTools/bin_linux_x64`
- Linux arm64: `MarsQuickTools/bin_linux_arm64`

## 2. 将库拷贝给 Example（手动）

将 `MarsQuickTools/bin_xxx/MarsQuickTools/` 整个目录拷贝到：

- `MarsQuickToolsExample/modules/MarsQuickTools`

运行时至少需要以下关键文件：

- `qmldir`
- `MarsQuickTools.qmltypes`
- `MarsQuickToolsplugin.dll`（Linux 为 `.so`）
- `MarsQuickTools.dll`（Linux 为 `.so`）
- QML 资源与 shader 资源（位于模块目录中）

## 3. 构建 MarsQuickToolsExample（示例）

```powershell
cmake -S MarsQuickToolsExample -B build-MarsQuickToolsExample
cmake --build build-MarsQuickToolsExample --config Debug
```

输出目录（按平台）：

- Windows: `MarsQuickToolsExample/bin/marsquicktoolsexample-windows-amd64`
- Linux amd64: `MarsQuickToolsExample/bin/marsquicktoolsexample-linux-amd64`
- Linux arm64: `MarsQuickToolsExample/bin/marsquicktoolsexample-linux-arm64`

## 4. Example 如何识别 MarsQuickTools

`MarsQuickToolsExample/app/main.cpp` 会自动追加这些 QML import path：

- 编译期定义：`MarsQuickToolsExample/modules`
- 运行目录：`bin*/modules`
- 运行目录上一级：`bin*/../modules`
- 运行目录上两级：`bin*/../../modules`

只要模块目录结构正确，就可以在 QML 中直接：

```qml
import MarsQuickTools
```

## 5. 主题切换动画说明

`MarsApplicationWindow` 使用的是 C++ 覆盖层路径：

- 先 `grabToImage` 截取当前画面
- 切换主题
- 使用 C++ `ThemeRevealOverlay` 做圆形扩散揭幕

这是当前项目中稳定且跨屏缩放兼容性更好的实现路径。


