#include "appcontroller.h"

AppController::AppController(QObject* parent)
    : QObject(parent)
{
}

QString AppController::appName() const
{
    return QStringLiteral("MarsQuickTools");
}
