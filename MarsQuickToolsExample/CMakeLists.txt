cmake_minimum_required(VERSION 3.21)
project(MarsQuickToolsExample VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (WIN32)
    set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_win64")
elseif(UNIX AND NOT APPLE)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" SYSTEM_ARCH_LOWER)
    if (SYSTEM_ARCH_LOWER MATCHES "^(aarch64|arm64)")
        set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_linux_arm64")
    else()
        set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin_linux_x64")
    endif()
endif()

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

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Qml)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(MarsQuickToolsExample
    app/main.cpp
)

qt_add_qml_module(MarsQuickToolsExample
    URI MarsQuickToolsExample
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/pages/HomePage.qml
)

target_compile_definitions(MarsQuickToolsExample
    PRIVATE
        MARS_QUICKTOOLS_MODULE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/modules"
)

target_link_libraries(MarsQuickToolsExample
    PRIVATE
        Qt6::Quick
        Qt6::Qml
)

qt_import_qml_plugins(MarsQuickToolsExample)
qt_finalize_executable(MarsQuickToolsExample)
