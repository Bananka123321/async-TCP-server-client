#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcp_client.h"
#include "protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindows;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setLogin(const std::string& login);

private:
    Ui::MainWindows *ui;
    TCPClient* client;
    std::string login;
    std::string selectedUser;
};
#endif // MAINWINDOW_H
