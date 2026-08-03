import QtQuick
import QtQuick.Controls

Item {
    StackView {
        id: mobileStack
        anchors.fill: parent

        initialItem: ChatList {
            onChatClicked: function(chatId) {
                console.log("Открываем чат:", chatId)
                mobileStack.push(chatViewComponent)
            }
        }
    }

    Component {
        id: chatViewComponent
        ChatView {
        }
    }
}