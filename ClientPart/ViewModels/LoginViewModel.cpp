#include "LoginViewModel.h"
#include "Logging.h"
#include "Validator.h"

LoginViewModel::LoginViewModel(Router* router, Handler* handler, AppController* appCtrl, QObject* parent)
    : QObject(parent), router_(router), handler_(handler), appController_(appCtrl)
{
    qCDebug(logAuth) << "Инициализация LoginViewModel";

    connect(handler_, &Handler::S_loginSuccess, this, [this]() {
        qCInfo(logAuth) << "Получен сигнал успешного входа от Handler";
        emit loginSuccess();
    });

    connect(handler_, &Handler::S_loginFailed, this, [this](const std::string& reason) {
        qCWarning(logAuth) << "Получен сигнал ошибки входа от Handler. Причина:" << QString::fromStdString(reason);
        emit loginFailed(QString::fromStdString(reason));
    });

    connect(handler_, &Handler::S_registerSuccess, this, [this]() {
        qCInfo(logAuth) << "Получен сигнал успешной регистрации от Handler";
        emit registerSuccess();
    });

    connect(handler_, &Handler::S_registerFailed, this, [this](const std::string& reason) {
        qCWarning(logAuth) << "Получен сигнал ошибки регистрации от Handler. Причина:" << QString::fromStdString(reason);
        emit registerFailed(QString::fromStdString(reason));
    });
}

void LoginViewModel::tryLogin(const QString& login, const QString& password) {
    qCInfo(logAuth) << "Попытка входа пользователя:" << login;

    if (auto err = Validator::username(login.toStdString())) {
        qCWarning(logAuth) << "Ошибка валидации логина:" << QString::fromStdString(*err);
        emit loginFailed(QString::fromStdString(*err));
        return;
    }
    if (auto err = Validator::password(password.toStdString())) {
        qCWarning(logAuth) << "Ошибка валидации пароля:" << QString::fromStdString(*err);
        emit loginFailed(QString::fromStdString(*err));
        return;
    }

    qCInfo(logAuth) << "Валидация пройдена. Отправка запроса на вход в Router";
    router_->loginRequest(login.toStdString(), password.toStdString());
}

void LoginViewModel::tryRegister(const QString& login, const QString& password, const QString& passwordRepeat) {
    qCInfo(logAuth) << "Попытка регистрации пользователя:" << login;

    if (password != passwordRepeat) {
        qCWarning(logAuth) << "Ошибка валидации: пароли не совпадают";
        emit registerFailed("Passwords do not match");
        return;
    }
    if (auto err = Validator::username(login.toStdString())) {
        qCWarning(logAuth) << "Ошибка валидации логина:" << QString::fromStdString(*err);
        emit registerFailed(QString::fromStdString(*err));
        return;
    }
    if (auto err = Validator::password(password.toStdString())) {
        qCWarning(logAuth) << "Ошибка валидации пароля:" << QString::fromStdString(*err);
        emit registerFailed(QString::fromStdString(*err));
        return;
    }

    qCInfo(logAuth) << "Валидация пройдена. Отправка запроса на регистрацию в Router";
    router_->registerRequest(login.toStdString(), password.toStdString());
}

void LoginViewModel::validateLoginForm(const QString& login, const QString& password) {
    bool loginOk = Validator::username(login.toStdString()) == std::nullopt;
    bool passwordOk = Validator::password(password.toStdString()) == std::nullopt;

    if (loginValid_ != loginOk || passwordValid_ != passwordOk) {
        loginValid_ = loginOk;
        passwordValid_ = passwordOk;

        qCDebug(logAuth) << "Изменение состояния валидации формы входа. Login:"
                         << (loginValid_ ? "OK" : "Error")
                         << "Password:" << (passwordValid_ ? "OK" : "Error");

        emit validationChanged();
    }
}

void LoginViewModel::validateRegisterForm(const QString& login, const QString& password, const QString& passwordRepeat) {
    bool loginOk = Validator::username(login.toStdString()) == std::nullopt;
    bool passwordOk = Validator::password(password.toStdString()) == std::nullopt;
    bool repeatOk = Validator::password(passwordRepeat.toStdString()) == std::nullopt;
    bool match = (password == passwordRepeat);

    if (registerLoginValid_ != loginOk || registerPasswordValid_ != passwordOk ||
        registerPasswordRepeatValid_ != repeatOk || passwordsMatch_ != match) {

        registerLoginValid_ = loginOk;
        registerPasswordValid_ = passwordOk;
        registerPasswordRepeatValid_ = repeatOk;
        passwordsMatch_ = match;

        qCDebug(logAuth) << "Изменение состояния валидации формы регистрации. Login:"
                         << (registerLoginValid_ ? "OK" : "Error")
                         << "Pass:" << (registerPasswordValid_ ? "OK" : "Error")
                         << "Repeat:" << (registerPasswordRepeatValid_ ? "OK" : "Error")
                         << "Match:" << (passwordsMatch_ ? "OK" : "Error");

        emit validationChanged();
    }
}