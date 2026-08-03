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

    StackView {
        id: authStackView
        anchors.fill: parent

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
        authStackView.clear()
        authStackView.push(root.width < 600 ? "MobileLayout.qml" : "DesktopLayout.qml")
    }
}