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
#include <sys/stat.h>
#include <QtGui/QApplication>
#include <QtCore/QSettings>
//#include <QtCore/QTextCodec>
#include <QtCore/QFile>
#include <QtCore/QDir>

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
	QSettings config(config_dir() + "/" CONFIG_FILE_NAME, (QSettings::Format)1);
	config.setIniCodec("UTF-8");
	QString style = config.value("Style").toString();
	if(!style.isEmpty()) QApplication::setStyle(style);
	QApplication a(argc, argv);
	ConnectionWindow w;
	w.show();
	
	return a.exec();
}
