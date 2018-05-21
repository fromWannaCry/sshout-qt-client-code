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

#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QtCore/qglobal.h>
#if QT_VERSION < 0x050000
#include <QtGui/QDialog>
#else
#include <QtWidgets/QDialog>
#endif
#include <QtCore/QSettings>
//class QSettings;

namespace Ui {
class ConnectionWindow;
}

class ConnectionWindow : public QDialog
{
Q_OBJECT

public:
	explicit ConnectionWindow(QWidget *, QSettings *);
	~ConnectionWindow();

private:
	Ui::ConnectionWindow *ui;
	QSettings *config;
#if 0
	QStringList *host_list;
	QList<QVariant> *port_list;
	QStringList *identify_list;
#else
	QList<QVariant> server_list;
#endif

protected:
	void closeEvent(QCloseEvent *);

private slots:
	void browse_identity_file();
	void change_settings();
	void start_main_window();
	void remote_host_name_change_event(int);
	void remote_host_name_change_event(QString);
};

#endif // CONNECTIONWINDOW_H
