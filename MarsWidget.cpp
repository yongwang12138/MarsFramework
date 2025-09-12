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
    connect(_titleBar, &MarsTitleBar::minimizeClicked, this, &MarsWidget::showMinimized);
    connect(_titleBar, &MarsTitleBar::maximizeClicked, this, [this]() {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    });
    connect(_titleBar, &MarsTitleBar::closeClicked, this, &MarsWidget::close);

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
    updateWindowStyle();
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

#ifdef Q_OS_WIN
void MarsWidget::updateWindowStyle()
{
    HWND hwnd = reinterpret_cast<HWND>(winId());

    // 获取当前样式
    LONG style = GetWindowLong(hwnd, GWL_STYLE);

    if (!isMaximized()) {
        // 启用调整大小
        style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
    } else {
        // 禁用调整大小
        style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    }

    SetWindowLong(hwnd, GWL_STYLE, style);

    // 更新窗口
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

    // 启用阴影效果
    BOOL compositionEnabled = FALSE;
    HRESULT hr = DwmIsCompositionEnabled(&compositionEnabled);
    if (SUCCEEDED(hr) && compositionEnabled) {
        // 设置默认阴影
        const MARGINS shadow = { 1, 1, 1, 1 };
        DwmExtendFrameIntoClientArea(hwnd, &shadow);

        // 启用窗口阴影
        DWMNCRENDERINGPOLICY policy = DWMNCRP_ENABLED;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &policy, sizeof(policy));

        // 启用非客户区渲染
        BOOL enableNcRender = TRUE;
        DwmSetWindowAttribute(hwnd, DWMWA_ALLOW_NCPAINT, &enableNcRender, sizeof(enableNcRender));
    }
}
#endif
