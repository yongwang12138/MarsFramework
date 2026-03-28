#pragma once

#include <QObject>
#include <qqmlintegration.h>

// AppController：模块级全局配置入口。
// 目前对外提供 appName，后续可以继续扩展为版本号、作者信息、全局配置等。
class AppController : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(AppController)
    QML_SINGLETON
    Q_PROPERTY(QString appName READ appName CONSTANT)

public:
    explicit AppController(QObject* parent = nullptr);

    // 返回应用展示名（供窗口标题、关于页等位置复用）。
    QString appName() const;
};
