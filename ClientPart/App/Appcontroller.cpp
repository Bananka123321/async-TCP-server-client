#define NOMINMAX
#include "Appcontroller.h"

AppController::AppController(Router* router, AppState* state, Handler* handler, TCPClient* client)
    : router_(router), state_(state), handler_(handler), client_(client) {

    connect(handler_, &Handler::S_loginSuccess, this, [this](const std::string& login, const int user_id, const std::string& connectionToken, const std::string& sessionToken){
        router_->setReconnecting(false);
        state_->saveSession(QString::fromStdString(sessionToken), user_id, QString::fromStdString(login));
        state_->setConnectionToken(connectionToken);
    });

    connect(handler, &Handler::S_registerSuccess, this, [this](const std::string& login, const int user_id, const std::string& connectionToken, const std::string& sessionToken){
        router_->setReconnecting(false);
        state_->saveSession(QString::fromStdString(sessionToken), user_id, QString::fromStdString(login));
        state_->setConnectionToken(connectionToken);
    });

    connect(client_, &TCPClient::connectionLose, this, [this](){
        stopPing();
        state_->setConnectionStatus(ConnectionState::Disconnected);
    });

    connect(client_, &TCPClient::connected, this, [this](){
        state_->setConnectionStatus(ConnectionState::Connected);

        if(pendingResumeSession_) {
            pendingResumeSession_ = false;
            router_->resumeSessionRequest(state_->getSessionToken().toStdString());
        } else if(!state_->getConnectionToken().empty()) {
            router_->resumeConnectionRequest(state_->getConnectionToken());
        }

        startPing();
    });

    connect(client_, &TCPClient::connecting, this, [this](){
        state_->setConnectionStatus(ConnectionState::Connecting);
    });

    connect(handler_, &Handler::S_ResumeConnectionSuccess, this, [this](){
        router_->setReconnecting(false);
        startPing();
    });

    connect(handler_, &Handler::S_ResumeSessionResponse, this, [this](const bool success, const std::string& token){
        if (success) {
            state_->setConnectionToken(token);
        }
        emit resumeSessionFinished(success);
    });
}

AppController::~AppController() {
    stopPing();
}

void AppController::loginRequest(const std::string& login, const std::string& password) {
    router_->loginRequest(login, password);
}

void AppController::registerUser(const std::string& login, const std::string& password) {
    router_->registerRequest(login, password);
}

void AppController::sendMessage(const Message& msg) {
    router_->sendMessage(msg);
}

void AppController::searchUser(const std::string& text) {
    router_->searchUser(text);
}

void AppController::loadHistory(int64_t dialog_id, int64_t last_msg_id) {
    router_->historyRequest(dialog_id, last_msg_id);
}

void AppController::checkAndResumeSession() {
    if (!state_->hasSession()) {
        emit resumeSessionFinished(false);
        return;
    }

    if(client_->isConnected()) {
        router_->resumeSessionRequest(state_->getSessionToken().toStdString());
    } else {
        pendingResumeSession_ = true;
    }
}

void AppController::startPing() {
    if(pingTimer) return;

    pingTimer = new QTimer(this);

    connect(pingTimer, &QTimer::timeout, this, [this](){
        router_->ping();
    });
    pingTimer->start(PING_TIME_MS);
}

void AppController::stopPing() {
    if(!pingTimer) return;
    pingTimer->stop();
    pingTimer->deleteLater();
    pingTimer = nullptr;
}