#ifndef MARSAPPLICATION_H
#define MARSAPPLICATION_H

#include <QObject>
#include "MarsDef.h"
#include "MarsProperty.h"
#include "MarsSingleton.h"

#define mApp MarsApplication::instance()
class MarsApplication : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(MarsApplication)
    Q_PROPERTY_CREATE_H(MarsApplicationType::WindowDisplayMode, WindowDisplayMode)
public:
    explicit MarsApplication(QObject* parent = nullptr);
    ~MarsApplication() = default;

    void syncWindowDisplayMode(QWidget* widget, bool isSync = true);

private:
    QList<QWidget*> _micaWidgetList;
    MarsThemeType::ThemeMode _themeMode;
};

#endif // MARSAPPLICATION_H
