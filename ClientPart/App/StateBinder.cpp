#include "StateBinder.h"

StateChanger::StateChanger(Handler* handler, AppState* state) : handler_(handler), state_(state) {
    connect(handler, &Handler::S_loginSuccess, this, [this](const std::string& login, const int user_id, const std::string& connectionToken, const std::string& sessionToken){
        state_->setCurrentUsername(login);
        state_->setCurrentUserId(user_id);
        state_->setSessionToken(sessionToken);
        state_->setConnectionToken(connectionToken);
    });

    connect(handler, &Handler::S_registerSuccess, this, [this](const std::string& login, const int user_id, const std::string& connectionToken, const std::string& sessionToken){
        state_->setCurrentUsername(login);
        state_->setCurrentUserId(user_id);
        state_->setSessionToken(sessionToken);
        state_->setConnectionToken(connectionToken);
    });

    connect(handler, &Handler::S_ResumeSessionSucess, this, [this](const std::string& connectionToken){
        state_->setConnectionToken(connectionToken);
    });
}

