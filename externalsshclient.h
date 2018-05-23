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

class QTemporaryFile;

class ExternalSSHClient : public SSHClient {
	Q_OBJECT
public:
	ExternalSSHClient(QObject * = NULL);
	ExternalSSHClient(QObject *, const QString &);
	~ExternalSSHClient();
	void set_ssh_program_path(const QString &);
	void set_extra_args(const QStringList &);
	bool connect(const QString &, quint16, const QString &, const QString & = QString());
	void disconnect();
	void set_known_hosts(const QStringList &);
	void set_identify_file(const QString &);
	void setenv(const QString &, const QString &);
	void unsetenv(const QString &);
	void set_reconnect_interval(int);
	SSHState state();
	bool is_connected();
	bool atEnd() const;
	qint64 bytesAvailable() const;
	qint64 bytesToWrite() const;
	bool canReadLine() const;
	bool isSequential() const;
	bool waitForBytesWritten(int msecs);
	bool waitForReadyRead(int msecs);
	void register_ready_read_stderr_slot(QObject *, const char *, Qt::ConnectionType = Qt::AutoConnection);
	bool can_read_line_from_stderr();
	qint64 read_line_from_stderr(char *, qint64);
	QByteArray read_line_from_stderr(qint64 = 0);

protected:
	qint64 readData(char *, qint64);
	qint64 writeData(const char *, qint64);

/*
signals:
	void state_changed(SSHClient::SSHState);
	void connected();
	void disconnected(int);
	void readyRead();
*/

private:
	SSHState ssh_state;
	QString ssh_program_path;
	QStringList ssh_args;
	QStringList ssh_args_extra;
	QProcess *ssh_process;
	QStringList known_hosts;
	QTemporaryFile *temp_known_hosts_file;
	QString identify_file;
	QSet<QString> environment;
	int reconnect_interval;

private slots:
	void reconnect();
	void from_process_state_change(QProcess::ProcessState);
	void from_process_started();
	void from_process_finished(int);
	void from_process_ready_read();
};

#endif // EXTERNALSSHCLIENT_H
