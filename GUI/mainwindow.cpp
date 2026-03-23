#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindows) {
    ui->setupUi(this);
    client = new TCPClient(5555);

    connect(ui->sendButton, &QPushButton::clicked, this, [this]() {
        QString text = ui->inputField->text();
        std::string msg;

        if (text.isEmpty()) return;

        if(!selectedUser.empty())
            msg = protocol::privateMessage(login, selectedUser, text.toStdString());
        else
            msg = protocol::broadcastMessage(login, text.toStdString());

        ui->chatView->append(text);
        client->sendMessage(msg);

        ui->inputField->clear();
    });

    client->onMessage = [this](const std::string& msg) {
        QMetaObject::invokeMethod(this, [this, msg]() {
            auto j = json::parse(msg);

            QString text;

            if (j["type"] == "userList") {
                ui->userWidget->clear();

                for (auto& user : j["users"])
                    ui->userWidget->addItem(QString::fromStdString(user));
                return;
            }

            if (j["type"] == "broadcastMessage")
                text = "[" + QString::fromStdString(j["from"]) + "] " + QString::fromStdString(j["text"]);

            else if (j["type"] == "privateMessage")
                text = "(private) [" + QString::fromStdString(j["from"]) + "] " + QString::fromStdString(j["text"]);

            ui->chatView->append(text);
        });
    };

    connect(ui->userWidget, &QListWidget::itemClicked, this, [this](QListWidgetItem* item) {
        selectedUser = item->text().toStdString();

        ui->user->setText(item->text());
    });
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setLogin(const std::string& l) {
    login = l;
    client->setLogin(login);
    std::thread([this]() { client->start(); }).detach();
}

