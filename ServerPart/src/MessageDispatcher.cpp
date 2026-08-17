#include "../include/MessageDispatcher.h"
#include "../include/Logger.h"

MessageDispatcher::MessageDispatcher(SessionManager& sm) : sessionManager_(sm) {
    LOG_INFO(NETWORK, "Инициализация MessageDispatcher");
}

void MessageDispatcher::sendTo(const std::shared_ptr<ClientSession>& client, const std::string& msg) const {
    if (!client) {
        LOG_WARNING(NETWORK, "Попытка отправки сообщения нулевому указателю клиента (nullptr)");
        return;
    }

    if (bool success = client->send(msg); !success) {
        LOG_WARNING(NETWORK, "Сбой отправки сообщения клиенту. userId=", client->getUserId(), " socket_fd=", client->getSocket(), " размер_данных=", msg.size());

        if (onDisconnect) {
            LOG_INFO(NETWORK, "Инициировано отключение клиента из-за сбоя отправки. userId=", client->getUserId());
            onDisconnect(client);
        }
    } else {
        LOG_DEBUG(NETWORK, "Сообщение успешно отправлено. userId=", client->getUserId(), " размер_данных=", msg.size());
    }
}

void MessageDispatcher::broadcast(const std::string& msg) const {
    const auto clients = sessionManager_.getAll();

    LOG_DEBUG(NETWORK, "Начало широковещательной рассылки (broadcast). Получателей=", clients.size(), " размер_сообщения=", msg.size());

    int successCount = 0;
    int failCount = 0;

    for (auto& c : clients) {
        if (!c) continue;

        if (!c->send(msg)) {
            failCount++;
            LOG_WARNING(NETWORK, "Сбой отправки при broadcast. userId=", c->getUserId(), " socket_fd=", c->getSocket());

            if (onDisconnect) {
                onDisconnect(c);
            }
        } else {
            successCount++;
        }
    }

    if (failCount > 0) {
        LOG_WARNING(NETWORK, "Завершение broadcast. Успешно=", successCount, " Сбоев=", failCount);
    } else {
        LOG_DEBUG(NETWORK, "Завершение broadcast. Успешно доставлено всем: ", successCount);
    }
}