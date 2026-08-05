#pragma once

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <string>
#include <atomic>
#include <functional>

#include "Router.h"
#include "PacketIO_Client.h"

class TCPClient : public QObject {
    Q_OBJECT

public:
    TCPClient(int port, Router* msgRouter);
    ~TCPClient();

    bool start();
    void disconnect();
    bool isConnected() const;

    std::function<void(const std::string&)> onMessage;

signals:
    void connectionLose();
    void connected();

private slots:
    void onEncrypted();
    void onReadyRead();
    void onDisconnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    int port_;
    QSslSocket* socket_ = nullptr;
    Router* router_;

    QByteArray m_buffer;
    std::atomic<bool> bConnected{false};

    bool setupSocket();
};