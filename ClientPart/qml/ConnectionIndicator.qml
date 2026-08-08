pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls
import ClientModule

Item {
    id: root
    width: 40
    height: 40

    Rectangle {
        id: indicator
        width: 12
        height: 12
        radius: 6
        anchors.centerIn: parent
        color: {
            switch (appState.connectionStatus) {
            case 2:
                return "#22C55E";
            case 1:
                return "#EAB308";
            case 0:
                return "#EF4444";
            default:
                return "#6B7280";
            }
        }
        SequentialAnimation on scale {
            running: appState.connectionStatus === 1
            loops: Animation.Infinite
            NumberAnimation {
                to: 1.3
                duration: 500
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                to: 1.0
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }
    }

    ToolTip {
        visible: mouseArea.containsMouse
        text: {
            switch (appState.connectionStatus) {
            case 2:
                return "Подключено";
            case 1:
                return "Подключение...";
            case 0:
                return "Нет соединения";
            default:
                return "Неизвестно";
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}
