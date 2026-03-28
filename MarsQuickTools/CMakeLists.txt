cmake_minimum_required(VERSION 3.21)
project(MarsQuickTools VERSION 1.0.0 LANGUAGES CXX)

# ============================================================
# MarsQuickTools 构建说明
# 1) 本工程产物按“平台目录/模块名”输出，方便直接拷贝成 QML 模块目录
# 2) 通过 qt_add_qml_module 生成 QML 插件、qmldir、qmltypes
# 3) 同时编译 C++ 类型并导出给 QML（FramelessWindowAgent / ThemeRevealOverlay 等）
# ============================================================

# 项目名（模块名）统一变量，后续改名只改 project 即可。
set(MARS_MODULE_NAME ${PROJECT_NAME})

# 设置 C++ 标准：强制使用 C++17。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 构建产物输出目录：
# Windows -> bin_win64/MarsQuickTools
# Linux x64 -> bin_linux_x64/MarsQuickTools
# Linux arm64 -> bin_linux_arm64/MarsQuickTools
if (WIN32)
    set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_win64/${MARS_MODULE_NAME}")
elseif(UNIX AND NOT APPLE)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" SYSTEM_ARCH_LOWER)
    if (SYSTEM_ARCH_LOWER MATCHES "^(aarch64|arm64)")
        set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_linux_arm64/${MARS_MODULE_NAME}")
    else()
        set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_linux_x64/${MARS_MODULE_NAME}")
    endif()
endif()

# 将所有类型产物（exe/dll/a）统一放到 OUTPUT_DIR，避免 Debug/Release 分散难找。
if (DEFINED OUTPUT_DIR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

    foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
        string(TOUPPER "${cfg}" cfg_upper)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${OUTPUT_DIR}")
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${OUTPUT_DIR}")
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} "${OUTPUT_DIR}")
    endforeach()
endif()

# Qt 依赖：Quick + Qml。
find_package(Qt6 6.8.3 REQUIRED COMPONENTS Quick Qml)
qt_standard_project_setup(REQUIRES 6.8.3)

# Qt 官方推荐：含额外 QML 子目录时启用 QTP0004 NEW，避免策略警告。
qt_policy(SET QTP0004 NEW)

# 声明 Theme.qml 为单例（import 后可直接 Theme.xxx）。
set_source_files_properties(qml/theme/Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

# 定义 QML 模块：
# - URI 决定 import 名称（import MarsQuickTools）
# - OUTPUT_DIRECTORY 决定模块文件最终落盘目录
qt_add_qml_module(${MARS_MODULE_NAME}
    URI ${MARS_MODULE_NAME}
    VERSION 1.0
    OUTPUT_DIRECTORY "${OUTPUT_DIR}"
    SOURCES
        src/appcontroller.cpp
        src/appcontroller.h
        src/framelesswindowagent.cpp
        src/framelesswindowagent.h
        src/themerevealoverlay.cpp
        src/themerevealoverlay.h
    QML_FILES
        qml/components/AppButton.qml
        qml/components/SystemTitleButton.qml
        qml/components/WindowTitleBar.qml
        qml/components/WindowResizeHandles.qml
        qml/components/MarsApplicationWindow.qml
        qml/theme/Theme.qml
    RESOURCES
        qml/components/qmldir
        qml/theme/qmldir
)

# 对外暴露 C++ 头文件路径（供同工程其他 target include）。
target_include_directories(${MARS_MODULE_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 链接 Qt 运行时。
target_link_libraries(${MARS_MODULE_NAME}
    PUBLIC
        Qt6::Quick
        Qt6::Qml
)
