import QtQuick
import MarsQuickTools

Rectangle {
    id: root

    property string label: ""
    property color normalColor: "transparent"
    property color hoverColor: Theme.titleButtonHover
    property color pressedColor: Theme.titleButtonPressed
    property color foregroundColor: Theme.titleBarText
    property color hoverForegroundColor: foregroundColor

    signal clicked

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
        onClicked: root.clicked()
    }
}
