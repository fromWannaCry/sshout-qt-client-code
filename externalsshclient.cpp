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

#include "externalsshclient.h"
#include <QtCore/QProcess>
#include <QtCore/QSet>
#include <QtCore/QTimer>
#include <QtCore/QTemporaryFile>
#include <QtCore/QDebug>

ExternalSSHClient::ExternalSSHClient(QObject *parent, const QString &ssh_program_path) : SSHClient(parent) {
	ssh_state = DISCONNECTED;
	this->ssh_program_path = ssh_program_path;
	ssh_process = new QProcess(this);
	environment = ssh_process->systemEnvironment().toSet();
	reconnect_interval = -1;
	temp_known_hosts_file = NULL;
	QObject::connect(ssh_process, SIGNAL(stateChanged(QProcess::ProcessState)), SLOT(from_process_state_change(QProcess::ProcessState)));
	QObject::connect(ssh_process, SIGNAL(started()), SLOT(from_process_started()));
	QObject::connect(ssh_process, SIGNAL(finished(int)), SLOT(from_process_finished(int)));
	QObject::connect(ssh_process, SIGNAL(readyReadStandardOutput()), SLOT(from_process_ready_read()));
}

ExternalSSHClient::~ExternalSSHClient() {
	delete temp_known_hosts_file;
}

void ExternalSSHClient::set_ssh_program_path(const QString &path) {
	ssh_program_path = path;
}

void ExternalSSHClient::set_extra_args(const QStringList &args) {
	ssh_args_extra = args;
}

bool ExternalSSHClient::connect(const QString &host, quint16 port, const QString &user, const QString &command) {
	if(ssh_program_path.isEmpty()) return false;
	if(ssh_state != DISCONNECTED) {
		qWarning("ExternalSSHClient::connect: current state is not disconnected");
		return false;
	}
	if(host[0] == '-') {
		qWarning("ExternalSSHClient::connect: invalid host name");
		return false;
	}
	ssh_process->setEnvironment(environment.toList());
	ssh_args.clear();
	//ssh_args << "-o" << "BatchMode=yes";
	ssh_args << "-o" << "ConnectTimeout=30";
	ssh_args << "-o" << "ServerAliveInterval=60";
	ssh_args << "-o" << "StrictHostKeyChecking=yes";
	if(!known_hosts.isEmpty()) {
		temp_known_hosts_file = new QTemporaryFile;
		temp_known_hosts_file->open();
		foreach(const QString &i, known_hosts) {
			QByteArray bytes = i.toLocal8Bit().append('\n');
			temp_known_hosts_file->write(bytes);
		}
		temp_known_hosts_file->close();
		ssh_args << "-o" << "UserKnownHostsFile=" + temp_known_hosts_file->fileName();
	}
	ssh_args << "-o" << "ChallengeResponseAuthentication=no";
	ssh_args << "-o" << "PasswordAuthentication=no";
	ssh_args << "-o" << "PubkeyAuthentication=yes";
	ssh_args << host;
	ssh_args << "-p" << QString::number(port);
	ssh_args << "-l" << user;
	if(!identify_file.isEmpty()) ssh_args << "-i" << identify_file;
	ssh_args << "-T";
	if(!ssh_args_extra.isEmpty()) ssh_args << ssh_args_extra;
	if(!command.isEmpty()) ssh_args << "--" << command;
	ssh_process->start(ssh_program_path, ssh_args, QIODevice::ReadWrite);
	//QIODevice::open(QIODevice::ReadWrite | QIODevice::Unbuffered);
	QIODevice::open(QIODevice::ReadWrite);
	return true;
}

void ExternalSSHClient::disconnect() {
	//qDebug("function: ExternalSSHClient::disconnect()");
	ssh_process->terminate();
	ssh_process->close();
	QIODevice::close();
}

void ExternalSSHClient::reconnect() {
	if(ssh_program_path.isEmpty()) return;
	if(ssh_args.isEmpty()) return;
	ssh_process->start(ssh_program_path, ssh_args);
}

void ExternalSSHClient::set_known_hosts(const QStringList &list) {
	known_hosts = list;
}

void ExternalSSHClient::set_identify_file(const QString &path) {
	identify_file = path;
}

void ExternalSSHClient::setenv(const QString &name, const QString &value) {
	environment.insert(QString("%1=%2").arg(name).arg(value));
}

