#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <optional>

#include "../common/Message.h"
#include "nlohmann/json.hpp"

class DB_MessageManager {
public:
    explicit DB_MessageManager(const std::string& conn_str);

    std::optional<int64_t> saveMessage(const Message& message);
    std::vector<Message> getHistory(int dialog_id, int before_id, int limit);
    
private:
    pqxx::connection conn_;

    static std::string serializePayload(const MessagePayload& payload);
    static MessagePayload deserializePayload(const std::string& json, MessageType type);
};