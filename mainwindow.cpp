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

#include "common.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#ifdef HAVE_OPENSSH_LIBRARY
#include "internalsshclient.h"
#endif
#include "externalsshclient.h"
#include <QtCore/QSettings>
#include <QtCore/QTimer>
//#include <QtGui/QRubberBand>

MainWindow::MainWindow(QWidget *parent, QSettings *config, const QString &host, quint16 port, const QString &identify_file) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//setMouseTracking(true);
#ifdef HAVE_OPENSSH_LIBRARY
	use_internal_ssh_library = config->value("UseInternalSSHLibrary", false).toBool();
	if(use_internal_ssh_library) ssh_client = new InternalSSHClient(this);
	else
#endif
	ssh_client = new ExternalSSHClient(this, config->value("SSHProgramPath", DEFAULT_SSH_PROGRAM_PATH).toString());
	ssh_client->set_identify_file(identify_file);
	connect_ssh();
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::eventFilter(QObject *o, QEvent *e) {
	qDebug("function: MainWindow::eventFilter(%p, %p)", o, e);
	return true;
}

void MainWindow::connect_ssh() {
	ssh_client->connect(host, port, DEFAULT_SSH_USER_NAME, "api");
}

void MainWindow::send_hello() {

}

void MainWindow::on_ssh_state_change(SSHClient::SSHState state) {
	switch(state) {
		case SSHClient::DISCONNECTED:
			ui->statusbar->showMessage(tr("Disconnected"));
			QTimer::singleShot(10000, this, SLOT(connect_ssh()));
			break;
		case SSHClient::CONNECTIING:
			if(use_internal_ssh_library) ui->statusbar->showMessage(tr("Connecting"));
			break;
		case SSHClient::AUTHENTICATING:
			if(use_internal_ssh_library) ui->statusbar->showMessage(tr("Authenticating"));
			break;
		case SSHClient::AUTHENTICATED:
			ui->statusbar->showMessage(tr("Connected"));
			send_hello();
			break;
	}
}
