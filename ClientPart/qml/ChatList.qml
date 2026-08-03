import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {

    signal chatClicked(int chatId)

    color: "#202023"

    ListView {
        anchors.fill: parent

        delegate: Rectangle {
            color: mouseArea.containsMouse ? "#2C2C30" : "transparent"
            height: 70
            width: parent.width

            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: "#F4F4F5"
                font.pixelSize: 16
                text: "Имя чата " + index
                x: 16
            }
            MouseArea {
                id: mouseArea

                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    chatClicked(index);
                }
            }
        }
    }
}