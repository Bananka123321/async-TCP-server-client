#include "DB_DevicesSessionManager.h"

#include <random>
#include "iostream"

DB_DevicesSessionManager::DB_DevicesSessionManager(const std::string& conn_str) : conn_(conn_str) {};

std::optional<std::string> DB_DevicesSessionManager::createToken(const int user_id, const std::string &device_info, const std::string &ip_address) {
    try {
        std::lock_guard<std::mutex> lock(rng_mutex);

        std::mt19937 gen(rd());
        std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);
        std::stringstream ss;
        for(int i = 0; i < 8; i++) {
            ss << std::hex << std::setw(8) << std::setfill('0') << dis(gen);
        }

        pqxx::work txn(conn_);
        txn.exec(
            "INSERT INTO user_sessions (user_id, token, device_info, ip_address) "
            "VALUES ($1, $2, $3, $4);",
            pqxx::params(user_id, ss.str(), device_info, ip_address));
        txn.commit();

        return ss.str();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return std::nullopt;
    }
}

bool DB_DevicesSessionManager::isValid(const std::string& token) {
    try {
        pqxx::work txn(conn_);
        const auto result = txn.exec(
            "SELECT * FROM user_sessions WHERE token = $1 AND "
            "expires_at > CURRENT_TIMESTAMP;"
            , pqxx::params(token));

        if (!result.empty()) return true;

        const auto check = txn.exec(
            "SELECT 1 FROM user_sessions WHERE token = $1",
            pqxx::params(token));

        if (!check.empty()) {
            txn.exec("DELETE FROM user_sessions WHERE token = $1", pqxx::params(token));
            txn.commit();
        }

        return false;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void DB_DevicesSessionManager::updateActivity(const std::string& token) {
    try {
        pqxx::work txn(conn_);
        txn.exec("UPDATE user_sessions SET last_activity = CURRENT_TIMESTAMP "
                 "WHERE token = $1;",
                 pqxx::params(token));
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Failed to update activity: " << e.what() << std::endl;
    }
}

void DB_DevicesSessionManager::deleteSession(const std::string &token) {
    try {
        pqxx::work txn(conn_);
        txn.exec("DELETE FROM user_sessions WHERE token = $1;", pqxx::params(token));
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Failed to delete session: " << e.what() << std::endl;
    }
}

void DB_DevicesSessionManager::deleteAllSessions(int user_id, const std::string &tokenSafe) {
    try {
        pqxx::work txn(conn_);
        txn.exec("DELETE FROM user_sessions WHERE user_id = $1 AND token != $2;",
            pqxx::params(user_id, tokenSafe));
        txn.commit();
    } catch (const std::exception& e) {
        std::cerr << "Failed to close all sessions:" << e.what() << std::endl;
    }
}

std::optional<int> DB_DevicesSessionManager::getUserIdByToken(const std::string& token) {
    try {
        pqxx::work txn(conn_);
        const auto result = txn.exec("SELECT user_id FROM user_sessions WHERE token = $1 "
                                     "AND expires_at > CURRENT_TIMESTAMP;",
            pqxx::params(token));
        if (result.empty()) return std::nullopt;
        return result[0]["user_id"].as<int>();
    } catch (const std::exception& e) {
        std::cerr << "Failed to get userId: " << e.what() << std::endl;
        return std::nullopt;
    }
}