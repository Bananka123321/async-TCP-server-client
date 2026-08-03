import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    color: "#18181B"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#27272A"

            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: "#F4F4F5"
                font.bold: true
                font.pixelSize: 18
                text: "Текущий чат"
                x: 16
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "#18181B"

            Text {
                anchors.centerIn: parent
                color: "#71717A"
                text: "Здесь будут сообщения"
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#27272A"

            TextField {
                anchors.fill: parent
                anchors.margins: 10
                color: "#F4F4F5"
                placeholderText: "Написать сообщение..."
                placeholderTextColor: "#71717A"

                background: Rectangle {
                    border.color: "#3F3F46"
                    color: "#18181B"
                    radius: 8
                }
            }
        }
    }
}