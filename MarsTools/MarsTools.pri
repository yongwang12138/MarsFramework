INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/MarsDef.h \
    $$PWD/MarsProperty.h \
    $$PWD/MarsSingleton.h \
    $$PWD/MarsText.h \
    $$PWD/MarsTheme.h \
    $$PWD/MarsThemeAnimationWidget.h \
    $$PWD/MarsTitleBar.h \
    $$PWD/MarsWidget.h \
    $$PWD/MarsWinShadowHelper.h

SOURCES += \
    $$PWD/MarsText.cpp \
    $$PWD/MarsTheme.cpp \
    $$PWD/MarsThemeAnimationWidget.cpp \
    $$PWD/MarsTitleBar.cpp \
    $$PWD/MarsWidget.cpp \
    $$PWD/MarsWinShadowHelper.cpp

LIBS += -ldwmapi
LIBS += -lgdi32
