import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: registerScreenRoot

    signal registerSucceeded()
    signal switchToLogin()

    Rectangle {
        anchors.fill: parent
        color: "#18181B"
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 20

        Label {
            Layout.alignment: Qt.AlignHCenter
            font.bold: true
            font.pixelSize: 32
            color: "#F4F4F5"
            text: "Create Account"
        }

        TextField {
            id: regLoginField
            Layout.fillWidth: true
            placeholderText: "Username"
            placeholderTextColor: "#71717A"
            color: "#F4F4F5"
            font.pixelSize: 15

            background: Rectangle {
                color: "#18181B"
                radius: 12
                border.color: regLoginField.text === "" ? (regLoginField.activeFocus ? "#3B82F6" : "#3F3F46") : (loginViewModel.registerLoginValid ? "#10B981" : "#EF4444")
                border.width: regLoginField.activeFocus ? 2 : 1
            }
            onTextChanged: loginViewModel.validateRegisterForm(regLoginField.text, regPasswordField.text, regPasswordRepeatField.text)
        }

        TextField {
            id: regPasswordField
            Layout.fillWidth: true
            echoMode: TextInput.Password
            placeholderText: "Password"
            placeholderTextColor: "#71717A"
            color: "#F4F4F5"
            font.pixelSize: 15

            background: Rectangle {
                color: "#18181B"
                radius: 12
                border.color: regPasswordField.text === "" ? (regPasswordField.activeFocus ? "#3B82F6" : "#3F3F46") : (loginViewModel.registerPasswordValid ? "#10B981" : "#EF4444")
                border.width: regPasswordField.activeFocus ? 2 : 1
            }
            onTextChanged: loginViewModel.validateRegisterForm(regLoginField.text, regPasswordField.text, regPasswordRepeatField.text)
        }

        TextField {
            id: regPasswordRepeatField
            Layout.fillWidth: true
            echoMode: TextInput.Password
            placeholderText: "Repeat Password"
            placeholderTextColor: "#71717A"
            color: "#F4F4F5"
            font.pixelSize: 15

            background: Rectangle {
                color: "#18181B"
                radius: 12
                border.color: {
                    if (regPasswordRepeatField.text === "") return regPasswordRepeatField.activeFocus ? "#3B82F6" : "#3F3F46"
                    if (!loginViewModel.passwordsMatch) return "#EF4444"
                    return loginViewModel.registerPasswordRepeatValid ? "#10B981" : "#EF4444"
                }
                border.width: regPasswordRepeatField.activeFocus ? 2 : 1
            }
            Keys.onReturnPressed: registerButton.clicked()
            onTextChanged: loginViewModel.validateRegisterForm(regLoginField.text, regPasswordField.text, regPasswordRepeatField.text)
        }

        Button {
            id: registerButton
            Layout.fillWidth: true
            Layout.preferredHeight: 52
            text: "Sign Up"

            background: Rectangle {
                color: registerButton.pressed ? "#047857" : (registerButton.hovered ? "#059669" : "#10B981")
                radius: 12
                Behavior on color { ColorAnimation { duration: 150 } }
            }

            contentItem: Text {
                color: "#FFFFFF"
                font.bold: true
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: registerButton.text
            }

            onClicked: loginViewModel.tryRegister(regLoginField.text, regPasswordField.text, regPasswordRepeatField.text)
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 8

            Label { color: "#A1A1AA"; font.pixelSize: 14; text: "Already have an account?" }

            Label {
                id: loginLink
                color: "#60A5FA"
                font.bold: true
                font.pixelSize: 14
                text: "Sign In"

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true
                    onEntered: loginLink.color = "#93C5FD"
                    onExited: loginLink.color = "#60A5FA"
                    onClicked: registerScreenRoot.switchToLogin()
                }
            }
        }
    }

    Connections {
        target: loginViewModel
        function onRegisterSuccess() { registerScreenRoot.registerSucceeded() }
        function onRegisterFailed(error) { console.log("Register failed:", error) }
    }
}