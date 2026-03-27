#pragma once

#include <QObject>
#include <qqmlintegration.h>

class AppController : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AppController)
    QML_SINGLETON
    Q_PROPERTY(QString appName READ appName CONSTANT)

public:
    explicit AppController(QObject* parent = nullptr);

    QString appName() const;
};
