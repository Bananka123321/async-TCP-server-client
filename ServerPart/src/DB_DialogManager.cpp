#include "../include/DB_DialogManager.h"

DB_DialogManager::DB_DialogManager(const std::string& conn_str) : conn_(conn_str) {}

void DB_DialogManager::upsertDialog(int user_id, int peer_id, int64_t msg_id, const std::string& preview, int64_t timestamp) {
    try {
        pqxx::work txn(conn_);

        txn.exec(
            "INSERT INTO dialogs (user_id, peer_id, last_msg_id, last_msg_text, last_msg_timestamp, updated_at) "
            "VALUES ($1, $2, $3, $4, to_timestamp($5 / 1000.0), NOW()) "
            "ON CONFLICT (user_id, peer_id) DO UPDATE SET "
            "last_msg_id = EXCLUDED.last_msg_id, "
            "last_msg_text = EXCLUDED.last_msg_text, "
            "last_msg_timestamp = EXCLUDED.last_msg_timestamp, "
            "updated_at = NOW()",
            pqxx::params(user_id, peer_id, msg_id, preview, timestamp)
        );

        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "DB error in upsertDialog: " << e.what() << '\n';
    }
}

void DB_DialogManager::updateLastMessage(const Message &msg) {
    try {
        pqxx::work txn(conn_);
        txn.exec(
            "INSERT INTO dialog_last_message (dialog_id, last_msg_id, last_msg_text, last_msg_sender_id, last_msg_timestamp)"
            " VALUES ($1, $2, $3, $4, to_timestamp($5 / 1000.0))"
            " ON CONFLICT (dialog_id) DO UPDATE SET"
            " last_msg_id = EXCLUDED.last_msg_id,"
            " last_msg_text = EXCLUDED.last_msg_text,"
            " last_msg_sender_id = EXCLUDED.last_msg_sender_id,"
            " last_msg_timestamp = EXCLUDED.last_msg_timestamp",
            pqxx::params(msg.dialog_id, msg.id, msg.getPreview(), msg.sender_id, msg.created_at_ms)
        );
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "DB error in updateLastMessage: " << e.what() << '\n';
    }
}

std::vector<MetaDialog> DB_DialogManager::getUserDialogs(int user_id) {
    std::vector<MetaDialog> result;
    try {
        pqxx::work txn(conn_);
        const auto rows = txn.exec(
            "SELECT d.peer_id, u.username, d.last_msg_text, d.last_msg_timestamp"
            " FROM dialogs d"
            " JOIN users u ON d.peer_id = u.id"
            " WHERE d.user_id = $1"
            " ORDER BY d.updated_at DESC",
            pqxx::params(user_id)
        );

        for (const auto& row : rows) {
            result.push_back({
                .peer_id = row["peer_id"].as<int>(),
                .username = row["username"].as<std::string>(),
                .last_msg_text = row["last_msg_text"].as<std::string>(),
                .last_msg_timestamp = row["last_msg_timestamp"].as<int64_t>()
            });
        }
    } catch(const std::exception& e) {
        std::cerr << "GET dialogs ERROR: " << e.what() << '\n';
    }
    return result;
}