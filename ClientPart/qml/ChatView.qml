import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#18181B"

    // Свойства, которые мы будем передавать из Layout'ов
    property int currentChatId: 0
    property string currentChatName: "Выберите чат"
    property bool isMobile: false // Флаг, чтобы показать кнопку "Назад" только на телефоне

    signal backRequested

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // === ШАПКА ===
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#27272A"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: 16
                spacing: 12

                // Кнопка "Назад" (только для мобильных)
                Button {
                    visible: root.isMobile
                    text: "←"
                    font.pixelSize: 20
                    font.bold: true
                    background: Rectangle {
                        color: "transparent"
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#F4F4F5"
                        font: parent.font
                    }
                    onClicked: root.backRequested()
                    Layout.preferredWidth: 40
                }

                Text {
                    text: root.currentChatName
                    color: "#F4F4F5"
                    font.bold: true
                    font.pixelSize: 18
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
            }
        }

        // === ОБЛАСТЬ СООБЩЕНИЙ ===
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#18181B"

            ListView {
                id: messagesView
                anchors.fill: parent
                anchors.margins: 16
                spacing: 8
                model: ListModel {
                    id: msgModel
                } // Пока пусто, позже подключим C++
                verticalLayoutDirection: ListView.BottomToTop // Новые сообщения снизу

                delegate: Rectangle {
                    width: Math.min(messagesView.width * 0.7, implicitWidth + 32)
                    height: msgText.implicitHeight + 24
                    radius: 12
                    color: isMine ? "#3B82F6" : "#27272A"
                    anchors.right: isMine ? parent.right : undefined

                    Text {
                        id: msgText
                        text: messageText
                        color: "#F4F4F5"
                        font.pixelSize: 15
                        wrapMode: Text.Wrap
                        anchors.fill: parent
                        anchors.margins: 12
                    }
                }
            }
        }

        // === ПОЛЕ ВВОДА ===
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 70
            color: "#27272A"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                TextField {
                    id: messageInput
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    placeholderText: "Написать сообщение..."
                    placeholderTextColor: "#71717A"
                    color: "#F4F4F5"
                    font.pixelSize: 15

                    background: Rectangle {
                        color: "#18181B"
                        radius: 20
                        border.color: messageInput.activeFocus ? "#3B82F6" : "#3F3F46"
                        border.width: 1
                    }

                    Keys.onReturnPressed: sendButton.clicked()
                }

                Button {
                    id: sendButton
                    text: "➤"
                    font.pixelSize: 20
                    Layout.preferredWidth: 50
                    Layout.preferredHeight: 50

                    background: Rectangle {
                        color: parent.pressed ? "#1D4ED8" : "#3B82F6"
                        radius: 25
                    }
                    contentItem: Text {
                        text: parent.text
                        color: "#FFFFFF"
                        font: parent.font
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: {
                        if (messageInput.text.trim() !== "") {
                            msgModel.insert(0, {
                                messageText: messageInput.text,
                                isMine: true
                            });
                            messageInput.text = "";
                            // TODO: Здесь будет вызов C++ для отправки
                        }
                    }
                }
            }
        }
    }
}
