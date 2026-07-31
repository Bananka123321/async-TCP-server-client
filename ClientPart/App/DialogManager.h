#pragma once

#include <unordered_map>
#include <QObject>
#include <QMutex>
#include <string>

#include "Message.h"
#include "Handler.h"
#include "AppState.h"
#include "MetaDialog.h"

class DialogManager : public QObject{
    Q_OBJECT

public:
    DialogManager(Handler* handler, AppState* state);

    void start();

    void addMessage(int64_t dialog_id, const Message& msg);
    void setHistory(int64_t dialog_id, const std::vector<Message>& messages);
    const std::vector<Message>* getMessages(int64_t dialog_id) const;
    void prependHistory(int64_t dialog_id, const std::vector<Message>& messages);
    void dialogsLoaded(const std::vector<MetaDialog>& dialogs);
    std::vector<MetaDialog> getDialogs() const;

signals:
    void messagesUpdated(int64_t dialog_id);
    void historyLoaded(int64_t dialog_id, const std::vector<Message>& batch);
    void findUsers(const std::vector<User>& users);
    void dialogsUpdated();

private:
    mutable QMutex mtx_;

    struct DialogData {
        std::vector<Message> messages;
        MetaDialog meta;
    };

    std::unordered_map<int64_t, DialogData> data_;

    Handler* handler_;
    AppState* state_;

    void updateDialog(int64_t dialog_id);
    int64_t now() const;
};
