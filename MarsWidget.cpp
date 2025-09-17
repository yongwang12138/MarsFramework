#include "MarsWidget.h"

#include <QApplication>
#include <QShowEvent>
#include <QEvent>
#include <QTimer>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#endif

MarsWidget::MarsWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    // setAttribute(Qt::WA_TranslucentBackground);

    setupUi();
}

void MarsWidget::setupUi()
{
    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);

    // 标题栏
    _titleBar = new MarsTitleBar(this);
    _layout->addWidget(_titleBar);

    // 内容区域
    _contentWidget = new QWidget(this);
    _contentWidget->setObjectName("contentWidget");
    _contentWidget->setStyleSheet("QWidget#contentWidget { background: white; border: none; }");
    _layout->addWidget(_contentWidget, 1);
}

MarsTitleBar* MarsWidget::titleBar() const
{
    return _titleBar;
}

void MarsWidget::setContentWidget(QWidget* widget)
{
    if (_contentWidget) {
        _layout->removeWidget(_contentWidget);
        _contentWidget->deleteLater();
    }

    _contentWidget = widget;
    if (_contentWidget) {
        _layout->addWidget(_contentWidget, 1);
    }
}

QWidget* MarsWidget::contentWidget() const
{
    return _contentWidget;
}

bool MarsWidget::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#ifdef Q_OS_WIN
    /// "windows_generic_MSG": 直接来自 Windows系统的原生消息  "windows_dispatcher_MSG": Qt消息调度器的消息
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
        MSG *msg = static_cast<MSG *>(message);
        return _titleBar->takeOverNativeEvent(msg, result);
    }
#endif
    return QWidget::nativeEvent(eventType, message, result);
}

void MarsWidget::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);
#ifdef Q_OS_WIN
    // 调用标题栏的 initWindowStyle 方法
    _titleBar->initWindowStyle(reinterpret_cast<HWND>(winId()));
#endif
}

void MarsWidget::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange) {
        // 更新最大化按钮图标
        QTimer::singleShot(0,this,  [this]() {
            if (_titleBar) {
                _titleBar->updateMaximizeIcon();
            }
        });
    }
    QWidget::changeEvent(event);
}
