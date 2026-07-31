#include "../include/MessageDispatcher.h"

MessageDispatcher::MessageDispatcher(SessionManager& sm) : sessionManager_(sm) {}

void MessageDispatcher::sendTo(const std::shared_ptr<ClientSession>& client, const std::string& msg) const {
    if (!client->send(msg) && onDisconnect)
        onDisconnect(client);
}

void MessageDispatcher::broadcast(const std::string& msg) const {
    for (const auto clients = sessionManager_.getAll(); auto& c : clients) {
        if (!c->send(msg) && onDisconnect)
            onDisconnect(c);
    }
}
