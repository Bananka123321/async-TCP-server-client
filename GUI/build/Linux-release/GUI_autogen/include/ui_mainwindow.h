/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindows
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *userChat;
    QLabel *user;
    QTextEdit *chatView;
    QHBoxLayout *horizontalLayout;
    QLineEdit *inputField;
    QPushButton *sendButton;
    QListWidget *userWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindows)
    {
        if (MainWindows->objectName().isEmpty())
            MainWindows->setObjectName(QString::fromUtf8("MainWindows"));
        MainWindows->resize(880, 568);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindows->sizePolicy().hasHeightForWidth());
        MainWindows->setSizePolicy(sizePolicy);
        MainWindows->setMinimumSize(QSize(640, 500));
        MainWindows->setCursor(QCursor(Qt::CrossCursor));
        MainWindows->setWindowTitle(QString::fromUtf8("IVAN-MESSANGER"));
        MainWindows->setWindowOpacity(1.000000000000000);
        MainWindows->setAutoFillBackground(false);
        MainWindows->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindows);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setHorizontalSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        userChat = new QLabel(centralwidget);
        userChat->setObjectName(QString::fromUtf8("userChat"));
        userChat->setEnabled(true);
        QFont font;
        font.setBold(true);
        userChat->setFont(font);
        userChat->setLayoutDirection(Qt::LeftToRight);
        userChat->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(userChat);

        user = new QLabel(centralwidget);
        user->setObjectName(QString::fromUtf8("user"));
        user->setFont(font);

        horizontalLayout_2->addWidget(user);


        verticalLayout->addLayout(horizontalLayout_2);

        chatView = new QTextEdit(centralwidget);
        chatView->setObjectName(QString::fromUtf8("chatView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(5);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(chatView->sizePolicy().hasHeightForWidth());
        chatView->setSizePolicy(sizePolicy1);
        chatView->viewport()->setProperty("cursor", QVariant(QCursor(Qt::CrossCursor)));
        chatView->setReadOnly(true);

        verticalLayout->addWidget(chatView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, -1, -1, -1);
        inputField = new QLineEdit(centralwidget);
        inputField->setObjectName(QString::fromUtf8("inputField"));

        horizontalLayout->addWidget(inputField);

        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sendButton->sizePolicy().hasHeightForWidth());
        sendButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(sendButton);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 2, 1, 1, 1);

        userWidget = new QListWidget(centralwidget);
        userWidget->setObjectName(QString::fromUtf8("userWidget"));

        gridLayout->addWidget(userWidget, 2, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindows->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindows);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setEnabled(true);
        menubar->setGeometry(QRect(0, 0, 880, 23));
        MainWindows->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindows);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setEnabled(true);
        MainWindows->setStatusBar(statusbar);

        retranslateUi(MainWindows);

        QMetaObject::connectSlotsByName(MainWindows);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindows)
    {
        userChat->setText(QCoreApplication::translate("MainWindows", "Chat with: ", nullptr));
        user->setText(QCoreApplication::translate("MainWindows", "ADMINUSERNAME", nullptr));
        sendButton->setText(QCoreApplication::translate("MainWindows", "SEND", nullptr));
        (void)MainWindows;
    } // retranslateUi

};

namespace Ui {
    class MainWindows: public Ui_MainWindows {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
