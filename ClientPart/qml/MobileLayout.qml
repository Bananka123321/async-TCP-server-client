import QtQuick
import QtQuick.Controls

Item {
    StackView {
        id: mobileStack
        anchors.fill: parent

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 150
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 150
            }
        }

        initialItem: ChatList {
            onChatClicked: function (chatId, chatName) {
                if (mobileStack.depth === 1) {
                    mobileStack.push(chatViewComponent, {
                        currentChatId: chatId,
                        currentChatName: chatName,
                        isMobile: true
                    });
                } else {
                    mobileStack.replace(chatViewComponent, {
                        currentChatId: chatId,
                        currentChatName: chatName,
                        isMobile: true
                    });
                }
            }

            onUserSelected: function (userId, username) {
                if (appController) {
                    appController.getOrCreateDialog(userId);
                }
            }
        }
    }

    Component {
        id: chatViewComponent
        ChatView {
            onBackRequested: mobileStack.pop()
        }
    }
}
