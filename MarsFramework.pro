QT += core gui widgets

CONFIG += c++17

HEADERS += \
    MarsTitleBar.h \
    MarsWidget.h

SOURCES += \
    MarsTitleBar.cpp \
    MarsWidget.cpp \
    main.cpp 

LIBS += -ldwmapi
