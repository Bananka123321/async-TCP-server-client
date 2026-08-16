#include "TcpClient.h"
#include "App/Logging.h"

#ifdef Q_OS_ANDROID
const QString IPADRESS_dev = "192.168.0.182";
#else
const QString IPADRESS_dev = "127.0.0.1";
#endif

const QString IPADRESS_dep = "31.192.108.37";

TCPClient::TCPClient(int port, Router* msgRouter)
    : port_(port), router_(msgRouter), socket_(nullptr), bConnected(false), reconnectDelay_(1000) {}

TCPClient::~TCPClient() {
    qCDebug(logTcp) << "Уничтожение TCPClient";
    disconnect();
    if(reconnectTimer_) {
        reconnectTimer_->deleteLater();
    }
}

bool TCPClient::start() {
    if (socket_ && socket_->state() == QAbstractSocket::ConnectedState) {
        qCDebug(logTcp) << "start() вызван, но сокет уже подключен. Пропускаем.";
        return true;
    }

    qCInfo(logTcp) << "Инициализация подключения...";
    return setupSocket();
}

bool TCPClient::setupSocket() {
    qCInfo(logTcp) << "Настройка сокета. Цель:" << IPADRESS_dev << ":" << port_;

    if(socket_) {
        qCDebug(logTcp) << "Старый сокет существует, очищаем его";
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
        qCWarning(logTcp) << "Ошибка сокета:" << error << "|" << socket_->errorString();
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
    qCInfo(logTcp) << "TLS-соединение успешно установлено!";
    bConnected.store(true);
    router_->setSocket(socket_);

    reconnectDelay_ = 1000;

    emit connected();
}

void TCPClient::onReadyRead() {
    QByteArray newData = socket_->readAll();

    qCDebug(logTcp) << "Получено новых данных:" << newData.size() << "байт";

    m_buffer.append(newData);

    while (auto packet = PacketIO_Client::extractPacket(m_buffer)) {
        qCDebug(logTcp) << "Успешно извлечён пакет из буфера";
        if (onMessage) {
            onMessage(*packet);
        }
    }
}

void TCPClient::onDisconnected() {
    qCWarning(logTcp) << "Соединение разорвано. Причина: " << (socket_ ? socket_->errorString() : " сокет уже уничтожен");

    bConnected.store(false);
    router_->setSocket(nullptr);

    if(socket_) {
        socket_->deleteLater();
        socket_ = nullptr;
    }

    emit connectionLose();
}

void TCPClient::onSslErrors(const QList<QSslError> &errors) {
    qCWarning(logTcp) << "Обнаружены ошибки SSL при рукопожатии:";
    for (const auto &error : errors) {
        qCWarning(logTcp) << "   -" << error.errorString();
    }
    socket_->ignoreSslErrors();
}

void TCPClient::disconnect() {
    if (!bConnected.load() || !socket_) {
        qCDebug(logTcp) << "disconnect() вызван, но сокет уже неактивен. Ничего не делаем.";
        return;
    }

    qCInfo(logTcp) << "Принудительное отключение сокета";
    router_->setSocket(nullptr);

    socket_->disconnectFromHost();

    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->waitForDisconnected(1000);
    }

    if(socket_) {
        socket_->deleteLater();
        socket_ = nullptr;
    }

    bConnected.store(false);
}

bool TCPClient::isConnected() const {
    return bConnected.load() && socket_ && socket_->state() == QAbstractSocket::ConnectedState;
}

void TCPClient::reconnect() {
    if(reconnectTimer_ && reconnectTimer_->isActive()) {
        qCDebug(logTcp) << "Таймер переподключения уже активен, пропускаем вызов reconnect()";
        return;
    }

    if(!reconnectTimer_) {
        reconnectTimer_ = new QTimer(this);
        reconnectTimer_->setSingleShot(true);
        connect(reconnectTimer_, &QTimer::timeout, this, &TCPClient::start);
    }

    qCInfo(logTcp) << "Переподключение запланировано через" << reconnectDelay_ << "мс";

    reconnectTimer_->start(reconnectDelay_);

    reconnectDelay_ = std::min(reconnectDelay_ * 2, MAX_RECONNECT_TIME_MS);
}