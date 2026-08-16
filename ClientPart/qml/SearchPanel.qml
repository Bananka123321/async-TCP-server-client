import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import ClientModule

Rectangle {
    id: root
    color: "#202023"

    property bool isSearching: SearchViewModel.isSearching
    property var users: SearchViewModel.users

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            color: "#27272A"

            TextField {
                id: searchField
                anchors.fill: parent
                anchors.margins: 8
                placeholderText: "Поиск пользователей..."
                placeholderTextColor: "#71717A"
                color: "#F4F4F5"
                font.pixelSize: 15

                background: Rectangle {
                    color: "#18181B"
                    radius: 8
                    border.color: searchField.activeFocus ? "#3B82F6" : "#3F3F46"
                    border.width: 1
                }

                onTextChanged: {
                    searchTimer.restart();
                }

                Timer {
                    id: searchTimer
                    interval: 300
                    onTriggered: SearchViewModel.searchUser(searchField.text)
                }
            }
        }

        Rectangle {
            visible: root.isSearching
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            color: "#202023"

            Text {
                anchors.centerIn: parent
                text: "Поиск..."
                color: "#A1A1AA"
                font.pixelSize: 14
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: !root.isSearching && root.users.length > 0
            model: root.users
            clip: true

            delegate: Rectangle {
                width: parent.width
                height: 60
                color: mouseArea.containsMouse ? "#2C2C30" : "transparent"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 16
                    anchors.rightMargin: 16
                    spacing: 12

                    Rectangle {
                        Layout.preferredWidth: 40
                        Layout.preferredHeight: 40
                        radius: 20
                        color: "#10B981"

                        Text {
                            anchors.centerIn: parent
                            text: modelData.username.charAt(0).toUpperCase()
                            color: "#FFFFFF"
                            font.bold: true
                            font.pixelSize: 18
                        }
                    }

                    Text {
                        text: modelData.username
                        color: "#F4F4F5"
                        font.pixelSize: 16
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        console.log("Выбран пользователь:", modelData.username);
                    }
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

        Rectangle {
            visible: !root.isSearching && searchField.text.length > 0 && root.users.length === 0
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#202023"

            Text {
                anchors.centerIn: parent
                text: "Пользователи не найдены"
                color: "#71717A"
                font.pixelSize: 14
            }
        }
    }
}
