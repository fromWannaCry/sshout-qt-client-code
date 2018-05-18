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
#include "ui_connectionwindow.h"
#include <QtGui/QCompleter>
#include <QtGui/QFileDialog>
#include <QtCore/QFile>
#include <QtCore/QDir>

ConnectionWindow::ConnectionWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ConnectionWindow)
{
	ui->setupUi(this);
	QCompleter *completer = new QCompleter(this);
	completer->setCompletionMode(QCompleter::PopupCompletion);
	ui->remote_host_comboBox->setCompleter(completer);
}

ConnectionWindow::~ConnectionWindow()
{
	delete ui;
}

static QString ssh_config_dir() {
#ifdef Q_OS_WINCE
	return QApplication::applicationDirPath();
#else
	return QDir::homePath() + "/.ssh";
#endif
}

void ConnectionWindow::browse_identity_file() {
	QFileDialog d(this, tr("Choose the identity"), ssh_config_dir());
	d.setAcceptMode(QFileDialog::AcceptOpen);
	d.setFileMode(QFileDialog::ExistingFile);
	d.setOption(QFileDialog::DontUseNativeDialog);
	if(d.exec()) {
		ui->identify_file_lineEdit->setText(d.selectedFiles()[0]);
	}
}
