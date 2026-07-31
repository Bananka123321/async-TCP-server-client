#pragma once

#include <string>
#include <openssl/ssl.h>
#include <QObject>
#include <mutex>
#include <atomic>

#include "protocol.h"
#include "PacketIO.h"

class MessageRouter : public QObject{
    Q_OBJECT
public:
    MessageRouter();

    void loginRequest(const std::string& login, const std::string& password);
    void registerRequest(const std::string& login, const std::string& password);
    void sendMessage(const Message& msg);
    void searchUser(const std::string& text);
    void historyRequest(const int64_t dialog_id, const int64_t last_msg_id);
    void getDialogsRequest();
    void ping();
    void resumeConnectionRequest(const std::string& token);

    void setSSL(SSL* ssl);
    void setReconnecting(bool value);

private:
    SSL* ssl_;
    std::mutex mutex_;
    std::atomic<bool> isReconnecting_{false};

private:
    void sendPacket(const std::string& msg, bool force = false);
};
