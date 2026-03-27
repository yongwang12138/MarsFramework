# C++ 代码规范（ElaQuickTools）

本规范用于约束本项目的 C++/Qt 代码风格，目标是现代 C++、可维护、可扩展。
后续可持续追加新规则；新规则默认对新代码生效，重构时逐步落地到旧代码。

## 1. 命名规范

- 类名：`PascalCase`，如 `FramelessWindowAgent`。
- 函数名：`camelCase`，如 `setTitleBarHeight()`。
- 局部变量：`camelCase`，如 `targetHwnd`。
- 成员变量：`小写 + 下划线后缀`，如 `title_bar_height_`、`window_`。
- 常量：`kPascalCase`，如 `kDefaultBorderThickness`。
- 宏：仅在必须时使用，全大写下划线。

## 2. 指针与引用

- 指针/引用符号靠类型，不靠变量名：
  - `QObject* obj`（正确）
  - `QObject *obj`（不使用）
- 能用引用就不用裸指针。
- Qt 对象优先使用父子对象管理生命周期；跨对象引用优先 `QPointer<T>`。

## 3. 初始化与构造

- 成员变量初始化统一使用 `{}`。
- 优先在声明处给默认值，在构造函数中只处理必要逻辑。
- 单参数构造函数默认 `explicit`。
- 禁止未初始化成员。

## 4. 现代 C++ 实践

- 优先使用 `nullptr`，禁止 `NULL`/`0` 代表空指针。
- 能 `const` 就 `const`；只读接口优先 `const` 成员函数。
- 能 `override` 必须 `override`。
- 使用范围 `for`、`auto`（仅在可读性不下降时）。
- 优先早返回，减少嵌套层级。

## 5. Qt/跨平台约定

- 平台分支使用 `#ifdef Q_OS_WIN` 等宏，分支最小化。
- Windows 原生消息处理仅放在平台实现层，不污染通用业务层。
- QML 暴露对象应保持窄接口：只暴露必要 `Q_PROPERTY`/`Q_INVOKABLE`。

## 6. 错误处理与日志

- 对外部输入做参数校验，失败时快速返回。
- 调试日志使用 `qWarning()`/`qDebug()`，避免无意义日志噪声。
- 不吞异常状态；可恢复错误需显式处理或注释说明。

## 7. 提交要求（代码层面）

- 新增/修改代码必须符合本规范。
- 旧代码不强制一次性重写；触达时顺手修正。
- 评审重点：命名、初始化、生命周期、跨平台边界。

## 8. 渐进升级计划（可按阶段启用）

- 阶段 A（当前）
  - 命名统一
  - 初始化统一 `{}`
  - 指针样式统一 `Type* var`
- 阶段 B
  - 对关键类补拷贝/移动语义约束（`Q_DISABLE_COPY_MOVE` 或显式 `=delete`）
  - 补充 clang-format 配置并自动化
- 阶段 C
  - 补充 clang-tidy 规则（modernize/readability/performance）
  - 引入最小单测模板

---

如果与 Qt 官方 API 风格冲突，以“可读性 + 团队一致性 + Qt 生态兼容”三者平衡为准。
