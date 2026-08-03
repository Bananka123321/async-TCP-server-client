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

std::vector<MetaDialog_Client> DB_DialogManager::getUserDialogs(int user_id) {
    std::vector<MetaDialog_Client> result;
    try {
        pqxx::work txn(conn_);

        const auto rows = txn.exec(
            "SELECT "
            "    d.id AS dialog_id, "
            "    dp_peer.user_id AS peer_id, "
            "    u_peer.username AS peer_username, "
            "    dlm.last_msg_text, "
            "    EXTRACT(EPOCH FROM dlm.last_msg_timestamp) * 1000 AS last_msg_timestamp, "
            "    dlm.last_msg_sender_id, "
            "    CASE "
            "        WHEN dlm.last_msg_sender_id = $1 THEN 'Вы' "
            "        ELSE u_peer.username "
            "    END AS display_name, "
            "    (SELECT COUNT(*) FROM messages m "
            "     WHERE m.dialog_id = d.id AND m.id > dp.last_read_msg_id) AS unread_count "
            "FROM dialog_participants dp "
            "JOIN dialogs d ON dp.dialog_id = d.id "
            "JOIN dialog_participants dp_peer "
            "    ON d.id = dp_peer.dialog_id AND dp_peer.user_id != $1 "
            "JOIN users u_peer ON u_peer.id = dp_peer.user_id "
            "LEFT JOIN dialog_last_message dlm ON d.id = dlm.dialog_id "
            "WHERE dp.user_id = $1 "
            "ORDER BY d.updated_at DESC NULLS LAST",
            pqxx::params(user_id)
        );

        result.reserve(rows.size());

        for (const auto& row : rows) {
            MetaDialog_Client md;
            md.dialog_id = row["dialog_id"].as<int64_t>();
            md.peer_id = row["peer_id"].as<int>();
            md.username = row["peer_username"].as<std::string>();
            md.display_name = row["display_name"].as<std::string>();
            md.last_msg_preview = row["last_msg_text"].as<std::string>();
            md.last_msg_timestamp = row["last_msg_timestamp"].as<int64_t>();
            md.unread_count = row["unread_count"].as<int>();
            result.push_back(std::move(md));
        }
    } catch (const std::exception& e) {
        std::cerr << "DB error in getUserDialogs: " << e.what() << '\n';
    }
    return result;
}

std::vector<int> DB_DialogManager::getDialogParticipants(int64_t dialog_id) {
    std::vector<int> result;
    try {
        pqxx::work txn(conn_);
        const auto& rows = txn.exec(
            "SELECT user_id FROM dialog_participants"
            " WHERE dialog_id = $1",
            pqxx::params(dialog_id)
            );

        for (const auto& row : rows) {
            result.push_back(row["dialog_id"].as<int>());
        }
    } catch (const std::exception& e) {
        std::cerr << "DB error in getDialogParticipants: " << e.what() << '\n';
    }
    return result;
}