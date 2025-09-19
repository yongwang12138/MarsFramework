QT += core gui widgets

CONFIG += c++17

HEADERS += \
    MarsDef.h \
    MarsProperty.h \
    MarsSingleton.h \
    MarsTheme.h \
    MarsTitleBar.h \
    MarsWidget.h \
    MarsWinShadowHelper.h

SOURCES += \
    MarsTheme.cpp \
    MarsTitleBar.cpp \
    MarsWidget.cpp \
    MarsWinShadowHelper.cpp \
    main.cpp 

LIBS += -ldwmapi
LIBS += -lgdi32
