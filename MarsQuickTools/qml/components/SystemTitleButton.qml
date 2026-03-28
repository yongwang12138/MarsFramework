import QtQuick
import MarsQuickTools

// SystemTitleButton：标题栏系统按钮基类。
// 最小化、最大化、关闭、亮暗切换按钮都复用该组件。
Rectangle {
    id: root

    property string label: ""
    property color normalColor: "transparent"
    property color hoverColor: Theme.titleButtonHover
    property color pressedColor: Theme.titleButtonPressed
    property color foregroundColor: Theme.titleBarText
    property color hoverForegroundColor: foregroundColor

    signal clicked(real x, real y)

    implicitWidth: 46
    implicitHeight: 32
    color: mouseArea.pressed ? pressedColor : (mouseArea.containsMouse ? hoverColor : normalColor)

    Text {
        anchors.centerIn: parent
        text: root.label
        color: mouseArea.containsMouse ? root.hoverForegroundColor : root.foregroundColor
        font.pixelSize: 13
        font.bold: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: function(mouse) {
            root.clicked(mouse.x, mouse.y)
        }
    }
}
