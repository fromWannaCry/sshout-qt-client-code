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

#ifndef EXTERNALSSHCLIENT_H
#define EXTERNALSSHCLIENT_H

#include "sshclient.h"
#include <QtCore/QSet>
#include <QtCore/QProcess>

class ExternalSSHClient : public SSHClient {
	Q_OBJECT
public:
	ExternalSSHClient(QObject * = NULL);
	ExternalSSHClient(QObject *, const QString &);
	void set_ssh_program_path(const QString &);
	bool connect(const QString &, quint16, const QString &, const QString & = QString());
	void set_identify_file(const QString &);
	void setenv(const QString &, const QString &);
	void unsetenv(const QString &);
	void set_reconnect_interval(int);
	qint64 bytesAvailable() const;

protected:
	qint64 readData(char *, qint64);
	qint64 writeData(const char *, qint64);

signals:
	void state_changed(SSHState);

private:
	SSHState ssh_state;
	QString ssh_program_path;
	QStringList ssh_args;
	QProcess *ssh_process;
	QString identify_file;
	QSet<QString> environment;
	int reconnect_interval;

private slots:
	void reconnect();
	void from_process_state_change(QProcess::ProcessState);
};

#endif // EXTERNALSSHCLIENT_H
