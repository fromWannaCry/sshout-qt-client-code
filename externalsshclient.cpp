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

ExternalSSHClient::ExternalSSHClient(QObject *parent, const QString &ssh_program_path) : SSHClient(parent) {
	ssh_state = DISCONNECTED;
	this->ssh_program_path = ssh_program_path;
	ssh_process = new QProcess(this);
	environment = ssh_process->systemEnvironment().toSet();
	QObject::connect(ssh_process, SIGNAL(started()), SLOT(from_process_started()));
	QObject::connect(ssh_process, SIGNAL(finished(int)), SLOT(from_process_finished()));
	QObject::connect(ssh_process, SIGNAL(readyReadStandardOutput()), SLOT(from_process_ready_read()));
}

void ExternalSSHClient::set_ssh_program_path(const QString &path) {
	ssh_program_path = path;
}

bool ExternalSSHClient::connect(const QString &host, quint16 port, const QString &user, const QString &command) {
	if(ssh_program_path.isEmpty()) return false;
	ssh_process->setEnvironment(environment.toList());
	ssh_args.clear();
	ssh_args << "-o" << "BatchMode=yes";
	ssh_args << "-o" << "ServerAliveInterval=60";
	ssh_args << host;
	ssh_args << "-p" << QString::number(port);
	ssh_args << "-l" << user;
	if(!identify_file.isEmpty()) ssh_args << "-i" << identify_file;
	ssh_args << "-T";
	if(!command.isEmpty()) ssh_args << command;
	ssh_process->start(ssh_program_path, ssh_args);
	//ssh_process->waitForStarted();
	return true;
}

void ExternalSSHClient::disconnect() {
	ssh_process->terminate();
	ssh_process->close();
}

void ExternalSSHClient::reconnect() {
	if(ssh_program_path.isEmpty()) return;
	if(ssh_args.isEmpty()) return;
	ssh_process->start(ssh_program_path, ssh_args);
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

bool ExternalSSHClient::atEnd() const {
	return ssh_process->atEnd();
}

qint64 ExternalSSHClient::bytesAvailable() const {
	return ssh_process->bytesAvailable();
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

void ExternalSSHClient::register_ready_read_stderr_slot(const char *slot, Qt::ConnectionType type) {
	QObject::connect(ssh_process, SIGNAL(readyReadStandardError()), slot, type);
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
	return ssh_process->write(data, len);
}

void ExternalSSHClient::from_process_state_change(QProcess::ProcessState proc_state) {
	switch(proc_state) {
		case QProcess::NotRunning:
			ssh_state = DISCONNECTED;
			break;
		case QProcess::Starting:
			ssh_state = CONNECTIING;
			break;
		case QProcess::Running:
			//ssh_state = AUTHENTICATING;
			emit state_changed(AUTHENTICATING);
			emit state_changed(AUTHENTICATED);
			break;
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
	emit disconnected(status);
}

void ExternalSSHClient::from_process_ready_read() {
	emit readyRead();
}
