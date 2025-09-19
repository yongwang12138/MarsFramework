#include "MarsWidget.h"

#include <QPainter>
#include <QScreen>
#include <QStyle>
#include <QVBoxLayout>
#include "MarsTheme.h"

MarsWidget::MarsWidget(QWidget* parent)
    : QWidget{parent}
{
    resize(800, 600);
    setWindowIcon(style()->standardIcon(QStyle::SP_ComputerIcon));
    setWindowTitle("MarsWidget框架");
    setObjectName("MarsWidget");
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
    _themeMode = MarsTheme::instance().getThemeMode();
    connect(&MarsTheme::instance(), &MarsTheme::themeModeChanged, this, [=](MarsThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

void MarsWidget::moveToCenter()
{
    if (isMaximized() || isFullScreen())
    {
        return;
    }

    auto geometry = screen()->availableGeometry();
    setGeometry((geometry.left() + geometry.right() - width()) / 2, (geometry.top() + geometry.bottom() - height()) / 2, width(), height());
}

void MarsWidget::setMainWidget(QWidget *widget)
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

void MarsWidget::paintEvent(QPaintEvent* event)
{
    if (_windowDisplayMode == MarsApplicationType::WindowDisplayMode::Normal)
    {
        QPainter painter(this);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(MarsTheme::instance().getThemeColor(_themeMode, MarsThemeType::WindowBase));
        painter.drawRect(rect());
        painter.restore();
    }
    QWidget::paintEvent(event);
}

bool MarsWidget::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    int ret = _titleBar->takeOverNativeEvent(eventType, message, result);
    if (ret == -1)
    {
        return QWidget::nativeEvent(eventType, message, result);
    }
    return (bool)ret;
}
