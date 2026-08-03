#include "LoginViewModel.h"
#include "Validator.h"

LoginViewModel::LoginViewModel(MessageRouter* router, Handler* handler, AppController* appCtrl, QObject* parent)
    : QObject(parent), router_(router), handler_(handler), appController_(appCtrl) {
    connect(handler_, &Handler::S_loginSuccess, this, [this]() {
        emit loginSuccess();
    });

    connect(handler_, &Handler::S_loginFailed, this, [this](const std::string& reason) {
        emit loginFailed(QString::fromStdString(reason));
    });

    connect(handler_, &Handler::S_registerSuccess, this, [this]() {
        emit registerSuccess();
    });

    connect(handler_, &Handler::S_registerFailed, this, [this](const std::string& reason) {
        emit registerFailed(QString::fromStdString(reason));
    });
}

void LoginViewModel::tryLogin(const QString& login, const QString& password) {
    if (auto err = Validator::username(login.toStdString())) {
        emit loginFailed(QString::fromStdString(*err));
        return;
    }
    if (auto err = Validator::password(password.toStdString())) {
        emit loginFailed(QString::fromStdString(*err));
        return;
    }

    router_->loginRequest(login.toStdString(), password.toStdString());
}

void LoginViewModel::tryRegister(const QString& login, const QString& password, const QString& passwordRepeat) {
    if (password != passwordRepeat) {
        emit registerFailed("Passwords do not match");
        return;
    }
    if (auto err = Validator::username(login.toStdString())) {
        emit registerFailed(QString::fromStdString(*err));
        return;
    }
    if (auto err = Validator::password(password.toStdString())) {
        emit registerFailed(QString::fromStdString(*err));
        return;
    }

    router_->registerRequest(login.toStdString(), password.toStdString());
}

void LoginViewModel::validateLoginForm(const QString& login, const QString& password) {
    bool loginOk = Validator::username(login.toStdString()) == std::nullopt;
    bool passwordOk = Validator::password(password.toStdString()) == std::nullopt;

    if (m_loginValid != loginOk || m_passwordValid != passwordOk) {
        m_loginValid = loginOk;
        m_passwordValid = passwordOk;
        emit validationChanged();
    }
}

void LoginViewModel::validateRegisterForm(const QString& login, const QString& password, const QString& passwordRepeat) {
    bool loginOk = Validator::username(login.toStdString()) == std::nullopt;
    bool passwordOk = Validator::password(password.toStdString()) == std::nullopt;
    bool repeatOk = Validator::password(passwordRepeat.toStdString()) == std::nullopt;
    bool match = (password == passwordRepeat);

    if (m_registerLoginValid != loginOk || m_registerPasswordValid != passwordOk ||
        m_registerPasswordRepeatValid != repeatOk || m_passwordsMatch != match) {
        m_registerLoginValid = loginOk;
        m_registerPasswordValid = passwordOk;
        m_registerPasswordRepeatValid = repeatOk;
        m_passwordsMatch = match;
        emit validationChanged();
    }
}