QT += core gui widgets

CONFIG += c++17

HEADERS += \
    MarsTitleBar.h \
    MarsWidget.h \
    MarsWinShadowHelper.h

SOURCES += \
    MarsTitleBar.cpp \
    MarsWidget.cpp \
    MarsWinShadowHelper.cpp \
    main.cpp 

LIBS += -ldwmapi
