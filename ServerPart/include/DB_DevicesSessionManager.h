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
    void insertToken(int user, const std::string& token, const std::string& device_info, const std::string& ip_address);
    void updateActivity(const std::string& token, const std::string& device_info, const std::string& ip_address);
    void deleteSession(const std::string& token);
    void deleteAllSessions(int user_id, const std::string& tokenSafe);


private:
    pqxx::connection conn_;
    std::mutex rng_mutex;
    std::random_device rd;
};