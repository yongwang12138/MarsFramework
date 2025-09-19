#include "MarsTitleBar.h"

#include <QApplication>
#include <QDebug>

#include "MarsWinShadowHelper.h"

#ifndef Q_OS_WIN
#include <QDateTime>
#include <QWindow>
#endif
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QScreen>
#include <QStyle>
#include <QTimer>
#include <QVBoxLayout>

#include "MarsDef.h"
#include "MarsTheme.h"

MarsTitleBar::MarsTitleBar(QWidget* parent)
    : QWidget{parent}
{
    _buttonFlags = MarsTitleBarType::ThemeButtonHint | MarsTitleBarType::MinimizeButtonHint
                   | MarsTitleBarType::MaximizeButtonHint | MarsTitleBarType::CloseButtonHint;
    window()->setAttribute(Qt::WA_Mapped);
    setFixedHeight(40);
    window()->installEventFilter(this);
#ifdef Q_OS_WIN
    eWinHelper.initWinAPI();
#endif
    setMouseTracking(true);
    setObjectName("MarsTitleBar");

    // 图标
    _iconLabel = new QLabel(this);
    _iconLabel->setFixedSize(30, 30);
    _iconLabel->setScaledContents(true);
    if (parent->windowIcon().isNull())
    {
        _iconLabel->setVisible(false);
    } else {
        _iconLabel->setPixmap(parent->windowIcon().pixmap(30, 30));
    }
    connect(parent, &QWidget::windowIconChanged, this, [=](const QIcon& icon) {
        _iconLabel->setPixmap(icon.pixmap(30, 30));
        _iconLabel->setVisible(icon.isNull() ? false : true);
    });

    //标题
    _titleLabel = new QLabel(this);
    if (parent->windowTitle().isEmpty())
    {
        _titleLabel->setVisible(false);
    }
    else
    {
        _titleLabel->setText(parent->windowTitle());
    }
    connect(parent, &QWidget::windowTitleChanged, this, [=](const QString& title) {
        _titleLabel->setText(title);
        _titleLabel->setVisible(title.isEmpty() ? false : true);
    });

    // 主题按钮
    _themeButton = new QToolButton(this);
    _themeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarContextHelpButton));
    _themeButton->setFixedSize(40, 40);
    connect(_themeButton, &QToolButton::clicked, this, &MarsTitleBar::themeButtonClicked);

    // 最小化按钮
    _minButton = new QToolButton(this);
    _minButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMinButton));
    _minButton->setFixedSize(40, 40);
    connect(_minButton, &QToolButton::clicked, this, &MarsTitleBar::onMinButtonClicked);
    // 最大化/恢复按钮
    _maxButton = new QToolButton(this);
    _maxButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
    _maxButton->setFixedSize(40, 40);
    connect(_maxButton, &QToolButton::clicked, this, &MarsTitleBar::onMaxButtonClicked);
    // 关闭按钮
    _closeButton = new QToolButton(this);
    _closeButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarCloseButton));
    _closeButton->setFixedSize(40, 40);
    connect(_closeButton, &QToolButton::clicked, this, &MarsTitleBar::onCloseButtonClicked);

    _mainLayout = new QHBoxLayout(this);
    _mainLayout->setContentsMargins(5, 0, 0, 0);
    _mainLayout->setSpacing(5);
    _mainLayout->addWidget(_iconLabel);
    _mainLayout->addWidget(_titleLabel);
    _mainLayout->addStretch();
    _mainLayout->addWidget(_themeButton);
    _mainLayout->addWidget(_minButton);
    _mainLayout->addWidget(_maxButton);
    _mainLayout->addWidget(_closeButton);

#ifdef Q_OS_WIN
    //主屏幕变更处理
    connect(qApp, &QApplication::primaryScreenChanged, this, [=]() {
        HWND hwnd = (HWND)(_currentWinID);
        ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
        ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
    });
    _lastScreen = qApp->screenAt(window()->geometry().center());
