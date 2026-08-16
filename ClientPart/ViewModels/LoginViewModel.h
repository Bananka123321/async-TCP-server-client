#pragma once
#include <QObject>
#include <QString>
#include <QtQml>

#include "Appcontroller.h"
#include "Router.h"
#include "Handler.h"

class LoginViewModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool loginValid READ loginValid NOTIFY validationChanged)
    Q_PROPERTY(bool passwordValid READ passwordValid NOTIFY validationChanged)
    Q_PROPERTY(bool registerLoginValid READ registerLoginValid NOTIFY validationChanged)
    Q_PROPERTY(bool registerPasswordValid READ registerPasswordValid NOTIFY validationChanged)
    Q_PROPERTY(bool registerPasswordRepeatValid READ registerPasswordRepeatValid NOTIFY validationChanged)
    Q_PROPERTY(bool passwordsMatch READ passwordsMatch NOTIFY validationChanged)

public:
    explicit LoginViewModel(Router* router, Handler* handler, AppController* appCtrl, QObject* parent);

    bool loginValid() const { return loginValid_; }
    bool passwordValid() const { return passwordValid_; }
    bool registerLoginValid() const { return registerLoginValid_; }
    bool registerPasswordValid() const { return registerPasswordValid_; }
    bool registerPasswordRepeatValid() const { return registerPasswordRepeatValid_; }
    bool passwordsMatch() const { return passwordsMatch_; }

signals:
    void validationChanged();

    void loginSuccess();
    void loginFailed(const QString& error);
    void registerSuccess();
    void registerFailed(const QString& error);

public slots:
    void tryLogin(const QString& login, const QString& password);
    void tryRegister(const QString& login, const QString& password, const QString& passwordRepeat);

    void validateLoginForm(const QString& login, const QString& password);
    void validateRegisterForm(const QString& login, const QString& password, const QString& passwordRepeat);

private:
    Router* router_;
    Handler* handler_;
    AppController* appController_;

    bool loginValid_ = false;
    bool passwordValid_ = false;
    bool registerLoginValid_ = false;
    bool registerPasswordValid_ = false;
    bool registerPasswordRepeatValid_ = false;
    bool passwordsMatch_ = false;
};