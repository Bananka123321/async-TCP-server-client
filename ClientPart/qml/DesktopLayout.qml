import QtQuick
import QtQuick.Layouts

Item {
    RowLayout {
        anchors.fill: parent
        spacing: 0

        ChatList {
            Layout.preferredWidth: 320
            Layout.fillHeight: true

            onChatClicked: function(chatId) {
                console.log("Выбран чат:", chatId)
            }
        }

        ChatView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}