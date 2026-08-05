#include "AppState.h"

AppState::AppState() {}

void AppState::setUsers(const std::unordered_map<int, std::string>& newUsers) {
    users = newUsers;
    emit usersChanged(users);
}

const std::unordered_map<int, std::string>& AppState::getUsers() const {
    return users;
}

const std::string& AppState::getCurrentUsername() const {
    return user.username;
}

void AppState::setCurrentUsername(const std::string& login) {
    user.username = login;
}

const int AppState::getCurrentUserId() const {
    return user.user_id;
}

void AppState::setCurrentUserId(const int user_id) {
    user.user_id = user_id;
}

const std::string AppState::getUsernameByUserId(const int id) const {
    auto it = users.find(id);
    if (it != users.end())
        return it->second;

    return "Unknown";
}

const std::string AppState::getSessionToken() const {
    return sessionToken_;
}

void AppState::setSessionToken(const std::string& token) {
    sessionToken_ = token;
}

const std::string AppState::getConnectionToken() const {
    return connectionToken_;
}

void AppState::setConnectionToken(const std::string& token) {
    connectionToken_ = token;
}