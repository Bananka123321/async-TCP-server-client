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
}

bool TCPClient::start() {
    if (socket_ && socket_->state() == QAbstractSocket::ConnectedState) {
        return true;
    }

    return setupSocket();
}

bool TCPClient::setupSocket() {
    socket_ = new QSslSocket(this);

    connect(socket_, &QSslSocket::encrypted, this, &TCPClient::onEncrypted);
    connect(socket_, &QSslSocket::readyRead, this, &TCPClient::onReadyRead);
    connect(socket_, &QSslSocket::disconnected, this, &TCPClient::onDisconnected);
    connect(socket_, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors), this, &TCPClient::onSslErrors);

    socket_->setPeerVerifyMode(QSslSocket::VerifyNone);
    socket_->setProtocol(QSsl::TlsV1_2OrLater);

    socket_->connectToHostEncrypted(IPADRESS_dep, port_);

    if (!socket_->waitForConnected(5000)) {
        socket_->deleteLater();
        socket_ = nullptr;
        return false;
    }

    if (!socket_->waitForEncrypted(5000)) {
        socket_->deleteLater();
        socket_ = nullptr;
        return false;
    }

    return true;
}

void TCPClient::onEncrypted() {
    bConnected.store(true);
    router_->setSSL(socket_);

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