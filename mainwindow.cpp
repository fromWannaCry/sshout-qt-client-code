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
#include <QtGui/QRubberBand>

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
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::eventFilter(QObject *o, QEvent *e) {
	qDebug("function: MainWindow::eventFilter(%p, %p)", o, e);
	return true;
}
