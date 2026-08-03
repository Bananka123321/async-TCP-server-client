import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#202023"

    signal chatClicked(int chatId, string chatName)

    // Демо-модель (без id, используем только данные)
    ListModel {
        id: chatModel
        ListElement {
            chatId: 1
            name: "Павел Дуров"
            lastMsg: "Верни стену"
            time: "12:00"
        }
        ListElement {
            chatId: 2
            name: "Илон Маск"
            lastMsg: "To the Moon! 🚀"
            time: "11:45"
        }
        ListElement {
            chatId: 3
            name: "Рабочий чат"
            lastMsg: "Где отчет?"
            time: "Вчера"
        }
        ListElement {
            chatId: 4
            name: "Мама"
            lastMsg: "Купи хлеба"
            time: "Вчера"
        }
    }

    ListView {
        anchors.fill: parent
        model: chatModel
        clip: true

        delegate: Rectangle {
            width: parent.width
            height: 70
            color: mouseArea.containsMouse || mouseArea.pressed ? "#2C2C30" : "transparent"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 12

                // Аватарка
                Rectangle {
                    Layout.preferredWidth: 48
                    Layout.preferredHeight: 48
                    radius: 24
                    color: "#3B82F6"
                    Layout.alignment: Qt.AlignVCenter

                    Text {
                        anchors.centerIn: parent
                        text: name.toString().charAt(0)
                        color: "#FFFFFF"
                        font.bold: true
                        font.pixelSize: 20
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    spacing: 4

                    Text {
                        text: name
                        color: "#F4F4F5"
                        font.bold: true
                        font.pixelSize: 16
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    Text {
                        text: lastMsg
                        color: "#A1A1AA"
                        font.pixelSize: 14
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }

                Text {
                    text: time
                    color: "#71717A"
                    font.pixelSize: 12
                    Layout.alignment: Qt.AlignTop | Qt.AlignRight
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: root.chatClicked(model.chatId, model.name)
            }

            Rectangle {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: "#2C2C30"
            }
        }
    }
}
