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

# ===================== 输出到 plugins =====================
set(PLUGINS_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/plugins")
set(PLUGINS_BIN      "${PLUGINS_DIR}/bin")
set(PLUGINS_LIB      "${PLUGINS_DIR}/lib")
set(PLUGINS_INCLUDE  "${PLUGINS_DIR}/include")
set(MODULE_DIR      "${PLUGINS_BIN}/${MARS_MODULE_NAME}")

# 核心库 dll 输出到 SDK/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PLUGINS_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PLUGINS_BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PLUGINS_LIB})

# 所有编译版本都输出到同一目录 避免 Debug/Release 分散
foreach(cfg IN ITEMS Debug Release RelWithDebInfo MinSizeRel)
    string(TOUPPER "${cfg}" cfg_upper)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${PLUGINS_BIN}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${PLUGINS_BIN}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} "${PLUGINS_LIB}")
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
    OUTPUT_DIRECTORY ${MODULE_DIR}  # 插件输出到子目录
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
        resources/fonts.qrc
        resources/iconfont.ttf
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
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MODULE_DIR}"
    # 2. 移动插件到模块目录（外面不留）
    COMMAND ${CMAKE_COMMAND} -E rename
        "$<TARGET_FILE:${MARS_MODULE_NAME}plugin>"
        "${MODULE_DIR}/$<TARGET_FILE_NAME:${MARS_MODULE_NAME}plugin>"
    COMMENT "=> 插件已移动到模块目录，外部无残留"
    # 3. 删除冗余qrc文件
    COMMAND ${CMAKE_COMMAND} -E remove -f
        "${MODULE_DIR}/${MARS_MODULE_NAME}_qml_module_dir_map.qrc"
    # 4. 删除qml源码文件夹（隐藏源码）
    COMMAND ${CMAKE_COMMAND} -E remove_directory
        "${MODULE_DIR}/qml"
    COMMENT "=> 已自动清理冗余文件，仅保留运行必需文件"
)

# ===================== 自动复制头文件 =====================
add_custom_command(
    TARGET ${MARS_MODULE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src" ${PLUGINS_INCLUDE}
    COMMENT "=> 发布完成：${PLUGINS_DIR}"
)
