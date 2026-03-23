#include "mainwindow.h"
#include "loginwindow.h"
#include "tcp_client.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    LoginWindow loginW;

    if(loginW.exec() == QDialog::Accepted) {
        MainWindow w;
        w.setLogin(loginW.getLogin());

        w.show();

        return a.exec();
    }

    return 0;
}