void ExternalSSHClient::unsetenv(const QString &name) {
	environment.remove(name);
}

void ExternalSSHClient::set_reconnect_interval(int v) {
	reconnect_interval = v;
}

SSHClient::SSHState ExternalSSHClient::state() {
	QProcess::ProcessState s = ssh_process->state();
	switch(s) {
		case QProcess::NotRunning:
			return DISCONNECTED;
		case QProcess::Starting:
			return CONNECTIING;
		case QProcess::Running:
			// Currently not way to check whether the authentication is done
			return AUTHENTICATED;
		default:
			qFatal("QProcess::state returned surprising value %d", s);
			return DISCONNECTED;
	}
}

bool ExternalSSHClient::is_connected() {
	return ssh_process->state() == QProcess::Running;
}

bool ExternalSSHClient::atEnd() const {
	return ssh_process->atEnd();
}

qint64 ExternalSSHClient::bytesAvailable() const {
	return ssh_process->bytesAvailable() + QIODevice::bytesAvailable();
}

qint64 ExternalSSHClient::bytesToWrite() const {
	return ssh_process->bytesToWrite();
}

bool ExternalSSHClient::canReadLine() const {
	return ssh_process->canReadLine();
}

bool ExternalSSHClient::isSequential() const {
	return true;
}

void ExternalSSHClient::register_ready_read_stderr_slot(QObject *receiver, const char *slot, Qt::ConnectionType type) {
	//qDebug("function: ExternalSSHClient::register_ready_read_stderr_slot(%p, %p<%s>, %d)", receiver, slot, slot, type);
	QObject::connect(ssh_process, SIGNAL(readyReadStandardError()), receiver, slot, type);
}

bool ExternalSSHClient::can_read_line_from_stderr() {
	ssh_process->setReadChannel(QProcess::StandardError);
	bool r = ssh_process->canReadLine();
	ssh_process->setReadChannel(QProcess::StandardOutput);
	return r;
}

qint64 ExternalSSHClient::read_line_from_stderr(char *buffer, qint64 max_len) {
	ssh_process->setReadChannel(QProcess::StandardError);
	qint64 r = ssh_process->readLine(buffer, max_len);
	ssh_process->setReadChannel(QProcess::StandardOutput);
	return r;
}

QByteArray ExternalSSHClient::read_line_from_stderr(qint64 max_len) {
	ssh_process->setReadChannel(QProcess::StandardError);
	QByteArray r = ssh_process->readLine(max_len);
	ssh_process->setReadChannel(QProcess::StandardOutput);
	return r;
}

bool ExternalSSHClient::waitForBytesWritten(int msecs) {
	return ssh_process->waitForBytesWritten(msecs);
}

bool ExternalSSHClient::waitForReadyRead(int msecs) {
	return ssh_process->waitForReadyRead(msecs);
}

qint64 ExternalSSHClient::readData(char *data, qint64 maxlen) {
	return ssh_process->read(data, maxlen);
}

qint64 ExternalSSHClient::writeData(const char *data, qint64 len) {
	//qDebug("function: ExternalSSHClient::writeData(%p, %lld)", data, (long long int)len);
	return ssh_process->write(data, len);
}

void ExternalSSHClient::from_process_state_change(QProcess::ProcessState proc_state) {
	qDebug("slot: ExternalSSHClient::from_process_state_change(%d)", proc_state);
	switch(proc_state) {
		case QProcess::NotRunning:
			ssh_state = DISCONNECTED;
			break;
		case QProcess::Starting:
			ssh_state = CONNECTIING;
			break;
		case QProcess::Running:
			//ssh_state = AUTHENTICATING;
			//break;
			emit state_changed(AUTHENTICATING);
			emit state_changed(AUTHENTICATED);
			return;
	}
	emit state_changed(ssh_state);
	if(proc_state == QProcess::NotRunning && reconnect_interval >= 0) {
		QTimer::singleShot(reconnect_interval * 1000, this, SLOT(reconnect()));
	}
}

void ExternalSSHClient::from_process_started() {
	emit connected();
}

void ExternalSSHClient::from_process_finished(int status) {
	delete temp_known_hosts_file;
	temp_known_hosts_file = NULL;
	emit disconnected(status);
}

void ExternalSSHClient::from_process_ready_read() {
	//qDebug("slot: ExternalSSHClient::from_process_ready_read()");
	emit readyRead();
}
