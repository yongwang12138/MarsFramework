import QtQuick
import QtQuick.Window

// WindowResizeHandles：
// 在非 Windows 平台补充边缘缩放热区。
// Windows 下由 FramelessWindowAgent 的原生命中测试负责缩放，一般会禁用本组件。
Item {
    id: root

    required property Window targetWindow
    property bool enabledResize: true
    property int resizeMargin: 6

    anchors.fill: parent
    visible: enabledResize
             && root.targetWindow.visibility !== Window.Maximized
             && root.targetWindow.visibility !== Window.FullScreen
    z: 999

    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: root.resizeMargin
        cursorShape: Qt.SizeHorCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.LeftEdge)
            }
        }
    }

    MouseArea {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: root.resizeMargin
        cursorShape: Qt.SizeHorCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.RightEdge)
            }
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: root.resizeMargin
        cursorShape: Qt.SizeVerCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.TopEdge)
            }
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: root.resizeMargin
        cursorShape: Qt.SizeVerCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.BottomEdge)
            }
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        width: root.resizeMargin
        height: root.resizeMargin
        cursorShape: Qt.SizeFDiagCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.LeftEdge | Qt.TopEdge)
            }
        }
    }

    MouseArea {
        anchors.right: parent.right
        anchors.top: parent.top
        width: root.resizeMargin
        height: root.resizeMargin
        cursorShape: Qt.SizeBDiagCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.RightEdge | Qt.TopEdge)
            }
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: root.resizeMargin
        height: root.resizeMargin
        cursorShape: Qt.SizeBDiagCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.LeftEdge | Qt.BottomEdge)
            }
        }
    }

    MouseArea {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        width: root.resizeMargin
        height: root.resizeMargin
        cursorShape: Qt.SizeFDiagCursor
        onPressed: function(mouse) {
            if (mouse.button === Qt.LeftButton) {
                root.targetWindow.startSystemResize(Qt.RightEdge | Qt.BottomEdge)
            }
        }
    }
}
