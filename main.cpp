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

#include "serverinformation.h"
#include "connectionwindow.h"
#include "mainwindow.h"
#include <sys/stat.h>
#include <QtGui/QApplication>
#include <QtCore/QSettings>
//#include <QtCore/QTextCodec>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#define CONFIG_FILE_NAME "sshout.cfg"

QString config_dir() {
	QString appath = QApplication::applicationDirPath();
#ifndef Q_OS_WINCE
	if(QFile::exists(appath + "/" CONFIG_FILE_NAME)) {
#endif
		return appath;
#ifndef Q_OS_WINCE
	}
	QString in_home = QDir::homePath() + "/.sshout";
	if(!QFile::exists(in_home)) mkdir(in_home.toLocal8Bit().data(), 0750);
	return in_home;
#endif
}

int main(int argc, char *argv[]) {
	qRegisterMetaTypeStreamOperators<ServerInformation>("ServerInformation");
	QSettings config(config_dir() + "/" CONFIG_FILE_NAME, (QSettings::Format)1);
	config.setIniCodec("UTF-8");
	QString style = config.value("Style").toString();
	if(!style.isEmpty()) QApplication::setStyle(style);
	QApplication a(argc, argv);
	//QStringList server_list = config.value("ServerList").toStringList();
	QList<QVariant> server_list = config.value("ServerList").toList();
	QDebug d(QtDebugMsg);
	//d.operator <<("1");
	//d << "1";
	qDebug() << server_list;
	QWidget *w;
	if(server_list.isEmpty()) {
		w = new ConnectionWindow(NULL, &config);
	} else if(config.value("AutoConnect", false).toBool()) {
		int index = config.value("LastServerIndex", 0).toUInt();
		if(index < 0 || index >= server_list.count()) index = 0;
		const ServerInformation &info = server_list[index].value<ServerInformation>();
		w = new MainWindow(NULL, &config, info.host, info.port, info.identify_file);
	} else {
		w = new ConnectionWindow(NULL, &config);
	}
	w->show();
	return a.exec();
}