#endif

    _themeMode = MarsTheme::instance().getThemeMode();
    connect(&MarsTheme::instance(), &MarsTheme::themeModeChanged, this, [=](MarsThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

void MarsTitleBar::setWindowButtonFlag(MarsTitleBarType::ButtonType buttonFlag, bool isEnable)
{
    if (isEnable)
    {
        setWindowButtonFlags(_buttonFlags | buttonFlag);
    }
    else
    {
        setWindowButtonFlags(_buttonFlags & ~buttonFlag);
    }
}

void MarsTitleBar::setWindowButtonFlags(MarsTitleBarType::ButtonFlags buttonFlags)
{
    _buttonFlags = buttonFlags;
    if (_buttonFlags.testFlag(MarsTitleBarType::NoneButtonHint))
    {
        _themeButton->setVisible(false);
        _minButton->setVisible(false);
        _maxButton->setVisible(false);
        _closeButton->setVisible(false);
    }
    else
    {
        _themeButton->setVisible(_buttonFlags.testFlag(MarsTitleBarType::ThemeButtonHint));
        _minButton->setVisible(_buttonFlags.testFlag(MarsTitleBarType::MinimizeButtonHint));
        _maxButton->setVisible(_buttonFlags.testFlag(MarsTitleBarType::MaximizeButtonHint));
        _closeButton->setVisible(_buttonFlags.testFlag(MarsTitleBarType::CloseButtonHint));
    }
}

MarsTitleBarType::ButtonFlags MarsTitleBar::getWindowButtonFlags() const
{
    return _buttonFlags;
}

#ifdef Q_OS_WIN
int MarsTitleBar::takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    if ((eventType != "windows_generic_MSG") || !message)
    {
        return 0;
    }
    const auto msg = static_cast<const MSG*>(message);
    const HWND hwnd = msg->hwnd;
    if (!hwnd || !msg)
    {
        return 0;
    }
    _currentWinID = (qint64)hwnd;
    const UINT uMsg = msg->message;
    const WPARAM wParam = msg->wParam;
    const LPARAM lParam = msg->lParam;
    switch (uMsg)
    {
    case WM_WINDOWPOSCHANGING:
    {
        WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
        if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
        {
            wp->flags |= SWP_NOCOPYBITS;
            *result = ::DefWindowProcW(hwnd, uMsg, wParam, lParam);
            return 1;
        }
        return 0;
    }
    case WM_NCPAINT:
    {
        if (!eWinHelper.getIsCompositionEnabled())
        {
            *result = FALSE;
            return 1;
        }
        else
        {
            return -1;
        }
    }
    case WM_NCACTIVATE:
    {
        if (eWinHelper.getIsCompositionEnabled())
        {
            *result = ::DefWindowProcW(hwnd, WM_NCACTIVATE, wParam, -1);
        }
        else
        {
            *result = TRUE;
        }
        return 1;
    }
    case WM_SIZE:
    {
        if (wParam == SIZE_RESTORED)
        {
            _maxButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarMaxButton));
        }
        else if (wParam == SIZE_MAXIMIZED)
        {
            _maxButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarNormalButton));
        }
        return 0;
    }
    case WM_NCCALCSIZE:
    {
        if (wParam == FALSE)
        {
            return 0;
        }
        RECT* clientRect = &((NCCALCSIZE_PARAMS*)(lParam))->rgrc[0];
        if (eWinHelper.getIsWinVersionGreater10())
        {
            const LONG originTop = clientRect->top;
            const LRESULT hitTestResult = ::DefWindowProcW(hwnd, WM_NCCALCSIZE, wParam, lParam);
            if ((hitTestResult != HTERROR) && (hitTestResult != HTNOWHERE))
            {
                *result = static_cast<long>(hitTestResult);
                return 1;
            }
            clientRect->top = originTop;
        }
        if (::IsZoomed(hwnd))
        {
            auto geometry = window()->screen()->geometry();
            clientRect->top = geometry.top();
            if (!eWinHelper.getIsWinVersionGreater10())
            {
                quint32 borderThickness = eWinHelper.getResizeBorderThickness(hwnd);
                clientRect->left = geometry.left();
                clientRect->bottom -= borderThickness;
                clientRect->right -= borderThickness;
            }
        }
        *result = WVR_REDRAW;
        return 1;
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
        break;
    }
    case WM_NCHITTEST:
    {
        if (_containsCursorToItem(_maxButton))
        {
            if (*result == HTNOWHERE)
            {
                if (!_isHoverMaxButton)
                {
                    _isHoverMaxButton = true;
                    _maxButton->update();
                }
                *result = HTZOOM;
            }
            return 1;
        }
        else
        {
            if (_isHoverMaxButton)
            {
                _isHoverMaxButton = false;
                _maxButton->update();
            }
        }
        POINT nativeLocalPos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        ::ScreenToClient(hwnd, &nativeLocalPos);
        RECT clientRect{0, 0, 0, 0};
        ::GetClientRect(hwnd, &clientRect);
        bool left = nativeLocalPos.x < _margins;
        bool right = nativeLocalPos.x > (clientRect.right - clientRect.left - _margins);
        bool top = nativeLocalPos.y < _margins;
        bool bottom = nativeLocalPos.y > (clientRect.bottom - clientRect.top - _margins);
        *result = HTNOWHERE;
        if (!window()->isFullScreen() && !window()->isMaximized())
        {
            if (left && top)
            {
                *result = HTTOPLEFT;
            }
            else if (left && bottom)
            {
                *result = HTBOTTOMLEFT;
            }
            else if (right && top)
            {
                *result = HTTOPRIGHT;
            }
            else if (right && bottom)
            {
                *result = HTBOTTOMRIGHT;
            }
            else if (left)
            {
                *result = HTLEFT;
            }
            else if (right)
            {
                *result = HTRIGHT;
            }
            else if (top)
            {
                *result = HTTOP;
            }
            else if (bottom)
            {
                *result = HTBOTTOM;
            }
        }
        if (0 != *result)
        {
            return 1;
        }
        if (_containsCursorToItem(this) && !window()->isFullScreen())
        {
            *result = HTCAPTION;
            return 1;
        }
        *result = HTCLIENT;
        return 1;
    }
    case WM_GETMINMAXINFO:
    {
        // 获取窗口的"最小/最大信息"
        MINMAXINFO* minMaxInfo = reinterpret_cast<MINMAXINFO*>(lParam);

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
        QSize minSize = window()->minimumSize();
        if (minSize.isValid()) {
            // 转换为屏幕坐标（需要考虑 DPI 缩放）
            qreal dpiScale = devicePixelRatioF();
            minMaxInfo->ptMinTrackSize.x = static_cast<LONG>(minSize.width() * dpiScale);
            minMaxInfo->ptMinTrackSize.y = static_cast<LONG>(minSize.height() * dpiScale);
        }
        return 1;
    }
    case WM_NCLBUTTONDOWN:
    {
        if (_containsCursorToItem(_maxButton))
        {
            return 1;
        }
        break;
    }
    case WM_NCLBUTTONUP:
    {
        if (_containsCursorToItem(_maxButton))
        {
            onMaxButtonClicked();
            return 1;
        }
        break;
    }
    case WM_NCLBUTTONDBLCLK:
    {
        return 0;
    }
    case WM_NCRBUTTONDOWN:
    {
        if (wParam == HTCAPTION)
        {
            _showAppBarMenu(QCursor::pos());
        }
        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        if ((GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_SPACE) & 0x8000))
        {
            auto pos = window()->geometry().topLeft();
            _showAppBarMenu(QPoint(pos.x(), pos.y() + this->height()));
        }
        break;
    }
    }
    return -1;
}


