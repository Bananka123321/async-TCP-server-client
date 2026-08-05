#define NOMINMAX
#include "appcontroller.h"

AppController::AppController(Router* router, AppState* state, Handler* handler, TCPClient* client)
    : router_(router), state_(state), handler_(handler), client_(client) {

    connect(handler_, &Handler::S_loginSuccess, this, [this](const std::string&, int, const std::string&){
        router_->setReconnecting(false);
        startPing();
    });

    connect(client_, &TCPClient::connectionLose, this, [this](){
        stopPing();
        startReconnect();
    });

    connect(client_, &TCPClient::connected, this, [this](){
        if(!state_->getConnectionToken().empty()) {
            router_->resumeConnectionRequest(state_->getConnectionToken());
        }
    });

    connect(handler_, &Handler::S_ResumeConnectionSucess, this, [this](){
        router_->setReconnecting(false);
        startPing();
    });
}

AppController::~AppController() {
    stopPing();
    if (reconnectTimer) {
        reconnectTimer->stop();
        reconnectTimer->deleteLater();
    }
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

void AppController::startPing() {
    if(pingTimer) return;

    pingTimer = new QTimer(this);
    connect(pingTimer, &QTimer::timeout, this, [this](){
        emit ping();
    });
    pingTimer->start(PING_TIME_MS);
}

void AppController::stopPing() {
    if(!pingTimer) return;
    pingTimer->stop();
    pingTimer->deleteLater();
    pingTimer = nullptr;
}

void AppController::startReconnect() {
    router_->setReconnecting(true);

    if(reconnectTimer && reconnectTimer->isActive()) return;
    if(!reconnectTimer) {
        reconnectTimer = new QTimer(this);
        reconnectTimer->setSingleShot(true);

        connect(reconnectTimer, &QTimer::timeout, this, [this](){
            bool ok = client_->start();
            if(ok) {
                reconnectAttempts = 0;
            } else {
                reconnectAttempts++;
                if(reconnectAttempts <= 10) {
                    int delay = std::min(1000 * (1 << reconnectAttempts), MAX_RECONNECT_TIME_MS);
                    reconnectTimer->start(delay);
                } else {
                    router_->setReconnecting(false);
                }
            }
        });
    }

    reconnectTimer->start(1000);
}