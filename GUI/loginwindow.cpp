#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <iostream>

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    connect(ui->okButton, &QPushButton::clicked, this, [this]() {
        if(!ui->loginField->text().isEmpty())
            accept();
    });

    connect(ui->cancelButton, &QPushButton::clicked, this, [this]() {
        reject();
    });
}

LoginWindow::~LoginWindow() {
    delete ui;
}

std::string LoginWindow::getLogin() const {
    return ui->loginField->text().toStdString();
}
