QT += core gui widgets

CONFIG += c++17

# 条件：如果定义了MARSFRAMEWORK_LIB，则编译为静态库；否则编译为可执行文件
contains(DEFINES, MARSFRAMEWORK_LIB) {
    TEMPLATE = lib
    CONFIG += staticlib  # 静态库（或 dynamiclib 动态库）
} else {
    TEMPLATE = app
}

# 库文件输出目录
DESTDIR = $$PWD/../bin_win

include(MarsTools/MarsTools.pri)

HEADERS += \
    MarsWindow.h \
    setting_window.h

SOURCES += \
    MarsWindow.cpp \
    setting_window.cpp

# 屏蔽main函数（仅在作为库时生效）
contains(DEFINES, MARSFRAMEWORK_LIB) {
    # 不包含main.cpp
} else {
    # 作为独立程序时才包含main.cpp
    SOURCES += main.cpp
}
