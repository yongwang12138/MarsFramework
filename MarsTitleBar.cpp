#include "MarsTitleBar.h"

#include <QMouseEvent>
#include <QPainter>
#include <QStyle>

#ifdef Q_OS_WIN
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include "MarsWinShadowHelper.h"
#endif

MarsTitleBar::MarsTitleBar(QWidget* parent)
    : QWidget(parent)
{
    setFixedHeight(40);
    setupUi();
}

void MarsTitleBar::setupUi()
{
    _layout = new QHBoxLayout(this);
    _layout->setContentsMargins(5, 0, 0, 0);
    _layout->setSpacing(5);

    // 图标
    _iconLabel = new QLabel(this);
    _iconLabel->setFixedSize(30, 30);
    _iconLabel->setScaledContents(true);
    _iconLabel->setPixmap(style()->standardIcon(QStyle::SP_ComputerIcon).pixmap(30, 30));
    _layout->addWidget(_iconLabel);

    // 标题
    _titleLabel = new QLabel(this);
    _titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    _layout->addWidget(_titleLabel, 1); // 添加伸缩因子

    // 最小化按钮
    _minimizeButton = new QToolButton(this);
    _minimizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    _minimizeButton->setFixedSize(40, 40);
    connect(_minimizeButton, &QToolButton::clicked, this, &MarsTitleBar::onMinimizeClicked);
    _layout->addWidget(_minimizeButton);

    // 最大化/恢复按钮
    _maximizeButton = new QToolButton(this);
    updateMaximizeIcon();
    _maximizeButton->setFixedSize(40, 40);
    connect(_maximizeButton, &QToolButton::clicked, this, &MarsTitleBar::onMaximizeClicked);
    _layout->addWidget(_maximizeButton);

    // 关闭按钮
    _closeButton = new QToolButton(this);
    _closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    _closeButton->setFixedSize(40, 40);
    connect(_closeButton, &QToolButton::clicked, this, &MarsTitleBar::onCloseClicked);
    _layout->addWidget(_closeButton);

    // 上一次所在的屏幕
    _lastScreen = qApp->screenAt(window()->geometry().center());
}

HitArea MarsTitleBar::getHitArea(const QPoint &localPos) const
{
    // 检查是否在标题栏范围内
    if (!rect().contains(localPos)) {
        return HitArea::None;
    }

    // 检查是否在按钮上
    if (_minimizeButton && _minimizeButton->isVisible() &&
        _minimizeButton->geometry().contains(localPos)) {
        return HitArea::MinimizeButton;
    }

    if (_maximizeButton && _maximizeButton->isVisible() &&
        _maximizeButton->geometry().contains(localPos)) {
        return HitArea::MaximizeButton;
    }

    if (_closeButton && _closeButton->isVisible() &&
        _closeButton->geometry().contains(localPos)) {
        return HitArea::CloseButton;
    }

    // 在标题栏内但不在按钮上
    return HitArea::TitleBar;
}

void MarsTitleBar::setTitle(const QString& title)
{
    _titleLabel->setText(title);
}

void MarsTitleBar::setIcon(const QIcon& icon)
{
    _iconLabel->setPixmap(icon.pixmap(30, 30));
    _iconLabel->setVisible(!icon.isNull());
}

void MarsTitleBar::setMinimizeButtonVisible(bool visible)
{
    _minimizeButton->setVisible(visible);
}

void MarsTitleBar::setMaximizeButtonVisible(bool visible)
{
    _maximizeButton->setVisible(visible);
}

void MarsTitleBar::setCloseButtonVisible(bool visible)
{
    _closeButton->setVisible(visible);
}

QToolButton* MarsTitleBar::minimizeButton() const
{
    return _minimizeButton;
}

QToolButton* MarsTitleBar::maximizeButton() const
{
    return _maximizeButton;
}

QToolButton* MarsTitleBar::closeButton() const
{
    return _closeButton;
}

void MarsTitleBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), QColor(232, 232, 232));
}

void MarsTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        onMaximizeClicked();
    }
    QWidget::mouseDoubleClickEvent(event);
}

