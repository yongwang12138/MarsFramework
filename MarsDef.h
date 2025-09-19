#ifndef MARSDEF_H
#define MARSDEF_H

#include <QObject>

namespace MarsApplicationType
{
Q_NAMESPACE  // 启用元对象对命名空间内枚举的支持

// 窗口显示模式枚举（包含平台相关值）
enum WindowDisplayMode
{
    Normal = 0x0000,       // 基础模式（全平台通用）
    ElaMica = 0x0001,      // 自定义云母效果（框架自实现）
#if defined(Q_OS_WIN)        // Windows 特有效果（依赖系统 API）
    Mica = 0x0002,         // 系统原生 Mica 效果
    MicaAlt = 0x0003,      // 系统原生 MicaAlt 效果
    Acrylic = 0x0004,      // 系统原生亚克力效果
    DWMBlur = 0x0005,      // 系统 DWM 模糊效果
#endif
};

// 注册枚举到 Qt 元对象系统（关键：让枚举支持信号槽、QML 等）
Q_ENUM_NS(WindowDisplayMode)
}

namespace MarsThemeType
{
Q_NAMESPACE  // 启用元对象对命名空间内枚举的支持

enum ThemeMode
{
    Light = 0x0000,
    Dark = 0x0001,
};

Q_ENUM_NS(ThemeMode)

enum ThemeColor
{
    ScrollBarHandle,
    ToggleSwitchNoToggledCenter,
    WindowBase,
    WindowCentralStackBase,
    PrimaryNormal,
    PrimaryHover,
    PrimaryPress,
    PopupBorder,
    PopupBorderHover,
    PopupBase,
    PopupHover,
    DialogBase,
    DialogLayoutArea,
    BasicText,
    BasicTextInvert,
    BasicDetailsText,
    BasicTextNoFocus,
    BasicTextDisable,
    BasicTextPress,
    BasicBorder,
    BasicBorderDeep,
    BasicBorderHover,
    BasicBase,
    BasicBaseDeep,
    BasicDisable,
    BasicHover,
    BasicPress,
    BasicSelectedHover,
    BasicBaseLine,
    BasicHemline,
    BasicIndicator,
    BasicChute,
    BasicAlternating,
    BasicBaseAlpha,
    BasicBaseDeepAlpha,
    BasicHoverAlpha,
    BasicPressAlpha,
    BasicSelectedAlpha,
    BasicSelectedHoverAlpha,
    StatusDanger,
    Win10BorderActive,
    Win10BorderInactive
};
Q_ENUM_NS(ThemeColor)
}

namespace MarsTitleBarType
{
Q_NAMESPACE  // 启用元对象对命名空间内枚举的支持

enum ButtonType
{
    NoneButtonHint = 0x0000,
    ThemeButtonHint = 0x0001,
    MinimizeButtonHint = 0x0002,
    MaximizeButtonHint = 0x0004,
    CloseButtonHint = 0x0008
};
Q_ENUM_NS(ButtonType)
// 标志类型(flag type)
Q_DECLARE_FLAGS(ButtonFlags, ButtonType)

enum WMMouseActionType
{
    WMLBUTTONDOWN = 0x0001,
    WMLBUTTONUP = 0x0002,
    WMLBUTTONDBLCLK = 0x0004,
    WMNCLBUTTONDOWN = 0x0008,
};
Q_ENUM_NS(WMMouseActionType)
// 标志类型(flag type)
Q_DECLARE_FLAGS(WMMouseActionTypes, WMMouseActionType)

}

// 自动生成常用的位运算运算符
Q_DECLARE_OPERATORS_FOR_FLAGS(MarsTitleBarType::ButtonFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(MarsTitleBarType::WMMouseActionTypes)

#endif // MARSDEF_H
