import QtQuick
import QtQuick.Controls
import QtQuick.Window
import MarsQuickTools

// MarsApplicationWindow：
// 模块封装后的主窗口基类
// 使用者只需设置 windowTitle/page，即可获得：
// 1) 无边框窗口 + 原生控制能力
// 2) 统一标题栏
// 3) 亮暗主题切换与圆形揭幕动画
ApplicationWindow {
    id: root

    // 对外配置：窗口标题、页面内容、主题揭幕动画时长（毫秒）
    property string windowTitle: ""
    property Component page
    property int themeRevealDurationMs: 500

    width: 1200
    height: 760
    minimumWidth: 800
    minimumHeight: 520
    visible: true
    title: root.windowTitle

    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinMaxButtonsHint | Qt.WindowCloseButtonHint
    color: "transparent"

    // C++ 代理：接管无边框窗口的系统行为。
    FramelessWindowAgent {
        id: framelessAgent
        titleBarHeight: titleBar.height
        dragRegionRight: titleBar.dragRegionRight
        resizeBorderThickness: 8
    }

    Component.onCompleted: framelessAgent.attach(root)

    Rectangle {
        id: frameRoot
        anchors.fill: parent
        color: Theme.bg
        border.color: Theme.windowBorder
        border.width: (root.visibility === Window.Maximized || root.visibility === Window.FullScreen) ? 0 : 1

        Column {
            anchors.fill: parent
            spacing: 0

            WindowTitleBar {
                id: titleBar
                width: parent.width
                targetWindow: root
                windowAgent: framelessAgent
                titleText: root.windowTitle
                onThemeToggleRequested: function(x, y) {
                    // 第一步：先生成并展示“旧主题截图覆盖层”
                    themeRevealOverlay.prepare(root, x, y)
                }
            }

            Item {
                width: parent.width
                height: parent.height - titleBar.height
                clip: true

                Loader {
                    anchors.fill: parent
                    sourceComponent: root.page
                }
            }
        }
    }

    ThemeRevealOverlay {
        id: themeRevealOverlay
        anchors.fill: frameRoot
        z: 9999

        onPrepared: {
            // 第二步：覆盖层就绪后再切主题；第三步：执行揭幕扩散动画
            Theme.toggleMode()
            themeRevealOverlay.reveal(root.themeRevealDurationMs)
        }
    }

    // 非 Windows 下，补充 QML 侧缩放热区
    WindowResizeHandles {
        targetWindow: root
        enabledResize: Qt.platform.os !== "windows"
    }
}
