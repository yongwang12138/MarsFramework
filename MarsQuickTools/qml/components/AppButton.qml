import QtQuick
import QtQuick.Controls
import MarsQuickTools

Rectangle {
    id: root
    property alias text: label.text
    signal clicked

    radius: Theme.radiusM
    color: mouseArea.pressed ? "#0E5FC1" : Theme.primary
    implicitWidth: 120
    implicitHeight: 40

    Text {
        id: label
        anchors.centerIn: parent
        color: "white"
        font.pixelSize: 14
        font.bold: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
        cursorShape: Qt.PointingHandCursor
    }
}
