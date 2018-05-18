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
#include "serverinformation.h"
#include "settingsdialog.h"
#include "mainwindow.h"
#include <QtGui/QCompleter>
#include <QtGui/QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QList>
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>

ConnectionWindow::ConnectionWindow(QWidget *parent, QSettings *config) :
	QDialog(parent),
	ui(new Ui::ConnectionWindow)
{
	ui->setupUi(this);
	QCompleter *completer = new QCompleter(this);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	ui->remote_host_comboBox->setCompleter(completer);
	this->config = config;
#if 0
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
#else
	server_list = config->value("ServerList").toList();
	if(!server_list.isEmpty()) {
		foreach(const QVariant &i, server_list) {
			const QString &host = i.value<ServerInformation>().host;
			ui->remote_host_comboBox->addItem(host);
		}
		int index = config->value("LastServerIndex", 0).toInt();
		if(index < 0 || index >= server_list.count()) index = 0;
		const ServerInformation &info = server_list[index].value<ServerInformation>();
		ui->remote_host_comboBox->setCurrentIndex(index);
		ui->remote_port_lineEdit->setText(QString::number(info.port));
		ui->identify_file_lineEdit->setText(info.identify_file);
	}
#endif
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
	SettingsDialog d(this, config);
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
	quint16 port;
	const QString &port_str = ui->remote_port_lineEdit->text();
	if(port_str.isEmpty()) {
		port = 22;
		ui->remote_port_lineEdit->setText("22");
	} else {
		bool ok;
		port = port_str.toUInt(&ok);
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

	bool found = false;
	foreach(const QVariant &i, server_list) {
		if(i.value<ServerInformation>().host == host) {
			found = 1;
			break;
		}
	}
	if(!found) {
		ServerInformation info;
		info.host = host;
		info.port = port;
		info.identify_file = identify_file;
		QVariant v = QVariant::fromValue<ServerInformation>(info);
		server_list << v;
		//qDebug() << server_list.last().value<ServerInformation>().host;
		config->setValue("ServerList", server_list);
	}
	int index = ui->remote_host_comboBox->currentIndex();
	if(index >= 0) config->setValue("LastServerIndex", index);
	hide();
	MainWindow *w = new MainWindow(NULL, host, port, identify_file);
	w->show();
	accept();
}

void ConnectionWindow::remote_host_name_change_event(QString host_name) {
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(!host_name.isEmpty());
}
