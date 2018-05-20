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
#include "connectionwindow.h"
#include "sshout/api.h"
#include "packet.h"
#include <QtCore/QSettings>
#include <QtCore/QTimer>
#include <QtCore/QDataStream>
//#include <QtCore/QDateTime>
#include <QtCore/QTime>
#include <QtGui/QKeyEvent>
#include <QtGui/QListWidgetItem>
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
	connect(ssh_client, SIGNAL(readyRead()), SLOT(read_ssh()));
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
	need_reconnect = true;
	bool show_user_list = config->value("ShowUserList", true).toBool();
	ui->dockWidget_online_list->setVisible(show_user_list);
	ui->action_show_online_users->setChecked(show_user_list);
	if(config->value("WindowMaximized", false).toBool()) showMaximized();
	else {
		QVariant v = config->value("WindowSize");
		if(!v.isNull()) resize(v.toSize());
	}
	ui->textEdit_message_to_send->setFocus();
	data_stream = new QDataStream(ssh_client);
	//data_stream->setByteOrder(QDataStream::BigEndian);
	timer = new QTimer(this);
	timer->setInterval(60000);
	connect(timer, SIGNAL(timeout()), SLOT(send_request_online_users()));
	connect_ssh();
}

MainWindow::~MainWindow()
{
	delete ui;
	delete ssh_client;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
	//qDebug("function: MainWindow::keyPressEvent(%p)", e);
	if(focusWidget() == ui->textEdit_message_to_send) {
		bool show_tip = true;
		int key = e->key();
		//qDebug("key = 0x%x", key);
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
	//qDebug("function: MainWindow::keyReleaseEvent(%p)", e);
	if(focusWidget() == ui->textEdit_message_to_send) {
		int key = e->key();
		//qDebug("key = 0x%x", key);
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

void MainWindow::save_ui_layout() {
	config->setValue("ShowUserList", ui->dockWidget_online_list->isVisible());
	if(isMaximized()) config->setValue("WindowMaximized", true);
	else config->setValue("WindowSize", size());
}

void MainWindow::closeEvent(QCloseEvent *e) {
	need_reconnect = false;
	ssh_client->disconnect();
	save_ui_layout();
	e->accept();
}

void MainWindow::connect_ssh() {
	if(!ssh_client->connect(host, port, DEFAULT_SSH_USER_NAME, "api")) {
		ui->statusbar->showMessage(tr("Cannot connect"), 10000);
	}
}

void MainWindow::print_message(const QString &msg_from, const QString &msg_to, quint8 msg_type, const QByteArray &message) {
	//QDateTime dt = QDateTime::currentDateTime();
	QTime t = QTime::currentTime();
	QString tag = (msg_to.isEmpty() || msg_to == QString("GLOBAL")) ?
		QString("%1 %2").arg(msg_from).arg(t.toString("H:mm:ss")) :
		QString("%1 to %2 %3").arg(msg_from).arg(msg_to).arg(t.toString("H:mm:ss"));
	switch(msg_type) {
		case SSHOUT_API_MESSAGE_TYPE_PLAIN:
			ui->chat_area->append(tag + "\n" + message);
			break;
		case SSHOUT_API_MESSAGE_TYPE_RICH:
			ui->chat_area->insertHtml(tag + "<br>" + message);
			break;
		case SSHOUT_API_MESSAGE_TYPE_IMAGE:
			break;
	}
}

void MainWindow::send_hello() {
	quint32 length = 1 + 6 + 2;
	quint8 type = SSHOUT_API_HELLO;
	quint16 version = 1;
	*data_stream << length;
	*data_stream << type;
	data_stream->writeRawData("SSHOUT", 6);
	*data_stream << version;
	//ssh_client->write("Test");
}

void MainWindow::send_message() {
	qDebug("slot: MainWindow::send_message()");
	QString message = ui->textEdit_message_to_send->toPlainText();
	if(message.isEmpty()) return;

	QByteArray message_bytes = message.toUtf8();
	quint32 message_len = message_bytes.length();
	//QString ui->listWidget_online_users->currentItem()
	QByteArray to_user("GLOBAL");
	quint8 to_user_len = to_user.length();
	quint8 message_type = SSHOUT_API_MESSAGE_TYPE_PLAIN;
	quint32 packet_length = 1 + 1 + to_user_len + 1 + 4 + message_len;
	quint8 packet_type = SSHOUT_API_SEND_MESSAGE;
	//quint8 packet_type = 99;
	*data_stream << packet_length;
	*data_stream << packet_type;
	*data_stream << to_user_len;
	data_stream->writeRawData(to_user.data(), to_user_len);
	*data_stream << message_type;
	*data_stream << message_len;
	data_stream->writeRawData(message_bytes.data(), message_len);

	ui->textEdit_message_to_send->clear();
	ui->statusbar->showMessage(tr("Sending message"), 1000);
}

void MainWindow::add_user_item(const QString &user_name, QList<UserIdAndHostName> *logins) {
	QListWidgetItem *item;
	QList<QListWidgetItem *> orig_items = ui->listWidget_online_users->findItems(user_name, Qt::MatchFixedString | Qt::MatchCaseSensitive);
	int exists = orig_items.length();
	if(exists) {
		Q_ASSERT(exists == 1);
		item = orig_items[0];
	} else {
		item = new QListWidgetItem(user_name);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	item->setData((int)Qt::UserRole, QVariant::fromValue((void *)logins));
	if(!exists) ui->listWidget_online_users->addItem(item);
}

void MainWindow::remove_offline_user_items(const QSet<QString> &keep_set) {
	QList<QListWidgetItem *> items = ui->listWidget_online_users->findItems("*", Qt::MatchWildcard);
	//qDebug() << items;
	foreach(QListWidgetItem *i, items) {
		if(keep_set.contains(i->text())) continue;
		delete i;
	}
}

void MainWindow::update_user_list(const UserInfo *users, unsigned int count) {
	QSet<QString> user_set;
	QHash<QString, QList<UserIdAndHostName> > user_logins;
	for(unsigned int i=0; i<count; i++) {
		const UserInfo *p = users + i;
		user_set << p->user_name;
		user_logins[p->user_name] << (UserIdAndHostName){ p->id, p->host_name };
	}
	remove_offline_user_items(user_set);
	foreach(const QString &user, user_set) {
		add_user_item(user, new QList<UserIdAndHostName>(user_logins[user]));
	}
}

void MainWindow::send_request_online_users() {
	quint32 length = 1;
	quint8 type = SSHOUT_API_GET_ONLINE_USER;
	*data_stream << length;
	*data_stream << type;
}

void MainWindow::ssh_state_change(SSHClient::SSHState state) {
	qDebug("slot: MainWindow::on_ssh_state_change(%d)", state);
	switch(state) {
		case SSHClient::DISCONNECTED:
			//ui->statusbar->showMessage(tr("Disconnected"));
			if(need_reconnect) QTimer::singleShot(10000, this, SLOT(connect_ssh()));
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
	while(ssh_client->bytesAvailable() > 0) {
		QByteArray data = sshout_get_packet(ssh_client);
		if(data.isEmpty()) {
			ssh_client->disconnect();
			return;
		}
		QDataStream stream(&data, QIODevice::ReadOnly);
		quint8 packet_type;
		stream >> packet_type;
		switch(packet_type) {
			case SSHOUT_API_PASS:
				qDebug("SSHOUT_API_PASS received");
				if(data.mid(1, 6) != QString("SSHOUT")) {
					qWarning("Magic mismatch");
					ui->chat_area->append(tr("Magic mismatch"));
					need_reconnect = false;
					ssh_client->disconnect();
					return;
				}
				stream.skipRawData(6);
				quint16 version;
				stream >> version;
				if(version != 1) {
					qWarning("Version mismatch (%hu != 1)", version);
					ui->chat_area->append(tr("Version mismatch (%1 != 1)").arg(version));
					need_reconnect = false;
					ssh_client->disconnect();
					return;
				}
				send_request_online_users();
				break;
			case SSHOUT_API_ONLINE_USERS_INFO:
				qDebug("SSHOUT_API_ONLINE_USERS_INFO received");
				{
					quint16 my_id;
					stream >> my_id;
					quint16 count;
					stream >> count;
					UserInfo users[count];
					unsigned int i = 0;
					//while(count-- > 0) {
					while(i < count) {
						quint16 id;
						quint8 user_name_len, host_name_len;
						stream >> id;
						stream >> user_name_len;
						char user_name[user_name_len];
						stream.readRawData(user_name, user_name_len);
						stream >> host_name_len;
						char host_name[host_name_len];
						stream.readRawData(host_name, host_name_len);
						UserInfo *p = users + i++;
						p->id = id;
						p->user_name = QString::fromUtf8(user_name, user_name_len);
						p->host_name = QString::fromUtf8(host_name, host_name_len);
					}
					update_user_list(users, count);
				}
			case SSHOUT_API_MOTD:
				qDebug("SSHOUT_API_MOTD received");
				quint32 length;
				stream >> length;
				if((int)length > data.length() - 1 - 4) {
					qWarning("malformed packet: member size %u out of packet size %d", length, data.length());
					ssh_client->disconnect();
					return;
				}
				ui->chat_area->append(data.mid(5, length));
		}
	}
}

void MainWindow::read_ssh_stderr() {
	//qDebug("slot: MainWindow::read_ssh_stderr()");
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

void MainWindow::change_server() {
	ConnectionWindow *w = new ConnectionWindow(NULL, config);
	w->show();
	close();
}
