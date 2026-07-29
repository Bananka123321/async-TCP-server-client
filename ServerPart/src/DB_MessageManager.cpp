#include "../include/DB_MessageManager.h"
#include <iostream>

DB_MessageManager::DB_MessageManager(const std::string& conn_str) : conn_(conn_str) {};

std::optional<int64_t> DB_MessageManager::saveMessage(const Message& message) {
    try {
        pqxx::work txn(conn_);

        std::string payload_json = serializePayload(message.payload);
        const auto result = txn.exec(
            "INSERT INTO messages (dialog_id, sender_id, type, created_at, payload)"
            " VALUES ($1, $2, $3, to_timestamp($4 / 1000.0), $5::jsonb) RETURNING id",
            pqxx::params(message.dialog_id, message.sender_id, static_cast<uint8_t>(message.type), message.created_at_ms, message.payload));

        txn.commit();
        return result[0][0].as<int>();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
};

std::vector<Message> DB_MessageManager::getHistory(int dialog_id, int before_id, int limit) {
    std::vector<Message> result;
    try {
        pqxx::work txn(conn_);
        const auto rows = txn.exec(
        "SELECT id, dialog_id, sender_id, type, "
        "EXTRACT(EPOCH FROM created_at) * 1000 as timestamp_ms, "
        "payload::text "
        "FROM messages "
        "WHERE dialog_id = $1 AND id < $2 "
        "ORDER BY id DESC LIMIT $3",
            pqxx::params(dialog_id, before_id, limit)
        );

        for (const auto& row : rows) {
            Message msg;
            msg.id = row["id"].as<int64_t>();
            msg.dialog_id = row["dialog_id"].as<int>();
            msg.sender_id = row["sender_id"].as<int>();
            msg.type = static_cast<MessageType>(row["type"].as<uint8_t>());
            msg.created_at_ms = row["timestamp_ms"].as<int64_t>();

            auto payload_json = row["payload"].as<std::string>();
            msg.payload = deserializePayload(payload_json, msg.type);

            result.push_back(std::move(msg));
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';

    }
    return result;
};

std::string DB_MessageManager::serializePayload(const MessagePayload& payload) {
    return std::visit([]<typename T0>(const T0& content) -> std::string {
        using T = std::decay_t<T0>;

        nlohmann::json j;

        if constexpr (std::is_same_v<T, TextContent>) {
            j["text"] = content.text;
        }
        else if constexpr (std::is_same_v<T, MediaContent>) {
            j["url"] = content.url;
            j["filename"] = content.filename;
            j["size_bytes"] = content.size_bytes;
            j["caption"] = content.caption;
        }
        else if constexpr (std::is_same_v<T, VoiceContent>) {
            j["url"] = content.url;
            j["duration_sec"] = content.duration_sec;
        }

        return j.dump();
    }, payload);
}

MessagePayload DB_MessageManager::deserializePayload(const std::string& json, const MessageType type) {
    auto j = nlohmann::json::parse(json);

    switch (type) {
        case MessageType::Text:
            return TextContent{
            .text = j["text"].get<std::string>()
        };

        case MessageType::Image:
        case MessageType::File:
            return MediaContent{
            .url = j["url"].get<std::string>(),
            .caption = j.value("caption", ""),
            .filename = j.value("filename", ""),
            .size_bytes = j.value("size_bytes", 0ULL)
        };

        case MessageType::Voice:
            return VoiceContent{
            .url = j["url"].get<std::string>(),
            .duration_sec = j["duration_sec"].get<uint32_t>()
        };

        default:
            throw std::runtime_error("Unknown message type");
    }
}