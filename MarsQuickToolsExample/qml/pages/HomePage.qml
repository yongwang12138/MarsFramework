import QtQuick
import QtQuick.Controls
import MarsQuickTools

Item {
    anchors.fill: parent

    Rectangle {
        anchors.centerIn: parent
        width: Math.min(parent.width * 0.8, 640)
        height: 260
        radius: Theme.radiusM
        color: Theme.card
        border.color: Theme.windowBorder

        Column {
            anchors.fill: parent
            anchors.margins: Theme.spacingM
            spacing: 12

            Text {
                text: AppController.appName
                font.pixelSize: 28
                font.bold: true
                color: Theme.textMain
            }

            Text {
                text: "Qt6 推荐结构：CMake + qt_add_qml_module + 组件化分层"
                color: Theme.textMuted
                wrapMode: Text.WordWrap
            }

            AppButton {
                text: "开始开发"
                onClicked: console.log("Hello from HomePage")
            }
        }
    }
}
