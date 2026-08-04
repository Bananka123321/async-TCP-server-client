#pragma once

#include <QObject>
#include <QList>
#include <QString>

#include "User.h"
#include "Router.h"

class Router;

class SearchViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QVariantMap> users READ getUsers NOTIFY usersChanged)
    Q_PROPERTY(bool isSearching READ isSearching NOTIFY isSearchingChanged)

public:
    explicit SearchViewModel(Router* router, QObject* parent = nullptr);

    QList<QVariantMap> getUsers() const;
    bool isSearching() const;

public slots:
    void searchUser(const QString& username);
    void onUserSearchResults(const std::vector<User>& users);

signals:
    void usersChanged();
    void isSearchingChanged();

private:
    Router* router_;
    QList<QVariantMap> users_;
    bool isSearching_ = false;
};