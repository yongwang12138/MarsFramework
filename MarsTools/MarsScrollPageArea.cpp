#include "MarsScrollPageArea.h"

#include <QPainter>
#include "MarsTheme.h"

MarsScrollPageArea::MarsScrollPageArea(QWidget* parent)
    : QWidget(parent)
    , _borderRadius(6)
{
    setFixedHeight(75);

    // 主题
    _themeMode = mTheme.getThemeMode();
    connect(&mTheme, &MarsTheme::themeModeChanged, this,  [=](MarsThemeType::ThemeMode themeMode) {
        _themeMode = themeMode;
        update();
    });
}

void MarsScrollPageArea::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(mTheme.getThemeColor(_themeMode, MarsThemeType::BasicBorder));
    painter.setBrush(mTheme.getThemeColor(_themeMode, MarsThemeType::BasicBaseAlpha));
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    painter.drawRoundedRect(foregroundRect, _borderRadius, _borderRadius);
    painter.restore();
}
