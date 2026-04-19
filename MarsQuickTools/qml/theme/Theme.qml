pragma Singleton
import QtQuick

// Theme：模块级主题单例
// 统一管理亮暗模式、颜色、圆角、间距、动效时长等设计令牌
QtObject {
    property bool isDark: false

    function toggleMode() {
        isDark = !isDark
    }

    readonly property color bg: isDark ? "#0F1115" : "#F7F8FA"
    readonly property color card: isDark ? "#1A1D24" : "#FFFFFF"
    readonly property color primary: isDark ? "#5AA2FF" : "#1473E6"
    readonly property color textMain: isDark ? "#E5E7EB" : "#111827"
    readonly property color textMuted: isDark ? "#9CA3AF" : "#6B7280"

    readonly property color titleBarBg: isDark ? "#151922" : "#F3F4F6"
    readonly property color titleBarText: isDark ? "#E5E7EB" : "#111827"
    readonly property color titleButtonHover: isDark ? "#2B313D" : "#E5E7EB"
    readonly property color titleButtonPressed: isDark ? "#3A4251" : "#D1D5DB"
    readonly property color titleButtonCloseHover: "#E81123"
    readonly property color titleButtonClosePressed: "#C50F1F"
    readonly property color windowBorder: isDark ? "#2B313D" : "#D1D5DB"

    readonly property int titleBarHeight: 36
    readonly property int radiusM: 12
    readonly property int spacingM: 16
    readonly property int transitionDuration: 220
}
