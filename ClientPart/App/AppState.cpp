#include "AppState.h"

AppState::AppState(QObject* parent) : QObject(parent), settings_("IvanMessenger", "ClientConfig") {
    sessionToken_ = settings_.value("sessionToken", "").toString();
    user_.user_id = settings_.value("userId", -1).toInt();
    user_.username = settings_.value("username", "").toString().toStdString();
}

void AppState::setUsers(const std::unordered_map<int, std::string>& newUsers) {
    users_ = newUsers;
    emit usersChanged(users_);
}

const std::unordered_map<int, std::string>& AppState::getUsers() const {
    return users_;
}

const std::string AppState::getCurrentUsername() const {
    return user_.username;
}

void AppState::setCurrentUsername(const std::string& login) {
    user_.username = login;
    emit usernameChanged();
}

const int AppState::getCurrentUserId() const {
    return user_.user_id;
}

void AppState::setCurrentUserId(const int user_id) {
    user_.user_id = user_id;
    emit userIdChanged();
}

const std::string AppState::getUsernameByUserId(const int id) const {
    auto it = users_.find(id);
    if (it != users_.end())
        return it->second;

    return "Unknown";
}

const QString AppState::getSessionToken() const {
    return sessionToken_;
}

void AppState::setSessionToken(const std::string& token) {
    sessionToken_ = QString::fromStdString(token);
    emit sessionTokenChanged();
}

const std::string AppState::getConnectionToken() const {
    return connectionToken_;
}

void AppState::setConnectionToken(const std::string& token) {
    connectionToken_ = token;
}

void AppState::saveSession(const QString& token, int id, const QString& name) {
    sessionToken_ = token;
    user_.user_id = id;
    user_.username = name.toStdString();

    settings_.setValue("sessionToken", token);
    settings_.setValue("userId", id);
    settings_.setValue("username", name);

    emit sessionTokenChanged();
    emit userIdChanged();
    emit usernameChanged();
}

void AppState::clearSession() {
    sessionToken_ = "";
    user_.user_id = -1;
    user_.username = "";

    settings_.remove("sessionToken");
    settings_.remove("userId");
    settings_.remove("username");

    emit sessionTokenChanged();
    emit userIdChanged();
    emit usernameChanged();
}

bool AppState::hasSession() const {
    return !sessionToken_.isEmpty() && user_.user_id != -1;
}