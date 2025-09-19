#include "MarsTheme.h"

#include <QPainter>
#include <QPainterPath>

Q_SINGLETON_CREATE_CPP(MarsTheme)
MarsTheme::MarsTheme(QObject* parent)
    : QObject{parent}
{
    initThemeColor();
}

void MarsTheme::setThemeMode(MarsThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    Q_EMIT themeModeChanged(_themeMode);
}

MarsThemeType::ThemeMode MarsTheme::getThemeMode() const
{
    return _themeMode;
}

void MarsTheme::drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius)
{
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing);
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    QColor color = _themeMode == MarsThemeType::Light ? QColor(0x70, 0x70, 0x70) : QColor(0x9C, 0x9B, 0x9E);
    for (int i = 0; i < shadowBorderWidth; i++)
    {
        path.addRoundedRect(widgetRect.x() + shadowBorderWidth - i, widgetRect.y() + shadowBorderWidth - i, widgetRect.width() - (shadowBorderWidth - i) * 2, widgetRect.height() - (shadowBorderWidth - i) * 2, borderRadius + i, borderRadius + i);
        int alpha = 1 * (shadowBorderWidth - i + 1);
        color.setAlpha(alpha > 255 ? 255 : alpha);
        painter->setPen(color);
        painter->drawPath(path);
    }
    painter->restore();
}

void MarsTheme::setThemeColor(MarsThemeType::ThemeMode themeMode, MarsThemeType::ThemeColor themeColor, QColor newColor)
{
    if (themeMode == MarsThemeType::Light)
    {
        _lightThemeColorList[themeColor] = newColor;
    }
    else
    {
        _darkThemeColorList[themeColor] = newColor;
    }
}

const QColor& MarsTheme::getThemeColor(MarsThemeType::ThemeMode themeMode, MarsThemeType::ThemeColor themeColor)
{
    if (themeMode == MarsThemeType::Light)
    {
        return _lightThemeColorList[themeColor];
    }
    else
    {
        return _darkThemeColorList[themeColor];
    }
}

