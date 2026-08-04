#include "SearchViewModel.h"
#include <QDebug>

SearchViewModel::SearchViewModel(Router* router, QObject* parent)
    : QObject(parent), router_(router), isSearching_(false) {
}

QList<QVariantMap> SearchViewModel::getUsers() const {
    return users_;
}

bool SearchViewModel::isSearching() const {
    return isSearching_;
}

void SearchViewModel::searchUser(const QString& username) {
    if (username.trimmed().isEmpty()) {
        users_.clear();
        emit usersChanged();
        return;
    }

    qDebug() << "[SearchViewModel] Searching for:" << username;
    isSearching_ = true;
    emit isSearchingChanged();

    router_->searchUser(username.toStdString());
}

void SearchViewModel::onUserSearchResults(const std::vector<User>& users) {
    qDebug() << "[SearchViewModel] Received" << users.size() << "users";

    users_.clear();
    for (const auto& user : users) {
        QVariantMap userMap;
        userMap["id"] = user.user_id;
        userMap["username"] = QString::fromStdString(user.username);
        users_.append(userMap);
    }

    isSearching_ = false;
    emit usersChanged();
    emit isSearchingChanged();
}