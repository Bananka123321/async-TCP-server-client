import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 700
    minimumWidth: 320
    minimumHeight: 500
    title: "Ivan Messenger"
    color: "#18181B"

    StackView {
        id: authStackView
        anchors.fill: parent

        pushEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }

        pushExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }

        replaceEnter: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 0
                to: 1
                duration: 200
            }
        }

        replaceExit: Transition {
            PropertyAnimation {
                property: "opacity"
                from: 1
                to: 0
                duration: 200
            }
        }

        initialItem: LoginScreen {
            onLoginSucceeded: loadMainLayout()
            onSwitchToRegister: authStackView.push(registerScreenComponent)
        }
    }

    Component {
        id: registerScreenComponent
        RegisterScreen {
            onRegisterSucceeded: loadMainLayout()
            onSwitchToLogin: authStackView.pop()
        }
    }

    function loadMainLayout() {
        var layoutUrl = root.width < 600 ? "MobileLayout.qml" : "DesktopLayout.qml";

        authStackView.replace(authStackView.currentItem, layoutUrl);
    }
}
