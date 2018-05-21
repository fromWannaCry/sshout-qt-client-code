/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_press_enter_to_send_message;
    QAction *action_settings;
    QAction *action_change_server;
    QAction *action_logout;
    QAction *action_send;
    QAction *action_show_online_users;
    QAction *action_use_html_for_sending_messages;
    QAction *action_send_image;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QTextEdit *chat_area;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *label_message_to;
    QTextEdit *textEdit_message_to_send;
    QMenuBar *menubar;
    QMenu *menu_server;
    QMenu *menu_edit;
    QMenu *menu_View;
    QStatusBar *statusbar;
    QDockWidget *dockWidget_online_list;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_on_users;
    QListWidget *listWidget_online_users;
    QToolBar *tool_bar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(404, 384);
        action_press_enter_to_send_message = new QAction(MainWindow);
        action_press_enter_to_send_message->setObjectName(QStringLiteral("action_press_enter_to_send_message"));
        action_press_enter_to_send_message->setCheckable(true);
        action_settings = new QAction(MainWindow);
        action_settings->setObjectName(QStringLiteral("action_settings"));
        action_change_server = new QAction(MainWindow);
        action_change_server->setObjectName(QStringLiteral("action_change_server"));
        action_logout = new QAction(MainWindow);
        action_logout->setObjectName(QStringLiteral("action_logout"));
        action_send = new QAction(MainWindow);
        action_send->setObjectName(QStringLiteral("action_send"));
        action_show_online_users = new QAction(MainWindow);
        action_show_online_users->setObjectName(QStringLiteral("action_show_online_users"));
        action_show_online_users->setCheckable(true);
        action_use_html_for_sending_messages = new QAction(MainWindow);
        action_use_html_for_sending_messages->setObjectName(QStringLiteral("action_use_html_for_sending_messages"));
        action_use_html_for_sending_messages->setCheckable(true);
        action_send_image = new QAction(MainWindow);
        action_send_image->setObjectName(QStringLiteral("action_send_image"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        chat_area = new QTextEdit(centralwidget);
        chat_area->setObjectName(QStringLiteral("chat_area"));
        chat_area->setReadOnly(true);

        verticalLayout_2->addWidget(chat_area);

        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMaximumSize(QSize(16777215, 96));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_message_to = new QLabel(widget);
        label_message_to->setObjectName(QStringLiteral("label_message_to"));

        verticalLayout->addWidget(label_message_to);

        textEdit_message_to_send = new QTextEdit(widget);
        textEdit_message_to_send->setObjectName(QStringLiteral("textEdit_message_to_send"));

        verticalLayout->addWidget(textEdit_message_to_send);


        verticalLayout_2->addWidget(widget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 404, 27));
        menu_server = new QMenu(menubar);
        menu_server->setObjectName(QStringLiteral("menu_server"));
        menu_edit = new QMenu(menubar);
        menu_edit->setObjectName(QStringLiteral("menu_edit"));
        menu_View = new QMenu(menubar);
        menu_View->setObjectName(QStringLiteral("menu_View"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);
        dockWidget_online_list = new QDockWidget(MainWindow);
        dockWidget_online_list->setObjectName(QStringLiteral("dockWidget_online_list"));
        dockWidget_online_list->setMinimumSize(QSize(108, 152));
        dockWidget_online_list->setMaximumSize(QSize(220, 524287));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_on_users = new QLabel(dockWidgetContents);
        label_on_users->setObjectName(QStringLiteral("label_on_users"));

        verticalLayout_3->addWidget(label_on_users);

        listWidget_online_users = new QListWidget(dockWidgetContents);
        listWidget_online_users->setObjectName(QStringLiteral("listWidget_online_users"));

        verticalLayout_3->addWidget(listWidget_online_users);

        dockWidget_online_list->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_online_list);
        tool_bar = new QToolBar(MainWindow);
        tool_bar->setObjectName(QStringLiteral("tool_bar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, tool_bar);

        menubar->addAction(menu_server->menuAction());
        menubar->addAction(menu_edit->menuAction());
        menubar->addAction(menu_View->menuAction());
        menu_server->addAction(action_change_server);
        menu_server->addAction(action_logout);
        menu_edit->addAction(action_press_enter_to_send_message);
        menu_edit->addAction(action_use_html_for_sending_messages);
        menu_edit->addAction(action_settings);
        menu_View->addAction(action_show_online_users);
        tool_bar->addAction(action_send);
        tool_bar->addAction(action_send_image);

        retranslateUi(MainWindow);
        QObject::connect(action_press_enter_to_send_message, SIGNAL(triggered(bool)), MainWindow, SLOT(set_send_message_on_enter(bool)));
        QObject::connect(action_settings, SIGNAL(triggered()), MainWindow, SLOT(settings()));
        QObject::connect(action_send, SIGNAL(triggered()), MainWindow, SLOT(send_message()));
        QObject::connect(action_show_online_users, SIGNAL(triggered(bool)), dockWidget_online_list, SLOT(setVisible(bool)));
        QObject::connect(action_change_server, SIGNAL(triggered()), MainWindow, SLOT(change_server()));
        QObject::connect(action_logout, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(action_use_html_for_sending_messages, SIGNAL(triggered(bool)), MainWindow, SLOT(set_use_html(bool)));
        QObject::connect(action_send_image, SIGNAL(triggered()), MainWindow, SLOT(send_image()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_press_enter_to_send_message->setText(QApplication::translate("MainWindow", "Press &Enter to send message", nullptr));
        action_settings->setText(QApplication::translate("MainWindow", "&Settings ...", nullptr));
        action_change_server->setText(QApplication::translate("MainWindow", "&Change server", nullptr));
#ifndef QT_NO_TOOLTIP
        action_change_server->setToolTip(QApplication::translate("MainWindow", "Logout and choose another server", nullptr));
#endif // QT_NO_TOOLTIP
        action_logout->setText(QApplication::translate("MainWindow", "&Logout", nullptr));
#ifndef QT_NO_TOOLTIP
        action_logout->setToolTip(QApplication::translate("MainWindow", "Logout and exit", nullptr));
#endif // QT_NO_TOOLTIP
        action_send->setText(QApplication::translate("MainWindow", "&Send", nullptr));
#ifndef QT_NO_TOOLTIP
        action_send->setToolTip(QApplication::translate("MainWindow", "Send your message", nullptr));
#endif // QT_NO_TOOLTIP
        action_show_online_users->setText(QApplication::translate("MainWindow", "Show On-line Users", nullptr));
        action_use_html_for_sending_messages->setText(QApplication::translate("MainWindow", "Send message in HTML format", nullptr));
        action_send_image->setText(QApplication::translate("MainWindow", "Send Image...", nullptr));
        label_message_to->setText(QApplication::translate("MainWindow", "Brosdcast Message:", nullptr));
        menu_server->setTitle(QApplication::translate("MainWindow", "&Server", nullptr));
        menu_edit->setTitle(QApplication::translate("MainWindow", "&Edit", nullptr));
        menu_View->setTitle(QApplication::translate("MainWindow", "&View", nullptr));
        dockWidget_online_list->setWindowTitle(QApplication::translate("MainWindow", "On-line Users", nullptr));
        label_on_users->setText(QApplication::translate("MainWindow", "On-line Users:", nullptr));
        tool_bar->setWindowTitle(QApplication::translate("MainWindow", "Tool Bar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