#endif

void MarsTitleBar::onMinButtonClicked()
{
    window()->showMinimized();
}

void MarsTitleBar::onMaxButtonClicked()
{
    if (window()->isMaximized())
    {
        window()->showNormal();
    }
    else
    {
        window()->showMaximized();
    }
}

void MarsTitleBar::onCloseButtonClicked()
{
    window()->close();
}

bool MarsTitleBar::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Resize:
    {
        QSize size = parentWidget()->size();
        this->resize(size.width(), this->height());
        break;
    }
#ifdef Q_OS_WIN
    case QEvent::Show:
    {
        HWND hwnd = (HWND)_currentWinID;
        DWORD style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
        style &= ~WS_SYSMENU;
        ::SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME);
        if (!eWinHelper.getIsWinVersionGreater10())
        {
            SetClassLong(hwnd, GCL_STYLE, GetClassLong(hwnd, GCL_STYLE) | CS_DROPSHADOW);
        }
        break;
    }
#endif
    case QEvent::Close:
    {
        QCloseEvent* closeEvent = dynamic_cast<QCloseEvent*>(event);
        if (closeEvent->spontaneous())
        {
            event->ignore();
            if (window()->isMinimized())
            {
                window()->showNormal();
            }
            onCloseButtonClicked();
            return true;
        }
        else
        {
            break;
        }
    }
