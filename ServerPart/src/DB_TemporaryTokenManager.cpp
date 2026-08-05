#include "DB_TemporaryTokenManager.h"

DB_TemporaryTokenManager::DB_TemporaryTokenManager(const std::string& conn_str) : conn_(conn_str) {};

void DB_TemporaryTokenManager::createSession(int user_id, const std::string& token) {
    try {
        pqxx::work txn(conn_);
        txn.exec("INSERT INTO connect_tokens (user_id, token) VALUES($1, $2);", pqxx::params(user_id, token));
        txn.commit();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

bool DB_TemporaryTokenManager::isValid(const std::string& token) {
    try {
        pqxx::work txn(conn_);
        const auto result = txn.exec(
            "SELECT * FROM connect_tokens WHERE token = $1;", pqxx::params(token));

        if (result.empty()) return false;

        return true;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

std::optional<int> DB_TemporaryTokenManager::getUserIdByToken(const std::string& token) {
    try {
        pqxx::work txn(conn_);
        const auto res = txn.exec("SELECT user_id FROM connect_tokens WHERE token = $1;", pqxx::params(token));
        if(res.empty()) return std::nullopt;
        return res[0]["user_id"].as<int>();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return std::nullopt;
    }
}

void DB_TemporaryTokenManager::deleteSession(const std::string& token) {
    try {
        pqxx::work txn(conn_);
        txn.exec("DELETE FROM connect_tokens WHERE token = $1;", pqxx::params(token));
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "DB Error deleting session: " << e.what() << '\n';
    }
}