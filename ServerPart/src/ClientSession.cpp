#include "../include/ClientSession.h"
#include "../include/Logger.h"

ClientSession::ClientSession(const int sock, SSL* ssl)
    : ssl_(ssl), last_activity_time_(
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now().time_since_epoch()).count()), socket_(sock), user_id_(0),
      isAuthenticated_(false) {
    LOG_INFO(SESSION, "Создана новая сессия. socket_fd=", socket_);
}

ClientSession::~ClientSession() {
    LOG_INFO(SESSION, "Уничтожение сессии. userId=", user_id_, " username=", username_);

    if (ssl_) {
        SSL_shutdown(ssl_);
        SSL_free(ssl_);
        ssl_ = nullptr;
    }
}

int ClientSession::getSocket() const {
    return socket_;
}

const std::string& ClientSession::getUsername() const {
    return username_;
}

int ClientSession::getUserId() const {
    return user_id_;
}

bool ClientSession::send(const std::string& message) const {
    const bool result = PacketIO_Server::sendPacket(ssl_, message);
    if (!result) {
        LOG_WARNING(NETWORK, "Ошибка отправки данных клиенту. userId=", user_id_, " размер=", message.size());
    } else {
        LOG_DEBUG(NETWORK, "Отправлено данных клиенту. userId=", user_id_, " размер=", message.size());
    }
    return result;
}

bool ClientSession::receive(std::string& message) const {
    const bool result = PacketIO_Server::recvPacket(ssl_, message);
    if (!result) {
        LOG_WARNING(NETWORK, "Ошибка получения данных от клиента. userId=", user_id_);
    } else {
        LOG_DEBUG(NETWORK, "Получено данных от клиента. userId=", user_id_, " размер=", message.size());
    }
    return result;
}

void ClientSession::setUser(const int new_id, const std::string& new_username) {
    LOG_INFO(AUTH, "Авторизация сессии. socket_fd=", socket_, " userId=", new_id, " username=", new_username);

    username_ = new_username;
    user_id_ = new_id;
}

bool ClientSession::getIsAuthenticated() const {
    return isAuthenticated_;
}

void ClientSession::setIsAuthenticated(const bool value) {
    if (isAuthenticated_ != value) {
        LOG_INFO(AUTH, "Изменение статуса авторизации. userId=", user_id_, " новое значение=", (value ? "true" : "false"));
        isAuthenticated_ = value;
    }
}

int64_t ClientSession::getLastActivity() const {
    return last_activity_time_.load(std::memory_order_relaxed);
}

void ClientSession::setLastActivity(const int64_t newTimestamp) {
    last_activity_time_.store(newTimestamp);
}

bool ClientSession::getConnected() const {
    return connected_.load(std::memory_order_relaxed);
}

void ClientSession::setConnected(const bool newState) {
    if (const bool oldState = connected_.load(std::memory_order_relaxed); oldState != newState) {
        LOG_INFO(SESSION, "Изменение состояния подключения. userId=", user_id_, " новое состояние=", (newState ? "connected" : "disconnected"));
        connected_.store(newState);
    }
}

std::string ClientSession::getTempToken() const {
    return tempToken_;
}

void ClientSession::setTempToken(const std::string& newToken) {
    LOG_DEBUG(AUTH, "Установка временного токена. userId=", user_id_);
    tempToken_ = newToken;
}