void MarsTheme::initThemeColor()
{
    //MarsScrollBar
    _lightThemeColorList[MarsThemeType::ScrollBarHandle] = QColor(0xA0, 0xA0, 0xA0);
    _darkThemeColorList[MarsThemeType::ScrollBarHandle] = QColor(0x9F, 0x9F, 0x9F);

    //MarsToggleSwitch
    _lightThemeColorList[MarsThemeType::ToggleSwitchNoToggledCenter] = QColor(0x5A, 0x5A, 0x5A);
    _darkThemeColorList[MarsThemeType::ToggleSwitchNoToggledCenter] = QColor(0xD0, 0xD0, 0xD0);

    // 主题颜色
    _lightThemeColorList[MarsThemeType::PrimaryNormal] = QColor(0x00, 0x67, 0xC0);
    _darkThemeColorList[MarsThemeType::PrimaryNormal] = QColor(0x4C, 0xC2, 0xFF);
    _lightThemeColorList[MarsThemeType::PrimaryHover] = QColor(0x19, 0x75, 0xC5);
    _darkThemeColorList[MarsThemeType::PrimaryHover] = QColor(0x47, 0xB1, 0xE8);
    _lightThemeColorList[MarsThemeType::PrimaryPress] = QColor(0x31, 0x83, 0xCA);
    _darkThemeColorList[MarsThemeType::PrimaryPress] = QColor(0x42, 0xA1, 0xD2);

    // 通用颜色
    // 普通窗体
    _lightThemeColorList[MarsThemeType::WindowBase] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[MarsThemeType::WindowBase] = QColor(0x20, 0x20, 0x20);
    _lightThemeColorList[MarsThemeType::WindowCentralStackBase] = QColor(0xFF, 0xFF, 0xFF, 80);
    _darkThemeColorList[MarsThemeType::WindowCentralStackBase] = QColor(0x3E, 0x3E, 0x3E, 60);

    // 浮动窗体
    _lightThemeColorList[MarsThemeType::PopupBorder] = QColor(0xD6, 0xD6, 0xD6);
    _darkThemeColorList[MarsThemeType::PopupBorder] = QColor(0x47, 0x47, 0x47);
    _lightThemeColorList[MarsThemeType::PopupBorderHover] = QColor(0xCC, 0xCC, 0xCC);
    _darkThemeColorList[MarsThemeType::PopupBorderHover] = QColor(0x54, 0x54, 0x54);
    _lightThemeColorList[MarsThemeType::PopupBase] = QColor(0xFA, 0xFA, 0xFA);
    _darkThemeColorList[MarsThemeType::PopupBase] = QColor(0x2C, 0x2C, 0x2C);
    _lightThemeColorList[MarsThemeType::PopupHover] = QColor(0xF0, 0xF0, 0xF0);
    _darkThemeColorList[MarsThemeType::PopupHover] = QColor(0x38, 0x38, 0x38);

    // Dialog窗体
    _lightThemeColorList[MarsThemeType::DialogBase] = Qt::white;
    _darkThemeColorList[MarsThemeType::DialogBase] = QColor(0x1F, 0x1F, 0x1F);
    _lightThemeColorList[MarsThemeType::DialogLayoutArea] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[MarsThemeType::DialogLayoutArea] = QColor(0x20, 0x20, 0x20);

    // 基础颜色
    _lightThemeColorList[MarsThemeType::BasicText] = Qt::black;
    _darkThemeColorList[MarsThemeType::BasicText] = Qt::white;
    _lightThemeColorList[MarsThemeType::BasicTextInvert] = Qt::white;
    _darkThemeColorList[MarsThemeType::BasicTextInvert] = Qt::black;
    _lightThemeColorList[MarsThemeType::BasicDetailsText] = QColor(0x87, 0x87, 0x87);
    _darkThemeColorList[MarsThemeType::BasicDetailsText] = QColor(0xAD, 0xAD, 0xB0);
    _lightThemeColorList[MarsThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    _darkThemeColorList[MarsThemeType::BasicTextNoFocus] = QColor(0x86, 0x86, 0x8A);
    _lightThemeColorList[MarsThemeType::BasicTextDisable] = QColor(0xB6, 0xB6, 0xB6);
    _darkThemeColorList[MarsThemeType::BasicTextDisable] = QColor(0xA7, 0xA7, 0xA7);
    _lightThemeColorList[MarsThemeType::BasicTextPress] = QColor(0x5A, 0x5A, 0x5D);
    _darkThemeColorList[MarsThemeType::BasicTextPress] = QColor(0xBB, 0xBB, 0xBF);
    _lightThemeColorList[MarsThemeType::BasicBorder] = QColor(0xE5, 0xE5, 0xE5);
    _darkThemeColorList[MarsThemeType::BasicBorder] = QColor(0x4B, 0x4B, 0x4B);
    _lightThemeColorList[MarsThemeType::BasicBorderDeep] = QColor(0xA8, 0xA8, 0xA8);
    _darkThemeColorList[MarsThemeType::BasicBorderDeep] = QColor(0x5C, 0x5C, 0x5C);
    _lightThemeColorList[MarsThemeType::BasicBorderHover] = QColor(0xDA, 0xDA, 0xDA);
    _darkThemeColorList[MarsThemeType::BasicBorderHover] = QColor(0x57, 0x57, 0x57);
    _lightThemeColorList[MarsThemeType::BasicBase] = QColor(0xFD, 0xFD, 0xFD);
    _darkThemeColorList[MarsThemeType::BasicBase] = QColor(0x34, 0x34, 0x34);
    _lightThemeColorList[MarsThemeType::BasicBaseDeep] = QColor(0xE6, 0xE6, 0xE6);
    _darkThemeColorList[MarsThemeType::BasicBaseDeep] = QColor(0x61, 0x61, 0x61);
    _lightThemeColorList[MarsThemeType::BasicDisable] = QColor(0xF5, 0xF5, 0xF5);
    _darkThemeColorList[MarsThemeType::BasicDisable] = QColor(0x2A, 0x2A, 0x2A);
    _lightThemeColorList[MarsThemeType::BasicHover] = QColor(0xF3, 0xF3, 0xF3);
    _darkThemeColorList[MarsThemeType::BasicHover] = QColor(0x40, 0x40, 0x40);
    _lightThemeColorList[MarsThemeType::BasicPress] = QColor(0xF7, 0xF7, 0xF7);
    _darkThemeColorList[MarsThemeType::BasicPress] = QColor(0x3A, 0x3A, 0x3A);
    _lightThemeColorList[MarsThemeType::BasicBaseLine] = QColor(0xD1, 0xD1, 0xD1);
    _darkThemeColorList[MarsThemeType::BasicBaseLine] = QColor(0x45, 0x45, 0x45);
    _lightThemeColorList[MarsThemeType::BasicHemline] = QColor(0x86, 0x86, 0x86);
    _darkThemeColorList[MarsThemeType::BasicHemline] = QColor(0x9A, 0x9A, 0x9A);
    _lightThemeColorList[MarsThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    _darkThemeColorList[MarsThemeType::BasicIndicator] = QColor(0x75, 0x7C, 0x87);
    _lightThemeColorList[MarsThemeType::BasicChute] = QColor(0xB3, 0xB3, 0xB3);
    _darkThemeColorList[MarsThemeType::BasicChute] = QColor(0x63, 0x63, 0x63);

    // 基础透明
    _lightThemeColorList[MarsThemeType::BasicAlternating] = QColor(0xEF, 0xEF, 0xEF, 160);
    _darkThemeColorList[MarsThemeType::BasicAlternating] = QColor(0x45, 0x45, 0x45, 125);
    _lightThemeColorList[MarsThemeType::BasicBaseAlpha] = QColor(0xFF, 0xFF, 0xFF, 160);
    _darkThemeColorList[MarsThemeType::BasicBaseAlpha] = QColor(0x2D, 0x2D, 0x2D, 95);
    _lightThemeColorList[MarsThemeType::BasicBaseDeepAlpha] = QColor(0xCC, 0xCC, 0xCC, 160);
    _darkThemeColorList[MarsThemeType::BasicBaseDeepAlpha] = QColor(0x72, 0x72, 0x72, 95);
    _lightThemeColorList[MarsThemeType::BasicHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 70);
    _darkThemeColorList[MarsThemeType::BasicHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    _lightThemeColorList[MarsThemeType::BasicPressAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    _darkThemeColorList[MarsThemeType::BasicPressAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);
    _lightThemeColorList[MarsThemeType::BasicSelectedAlpha] = QColor(0xCC, 0xCC, 0xCC, 70);
    _darkThemeColorList[MarsThemeType::BasicSelectedAlpha] = QColor(0x4B, 0x4B, 0x4B, 75);
    _lightThemeColorList[MarsThemeType::BasicSelectedHoverAlpha] = QColor(0xCC, 0xCC, 0xCC, 40);
    _darkThemeColorList[MarsThemeType::BasicSelectedHoverAlpha] = QColor(0x4B, 0x4B, 0x4B, 55);

    // 状态颜色
    _lightThemeColorList[MarsThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);
    _darkThemeColorList[MarsThemeType::StatusDanger] = QColor(0xE8, 0x11, 0x23);

    // Win10顶部边框颜色
    _lightThemeColorList[MarsThemeType::Win10BorderActive] = QColor(0x6E, 0x6E, 0x6E);
    _darkThemeColorList[MarsThemeType::Win10BorderActive] = QColor(0x33, 0x33, 0x33);
    _lightThemeColorList[MarsThemeType::Win10BorderInactive] = QColor(0xA7, 0xA7, 0xA7);
    _darkThemeColorList[MarsThemeType::Win10BorderInactive] = QColor(0x3D, 0x3D, 0x3D);
}
