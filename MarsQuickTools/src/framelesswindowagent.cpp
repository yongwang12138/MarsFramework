#include "framelesswindowagent.h"

#include <QCoreApplication>
#include <QTimer>
#include <QtMath>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#endif

FramelessWindowAgent::FramelessWindowAgent(QObject* parent)
    : QObject(parent)
{
#ifdef Q_OS_WIN
    QCoreApplication::instance()->installNativeEventFilter(this);
#endif
}

FramelessWindowAgent::~FramelessWindowAgent()
{
#ifdef Q_OS_WIN
    QCoreApplication::instance()->removeNativeEventFilter(this);
#endif
}

int FramelessWindowAgent::titleBarHeight() const
{
    return title_bar_height_;
}

void FramelessWindowAgent::setTitleBarHeight(int value)
{
    if (title_bar_height_ == value) {
        return;
    }
    title_bar_height_ = value;
    emit titleBarHeightChanged();
}

int FramelessWindowAgent::resizeBorderThickness() const
{
    return resize_border_thickness_;
}

void FramelessWindowAgent::setResizeBorderThickness(int value)
{
    if (resize_border_thickness_ == value) {
        return;
    }
    resize_border_thickness_ = value;
    emit resizeBorderThicknessChanged();
}

int FramelessWindowAgent::dragRegionRight() const
{
    return drag_region_right_;
}

void FramelessWindowAgent::setDragRegionRight(int value)
{
    if (drag_region_right_ == value) {
        return;
    }
    drag_region_right_ = value;
    emit dragRegionRightChanged();
}

void FramelessWindowAgent::attach(QObject* windowObject)
{
    auto* windowPtr = qobject_cast<QWindow*>(windowObject);
    if (!windowPtr) {
        return;
    }

    if (window_ == windowPtr) {
        return;
    }

    window_ = windowPtr;
#ifdef Q_OS_WIN
    native_hwnd_ = 0;
    native_style_initialized_ = false;

    connect(window_, &QWindow::visibleChanged, this, [this]() {
        ensureNativeWindowStyle();
    });

    // 延迟到事件循环下一拍，避免窗口创建阶段重入
    QTimer::singleShot(0, this, [this]() {
        ensureNativeWindowStyle();
    });
#endif
}

void FramelessWindowAgent::minimize()
{
    if (!window()) {
        return;
    }
    window()->showMinimized();
}

void FramelessWindowAgent::maximize()
{
    if (!window()) {
        return;
    }
#ifdef Q_OS_WIN
    auto hwnd = reinterpret_cast<HWND>(nativeHwnd());
    if (hwnd) {
        ::PostMessageW(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        return;
    }
#endif
    window()->showMaximized();
}

void FramelessWindowAgent::restore()
{
    if (!window()) {
        return;
    }
#ifdef Q_OS_WIN
    auto hwnd = reinterpret_cast<HWND>(nativeHwnd());
    if (hwnd) {
        ::PostMessageW(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
        return;
    }
#endif
    window()->showNormal();
}

void FramelessWindowAgent::toggleMaximize()
{
    if (!window()) {
        return;
    }
    if (window()->visibility() == QWindow::Maximized) {
        restore();
    } else {
        maximize();
    }
}

void FramelessWindowAgent::closeWindow()
{
    if (!window()) {
        return;
    }
#ifdef Q_OS_WIN
    auto hwnd = reinterpret_cast<HWND>(nativeHwnd());
    if (hwnd) {
        ::PostMessageW(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
        return;
    }
#endif
    window()->close();
}

#ifdef Q_OS_WIN
bool FramelessWindowAgent::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
{
    if (eventType != "windows_generic_MSG" || !message || !window() || nativeHwnd() == 0) {
        return false;
    }

    const MSG* msg = static_cast<const MSG*>(message);
    const HWND hwnd = msg->hwnd;
    const HWND targetHwnd = reinterpret_cast<HWND>(nativeHwnd());
    if (!hwnd || hwnd != targetHwnd) {
        return false;
    }

    switch (msg->message) {
    case WM_NCCALCSIZE: {
        // 告诉系统我们接管非客户区计算，实现真正无边框
        if (msg->wParam) {
            *result = 0;
            return true;
        }
        return false;
    }
    case WM_NCHITTEST: {
        // 命中测试：决定当前鼠标点是边框缩放区、标题栏拖拽区还是客户区
        if (window()->visibility() == QWindow::FullScreen) {
            return false;
        }

        const LONG x = GET_X_LPARAM(msg->lParam);
        const LONG y = GET_Y_LPARAM(msg->lParam);
        const bool isMaximized = window()->visibility() == QWindow::Maximized;
        const qreal dpr = window()->devicePixelRatio();

        RECT windowRect{};
        ::GetWindowRect(hwnd, &windowRect);

        const int border = qMax(1, qRound(resize_border_thickness_ * dpr));
        const bool left = x < windowRect.left + border;
        const bool right = x >= windowRect.right - border;
        const bool top = y < windowRect.top + border;
        const bool bottom = y >= windowRect.bottom - border;

        if (!isMaximized) {
            if (top && left) {
                *result = HTTOPLEFT;
                return true;
            }
            if (top && right) {
                *result = HTTOPRIGHT;
                return true;
            }
            if (bottom && left) {
                *result = HTBOTTOMLEFT;
                return true;
            }
            if (bottom && right) {
                *result = HTBOTTOMRIGHT;
                return true;
            }
            if (left) {
                *result = HTLEFT;
                return true;
            }
            if (right) {
                *result = HTRIGHT;
                return true;
            }
            if (top) {
                *result = HTTOP;
                return true;
            }
            if (bottom) {
                *result = HTBOTTOM;
                return true;
            }
        }

        POINT localPos{x, y};
        ::ScreenToClient(hwnd, &localPos);

        const int dragRight = drag_region_right_ > 0 ? drag_region_right_ : window()->width();
        const int dragRightNative = qRound(dragRight * dpr);
        const int titleBarHeightNative = qRound(title_bar_height_ * dpr);
        if (localPos.y >= 0 && localPos.y < titleBarHeightNative && localPos.x >= 0 && localPos.x < dragRightNative) {
            // 返回标题栏命中，系统会处理拖动/贴靠/最大化还原等行为
            *result = HTCAPTION;
            return true;
        }

        return false;
    }
    default:
        return false;
    }
}
#endif

QWindow* FramelessWindowAgent::window() const
{
    return window_.data();
}

#ifdef Q_OS_WIN
qintptr FramelessWindowAgent::nativeHwnd() const
{
    return native_hwnd_;
}

void FramelessWindowAgent::ensureNativeWindowStyle()
{
    if (!window() || !window()->handle()) {
        return;
    }

    const HWND hwnd = reinterpret_cast<HWND>(window()->winId());
    if (!hwnd) {
        return;
    }
    native_hwnd_ = reinterpret_cast<qintptr>(hwnd);

    if (native_style_initialized_) {
        return;
    }

    // 保留系统边框相关样式，才能获得完整 Win32 窗口交互能力（如 Snap）
    LONG_PTR style = ::GetWindowLongPtrW(hwnd, GWL_STYLE);
    style |= WS_CAPTION;
    style |= WS_THICKFRAME;
    style |= WS_MINIMIZEBOX;
    style |= WS_MAXIMIZEBOX;
    style |= WS_SYSMENU;
    ::SetWindowLongPtrW(hwnd, GWL_STYLE, style);

    ::SetWindowPos(
        hwnd,
        nullptr,
        0,
        0,
        0,
        0,
        SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    native_style_initialized_ = true;
}
#endif