#ifndef Q_OS_WIN
    case QEvent::MouseButtonPress:
    {
        if (_edges != 0)
        {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                _updateCursor(_edges);
                window()->windowHandle()->startSystemResize(Qt::Edges(_edges));
            }
        }
        else
        {
            if (_containsCursorToItem(this))
            {
                qint64 clickTimer = QDateTime::currentMSecsSinceEpoch();
                qint64 offset = clickTimer - _clickTimer;
                _clickTimer = clickTimer;
                if (offset > 300)
                {
                    window()->windowHandle()->startSystemMove();
                }
            }
        }
        break;
    }
    case QEvent::MouseButtonDblClick:
    {
        if (_containsCursorToItem(this))
        {
            if (window()->isMaximized())
            {
                window()->showNormal();
            }
            else
            {
                window()->showMaximized();
            }
        }
        break;
    }
    case QEvent::MouseButtonRelease:
    {
        _edges = 0;
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::RightButton)
        {
            _showAppBarMenu(QCursor::pos());
        }
        break;
    }
    case QEvent::HoverMove:
    {
        if (window()->isMaximized() || window()->isFullScreen())
        {
            break;
        }
        if (_pIsFixedSize)
        {
            break;
        }
        QHoverEvent* mouseEvent = static_cast<QHoverEvent*>(event);
        QPoint p = mouseEvent->position().toPoint();
        if (p.x() >= _margins && p.x() <= (window()->width() - _margins) && p.y() >= _margins && p.y() <= (window()->height() - _margins))
        {
            if (_edges != 0)
            {
                _edges = 0;
                _updateCursor(_edges);
            }
            break;
        }
        _edges = 0;
        if (p.x() < _margins)
        {
            _edges |= Qt::LeftEdge;
        }
        if (p.x() > (window()->width() - _margins))
        {
            _edges |= Qt::RightEdge;
        }
        if (p.y() < _margins)
        {
            _edges |= Qt::TopEdge;
        }
        if (p.y() > (window()->height() - _margins))
        {
            _edges |= Qt::BottomEdge;
        }
        _updateCursor(_edges);
        break;
    }
#endif
    default:
    {
        break;
    }
    }
    return QObject::eventFilter(obj, event);
}

#ifdef Q_OS_WIN
void MarsTitleBar::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (eWinHelper.getIsWinVersionGreater10() && !eWinHelper.getIsWinVersionGreater11())
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing);
        auto borderWidth = eWinHelper.getSystemMetricsForDpi((HWND)_currentWinID, SM_CXBORDER);
        painter.setPen(QPen(window()->isActiveWindow() ?
                                MarsTheme::instance().getThemeColor(_themeMode, MarsThemeType::Win10BorderActive) :
                                MarsTheme::instance().getThemeColor(_themeMode, MarsThemeType::Win10BorderInactive), borderWidth));
        painter.drawLine(QPoint(0, 0), QPoint(window()->width(), 0));
        painter.restore();
    }
}
#endif

