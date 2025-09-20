QT += core gui widgets

CONFIG += c++17

HEADERS += \
    MarsDef.h \
    MarsProperty.h \
    MarsSingleton.h \
    MarsTheme.h \
    MarsThemeAnimationWidget.h \
    MarsTitleBar.h \
    MarsWidget.h \
    MarsWinShadowHelper.h \
    widget.h

SOURCES += \
    MarsTheme.cpp \
    MarsThemeAnimationWidget.cpp \
    MarsTitleBar.cpp \
    MarsWidget.cpp \
    MarsWinShadowHelper.cpp \
    main.cpp  \
    widget.cpp

LIBS += -ldwmapi
LIBS += -lgdi32

FORMS += \
    widget.ui
