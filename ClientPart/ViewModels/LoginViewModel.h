#pragma once
#include <QObject>
#include <QString>

#include "appcontroller.h"
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

    bool loginValid() const { return m_loginValid; }
    bool passwordValid() const { return m_passwordValid; }
    bool registerLoginValid() const { return m_registerLoginValid; }
    bool registerPasswordValid() const { return m_registerPasswordValid; }
    bool registerPasswordRepeatValid() const { return m_registerPasswordRepeatValid; }
    bool passwordsMatch() const { return m_passwordsMatch; }

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

    bool m_loginValid = false;
    bool m_passwordValid = false;
    bool m_registerLoginValid = false;
    bool m_registerPasswordValid = false;
    bool m_registerPasswordRepeatValid = false;
    bool m_passwordsMatch = false;
};