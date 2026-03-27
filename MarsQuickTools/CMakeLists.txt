cmake_minimum_required(VERSION 3.21)
project(MarsQuickTools VERSION 0.1.0 LANGUAGES CXX)

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

set_source_files_properties(qml/theme/Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

set(MARS_QML_MODULE_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MarsQuickTools")

qt_add_qml_module(MarsQuickTools
    URI MarsQuickTools
    VERSION 1.0
    OUTPUT_DIRECTORY "${MARS_QML_MODULE_OUTPUT_DIR}"
    SOURCES
        src/appcontroller.cpp
        src/appcontroller.h
        src/framelesswindowagent.cpp
        src/framelesswindowagent.h
    QML_FILES
        qml/components/AppButton.qml
        qml/components/SystemTitleButton.qml
        qml/components/WindowTitleBar.qml
        qml/components/WindowResizeHandles.qml
        qml/components/MarsApplicationWindow.qml
        qml/theme/Theme.qml
)

target_include_directories(MarsQuickTools
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(MarsQuickTools
    PUBLIC
        Qt6::Quick
        Qt6::Qml
)

install(TARGETS MarsQuickTools MarsQuickToolsplugin
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
