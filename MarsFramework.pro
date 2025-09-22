QT += core gui widgets

CONFIG += c++17

HEADERS += \
    MarsDef.h \
    MarsProperty.h \
    MarsSingleton.h \
    MarsText.h \
    MarsTheme.h \
    MarsThemeAnimationWidget.h \
    MarsTitleBar.h \
    MarsWidget.h \
    MarsWinShadowHelper.h \
    MarsWindow.h \
    widget.h

SOURCES += \
    MarsText.cpp \
    MarsTheme.cpp \
    MarsThemeAnimationWidget.cpp \
    MarsTitleBar.cpp \
    MarsWidget.cpp \
    MarsWinShadowHelper.cpp \
    MarsWindow.cpp \
    main.cpp  \
    widget.cpp

LIBS += -ldwmapi
LIBS += -lgdi32

FORMS += \
    widget.ui
