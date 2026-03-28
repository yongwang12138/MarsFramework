import QtQuick
import QtQuick.Window
import MarsQuickTools

// WindowTitleBar：
// 统一标题栏组件，负责：
// 1) 窗口拖动/双击最大化；2) 系统按钮；3) 主题切换按钮事件上抛。
Rectangle {
    id: root

    required property Window targetWindow
    property var windowAgent: null
    property string titleText: ""
    readonly property int dragRegionRight: controlButtons.x

    // 发出主题切换请求，并携带按钮点击点（窗口坐标）。
    signal themeToggleRequested(real x, real y)

    height: Theme.titleBarHeight
    color: Theme.titleBarBg

    function toggleMaximize() {
        if (root.windowAgent) {
            root.windowAgent.toggleMaximize()
            return
        }
        if (root.targetWindow.visibility === Window.Maximized) {
            root.targetWindow.showNormal()
        } else {
            root.targetWindow.showMaximized()
        }
    }

    Item {
        id: dragArea
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: controlButtons.left

        Row {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 12
            spacing: 8

            Rectangle {
                width: 16
                height: 16
                radius: 4
                color: Theme.primary
            }

            Text {
                text: root.titleText
                color: Theme.titleBarText
                font.pixelSize: 13
                font.bold: true
            }
        }

        MouseArea {
            anchors.fill: parent
            enabled: Qt.platform.os !== "windows"
            acceptedButtons: Qt.LeftButton
            onPressed: function(mouse) {
                if (mouse.button === Qt.LeftButton) {
                    root.targetWindow.startSystemMove()
                }
            }
            onDoubleClicked: function(mouse) {
                if (mouse.button === Qt.LeftButton) {
                    root.toggleMaximize()
                }
            }
        }
    }

    Row {
        id: controlButtons
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        spacing: 0

        // 白天/黑夜切换按钮
        SystemTitleButton {
            id: themeButton
            label: Theme.isDark ? "\u2600" : "\u263E"
            onClicked: function(x, y) {
                const point = themeButton.mapToItem(root.targetWindow.contentItem, x, y)
                root.themeToggleRequested(point.x, point.y)
            }
        }

        // 最小化按钮
        SystemTitleButton {
            label: "\u2014"
            onClicked: function() {
                root.windowAgent ? root.windowAgent.minimize() : root.targetWindow.showMinimized()
            }
        }

        // 最大化/还原按钮
        SystemTitleButton {
            label: root.targetWindow.visibility === Window.Maximized ? "\u2750" : "\u2610"
            onClicked: function() {
                root.toggleMaximize()
            }
        }

        // 关闭按钮
        SystemTitleButton {
            label: "\u2715"
            hoverColor: Theme.titleButtonCloseHover
            pressedColor: Theme.titleButtonClosePressed
            hoverForegroundColor: "#FFFFFF"
            onClicked: function() {
                root.windowAgent ? root.windowAgent.closeWindow() : root.targetWindow.close()
            }
        }
    }
}
