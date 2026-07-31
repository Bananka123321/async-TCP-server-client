#pragma once

#include <QMainWindow>
#include <unordered_map>
#include <QListWidget>
#include <QCompleter>
#include <QStringListModel>
#include <QDateTime>

#include "Handler.h"
#include "AppState.h"
#include "DialogManager.h"
#include "Validator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindows;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent, AppState* state, DialogManager* manager);
    ~MainWindow();

signals:
    void sendMessageRequest(const Message& message);
    void searchUser(const std::string& text);
    void loadHistoryRequest(const int64_t dialog_id, const int64_t last_msg_id);

private:
    Ui::MainWindows *ui_;

    QCompleter* completer_;
    QStringListModel* model_;

    AppState* state_;
    DialogManager* manager_;

    int selectedDialogId_ = -1;

    std::unordered_map<std::string, int> searchMap_;

private:
    void refreshDialogs();
    void refreshCurrentChat();
    void openDialog(int id);

    void appendMessageToView(const Message& msg);
    void prependMessagesToView(const std::vector<Message>& messages);
    // void loadMoreHistory();

    QString formatTime(int64_t timestamp);
    QString buildMessageHtml(const Message& msg);
};