void MarsTitleBar::onMinimizeClicked()
{
    window()->showMinimized();
}

void MarsTitleBar::onMaximizeClicked()
{
    if (window()->isMaximized()) {
        window()->showNormal();
    } else {
        window()->showMaximized();
    }
}

void MarsTitleBar::onCloseClicked()
{
    window()->close();
}

void MarsTitleBar::updateMaximizeIcon()
{
    if (parentWidget() && parentWidget()->isMaximized()) {
        _maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
    } else {
        _maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    }
}

bool MarsTitleBar::takeOverNativeEvent(MSG *msg, qintptr *result)
{
    if (!msg || !msg->hwnd) {
        return false;
    }

    const HWND hwnd = msg->hwnd;
    const UINT message = msg->message;
    const WPARAM wParam = msg->wParam;
    const LPARAM lParam = msg->lParam;

    switch (message) {
    case WM_WINDOWPOSCHANGING:
    {
        // 当窗口大小即将改变时 通过设置SWP_NOCOPYBITS标志优化窗口重绘行为 避免可能的视觉闪烁
        WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
        if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
        {
            wp->flags |= SWP_NOCOPYBITS;
            *result = ::DefWindowProcW(hwnd, message, wParam, lParam);
            return true;
        }
        return false;
    }
    case WM_NCPAINT:
    {
        if (!mWinHelper.getIsCompositionEnabled()) {
            *result = FALSE;
            return true;
        } else {
            return false;
        }
    }
    case WM_NCACTIVATE: {
        // 处理激活窗口时的阴影效果
        if (mWinHelper.getIsCompositionEnabled()) {
            // 调用默认处理，但阻止系统绘制非客户区
            *result = ::DefWindowProcW(hwnd, WM_NCACTIVATE, wParam, -1);

            // 更新窗口阴影效果
            bool isActive = wParam != FALSE;
            const MARGINS shadow = isActive ? MARGINS{10, 10, 10, 10} : MARGINS{5, 5, 5, 5};
            DwmExtendFrameIntoClientArea(hwnd, &shadow);
        } else {
            // 若未启用桌面组合：调用系统默认处理函数
            *result = DefWindowProcW(hwnd, message, wParam, lParam);
        }
        return true;
    }
    case WM_SIZE:
    {
        if (wParam == SIZE_RESTORED)
        {
            _maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        }
        else if (wParam == SIZE_MAXIMIZED)
        {
            _maximizeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        }
        return false;
    }
    /* 处理计算窗口客户区的大小和位置 */
    case WM_NCCALCSIZE: {
        if (wParam == TRUE) { // 表示需要计算客户区
            // 移除默认窗口边框
            *result = 0; // 告诉 Windows 系统：客户区就是整个窗口区域，不需要预留非客户区（边框、标题栏等）
            return true;
        } else {
            return false;
        }
    }
    case WM_MOVE:
    {
        QScreen* currentScreen = qApp->screenAt(window()->geometry().center());
        if (currentScreen && currentScreen != _lastScreen)
        {
            if (_lastScreen)
            {
                ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
                ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
            }
            _lastScreen = currentScreen;
        }
        return false;
    }
    /* 判断鼠标在窗口上的位置 返回相应的 "命中测试结果" */
    case WM_NCHITTEST: {
        /// 返回 HTCAPTION（标题栏），系统会允许拖动窗口
        /// 返回 HTCLIENT（客户区），则由程序自己处理（如鼠标点击事件）
        /// 返回 HTLEFT（左边界），系统会允许拖动调整窗口宽度

        // 从 lParam 中提取鼠标全局坐标（屏幕坐标）
        const POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

        // 获取窗口在屏幕上的矩形区域（全局坐标）
        RECT rect;
        GetWindowRect(hwnd, &rect);

        // 将鼠标全局坐标转换为窗口局部坐标（相对于窗口左上角）
        const int x = pt.x - rect.left;
        const int y = pt.y - rect.top;

        // 检查窗口是否最大化
        bool isMaximized = parentWidget() && parentWidget()->isMaximized();
        /// 实现 “鼠标放在窗口边缘时，可拖动调整窗口大小”，模拟系统默认窗口的边框拖动功能。
        if (!isMaximized) {
            const int borderWidth = 12;
            bool left = x < borderWidth;
            bool right = x > (rect.right - rect.left - borderWidth);
            bool top = y < borderWidth;
            bool bottom = y > (rect.bottom - rect.top - borderWidth);

            if (left && top) {
                *result = HTTOPLEFT;
                return true;
            } else if (left && bottom) {
                *result = HTBOTTOMLEFT;
                return true;
            } else if (right && top) {
                *result = HTTOPRIGHT;
                return true;
            } else if (right && bottom) {
                *result = HTBOTTOMRIGHT;
                return true;
            } else if (left) {
                *result = HTLEFT;
                return true;
            } else if (right) {
                *result = HTRIGHT;
                return true;
            } else if (top) {
                *result = HTTOP;
                return true;
            } else if (bottom) {
                *result = HTBOTTOM;
                return true;
            }
        }

        // 获取 DPI 缩放比例
        qreal dpiScale = devicePixelRatioF();
        // 将物理坐标转换为逻辑坐标
        int logicalY = static_cast<int>(y / dpiScale);

        /// 检查是否在标题栏区域
        if (logicalY  < height() && x > 0 && x < (rect.right - rect.left)) {
            QPoint localPos(static_cast<int>(x / dpiScale), logicalY);

            // 使用 hitTest 函数进行命中测试
            HitArea hitResult = getHitArea(localPos);

            switch (hitResult) {
            case HitArea::MaximizeButton:
                // 如果在最大化按钮上，返回 HTMAXBUTTON 以启用 Aero Snap
                // *result = HTMAXBUTTON; // 或 HTZOOM，两者值相同
                *result = HTCLIENT;  // 返回 HTCLIENT 让 Qt 处理按钮点击
                return true;
            case HitArea::MinimizeButton:
            case HitArea::CloseButton:
                *result = HTCLIENT;
                return true;
            case HitArea::TitleBar:
                /// 在最大化状态下 也允许整个标题栏区域用于拖动
                *result = HTCAPTION;  // 不是按钮：按标题栏处理（允许拖动窗口）
                return true;
            default:
                break;
            }
        }

        *result = HTCLIENT; // 所有区域都按客户区处理（不允许调整边框）
        return true;
    }
    /* 获取窗口的"最小/最大信息" */
    case WM_GETMINMAXINFO: {
        MINMAXINFO *minMaxInfo = reinterpret_cast<MINMAXINFO *>(lParam);

        // 获取工作区大小
        RECT workArea;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

        minMaxInfo->ptMaxPosition.x = workArea.left;
        minMaxInfo->ptMaxPosition.y = workArea.top;
        minMaxInfo->ptMaxSize.x = workArea.right - workArea.left;
        minMaxInfo->ptMaxSize.y = workArea.bottom - workArea.top;
        minMaxInfo->ptMaxTrackSize.x = workArea.right - workArea.left;
        minMaxInfo->ptMaxTrackSize.y = workArea.bottom - workArea.top;

        // 设置最小窗口尺寸
        if (parentWidget()) {
            QSize minSize = parentWidget()->minimumSize();
            if (minSize.isValid()) {
                // 转换为屏幕坐标（需要考虑 DPI 缩放）
                qreal dpiScale = devicePixelRatioF();
                minMaxInfo->ptMinTrackSize.x = static_cast<LONG>(minSize.width() * dpiScale);
                minMaxInfo->ptMinTrackSize.y = static_cast<LONG>(minSize.height() * dpiScale);
            }
        }
        *result = 0;
        return true;
    }

    }
    return false;
}

void MarsTitleBar::initWindowStyle(HWND hwnd)
{
    // 获取当前样式
    LONG style = GetWindowLong(hwnd, GWL_STYLE);

    // 移除默认标题栏但保留边框
    style |= WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

    SetWindowLong(hwnd, GWL_STYLE, style);

    // 更新窗口
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
}
