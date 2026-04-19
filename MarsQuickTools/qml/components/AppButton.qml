import QtQuick
import QtQuick.Controls
import MarsQuickTools

// AppButton：模块内通用主按钮
// 主要用于示例页和业务页面中的“主操作”按钮
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
