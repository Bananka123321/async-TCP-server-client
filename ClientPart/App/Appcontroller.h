#pragma once

#include <QObject>
#include <QTimer>
#include <string>

#include "Router.h"
#include "AppState.h"
#include "Handler.h"
#include "TcpClient.h"

class AppController : public QObject {
    Q_OBJECT

public:
    AppController(Router* router, AppState* state, Handler* handler, TCPClient* client);
    ~AppController();

    Q_INVOKABLE void checkAndResumeSession();

public slots:
    void loginRequest(const std::string& login, const std::string& password);
    void registerUser(const std::string& login, const std::string& password);
    void sendMessage(const Message& msg);
    void searchUser(const std::string& text);
    void loadHistory(int64_t dialog_id, int64_t last_msg_id);

    void startPing();
    void stopPing();
    void startReconnect();

signals:
    void ping();

    void resumeSessionFinished(bool success);

private:
    TCPClient* client_;
    Handler* handler_;
    Router* router_;
    AppState* state_;

    QTimer* pingTimer = nullptr;
    QTimer* reconnectTimer = nullptr;
    int reconnectAttempts = 0;

    static constexpr int64_t PING_TIME_MS = 30000;
    static constexpr int32_t MAX_RECONNECT_TIME_MS = 30000;
};