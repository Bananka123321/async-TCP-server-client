#pragma once

#include <string>
#include <QObject>
#include <unordered_map>

#include "User.h"

class AppState : public QObject {
    Q_OBJECT
public:
    AppState();

    const std::unordered_map<int, std::string>& getUsers() const;
    void setUsers(const std::unordered_map<int, std::string>& newUsers);

    const std::string& getCurrentUsername() const;
    void setCurrentUsername(const std::string& login);

    const int getCurrentUserId() const;
    void setCurrentUserId(const int user_id);

    const std::string getSessionToken() const;
    void setSessionToken(const std::string& token);

    const std::string getConnectionToken() const;
    void setConnectionToken(const std::string& token);

    const std::string getUsernameByUserId(const int id) const;

    User user;

signals:
    void usersChanged(const std::unordered_map<int, std::string>& users);

private:
    std::unordered_map<int, std::string> users;
    std::string connectionToken_;
    std::string sessionToken_;
};
