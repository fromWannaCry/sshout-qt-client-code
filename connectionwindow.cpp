/*
 * Copyright 2015-2018 Rivoreo
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 */

#include "connectionwindow.h"
#include "ui_connectionwindow.h"
#include "settingsdialog.h"
#include <QtGui/QCompleter>
#include <QtGui/QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QList>
#include <QtGui/QMessageBox>

ConnectionWindow::ConnectionWindow(QWidget *parent, QSettings *config) :
	QDialog(parent),
	ui(new Ui::ConnectionWindow)
{
	ui->setupUi(this);
	QCompleter *completer = new QCompleter(this);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	ui->remote_host_comboBox->setCompleter(completer);
	this->config = config;
	QStringList server_list = config->value("ServerList").toStringList();
	if(!server_list.isEmpty()) {
		int index = config->value("LastServerIndex", 0).toInt();
		ui->remote_host_comboBox->addItems(server_list);
		ui->remote_host_comboBox->setCurrentIndex(index);
		QList<QVariant> port_list = config->value("PortList").toList();
		if(port_list.count() > index) {
			bool ok;
			int port = port_list[index].toInt(&ok);
			if(ok) ui->remote_port_lineEdit->setText(QString(ok));
		}
		QStringList id_list = config->value("IdentifyList").toStringList();
		if(id_list.count() > index) {
			ui->identify_file_lineEdit->setText(id_list[index]);
		}
	}
	remote_host_name_change_event(ui->remote_host_comboBox->currentText());
}

ConnectionWindow::~ConnectionWindow()
{
	delete ui;
}

static QString ssh_config_dir() {
#ifdef Q_OS_WINCE
	return QApplication::applicationDirPath();
#else
	return QDir::homePath() + "/.ssh";
#endif
}

void ConnectionWindow::browse_identity_file() {
	QFileDialog d(this, tr("Choose the identity"), ssh_config_dir());
	d.setAcceptMode(QFileDialog::AcceptOpen);
	d.setFileMode(QFileDialog::ExistingFile);
	d.setOption(QFileDialog::DontUseNativeDialog);
	if(d.exec()) {
		ui->identify_file_lineEdit->setText(d.selectedFiles()[0]);
	}
}

void ConnectionWindow::change_settings() {
	SettingsDialog d(this);
	d.exec();
}

void ConnectionWindow::closeEvent(QCloseEvent *e) {
	qDebug("function: ConnectionWindow::closeEvent(%p)", e);
}

void ConnectionWindow::start_main_window() {
	qDebug("slot: ConnectionWindow::start_main_window()");
	const QString &host = ui->remote_host_comboBox->currentText();
	if(host.isEmpty()) {
		QMessageBox::critical(this, tr("Check Server Information"), tr("Host name cannot be empty"));
		return;
	}
	int port;
	const QString &port_str = ui->remote_port_lineEdit->text();
	if(port_str.isEmpty()) {
		port = 22;
		ui->remote_port_lineEdit->setText("22");
	} else {
		bool ok;
		port = port_str.toInt(&ok);
		if(!ok) {
			QMessageBox::critical(this, tr("Check Server Information"), tr("Invalid port number"));
			return;
		}
	}
	const QString &identify_file = ui->identify_file_lineEdit->text();
	if(identify_file.isEmpty()) {
		QMessageBox::critical(this, tr("Check Server Information"), tr("Identify file path cannot be empty"));
		return;
	}
	// TODO: Create an instance of MainWindow, and accept()
}

void ConnectionWindow::remote_host_name_change_event(QString host_name) {
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!host_name.isEmpty());
}
