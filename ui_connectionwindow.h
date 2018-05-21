/********************************************************************************
** Form generated from reading UI file 'connectionwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTIONWINDOW_H
#define UI_CONNECTIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ConnectionWindow
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *remote_host_label;
    QComboBox *remote_host_comboBox;
    QLineEdit *remote_port_lineEdit;
    QLabel *remote_port_label;
    QLabel *identify_label;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *identify_file_lineEdit;
    QPushButton *browse_identify_pushButton;
    QCheckBox *checkBox_auto_connect;
    QHBoxLayout *horizontalLayout;
    QPushButton *settings_pushButton;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ConnectionWindow)
    {
        if (ConnectionWindow->objectName().isEmpty())
            ConnectionWindow->setObjectName(QStringLiteral("ConnectionWindow"));
        ConnectionWindow->resize(287, 153);
        verticalLayout = new QVBoxLayout(ConnectionWindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        remote_host_label = new QLabel(ConnectionWindow);
        remote_host_label->setObjectName(QStringLiteral("remote_host_label"));

        gridLayout->addWidget(remote_host_label, 0, 0, 1, 1);

        remote_host_comboBox = new QComboBox(ConnectionWindow);
        remote_host_comboBox->setObjectName(QStringLiteral("remote_host_comboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(remote_host_comboBox->sizePolicy().hasHeightForWidth());
        remote_host_comboBox->setSizePolicy(sizePolicy);
        remote_host_comboBox->setMaximumSize(QSize(16777215, 28));
        remote_host_comboBox->setEditable(true);

        gridLayout->addWidget(remote_host_comboBox, 0, 1, 1, 1);

        remote_port_lineEdit = new QLineEdit(ConnectionWindow);
        remote_port_lineEdit->setObjectName(QStringLiteral("remote_port_lineEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(remote_port_lineEdit->sizePolicy().hasHeightForWidth());
        remote_port_lineEdit->setSizePolicy(sizePolicy1);
        remote_port_lineEdit->setMaximumSize(QSize(48, 16777215));
        remote_port_lineEdit->setText(QStringLiteral("22"));
        remote_port_lineEdit->setMaxLength(5);

        gridLayout->addWidget(remote_port_lineEdit, 0, 3, 1, 1);

        remote_port_label = new QLabel(ConnectionWindow);
        remote_port_label->setObjectName(QStringLiteral("remote_port_label"));

        gridLayout->addWidget(remote_port_label, 0, 2, 1, 1);

        identify_label = new QLabel(ConnectionWindow);
        identify_label->setObjectName(QStringLiteral("identify_label"));

        gridLayout->addWidget(identify_label, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        identify_file_lineEdit = new QLineEdit(ConnectionWindow);
        identify_file_lineEdit->setObjectName(QStringLiteral("identify_file_lineEdit"));

        horizontalLayout_3->addWidget(identify_file_lineEdit);

        browse_identify_pushButton = new QPushButton(ConnectionWindow);
        browse_identify_pushButton->setObjectName(QStringLiteral("browse_identify_pushButton"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(browse_identify_pushButton->sizePolicy().hasHeightForWidth());
        browse_identify_pushButton->setSizePolicy(sizePolicy2);
        browse_identify_pushButton->setMaximumSize(QSize(28, 16777215));
        browse_identify_pushButton->setText(QStringLiteral("..."));

        horizontalLayout_3->addWidget(browse_identify_pushButton);


        gridLayout->addLayout(horizontalLayout_3, 1, 1, 1, 3);


        verticalLayout->addLayout(gridLayout);

        checkBox_auto_connect = new QCheckBox(ConnectionWindow);
        checkBox_auto_connect->setObjectName(QStringLiteral("checkBox_auto_connect"));

        verticalLayout->addWidget(checkBox_auto_connect);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        settings_pushButton = new QPushButton(ConnectionWindow);
        settings_pushButton->setObjectName(QStringLiteral("settings_pushButton"));

        horizontalLayout->addWidget(settings_pushButton);

        buttonBox = new QDialogButtonBox(ConnectionWindow);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(ConnectionWindow);
        QObject::connect(buttonBox, SIGNAL(rejected()), ConnectionWindow, SLOT(reject()));
        QObject::connect(browse_identify_pushButton, SIGNAL(clicked()), ConnectionWindow, SLOT(browse_identity_file()));
        QObject::connect(settings_pushButton, SIGNAL(clicked()), ConnectionWindow, SLOT(change_settings()));
        QObject::connect(buttonBox, SIGNAL(accepted()), ConnectionWindow, SLOT(start_main_window()));
        QObject::connect(remote_host_comboBox, SIGNAL(editTextChanged(QString)), ConnectionWindow, SLOT(remote_host_name_change_event(QString)));
        QObject::connect(remote_host_comboBox, SIGNAL(currentIndexChanged(int)), ConnectionWindow, SLOT(remote_host_name_change_event(int)));

        QMetaObject::connectSlotsByName(ConnectionWindow);
    } // setupUi

    void retranslateUi(QDialog *ConnectionWindow)
    {
        ConnectionWindow->setWindowTitle(QApplication::translate("ConnectionWindow", "ConnectionWindow", nullptr));
        remote_host_label->setText(QApplication::translate("ConnectionWindow", "Server", nullptr));
        remote_port_label->setText(QApplication::translate("ConnectionWindow", "Port", nullptr));
        identify_label->setText(QApplication::translate("ConnectionWindow", "Identify", nullptr));
        checkBox_auto_connect->setText(QApplication::translate("ConnectionWindow", "&Auto connect", nullptr));
        settings_pushButton->setText(QApplication::translate("ConnectionWindow", "&Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectionWindow: public Ui_ConnectionWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTIONWINDOW_H
