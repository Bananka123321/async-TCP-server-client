#include "../include/SessionManager.h"
#include "../include/Logger.h"

void SessionManager::add(const std::shared_ptr<ClientSession>& client) {
    if (!client) {
        LOG_WARNING(SESSION, "Попытка добавить nullptr в менеджер сессий");
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    sessions_.push_back(client);

    LOG_INFO(SESSION, "Клиент добавлен в менеджер сессий. userId=", client->getUserId(), " socket_fd=", client->getSocket(), " всего_активных_сессий=", sessions_.size());
}

void SessionManager::remove(const std::shared_ptr<ClientSession>& client) {
    if (!client) {
        LOG_WARNING(SESSION, "Попытка удалить nullptr из менеджера сессий");
        return;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    const size_t before_size = sessions_.size();
    std::erase(sessions_, client);

    if (sessions_.size() < before_size) {
        LOG_INFO(SESSION, "Клиент удален из менеджера сессий. userId=", client->getUserId(),
                 " socket_fd=", client->getSocket(), " осталось_активных_сессий=", sessions_.size());
    } else {
        LOG_WARNING(SESSION, "Попытка удалить клиента, которого нет в списке. userId=", client->getUserId());
    }
}

std::shared_ptr<ClientSession> SessionManager::getByUserId(const int user_id) {
    std::lock_guard<std::mutex> lock(mutex_);

    for (const auto& s : sessions_) {
        if (s && s->getUserId() == user_id) {
            LOG_DEBUG(SESSION, "Найдена активная сессия. userId=", user_id);
            return s;
        }
    }

    LOG_DEBUG(SESSION, "Активная сессия не найдена. userId=", user_id);
    return nullptr;
}

std::vector<std::shared_ptr<ClientSession>> SessionManager::getAll() {
    std::lock_guard<std::mutex> lock(mutex_);

    LOG_DEBUG(SESSION, "Запрос списка всех сессий. Всего подключено=", sessions_.size());
    return sessions_;
}

void SessionManager::updateActivity(const std::shared_ptr<ClientSession>& client) {
    if (!client) return;

    const int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    client->setLastActivity(now);
}