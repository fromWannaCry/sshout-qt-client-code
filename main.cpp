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
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
// TODO: Use '#if QT_VERSION < 0x050000' to check Qt 5
#include <QtGui/QApplication>
#include <QtCore/QSettings>
#include <QtCore/QUrl>
//#include <QtCore/QTextCodec>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDebug>

#define CONFIG_FILE_NAME "sshout.cfg"

static void print_usage(const char *name) {
	fprintf(stderr, "Usage: %s [<options>] [<host>|ssh://<host>[:<port>][/]]\n"
		"Options:\n"
		"	--port <n>, -p <n>	Specify port for host as <n>, default 22\n"
		"	--identify-file <path>, -i <path>\n"
		"				Specify the identify file\n"
		"	--style <style>		Use QStyle <style>\n",
		name);
}

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

	static option long_options[] = {
		{ "port", 1, NULL, 'p' },
		{ "identify-file", 1, NULL, 'i' },
		{ "style", 1, NULL, 0 },
		{ "help", 0, NULL, 'h' },
		{ NULL, 0, NULL, 0 }
	};
	int port = -1;
	const char *identify_file = NULL;
	const char *style = NULL;
	while(true) {
		int option_index;
		int c = getopt_long(argc, argv, "p:i:h", long_options, &option_index);
		if(c == EOF) break;
		switch(c) {
			case 0:
				if(option_index == 2) {
					style = optarg;
				}
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'i':
				identify_file = optarg;
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;
			case '?':
				return -1;
		}
	}

	if(style) QApplication::setStyle(QString::fromLocal8Bit(style));

	QApplication a(argc, argv);
	QSettings config(config_dir() + "/" CONFIG_FILE_NAME, (QSettings::Format)1);
	config.setIniCodec("UTF-8");
	if(!style) {
		QString style = config.value("Style").toString();
		if(!style.isEmpty()) a.setStyle(style);
	}
	if(argc - optind > 1) {
		print_usage(argv[0]);
		return -1;
	}
	qsrand(time(NULL));
	if(argc - optind == 1) {
		QString host;
		char *maybe_url = argv[optind];
		QUrl url(maybe_url);
		if(url.scheme() == QString("ssh")) {
			host = url.host();
			if(port == -1) port = url.port();
		} else host = QString(maybe_url);
		if(port == -1) port = 22;
		MainWindow w(NULL, &config, host, port, QString(identify_file));
		w.show();
		return a.exec();
	}
	//QStringList server_list = config.value("ServerList").toStringList();
	QList<QVariant> server_list = config.value("ServerList").toList();
	//QDebug d(QtDebugMsg);
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
