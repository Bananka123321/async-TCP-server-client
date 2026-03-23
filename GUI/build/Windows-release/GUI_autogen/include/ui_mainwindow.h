/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
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

class Ui_POPA
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QTextEdit *chatView;
    QHBoxLayout *horizontalLayout;
    QLineEdit *inputField;
    QPushButton *sendButton;
    QListWidget *listWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *POPA)
    {
        if (POPA->objectName().isEmpty())
            POPA->setObjectName("POPA");
        POPA->resize(880, 568);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(POPA->sizePolicy().hasHeightForWidth());
        POPA->setSizePolicy(sizePolicy);
        POPA->setMinimumSize(QSize(640, 500));
        POPA->setStyleSheet(QString::fromUtf8("QMainWindow {\n"
"    background-color: #0d0d0d;  /* \320\277\320\276\321\207\321\202\320\270 \321\207\320\265\321\200\320\275\321\213\320\271 \321\204\320\276\320\275 */\n"
"}\n"
"\n"
"QListWidget#listWidget {\n"
"    background-color: #111111;        /* \321\202\321\221\320\274\320\275\321\213\320\271 \321\204\320\276\320\275 */\n"
"    border: 1px solid #00ff00;        /* \320\267\320\265\320\273\321\221\320\275\320\260\321\217 \321\200\320\260\320\274\320\272\320\260 */\n"
"    color: #00ff00;                   /* \320\267\320\265\320\273\321\221\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    font-family: 'Courier New', monospace;\n"
"    font-size: 12pt;\n"
"    padding: 5px;\n"
"    border-radius: 6px;\n"
"}\n"
"\n"
"/* \321\215\320\273\320\265\320\274\320\265\320\275\321\202\321\213 \320\262\320\275\321\203\321\202\321\200\320\270 \321\201\320\277\320\270\321\201\320\272\320\260 */\n"
"QListWidget#listWidget::item {\n"
"    background-color: #111111;        /* \321\204\320\276"
                        "\320\275 \321\215\320\273\320\265\320\274\320\265\320\275\321\202\320\276\320\262 */\n"
"    color: #00ff00;                   /* \321\202\320\265\320\272\321\201\321\202 \321\215\320\273\320\265\320\274\320\265\320\275\321\202\320\276\320\262 */\n"
"    padding: 4px 8px;\n"
"}\n"
"\n"
"/* \321\215\320\273\320\265\320\274\320\265\320\275\321\202 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"QListWidget#listWidget::item:hover {\n"
"    background-color: #222222;        /* \321\207\321\203\321\202\321\214 \321\201\320\262\320\265\321\202\320\273\320\265\320\265 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"}\n"
"\n"
"/* \320\262\321\213\320\261\321\200\320\260\320\275\320\275\321\213\320\271 \321\215\320\273\320\265\320\274\320\265\320\275\321\202 */\n"
"QListWidget#listWidget::item:selected {\n"
"    background-color: #00aa00;        /* \320\267\320\265\320\273\320\265\320\275\321\213\320\271 \321"
                        "\204\320\276\320\275 \320\264\320\273\321\217 \320\262\321\213\320\264\320\265\320\273\320\265\320\275\320\275\320\276\320\263\320\276 */\n"
"    color: #0d0d0d;                   /* \321\202\320\265\320\272\321\201\321\202 \321\202\320\265\320\274\320\275\321\213\320\274 */\n"
"}\n"
"\n"
"QTextEdit#chatView {\n"
"    background-color: #111111;       /* \321\207\321\203\321\202\321\214 \321\201\320\262\320\265\321\202\320\273\320\265\320\265 \321\204\320\276\320\275 \320\264\320\273\321\217 \321\201\320\276\320\276\320\261\321\211\320\265\320\275\320\270\320\271 */\n"
"    border-radius: 6px;\n"
"    padding: 8px;\n"
"    color: #00ff00;                  /* \321\217\321\200\320\272\320\276-\320\267\320\265\320\273\321\221\320\275\321\213\320\271 \321\202\320\265\320\272\321\201\321\202 */\n"
"    font-family: 'Courier New', monospace;\n"
"    font-size: 12pt;\n"
"    border: 1px solid #00ff00;       /* \321\202\320\276\320\275\320\272\320\260\321\217 \320\267\320\265\320\273\320\265\320\275\320\260\321\217 \321"
                        "\200\320\260\320\274\320\272\320\260 */\n"
"}\n"
"\n"
"QLineEdit#inputField {\n"
"    background-color: #111111;\n"
"    border: 1px solid #00ff00;\n"
"    border-radius: 6px;\n"
"    padding: 6px;\n"
"    color: #00ff00;\n"
"    font-family: 'Courier New', monospace;\n"
"    font-size: 11pt;\n"
"}\n"
"\n"
"QPushButton#sendButton {\n"
"    background-color: #111111;\n"
"    border: 1px solid #00ff00;\n"
"    border-radius: 6px;\n"
"    color: #00ff00;\n"
"    font-family: 'Courier New', monospace;\n"
"    font-weight: bold;\n"
"    padding: 6px 12px;\n"
"}\n"
"\n"
"QPushButton#sendButton:hover {\n"
"    background-color: #222222;        /* \321\207\321\203\321\202\321\214 \321\201\320\262\320\265\321\202\320\273\320\265\320\265 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"}\n"
"\n"
"QPushButton#sendButton:pressed {\n"
"    background-color: #00aa00;        /* \320\267\320\265\320\273\320\265\320\275\321\213\320\271 \320\277\321\200\320\270 \320\275\320"
                        "\260\320\266\320\260\321\202\320\270\320\270 */\n"
"    color: #0d0d0d;\n"
"}"));
        centralwidget = new QWidget(POPA);
        centralwidget->setObjectName("centralwidget");
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setHorizontalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName("verticalLayout");
        chatView = new QTextEdit(centralwidget);
        chatView->setObjectName("chatView");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy1.setHorizontalStretch(5);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(chatView->sizePolicy().hasHeightForWidth());
        chatView->setSizePolicy(sizePolicy1);
        chatView->setReadOnly(true);

        verticalLayout->addWidget(chatView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(2);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout->setContentsMargins(2, -1, -1, -1);
        inputField = new QLineEdit(centralwidget);
        inputField->setObjectName("inputField");

        horizontalLayout->addWidget(inputField);

        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName("sendButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sendButton->sizePolicy().hasHeightForWidth());
        sendButton->setSizePolicy(sizePolicy2);

        horizontalLayout->addWidget(sendButton);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 1);

        verticalLayout->addLayout(horizontalLayout);


        gridLayout->addLayout(verticalLayout, 2, 1, 1, 1);

        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");

        gridLayout->addWidget(listWidget, 2, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        POPA->setCentralWidget(centralwidget);
        menubar = new QMenuBar(POPA);
        menubar->setObjectName("menubar");
        menubar->setEnabled(true);
        menubar->setGeometry(QRect(0, 0, 880, 23));
        POPA->setMenuBar(menubar);
        statusbar = new QStatusBar(POPA);
        statusbar->setObjectName("statusbar");
        statusbar->setEnabled(true);
        POPA->setStatusBar(statusbar);

        retranslateUi(POPA);

        QMetaObject::connectSlotsByName(POPA);
    } // setupUi

    void retranslateUi(QMainWindow *POPA)
    {
        POPA->setWindowTitle(QCoreApplication::translate("POPA", "MainWindow", nullptr));
        sendButton->setText(QCoreApplication::translate("POPA", "SEND", nullptr));
    } // retranslateUi

};

namespace Ui {
    class POPA: public Ui_POPA {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
