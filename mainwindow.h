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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/qglobal.h>
#if QT_VERSION < 0x050000
#include <QtGui/QMainWindow>
#else
#include <QtWidgets/QMainWindow>
#endif
#include "sshclient.h"

class QSettings;
class QDateTime;
class QDir;
class QTextCursor;
class QListWidgetItem;
class MessageLog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *, QSettings *, const QString &, quint16, const QString &);
	~MainWindow();
	void set_ssh_user(const QString &);

public slots:
	void connect_ssh();
	void show();

protected:
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
	bool eventFilter(QObject *, QEvent *);
	void closeEvent(QCloseEvent *);
	void changeEvent(QEvent *);
	void dragEnterEvent(QDragEnterEvent *);
	void dragLeaveEvent(QDragLeaveEvent *);
	void dropEvent(QDropEvent *);

private:
	struct UserInfo {
		int id;
		QString user_name;
		QString host_name;
	};
	struct UserIdAndHostName {
		int id;
		QString host_name;
	};
	//QString create_random_hex_string(int);
	void print_image(const QByteArray &, QByteArray &);
	void print_message(const QDateTime &t, const QString &, const QString &, quint8, const QByteArray &);
	void send_hello();
	void send_message(const QString &, quint8, const QByteArray &);
	void send_image(const QImage &);
	void save_ui_layout();
	void add_user_item(const QString &, QList<UserIdAndHostName> *);
	void remove_offline_user_items(const QSet<QString> &);
	void update_user_list(const UserInfo *, unsigned int);
	void update_user_state(const QString &, quint8);
	void print_error(quint32, const QString &);
	void apply_chat_area_config();
	void update_window_title();
	void reset_unread_message_count();
	Ui::MainWindow *ui;
	bool use_internal_ssh_library;
	SSHClient *ssh_client;
	QDataStream *data_stream;
	QString host;
	quint16 port;
	QSettings *config;
	bool send_message_on_enter;
	bool control_key_pressed;
	bool ignore_key_event;
	bool need_reconnect;
	QTimer *timer;
	//QTemporaryFile *cache_file_allocator;
	QDir *log_dir, *image_cache_dir;
	QString last_message_html;
	//QPoint pos_in_chat_area;
	//QTextCursor *current_cursor_in_chat_area;
	unsigned int unread_message_count;
	int my_id;
	QString my_user_name;
	bool ready;
	MessageLog *message_log;
	QString ssh_user;

private slots:
	//void on_ssh_state_change(SSHClient::SSHState);
	void ssh_state_change(SSHClient::SSHState);
	//void on_ssh_ready_read();
	//void on_ssh_ready_read_stderr();
	void read_ssh();
	void read_ssh_stderr();
	void set_send_message_on_enter(bool);
	void settings();
	void send_message();
	void change_server();
	void send_request_online_users();
	void set_use_html(bool);
	void send_image();
	void open_project_page();
	void show_about();
	void show_chat_area_context_menu(const QPoint &);
	//void open_image_from_chat_area();
	//void copy_link_from_chat_area();
	void reset_unread_message_count_from_chat_area_vertical_scroll_bar(int);
	void show_sessions_of_user(QListWidgetItem *);
	//void show_session_list_context_menu(const QPoint &);
	void send_image_from_clipboard();
	//void show_about_qt();
};

#endif // MAINWINDOW_H
