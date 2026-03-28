# C++/QML 代码规范（MarsFramework）

本规范用于约束 `MarsQuickTools` 与 `MarsQuickToolsExample` 的开发风格，目标是：

- 现代 C++
- Qt6 标准写法
- 高可读、可维护、可扩展

后续规则可以增量追加，新代码必须遵守，旧代码在触达时逐步整理。

## 1. 命名规范

- 类名：`PascalCase`，如 `FramelessWindowAgent`
- 函数名：`camelCase`，如 `setTitleBarHeight()`
- 局部变量：`camelCase`
- 成员变量：`snake_case_`，如 `title_bar_height_`
- 常量：`kPascalCase`
- QML 文件名：`PascalCase.qml`，如 `MarsApplicationWindow.qml`
- QML id：`camelCase`

## 2. 指针与引用

- 指针符号跟类型：`Type* ptr`
- 优先引用，其次智能/受管指针，再考虑裸指针
- Qt 对象优先父子关系管理生命周期
- 跨对象悬垂风险场景优先 `QPointer<T>`

## 3. 初始化与构造

- 成员初始化统一 `{}`
- 优先在声明处给默认值
- 单参数构造默认 `explicit`
- 禁止未初始化成员

## 4. 现代 C++ 实践

- 使用 `nullptr`，禁用 `NULL`/`0` 空指针语义
- 能 `const` 就 `const`
- 覆写函数必须 `override`
- 适度使用 `auto`（不牺牲可读性）
- 优先早返回，减少嵌套

## 5. Qt/QML 约定

- 以 Qt6 API 为准，不引入 Qt5 兼容模块实现核心能力
- 平台分支最小化，使用 `Q_OS_WIN` / `Q_OS_LINUX` 等宏
- QML 对外接口保持窄接口：仅暴露必要 `Q_PROPERTY` / `Q_INVOKABLE`
- 主题和关键转场优先使用 C++ 可控实现（当前为 `ThemeRevealOverlay`），保证跨屏缩放一致性
- 避免在高频路径使用大开销 JavaScript 逻辑

## 6. CMake 约定

- 最低版本 `3.21`
- 统一 `C++17`
- 依赖通过 `find_package(Qt6 ... REQUIRED)` 显式声明
- QML 模块统一使用 `qt_add_qml_module`
- Shader 资源统一使用 `qt_add_shaders`

## 7. 注释与文档

- 对复杂逻辑写“为什么”的注释，不写“做了什么”的废话注释
- 公开接口（类、关键函数、QML 组件）应有简短中文说明
- 文档（README/规则）与代码改动同步更新

## 8. 提交前自检

- 能编译通过（至少 Debug）
- 无新增关键警告
- 变更涉及的 QML 页面可正常加载
- 规则文档与行为保持一致

---

如与 Qt 官方推荐实践冲突，以“Qt 官方 + 团队一致性 + 可维护性”三者平衡为准。

