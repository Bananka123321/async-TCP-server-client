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

    property bool isCheckingSession: true
    property bool shouldShowLogin: false

    StackView {
        id: authStackView
        anchors.fill: parent

        initialItem: isCheckingSession ? loadingScreenComponent : (shouldShowLogin ? loginScreenComponent : mainLayoutComponent)
    }

    Component {
        id: loadingScreenComponent
        Rectangle {
            color: "#18181B"
            Text {
                anchors.centerIn: parent
                text: "Подключение..."
                color: "#A1A1AA"
                font.pixelSize: 18
            }
        }
    }

    Component {
        id: loginScreenComponent
        LoginScreen {
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

    Component {
        id: mainLayoutComponent
        Item {
            Loader {
                anchors.fill: parent
                source: root.width < 600 ? "MobileLayout.qml" : "DesktopLayout.qml"
            }
        }
    }

    function loadMainLayout() {
        isCheckingSession = false;
        shouldShowLogin = false;
        authStackView.clear();
        authStackView.push(mainLayoutComponent);
    }

    function showLogin() {
        isCheckingSession = false;
        shouldShowLogin = true;
        authStackView.clear();
        authStackView.push(loginScreenComponent);
    }

    Component.onCompleted: {
        appController.checkAndResumeSession();
    }

    Connections {
        target: appController
        function onResumeSession(success) {
            isCheckingSession = false;
            if (success) {
                loadMainLayout();
            } else {
                showLogin();
            }
        }
    }
}
