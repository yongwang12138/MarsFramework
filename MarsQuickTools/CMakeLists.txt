cmake_minimum_required(VERSION 3.21)
project(MarsQuickTools VERSION 1.0.0 LANGUAGES CXX)

# ============================================================
# MarsQuickTools 构建说明
# 1) 本工程产物按“平台目录/模块名”输出，方便直接拷贝成 QML 模块目录
# 2) 通过 qt_add_qml_module 生成 QML 插件、qmldir、qmltypes
# 3) 同时编译 C++ 类型并导出给 QML（FramelessWindowAgent / ThemeRevealOverlay 等）
# ============================================================

# 项目名（模块名）统一变量
set(MARS_MODULE_NAME ${PROJECT_NAME})

# 设置 C++ 标准：强制使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 构建产物输出目录：
# Windows -> bin_win64/MarsQuickTools
# Linux x64 -> bin_linux_x64/MarsQuickTools
# Linux arm64 -> bin_linux_arm64/MarsQuickTools
if (WIN32)
    set(ROOT_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_win64")
elseif(UNIX AND NOT APPLE)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" SYSTEM_ARCH_LOWER)
    if (SYSTEM_ARCH_LOWER MATCHES "^(aarch64|arm64)")
        set(ROOT_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_linux_arm64")
    else()
        set(ROOT_BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_linux_x64")
    endif()
endif()

# QML 模块目录（plugin + qmldir + qmltypes 必须放这里）
set(QML_MODULE_DIR "${ROOT_BIN_DIR}/${MARS_MODULE_NAME}")
set(SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sdk")

# 核心库 dll 输出到 ROOT_BIN_DIR
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${ROOT_BIN_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${ROOT_BIN_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${ROOT_BIN_DIR}")

# 所有编译版本都输出到同一目录 避免 Debug/Release 分散
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${cfg}" cfg_upper)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${ROOT_BIN_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${ROOT_BIN_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} "${ROOT_BIN_DIR}")
endforeach()

# ===================== Qt 模块 =====================
find_package(Qt6 6.8.3 REQUIRED COMPONENTS Quick Qml)
qt_standard_project_setup(REQUIRES 6.8.3)
# Qt 官方推荐：含额外 QML 子目录时启用 QTP0004 NEW，避免策略警告。
qt_policy(SET QTP0004 NEW)

# 声明 Theme.qml 为单例
set_source_files_properties(qml/theme/Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

# QML 模块： URI 决定 import 名称（import MarsQuickTools）
qt_add_qml_module(${MARS_MODULE_NAME}
    URI ${MARS_MODULE_NAME}
    VERSION 1.0
    OUTPUT_DIRECTORY "${QML_MODULE_DIR}"  # 插件输出到子目录
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
    # RESOURCES
    #     qml/components/qmldir
    #     qml/theme/qmldir
)

# 对外暴露 C++ 头文件路径
target_include_directories(${MARS_MODULE_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 链接 Qt 运行时
target_link_libraries(${MARS_MODULE_NAME}
    PUBLIC
        Qt6::Quick
        Qt6::Qml
)

# ===================== 【自动移动插件和清理冗余文件】 =====================
add_custom_command(
    TARGET ${MARS_MODULE_NAME}plugin
    POST_BUILD
    # 1. 确保模块目录存在
    COMMAND ${CMAKE_COMMAND} -E make_directory "${QML_MODULE_DIR}"
    # 2. 移动插件到模块目录（外面不留）
    COMMAND ${CMAKE_COMMAND} -E rename
        "$<TARGET_FILE:${MARS_MODULE_NAME}plugin>"
        "${QML_MODULE_DIR}/$<TARGET_FILE_NAME:${MARS_MODULE_NAME}plugin>"
    COMMENT "=> 插件已移动到模块目录，外部无残留"
    # 3. 删除冗余qrc文件
    COMMAND ${CMAKE_COMMAND} -E remove -f
        "${QML_MODULE_DIR}/${MARS_MODULE_NAME}_qml_module_dir_map.qrc"
    # 4. 删除qml源码文件夹（隐藏源码）
    COMMAND ${CMAKE_COMMAND} -E remove_directory
        "${QML_MODULE_DIR}/qml"
    COMMENT "=> 已自动清理冗余文件，仅保留运行必需文件"
)

# ===================== ✅ 自动复制到 SDK（自动执行，无需操作） =====================
add_custom_command(
    TARGET ${MARS_MODULE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SDK_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SDK_DIR}/lib"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SDK_DIR}/include"

    # 复制运行库
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${ROOT_BIN_DIR}" "${SDK_DIR}/bin"

    # 复制链接库 .a
    COMMAND ${CMAKE_COMMAND} -E copy "${ROOT_BIN_DIR}/lib${MARS_MODULE_NAME}.a" "${SDK_DIR}/lib"

    # 复制头文件
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src" "${SDK_DIR}/include"

    COMMENT "✅ 自动打包完成：sdk 目录已生成"
)
