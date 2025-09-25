#include "MarsApplication.h"

#include "MarsWinShadowHelper.h"

#include <QWidget>

Q_SINGLETON_CREATE_CPP(MarsApplication)
MarsApplication::MarsApplication(QObject* parent)
    : QObject{parent}
{}

void MarsApplication::syncWindowDisplayMode(QWidget* widget, bool isSync)
{
    if (!widget)
    {
        return;
    }
    if (isSync)
    {
        _micaWidgetList.append(widget);
    }
    else
    {
        _micaWidgetList.removeOne(widget);
    }
    switch (_pWindowDisplayMode)
    {
    case MarsApplicationType::Normal:
    default:
    {
#ifdef Q_OS_WIN
        if (isSync)
        {
            mWinHelper.setWindowDisplayMode(widget, _pWindowDisplayMode, MarsApplicationType::Normal);
            mWinHelper.setWindowThemeMode(widget->winId(), _themeMode == MarsThemeType::Light);
        }
        else
        {
            mWinHelper.setWindowDisplayMode(widget, MarsApplicationType::Normal, _pWindowDisplayMode);
            mWinHelper.setWindowThemeMode(widget->winId(), true);
        }
#endif
        break;
    }
    }
}

void MarsApplication::setWindowDisplayMode(MarsApplicationType::WindowDisplayMode windowDisplayType)
{
    auto lastDisplayMode = _pWindowDisplayMode;
    if (lastDisplayMode == windowDisplayType)
    {
        return;
    }
#ifdef Q_OS_WIN
    for (int i = 0; i < _micaWidgetList.size(); ++i) {
        mWinHelper.setWindowDisplayMode(_micaWidgetList.at(i), windowDisplayType, lastDisplayMode);
        mWinHelper.setWindowThemeMode(_micaWidgetList.at(i)->winId(), _themeMode == MarsThemeType::Light);
    }
#endif
    _pWindowDisplayMode = windowDisplayType;
    emit pWindowDisplayModeChanged();
}

MarsApplicationType::WindowDisplayMode MarsApplication::getWindowDisplayMode() const
{
    return _pWindowDisplayMode;
}
