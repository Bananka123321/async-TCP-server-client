import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: loginScreenRoot

    signal loginSucceeded()
    signal switchToRegister()

    Rectangle {
        anchors.fill: parent
        color: "#18181B"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 24

        Label {
            Layout.alignment: Qt.AlignHCenter
            font.bold: true
            font.pixelSize: 32
            color: "#F4F4F5"
            text: "Welcome Back"
        }

        TextField {
            id: loginField
            Layout.fillWidth: true
            placeholderText: "Username"
            placeholderTextColor: "#71717A"
            color: "#F4F4F5"
            font.pixelSize: 15

            background: Rectangle {
                color: "#18181B"
                radius: 12
                border.color: loginField.text === "" ? (loginField.activeFocus ? "#3B82F6" : "#3F3F46") : (loginViewModel.loginValid ? "#10B981" : "#EF4444")
                border.width: loginField.activeFocus ? 2 : 1
                Behavior on border.color { ColorAnimation { duration: 200 } }
            }

            Keys.onReturnPressed: loginButton.clicked()
            onTextChanged: loginViewModel.validateLoginForm(loginField.text, passwordField.text)
        }

        TextField {
            id: passwordField
            Layout.fillWidth: true
            echoMode: TextInput.Password
            placeholderText: "Password"
            placeholderTextColor: "#71717A"
            color: "#F4F4F5"
            font.pixelSize: 15

            background: Rectangle {
                color: "#18181B"
                radius: 12
                border.color: passwordField.text === "" ? (passwordField.activeFocus ? "#3B82F6" : "#3F3F46") : (loginViewModel.passwordValid ? "#10B981" : "#EF4444")
                border.width: passwordField.activeFocus ? 2 : 1
                Behavior on border.color { ColorAnimation { duration: 200 } }
            }

            Keys.onReturnPressed: loginButton.clicked()
            onTextChanged: loginViewModel.validateLoginForm(loginField.text, passwordField.text)
        }

        Button {
            id: loginButton
            Layout.fillWidth: true
            Layout.preferredHeight: 52
            text: "Sign In"

            background: Rectangle {
                color: loginButton.pressed ? "#1D4ED8" : (loginButton.hovered ? "#2563EB" : "#3B82F6")
                radius: 12
                Behavior on color { ColorAnimation { duration: 150 } }
            }

            contentItem: Text {
                color: "#FFFFFF"
                font.bold: true
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: loginButton.text
            }

            onClicked: loginViewModel.tryLogin(loginField.text, passwordField.text)
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8

            Label { color: "#A1A1AA"; font.pixelSize: 14; text: "Don't have an account?" }

            Label {
                id: registerLink
                color: "#60A5FA"
                font.bold: true
                font.pixelSize: 14
                text: "Create one"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onEntered: registerLink.color = "#93C5FD"
                    onExited: registerLink.color = "#60A5FA"
                    onClicked: loginScreenRoot.switchToRegister()
                }
            }
        }
    }

    Connections {
        target: loginViewModel
        function onLoginSuccess() { loginScreenRoot.loginSucceeded() }
        function onLoginFailed(error) { console.log("Login failed:", error) }
    }
}