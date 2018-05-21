/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_ssh;
    QGridLayout *gridLayout;
    QCheckBox *checkBox_use_separete_known_hosts;
    QTextEdit *textEdit_known_hosts;
    QCheckBox *checkBox_use_internal_ssh_library;
    QLabel *label_ssh_program;
    QLineEdit *lineEdit_ssh_program;
    QToolButton *browse_ssh_program_button;
    QLabel *label_ssh_args;
    QLineEdit *lineEdit_ssh_args;
    QLabel *label_environment;
    QHBoxLayout *horizontalLayout_5;
    QTreeWidget *environment_list;
    QVBoxLayout *verticalLayout_2;
    QPushButton *button_add_env_item;
    QSpacerItem *verticalSpacer_2;
    QPushButton *button_remove_env_item;
    QWidget *tab_chat;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QStringLiteral("SettingsDialog"));
        SettingsDialog->resize(400, 424);
        verticalLayout = new QVBoxLayout(SettingsDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(SettingsDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_ssh = new QWidget();
        tab_ssh->setObjectName(QStringLiteral("tab_ssh"));
        gridLayout = new QGridLayout(tab_ssh);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        checkBox_use_separete_known_hosts = new QCheckBox(tab_ssh);
        checkBox_use_separete_known_hosts->setObjectName(QStringLiteral("checkBox_use_separete_known_hosts"));

        gridLayout->addWidget(checkBox_use_separete_known_hosts, 0, 0, 1, 2);

        textEdit_known_hosts = new QTextEdit(tab_ssh);
        textEdit_known_hosts->setObjectName(QStringLiteral("textEdit_known_hosts"));

        gridLayout->addWidget(textEdit_known_hosts, 1, 0, 1, 3);

        checkBox_use_internal_ssh_library = new QCheckBox(tab_ssh);
        checkBox_use_internal_ssh_library->setObjectName(QStringLiteral("checkBox_use_internal_ssh_library"));

        gridLayout->addWidget(checkBox_use_internal_ssh_library, 2, 0, 1, 2);

        label_ssh_program = new QLabel(tab_ssh);
        label_ssh_program->setObjectName(QStringLiteral("label_ssh_program"));

        gridLayout->addWidget(label_ssh_program, 3, 0, 1, 1);

        lineEdit_ssh_program = new QLineEdit(tab_ssh);
        lineEdit_ssh_program->setObjectName(QStringLiteral("lineEdit_ssh_program"));

        gridLayout->addWidget(lineEdit_ssh_program, 3, 1, 1, 1);

        browse_ssh_program_button = new QToolButton(tab_ssh);
        browse_ssh_program_button->setObjectName(QStringLiteral("browse_ssh_program_button"));

        gridLayout->addWidget(browse_ssh_program_button, 3, 2, 1, 1);

        label_ssh_args = new QLabel(tab_ssh);
        label_ssh_args->setObjectName(QStringLiteral("label_ssh_args"));

        gridLayout->addWidget(label_ssh_args, 4, 0, 1, 1);

        lineEdit_ssh_args = new QLineEdit(tab_ssh);
        lineEdit_ssh_args->setObjectName(QStringLiteral("lineEdit_ssh_args"));

        gridLayout->addWidget(lineEdit_ssh_args, 4, 1, 1, 2);

        label_environment = new QLabel(tab_ssh);
        label_environment->setObjectName(QStringLiteral("label_environment"));

        gridLayout->addWidget(label_environment, 5, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        environment_list = new QTreeWidget(tab_ssh);
        environment_list->setObjectName(QStringLiteral("environment_list"));
        environment_list->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::SelectedClicked);

        horizontalLayout_5->addWidget(environment_list);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        button_add_env_item = new QPushButton(tab_ssh);
        button_add_env_item->setObjectName(QStringLiteral("button_add_env_item"));
        button_add_env_item->setMaximumSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_add_env_item);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        button_remove_env_item = new QPushButton(tab_ssh);
        button_remove_env_item->setObjectName(QStringLiteral("button_remove_env_item"));
        button_remove_env_item->setMaximumSize(QSize(32, 32));

        verticalLayout_2->addWidget(button_remove_env_item);


        horizontalLayout_5->addLayout(verticalLayout_2);


        gridLayout->addLayout(horizontalLayout_5, 6, 0, 1, 3);

        tabWidget->addTab(tab_ssh, QString());
        tab_chat = new QWidget();
        tab_chat->setObjectName(QStringLiteral("tab_chat"));
        tabWidget->addTab(tab_chat, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(SettingsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(SettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), SettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SettingsDialog, SLOT(reject()));
        QObject::connect(checkBox_use_separete_known_hosts, SIGNAL(toggled(bool)), textEdit_known_hosts, SLOT(setEnabled(bool)));
        QObject::connect(checkBox_use_internal_ssh_library, SIGNAL(toggled(bool)), SettingsDialog, SLOT(use_internal_ssh_library_checked(bool)));
        QObject::connect(button_add_env_item, SIGNAL(clicked()), SettingsDialog, SLOT(add_environment_variable()));
        QObject::connect(button_remove_env_item, SIGNAL(clicked()), SettingsDialog, SLOT(remove_environment_variable()));
        QObject::connect(browse_ssh_program_button, SIGNAL(clicked()), SettingsDialog, SLOT(browse_ssh_program()));
        QObject::connect(SettingsDialog, SIGNAL(accepted()), SettingsDialog, SLOT(save_settings()));

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Dialog", nullptr));
        checkBox_use_separete_known_hosts->setText(QApplication::translate("SettingsDialog", "Set separate &known hosts", nullptr));
        checkBox_use_internal_ssh_library->setText(QApplication::translate("SettingsDialog", "Use &internal OpenSSH library", nullptr));
        label_ssh_program->setText(QApplication::translate("SettingsDialog", "SSH program path", nullptr));
        browse_ssh_program_button->setText(QApplication::translate("SettingsDialog", "...", nullptr));
        label_ssh_args->setText(QApplication::translate("SettingsDialog", "Addional options for SSH", nullptr));
        label_environment->setText(QApplication::translate("SettingsDialog", "Addional Environment:", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = environment_list->headerItem();
        ___qtreewidgetitem->setText(1, QApplication::translate("SettingsDialog", "Value", nullptr));
        ___qtreewidgetitem->setText(0, QApplication::translate("SettingsDialog", "Name", nullptr));
        button_add_env_item->setText(QApplication::translate("SettingsDialog", "+", nullptr));
        button_remove_env_item->setText(QApplication::translate("SettingsDialog", "-", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_ssh), QApplication::translate("SettingsDialog", "SSH", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_chat), QApplication::translate("SettingsDialog", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
