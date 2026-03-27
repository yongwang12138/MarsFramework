pragma Singleton
import QtQuick

QtObject {
    readonly property color bg: "#F7F8FA"
    readonly property color card: "#FFFFFF"
    readonly property color primary: "#1473E6"
    readonly property color textMain: "#111827"
    readonly property color textMuted: "#6B7280"

    readonly property color titleBarBg: "#F3F4F6"
    readonly property color titleBarText: "#111827"
    readonly property color titleButtonHover: "#E5E7EB"
    readonly property color titleButtonPressed: "#D1D5DB"
    readonly property color titleButtonCloseHover: "#E81123"
    readonly property color titleButtonClosePressed: "#C50F1F"
    readonly property color windowBorder: "#D1D5DB"

    readonly property int titleBarHeight: 36
    readonly property int radiusM: 12
    readonly property int spacingM: 16
}
