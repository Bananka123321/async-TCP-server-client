#include "TcpClient.h"

#ifdef Q_OS_ANDROID
const QString IPADRESS_dev = "192.168.0.182";
#else
const QString IPADRESS_dev = "127.0.0.1";
#endif

const QString IPADRESS_dep = "31.192.108.37";

TCPClient::TCPClient(int port, Router* msgRouter) : port_(port), router_(msgRouter), socket_(nullptr), bConnected(false) {}

TCPClient::~TCPClient() {
    disconnect();
    if(reconnectTimer_) {
        reconnectTimer_->deleteLater();
    }
}

bool TCPClient::start() {
    if (socket_ && socket_->state() == QAbstractSocket::ConnectedState) {
        return true;
    }

    return setupSocket();
}

bool TCPClient::setupSocket() {
    if(socket_) {
        socket_->disconnectFromHost();
        socket_->deleteLater();
        socket_ = nullptr;
    }

    socket_ = new QSslSocket(this);

    connect(socket_, &QSslSocket::encrypted, this, &TCPClient::onEncrypted);
    connect(socket_, &QSslSocket::readyRead, this, &TCPClient::onReadyRead);
    connect(socket_, &QSslSocket::disconnected, this, &TCPClient::onDisconnected);
    connect(socket_, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors), this, &TCPClient::onSslErrors);

    connect(socket_, &QAbstractSocket::errorOccurred, this, [this](QAbstractSocket::SocketError error) {
        qWarning() << "Socket error:" << error << socket_->errorString() << '\n';
        bConnected.store(false);

        reconnect();
    });

    socket_->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket_->setProtocol(QSsl::TlsV1_2OrLater);

    socket_->connectToHostEncrypted(IPADRESS_dev, port_);
    emit connecting();

    return true;
}

void TCPClient::onEncrypted() {
    bConnected.store(true);
    router_->setSSL(socket_);

    reconnectDelay_ = 1000;

    emit connected();
}

void TCPClient::onReadyRead() {
    QByteArray newData = socket_->readAll();
    m_buffer.append(newData);

    while (auto packet = PacketIO_Client::extractPacket(m_buffer)) {
        if (onMessage) {
            onMessage(*packet);
        }
    }
}

void TCPClient::onDisconnected() {
    bConnected.store(false);

    router_->setSSL(nullptr);

    socket_->deleteLater();
    socket_ = nullptr;

    emit connectionLose();
}

void TCPClient::onSslErrors(const QList<QSslError> &errors) {
    for (const auto &error : errors) {
        qWarning() << "  -" << error.errorString();
    }
    socket_->ignoreSslErrors();
}

void TCPClient::disconnect() {
    if (!bConnected.load() || !socket_) {
        return;
    }

    router_->setSSL(nullptr);

    socket_->disconnectFromHost();

    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->waitForDisconnected(1000);
    }

    socket_->deleteLater();
    socket_ = nullptr;
    bConnected.store(false);
}

bool TCPClient::isConnected() const {
    bool result = bConnected.load() && socket_ && socket_->state() == QAbstractSocket::ConnectedState;
    return result;
}

void TCPClient::reconnect() {
    if(reconnectTimer_ && reconnectTimer_->isActive()) {
        return;
    }

    if(!reconnectTimer_) {
        reconnectTimer_ = new QTimer(this);
        reconnectTimer_->setSingleShot(true);

        connect(reconnectTimer_, &QTimer::timeout, this, &TCPClient::start);
    }

    reconnectTimer_->start(reconnectDelay_);
    reconnectDelay_ = std::min(reconnectDelay_ * 2, MAX_RECONNECT_TIME_MS);
}