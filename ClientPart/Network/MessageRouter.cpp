#include "MessageRouter.h"

MessageRouter::MessageRouter() {}
\
void MessageRouter::setSSL(SSL* ssl) {
    std::lock_guard<std::mutex> lock(mutex_);
    ssl_ = ssl;
}

void MessageRouter::setReconnecting(bool value) {
    isReconnecting_.store(value);
}

void MessageRouter::loginRequest(const std::string& login, const std::string& password) {
    std::string request = protocol::loginRequest(login, password);
    sendPacket(request);
}

void MessageRouter::registerRequest(const std::string& login, const std::string& password) {
    std::string request = protocol::registerRequest(login, password);
    sendPacket(request);
}

void MessageRouter::sendMessage(const Message &msg) {
    std::string request = protocol::sendMessage(msg);
    sendPacket(request);
}

void MessageRouter::searchUser(const std::string& text) {
    std::string request = protocol::searchUserRequest(text);
    sendPacket(request);
}

void MessageRouter::historyRequest(const int64_t dialog_id, const int64_t last_msg_id) {
    std::string request = protocol::historyRequest(dialog_id, last_msg_id, 200);
    sendPacket(request);
}

void MessageRouter::getDialogsRequest() {
    std::string request = protocol::getDialogsRequest();
    sendPacket(request);
}

void MessageRouter::ping() {
    std::string request = protocol::ping();
    sendPacket(request);
}

void MessageRouter::resumeConnectionRequest(const std::string& token) {
    std::string request = protocol::resumeConnectionRequest(token);
    sendPacket(request, true);
}

void MessageRouter::sendPacket(const std::string& msg, bool force) {
    if(!force && isReconnecting_.load()) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    if(!force && isReconnecting_.load()) {
        return;
    }

    if(!ssl_) {
        return;
    }
    PacketIO::sendPacket(ssl_, msg);
}
