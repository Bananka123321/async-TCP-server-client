import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    Rectangle {
        anchors.fill: parent
        color: "#e0e0e0"

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20

            Label {
                text: "Welcome to Messenger!"
                font.pixelSize: 32
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: "This is the main screen"
                font.pixelSize: 18
                color: "#666"
                Layout.alignment: Qt.AlignHCenter
            }

            Label {
                text: "You will implement chat list here"
                font.pixelSize: 14
                color: "#999"
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}