cmake_minimum_required(VERSION 3.21)
project(MarsFramework LANGUAGES CXX)

# 顶层聚合工程：
# 1) 仅用于在一个工程里同时打开两个子项目
# 2) 不修改子项目原有 CMake 逻辑，子项目仍可独立构建
# 3) 构建时分别触发两个子项目

add_subdirectory(MarsQuickTools)
add_subdirectory(MarsQuickToolsExample)

# 便捷目标：统一构建两个子工程
add_custom_target(MarsFrameworkAll
    DEPENDS
        MarsQuickTools
        MarsQuickToolsExample
)

# 便捷目标：运行示例程序
add_custom_target(run_example
    COMMAND $<TARGET_FILE:MarsQuickToolsExample>
    DEPENDS MarsQuickToolsExample
    USES_TERMINAL
)
