import QtQuick
import QtQuick.Window
import MarsQuickTools

Rectangle {
    id: root

    required property Window targetWindow
    property var windowAgent: null
    property string titleText: ""
    readonly property int dragRegionRight: controlButtons.x

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

        // 最小化按钮
        SystemTitleButton {
            label: "\u2014"
            onClicked: root.windowAgent ? root.windowAgent.minimize() : root.targetWindow.showMinimized()
        }

        // 最大化/还原按钮
        SystemTitleButton {
            label: root.targetWindow.visibility === Window.Maximized ? "\u2750" : "\u2610"
            onClicked: root.toggleMaximize()
        }

        // 关闭按钮
        SystemTitleButton {
            label: "\u2715"
            hoverColor: Theme.titleButtonCloseHover
            pressedColor: Theme.titleButtonClosePressed
            hoverForegroundColor: "#FFFFFF"
            onClicked: root.windowAgent ? root.windowAgent.closeWindow() : root.targetWindow.close()
        }
    }
}
