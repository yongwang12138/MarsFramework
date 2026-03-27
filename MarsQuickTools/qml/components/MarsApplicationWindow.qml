import QtQuick
import QtQuick.Controls
import QtQuick.Window
import MarsQuickTools

ApplicationWindow {
    id: root

    property string windowTitle: ""
    property Component page

    width: 1200
    height: 760
    minimumWidth: 800
    minimumHeight: 520
    visible: true
    title: root.windowTitle

    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinMaxButtonsHint | Qt.WindowCloseButtonHint
    color: "transparent"

    FramelessWindowAgent {
        id: framelessAgent
        titleBarHeight: titleBar.height
        dragRegionRight: titleBar.dragRegionRight
        resizeBorderThickness: 8
    }

    Component.onCompleted: framelessAgent.attach(root)

    Rectangle {
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

    WindowResizeHandles {
        targetWindow: root
        enabledResize: Qt.platform.os !== "windows"
    }
}
