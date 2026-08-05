#pragma once
#include <string>
#include <optional>
#include <mutex>
#include <random>
#include "pqxx/pqxx"

class DB_DevicesSessionManager {
public:
    explicit DB_DevicesSessionManager(const std::string& conn_str);

    std::optional<std::string> createToken(int user_id, const std::string& device_info, const std::string& ip_address);
    bool isValid(const std::string& token);
    void updateActivity(const std::string& token);
    void deleteSession(const std::string& token);
    void deleteAllSessions(int user_id, const std::string& tokenSafe);
    std::optional<int> getUserIdByToken(const std::string& token);

private:
    pqxx::connection conn_;
    std::mutex rng_mutex;
    std::random_device rd;
};