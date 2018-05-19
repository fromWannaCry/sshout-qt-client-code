/* Secure Shout Host Oriented Unified Talk
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

#include "common.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef HAVE_OPENSSH_LIBRARY
#include "internalsshclient.h"
#endif
#include "externalsshclient.h"
#include "settingsdialog.h"
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtGui/QKeyEvent>
//#include <QtGui/QRubberBand>
#include <stdio.h>
#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent, QSettings *config, const QString &host, quint16 port, const QString &identify_file) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->config = config;
	//setMouseTracking(true);
#ifdef HAVE_OPENSSH_LIBRARY
	use_internal_ssh_library = config->value("UseInternalSSHLibrary", false).toBool();
	if(use_internal_ssh_library) ssh_client = new InternalSSHClient(this);
	else
#endif
	ssh_client = new ExternalSSHClient(this, config->value("SSHProgramPath", DEFAULT_SSH_PROGRAM_PATH).toString());
#ifndef HAVE_OPENSSH_LIBRARY
	use_internal_ssh_library = false;
#endif
	ssh_client->set_identify_file(identify_file);
	this->host = host;
	this->port = port;
	connect(ssh_client, SIGNAL(state_changed(SSHClient::SSHState)), SLOT(ssh_state_change(SSHClient::SSHState)));
	if(!use_internal_ssh_library) {
		ExternalSSHClient *extern_ssh_client = (ExternalSSHClient *)ssh_client;
		extern_ssh_client->register_ready_read_stderr_slot(this, SLOT(read_ssh_stderr()));
		QString args = config->value("SSHArgs").toString();
		if(!args.isEmpty()) extern_ssh_client->set_extra_args(args.split(' '));
	}
	send_message_on_enter = config->value("UseEnterToSendMessage", true).toBool();
	ui->action_press_enter_to_send_message->setChecked(send_message_on_enter);
	ui->textEdit_message_to_send->installEventFilter(this);
	control_key_pressed = false;
	ignore_key_event = false;
	connect_ssh();
}

MainWindow::~MainWindow()
{
	delete ui;
	delete ssh_client;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	qDebug("function: MainWindow::keyPressEvent(%p)", e);
	if(focusWidget() == ui->textEdit_message_to_send) {
		bool show_tip = true;
		int key = e->key();
		qDebug("key = 0x%x", key);
		switch(key) {
			case Qt::Key_Control:
				show_tip = false;
				control_key_pressed = true;
				break;
			case Qt::Key_Enter:
			case Qt::Key_Return:
				show_tip = false;
				if(send_message_on_enter) {
					if(control_key_pressed) ui->textEdit_message_to_send->insertPlainText("\n");
					else send_message();
					ignore_key_event = true;
				} else if(control_key_pressed) {
					send_message();
					ignore_key_event = true;
				}
				break;
			case Qt::Key_Backspace:
				show_tip = false;
				break;
		}
		if(show_tip) ui->statusbar->showMessage(tr("Press %1 to send").arg(send_message_on_enter ? tr("Enter") : tr("Ctrl-Enter")), 2000);
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
	qDebug("function: MainWindow::keyReleaseEvent(%p)", e);
	if(focusWidget() == ui->textEdit_message_to_send) {
		int key = e->key();
		qDebug("key = 0x%x", key);
		if(key == Qt::Key_Control) control_key_pressed = false;
	}
}

bool MainWindow::eventFilter(QObject *o, QEvent *e) {
	//qDebug("function: MainWindow::eventFilter(%p, %p)", o, e);
	if(o != ui->textEdit_message_to_send) return QObject::eventFilter(o, e);
	if(e->type() != QEvent::KeyPress) return QObject::eventFilter(o, e);
	keyPressEvent(static_cast<QKeyEvent *>(e));
	if(!ignore_key_event) return false;
	ignore_key_event = false;
	return true;
}

void MainWindow::connect_ssh() {
	if(!ssh_client->connect(host, port, DEFAULT_SSH_USER_NAME, "api")) {
		ui->statusbar->showMessage(tr("Cannot connect"), 10000);
	}
}

void MainWindow::send_hello() {

}

void MainWindow::send_message() {
	qDebug("slot: MainWindow::send_message()");
	QString message = ui->textEdit_message_to_send->toPlainText();
	if(message.isEmpty()) return;
	ui->textEdit_message_to_send->clear();
}

void MainWindow::ssh_state_change(SSHClient::SSHState state) {
	qDebug("slot: MainWindow::on_ssh_state_change(%d)", state);
	switch(state) {
		case SSHClient::DISCONNECTED:
			//ui->statusbar->showMessage(tr("Disconnected"));
			QTimer::singleShot(10000, this, SLOT(connect_ssh()));
			break;
		case SSHClient::CONNECTIING:
			if(use_internal_ssh_library) ui->statusbar->showMessage(tr("Connecting"));
			break;
		case SSHClient::AUTHENTICATING:
			if(use_internal_ssh_library) ui->statusbar->showMessage(tr("Authenticating"));
			break;
		case SSHClient::AUTHENTICATED:
			ui->statusbar->showMessage(tr("Connected"));
			send_hello();
			break;
	}
}

void MainWindow::read_ssh() {
	qDebug("slot: MainWindow::read_ssh()");
}

void MainWindow::read_ssh_stderr() {
	qDebug("slot: MainWindow::read_ssh_stderr()");
	if(use_internal_ssh_library) return;
	ExternalSSHClient *extern_ssh_client = (ExternalSSHClient *)ssh_client;
	while(extern_ssh_client->can_read_line_from_stderr()) {
		QByteArray data = extern_ssh_client->read_line_from_stderr();
		int len = data.count();
		if(!len) return;
		fputs(data.data(), stderr);
		if(data[len - 1] == '\n') data.resize(len - 1);
		ui->statusbar->showMessage(QString::fromLocal8Bit(data), 10000);
	}
}

void MainWindow::set_send_message_on_enter(bool v) {
	qDebug("slot: MainWindow::set_send_message_on_enter(%s)", v ? "true" : "false");
	send_message_on_enter = v;
	config->setValue("UseEnterToSendMessage", v);
}

void MainWindow::settings() {
	SettingsDialog d(this, config);
	d.exec();
}
