# MarsFramework Workspace

本仓库包含两个完全独立的项目：

- `MarsQuickTools`：QML/C++ 框架库项目
- `MarsQuickToolsExample`：示例应用（模拟第三方使用方）

## 1. 构建 MarsQuickTools（库项目）

在 `MarsQuickTools` 目录单独配置和构建：

```powershell
cmake -S MarsQuickTools -B MarsQuickTools/build
cmake --build MarsQuickTools/build
```

输出目录在库项目自身目录下：

- Windows: `MarsQuickTools/bin_win64`
- Linux x64: `MarsQuickTools/bin_linux_x64`
- Linux arm64: `MarsQuickTools/bin_linux_arm64`

### 生成发布包（仅 MarsQuickTools）

```powershell
cmake --build MarsQuickTools/build --target marsquicktools_package
```

发布目录：

- Windows: `MarsQuickTools/package_win64/MarsQuickTools`
- Linux x64: `MarsQuickTools/package_linux_x64/MarsQuickTools`
- Linux arm64: `MarsQuickTools/package_linux_arm64/MarsQuickTools`

## 2. 将库手动拷贝到 Example

把上一步生成的 `MarsQuickTools` 文件夹整体拷贝到：

- `MarsQuickToolsExample/modules/MarsQuickTools`

最终结构示例：

```text
MarsQuickToolsExample/
└─ modules/
   └─ MarsQuickTools/
      ├─ qmldir
      ├─ MarsQuickTools.qmltypes
      ├─ MarsQuickToolsplugin.*
      └─ MarsQuickTools.*
```

## 3. 构建 MarsQuickToolsExample（示例项目）

在 `MarsQuickToolsExample` 目录单独配置和构建：

```powershell
cmake -S MarsQuickToolsExample -B MarsQuickToolsExample/build
cmake --build MarsQuickToolsExample/build
```

输出目录在示例项目自身目录下：

- Windows: `MarsQuickToolsExample/bin_win64`
- Linux x64: `MarsQuickToolsExample/bin_linux_x64`
- Linux arm64: `MarsQuickToolsExample/bin_linux_arm64`

示例程序会自动尝试加载这些导入路径：

- `MarsQuickToolsExample/modules`
- `bin*/modules`
- `bin*/../modules`
- `bin*/../../modules`
