#pragma once

#include <string>
#include <QObject>
#include <QSettings>
#include <unordered_map>

#include "User.h"

class AppState : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString sessionToken READ getSessionToken NOTIFY sessionTokenChanged FINAL)
    Q_PROPERTY(std::string username READ getCurrentUsername NOTIFY usernameChanged FINAL)
    Q_PROPERTY(int userId READ getCurrentUserId NOTIFY userIdChanged FINAL)

public:
    explicit AppState(QObject* parent = nullptr);

    const std::unordered_map<int, std::string>& getUsers() const;
    void setUsers(const std::unordered_map<int, std::string>& newUsers);

    const std::string getCurrentUsername() const;
    void setCurrentUsername(const std::string& login);

    const int getCurrentUserId() const;
    void setCurrentUserId(const int user_id);

    const QString getSessionToken() const;
    void setSessionToken(const std::string& token);

    const std::string getConnectionToken() const;
    void setConnectionToken(const std::string& token);

    const std::string getUsernameByUserId(const int id) const;

    Q_INVOKABLE void saveSession(const QString& token, int id, const QString& username);
    Q_INVOKABLE void clearSession();
    Q_INVOKABLE bool hasSession() const;

signals:
    void usersChanged(const std::unordered_map<int, std::string>& users);
    void sessionTokenChanged();
    void usernameChanged();
    void userIdChanged();

private:
    User user_;
    std::unordered_map<int, std::string> users_;
    std::string connectionToken_;
    QString sessionToken_;
    QSettings settings_;
};
