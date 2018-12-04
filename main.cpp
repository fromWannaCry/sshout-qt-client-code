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
#if QT_VERSION < 0x050000
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif
#include <QtCore/QSettings>
#include <QtCore/QUrl>
//#include <QtCore/QTextCodec>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTranslator>
#include <QtCore/QDebug>

#define CONFIG_FILE_NAME "sshout.cfg"

static QTranslator *translator;

static void print_usage(const char *name) {
	fprintf(stderr, "Usage: %s [<options>] [<host>|ssh://[<user>@]<host>[:<port>][/]]\n"
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

void get_translations_directories(QStringList &directories) {
#ifdef Q_OS_MAC
	directories << QApplication::applicationDirPath() + "/../Resources/Translations";
	directories << QApplication::applicationDirPath() + "/../Translations";
#endif
	directories << QApplication::applicationDirPath() + "/translations";
#if !defined Q_OS_WIN || defined Q_OS_WINCE
	directories << QApplication::applicationDirPath() + "/../share/sshout-qt/translations";
	directories << "/usr/share/sshout-qt/translations";
	directories << "/usr/share/translations";
#endif
}

bool load_messages_translation(QString language) {
	language.replace('_', '-');
	int dot_i = language.indexOf('.');
	if(dot_i != -1) language.truncate(dot_i);
	QStringList translations_directories;
	get_translations_directories(translations_directories);
	QString file_name = QString("sshout.%1.qm").arg(language.toLower());
	//qDebug() << file_name;
	foreach(const QString &dir, translations_directories) {
		if(translator->load(file_name, dir, "-")) return true;
	}
	return false;
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
		if(c == -1) break;
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
		if(!style.isEmpty() && QString::compare(style, QObject::tr("Default"), Qt::CaseInsensitive)) {
			a.setStyle(style);
		}
	}
	if(argc - optind > 1) {
		print_usage(argv[0]);
		return -1;
	}
	translator = new QTranslator;
	QString language = config.value("Language").toString();
	if(language.isEmpty()) {
		const char *language = getenv("LANGUAGE");
		if(language) {
			QStringList languages = QString(language).split(':');
			foreach(const QString &l, languages) {
				if(l == QString("en_US") || load_messages_translation(l)) break;
			}
		} else {
			language = getenv("LANG");
			if(language) load_messages_translation(QString(language));
		}
	} else if(language != QString("en")) {
		load_messages_translation(language);
	}
	a.installTranslator(translator);
	qsrand(time(NULL));
	if(argc - optind == 1) {
		QString host;
		QString user;
		char *maybe_url = argv[optind];
		QUrl url(QString::fromUtf8(maybe_url));
		if(url.scheme() == QString("ssh")) {
			host = url.host();
			if(port == -1) port = url.port();
			user = url.userName();
		} else host = QString(maybe_url);
		if(port == -1) port = 22;
		MainWindow w(NULL, &config, host, port, QString(identify_file));
		if(!user.isEmpty()) w.set_ssh_user(user);
		w.show();
		w.connect_ssh();
		return a.exec();
	}
	QList<QVariant> server_list = config.value("ServerList").toList();
	QWidget *w;
	if(server_list.isEmpty()) {
		w = new ConnectionWindow(NULL, &config);
	} else if(config.value("AutoConnect", false).toBool()) {
		int index = config.value("LastServerIndex", 0).toUInt();
		if(index < 0 || index >= server_list.count()) index = 0;
		const ServerInformation &info = server_list[index].value<ServerInformation>();
		MainWindow *mw = new MainWindow(NULL, &config, info.host, info.port, info.identify_file);
		mw->connect_ssh();
		w = mw;
	} else {
		w = new ConnectionWindow(NULL, &config);
	}
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
	return a.exec();
}