void MarsTitleBar::_showAppBarMenu(QPoint point)
{
#ifdef Q_OS_WIN
    QScreen* screen = qApp->screenAt(QCursor::pos());
    if (!screen)
    {
        screen = QGuiApplication::primaryScreen();
    }
    if (!screen)
    {
        return;
    }
    const QPoint origin = screen->geometry().topLeft();
    auto nativePos = QPointF(QPointF(point - origin) * screen->devicePixelRatio()).toPoint() + origin;
    HWND hwnd = reinterpret_cast<HWND>(window()->winId());
    const HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
    if (window()->isMaximized() || window()->isFullScreen())
    {
        ::EnableMenuItem(hMenu, SC_MOVE, MFS_DISABLED);
        ::EnableMenuItem(hMenu, SC_RESTORE, MFS_ENABLED);
    }
    else
    {
        ::EnableMenuItem(hMenu, SC_MOVE, MFS_ENABLED);
        ::EnableMenuItem(hMenu, SC_RESTORE, MFS_DISABLED);
    }
    if (!window()->isMaximized() && !window()->isFullScreen())
    {
        ::EnableMenuItem(hMenu, SC_SIZE, MFS_ENABLED);
        ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_ENABLED);
    }
    else
    {
        ::EnableMenuItem(hMenu, SC_SIZE, MFS_DISABLED);
        ::EnableMenuItem(hMenu, SC_MAXIMIZE, MFS_DISABLED);
    }
    const int result = ::TrackPopupMenu(hMenu, (TPM_RETURNCMD | (QGuiApplication::isRightToLeft() ? TPM_RIGHTALIGN : TPM_LEFTALIGN)), nativePos.x(),
                                        nativePos.y(), 0, hwnd, nullptr);
    if (result != FALSE)
    {
        ::PostMessageW(hwnd, WM_SYSCOMMAND, result, 0);
    }
#endif
}

void MarsTitleBar::_updateCursor(int edges)
{
    switch (edges)
    {
    case 0:
    {
        window()->setCursor(Qt::ArrowCursor);
        break;
    }
    case Qt::LeftEdge:
    case Qt::RightEdge:
    {
        window()->setCursor(Qt::SizeHorCursor);
        break;
    }
    case Qt::TopEdge:
    case Qt::BottomEdge:
    {
        window()->setCursor(Qt::SizeVerCursor);
        break;
    }
    case Qt::LeftEdge | Qt::TopEdge:
    case Qt::RightEdge | Qt::BottomEdge:
    {
        window()->setCursor(Qt::SizeFDiagCursor);
        break;
    }
    case Qt::RightEdge | Qt::TopEdge:
    case Qt::LeftEdge | Qt::BottomEdge:
    {
        window()->setCursor(Qt::SizeBDiagCursor);
        break;
    }
    default:
    {
        break;
    }
    }
}

bool MarsTitleBar::_containsCursorToItem(QWidget *item)
{
    if (!item || !item->isVisible())
    {
        return false;
    }
    auto point = item->window()->mapFromGlobal(QCursor::pos());
    QRectF rect = QRectF(item->mapTo(item->window(), QPoint(0, 0)), item->size());
    if (item == this)
    {
        if (_containsCursorToItem(_themeButton) || _containsCursorToItem(_minButton) || _containsCursorToItem(_maxButton) || _containsCursorToItem(_closeButton))
        {
            return false;
        }
    }
    else if (item == _maxButton)
    {
        rect.adjust(0, 8, 0, 0);
    }
    if (rect.contains(point))
    {
        return true;
    }
    return false;
}
