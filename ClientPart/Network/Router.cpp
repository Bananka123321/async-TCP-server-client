#include "Router.h"
#include <QtEndian>
#include <QDebug>

Router::Router() : socket_(nullptr) {}

void Router::setSSL(QSslSocket* socket) {
    std::lock_guard<std::mutex> lock(mutex_);
    socket_ = socket;
}

void Router::setReconnecting(bool value) {
    isReconnecting_.store(value);
}

void Router::loginRequest(const std::string& login, const std::string& password) {
    std::string request = protocol::loginRequest(login, password);
    sendPacket(request);
}

void Router::registerRequest(const std::string& login, const std::string& password) {
    std::string request = protocol::registerRequest(login, password);
    sendPacket(request);
}

void Router::sendMessage(const Message &msg) {
    std::string request = protocol::sendMessage(msg);
    sendPacket(request);
}

void Router::searchUser(const std::string& text) {
    std::string request = protocol::searchUserRequest(text);
    sendPacket(request);
}

void Router::historyRequest(const int64_t dialog_id, const int64_t last_msg_id) {
    std::string request = protocol::historyRequest(dialog_id, last_msg_id, 200);
    sendPacket(request);
}

void Router::getDialogsRequest() {
    std::string request = protocol::getDialogsRequest();
    sendPacket(request);
}

void Router::ping() {
    std::string request = protocol::ping();
    sendPacket(request);
}

void Router::resumeConnectionRequest(const std::string& token) {
    std::string request = protocol::resumeConnectionRequest(token);
    sendPacket(request, true);
}

void Router::sendPacket(const std::string& msg, bool force) {
    if (!force && isReconnecting_.load()) {
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    if (!force && isReconnecting_.load()) {
        return;
    }

    if (!socket_) {
        return;
    }

    if (socket_->state() != QAbstractSocket::ConnectedState) {
        return;
    }

    QSslSocket* sslSocket = qobject_cast<QSslSocket*>(socket_);
    if (sslSocket) {
        if (!sslSocket->isEncrypted()) {
            return;
        }
    }

    try {
        uint32_t len = qToBigEndian(static_cast<uint32_t>(msg.size()));
        qint64 written1 = socket_->write(reinterpret_cast<const char*>(&len), sizeof(len));
        qint64 written2 = socket_->write(msg.data(), msg.size());

        if (written1 == -1 || written2 == -1) {
            qWarning() << "[MessageRouter] Write failed:" << socket_->errorString();
        }
    } catch (const std::exception& e) {
        qWarning() << "[MessageRouter] Exception during write:" << e.what();
    }
}