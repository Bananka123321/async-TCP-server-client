#pragma once

#include <string>
#include <openssl/ssl.h>
#include <atomic>
#include <chrono>

#include "../../common/PacketIO.h"

class ClientSession {
public:
    ClientSession(int sock, SSL* ssl_);
    ~ClientSession();

    ClientSession(const ClientSession&) = delete;
    ClientSession& operator=(const ClientSession&) = delete;

    [[nodiscard]] int getSocket() const;

    [[nodiscard]] const std::string& getUsername() const;
    [[nodiscard]] int getUserId() const;
    [[nodiscard]] bool getIsAuthenticated() const;
    [[nodiscard]] int64_t getLastActivity() const;
    [[nodiscard]] bool getConnected() const;

    void setUser(int new_id, const std::string& new_username);
    void setIsAuthenticated(bool value);
    void setLastActivity(int64_t newTimestamp);
    void setConnected(bool newState);

    [[nodiscard]] bool send(const std::string& message) const;
    bool receive(std::string& message) const;

private:
    int socket_;
    int user_id_;
    std::string username_;
    SSL* ssl_;
    std::atomic<bool> isAuthenticated_;
    std::atomic<int64_t> last_activity_time_{0};
    std::atomic<bool> connected_{false};
};