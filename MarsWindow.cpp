#include "MarsWindow.h"

#include <QPainter>
#include <QScreen>
#include <QStyle>
#include <QVBoxLayout>
#include "MarsTheme.h"

MarsWindow::MarsWindow(QWidget* parent)
    : QMainWindow{parent}
{
    resize(800, 600);
    setWindowIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    setWindowTitle("MarsWindow框架");
    setObjectName("MarsWindow");
    // setAttribute(Qt::WA_TranslucentBackground);

    _layout = new QVBoxLayout(this);
    _layout->setContentsMargins(0, 0, 0, 0);
    _layout->setSpacing(0);

    // 标题栏
    _titleBar = new MarsTitleBar(this);
    _titleBar->setWindowButtonFlags(MarsTitleBarType::ThemeButtonHint | MarsTitleBarType::MinimizeButtonHint
                                    | MarsTitleBarType::MaximizeButtonHint | MarsTitleBarType::CloseButtonHint);
    _layout->addWidget(_titleBar);

    // 内容区域
    _mainWidget = new QWidget(this);
    _mainWidget->setObjectName("mainWidget");
    _mainWidget->setStyleSheet("QWidget#mainWidget { background: white; border: none; }");
    _layout->addWidget(_mainWidget, 1);

    // 主题
    _themeMode = mTheme.getThemeMode();
    connect(&mTheme, &MarsTheme::themeModeChanged, this,  [=](MarsThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
    connect(_titleBar, &MarsTitleBar::themeButtonClicked, this, &MarsWindow::onThemeReadyChange);
}

void MarsWindow::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }

    auto geometry = screen()->availableGeometry();
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void MarsWindow::setMainWidget(QWidget* widget)
{
    if (_mainWidget) {
        _layout->removeWidget(_mainWidget);
        _mainWidget->deleteLater();
    }

    _mainWidget = widget;
    if (_mainWidget) {
        _layout->addWidget(_mainWidget, 1);
    }
}

void MarsWindow::paintEvent(QPaintEvent* event)
{
    if (_windowDisplayMode == MarsApplicationType::WindowDisplayMode::Normal)
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(mTheme.getThemeColor(_themeMode, MarsThemeType::WindowBase));
        painter.drawRect(rect());
        painter.restore();
    }
    QWidget::paintEvent(event);
}

bool MarsWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
    int ret = _titleBar->takeOverNativeEvent(eventType, message, result);
    if (ret == -1)
    {
        return QWidget::nativeEvent(eventType, message, result);
    }
    return (bool)ret;
}

void MarsWindow::onThemeReadyChange()
{
    // 主题变更绘制窗口
    switch (_windowDisplayMode)
    {
    case MarsApplicationType::Normal:
    {
        if (!_animationWidget)
        {
            QPoint centerPos = mapFromGlobal(QCursor::pos());
            _animationWidget = new MarsThemeAnimationWidget(this);
            connect(_animationWidget, &MarsThemeAnimationWidget::animationFinished, this, [=]() {
                _animationWidget = nullptr;
            });
            _animationWidget->move(0, 0);
            _animationWidget->setOldWindowBackground(grab(rect()).toImage());
            if (mTheme.getThemeMode() == MarsThemeType::Light)
            {
                mTheme.setThemeMode(MarsThemeType::Dark);
            } else if(mTheme.getThemeMode() == MarsThemeType::Dark)
            {
                mTheme.setThemeMode(MarsThemeType::Light);
            }
            _animationWidget->setCenter(centerPos);
            qreal topLeftDis = _distance(centerPos, QPoint(0, 0));
            qreal topRightDis = _distance(centerPos, QPoint(width(), 0));
            qreal bottomLeftDis = _distance(centerPos, QPoint(0, height()));
            qreal bottomRightDis = _distance(centerPos, QPoint(width(), height()));
            QList<qreal> disList{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
            std::sort(disList.begin(), disList.end());
            _animationWidget->setEndRadius(disList[3]);
            _animationWidget->resize(width(), height());
            _animationWidget->startAnimation(500);
            _animationWidget->show();
        }
        break;
    }
    default:
    {
        if (mTheme.getThemeMode() == MarsThemeType::Light)
        {
            mTheme.setThemeMode(MarsThemeType::Dark);
        }
        else
        {
            mTheme.setThemeMode(MarsThemeType::Light);
        }
        break;
    }
    }
}

qreal MarsWindow::_distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}
