#include "StateBinder.h"

StateChanger::StateChanger(Handler* handler, AppState* state) : handler_(handler), state_(state) {
    connect(handler, &Handler::S_loginSuccess, this, [this](const std::string& login, const int user_id, const std::string& connectionToken, const std::string& sessionToken){
        state_->saveSession(QString::fromStdString(sessionToken), user_id, QString::fromStdString(login));
        state_->setConnectionToken(connectionToken);
    });

    connect(handler, &Handler::S_registerSuccess, this, [this](const std::string& login, const int user_id, const std::string& connectionToken, const std::string& sessionToken){
        state_->saveSession(QString::fromStdString(sessionToken), user_id, QString::fromStdString(login));
        state_->setConnectionToken(connectionToken);
    });

    connect(handler, &Handler::S_ResumeSessionResponse, this, [this](const bool success, const std::string& token){
        if(success) state_->setConnectionToken(token);
    });
}

