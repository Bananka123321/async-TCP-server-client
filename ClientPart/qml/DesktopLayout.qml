import QtQuick
import QtQuick.Layouts

Item {
    RowLayout {
        anchors.fill: parent
        spacing: 0

        ChatList {
            Layout.preferredWidth: 320
            Layout.fillHeight: true

            onChatClicked: function (chatId, chatName) {
                console.log("Выбран чат:", chatName);
                chatView.currentChatId = chatId;
                chatView.currentChatName = chatName;
            }
        }

        ChatView {
            id: chatView
            Layout.fillWidth: true
            Layout.fillHeight: true
            isMobile: false
        }
    }
}
