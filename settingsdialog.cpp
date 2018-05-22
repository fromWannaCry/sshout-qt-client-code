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
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QtCore/QSettings>
#if QT_VERSION < 0x050000
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFont>
#else
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#endif

SettingsDialog::SettingsDialog(QWidget *parent, QSettings *config) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
#ifdef HAVE_OPENSSH_LIBRARY
	bool use_internal_ssh_library = config->value("UseInternalSSHLibrary", false).toBool()
	ui->checkBox_use_internal_ssh_library->setChecked(use_internet_ssh_library);
	use_internet_ssh_library_checked(use_internal_ssh_library);
#else
	bool use_internal_ssh_library = false;
	ui->checkBox_use_internal_ssh_library->setEnabled(false);
	ui->checkBox_use_internal_ssh_library->setChecked(false);
#endif
	if(!use_internal_ssh_library) {
		bool use_separate_known_hosts = config->value("UseSeparateKnownHosts", false).toBool();
		ui->checkBox_use_separete_known_hosts->setChecked(use_separate_known_hosts);
		if(!use_separate_known_hosts) ui->textEdit_known_hosts->setEnabled(false);
	}

	ui->textEdit_known_hosts->setPlainText(config->value("KnownHosts").toString());
	ui->lineEdit_ssh_program->setText(config->value("SSHProgramPath", DEFAULT_SSH_PROGRAM_PATH).toString());
	ui->lineEdit_ssh_args->setText(config->value("SSHArgs").toString());

	config->beginGroup("SSHEnvironment");
	QStringList key_list = config->allKeys();
	foreach(const QString &key, key_list) {
		add_environment_variable(key, config->value(key).toString());
	}
	config->endGroup();

	config->beginGroup("Text");
#if 1
	QVariant font = config->value("DefaultFontFamily");
	if(!font.isNull()) ui->fontComboBox->setCurrentFont(font.value<QFont>());
#else
	QString font_name = config->value("DefaultFontFamily").toString();
	if(!font_name.isEmpty()) ui->fontComboBox->setCurrentFont(QFont(font_name));
#endif
	ui->spinBox_font_size->setValue(config->value("DefaultFontSize", 2).toInt());
	config->endGroup();

	this->config = config;
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::set_current_tab(int index) {
	ui->tabWidget->setCurrentIndex(index);
}

void SettingsDialog::use_internal_ssh_library_checked(bool t) {
	ui->lineEdit_ssh_program->setEnabled(!t);
	ui->lineEdit_ssh_args->setEnabled(!t);
	ui->environment_list->setEnabled(!t);
	ui->button_add_env_item->setEnabled(!t);
	ui->button_remove_env_item->setEnabled(!t);
	ui->checkBox_use_separete_known_hosts->setEnabled(!t);
	ui->checkBox_use_separete_known_hosts->setChecked(t ? : config->value("UseSeparateKnownHosts", false).toBool());
}

void SettingsDialog::add_environment_variable(const QString &name, const QString &value) {
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	item->setText(0, name);
	item->setText(1, value);
	ui->environment_list->addTopLevelItem(item);
}

void SettingsDialog::add_environment_variable() {
	add_environment_variable(QString(), QString());
}

void SettingsDialog::remove_environment_variable() {
	QTreeWidgetItem *current = ui->environment_list->currentItem();
	if(!current) return;
	delete current;
}

void SettingsDialog::browse_ssh_program() {
	QFileDialog d(this, tr("Choose SSH client program to use"));
	d.setAcceptMode(QFileDialog::AcceptOpen);
	d.setFileMode(QFileDialog::ExistingFile);
	d.setOption(QFileDialog::DontUseNativeDialog);
	if(d.exec()) {
		QString path = d.selectedFiles()[0];
		if(!QFileInfo(path).isExecutable()) {
			QMessageBox::warning(this, tr("Warning"), tr("File %1 is not executable").arg(path));
		}
		ui->lineEdit_ssh_program->setText(path);
	}
}

void SettingsDialog::save_settings() {
#ifdef HAVE_OPENSSH_LIBRARY
	bool use_internal_ssh_library = ui->checkBox_use_internal_ssh_library->isChecked();
	config->setValue("UseInternalSSHLibrary", use_internal_ssh_library);
#else
	bool use_internal_ssh_library = false;
#endif
	if(!use_internal_ssh_library) config->setValue("UseSeparateKnownHosts", ui->checkBox_use_separete_known_hosts->isChecked());
	config->setValue("KnownHosts", ui->textEdit_known_hosts->toPlainText());
	config->setValue("SSHProgramPath", ui->lineEdit_ssh_program->text());
	config->setValue("SSHArgs", ui->lineEdit_ssh_args->text());
	config->beginGroup("SSHEnvironment");
	config->remove("");
	int i, m = ui->environment_list->topLevelItemCount();
	for(i=0; i<m; i++) {
		const QString &name = ui->environment_list->topLevelItem(i)->text(0);
		const QString &value = ui->environment_list->topLevelItem(i)->text(1);
		if(!name.isEmpty()) config->setValue(name, value);
	}
	config->endGroup();
	config->beginGroup("Text");
	config->setValue("DefaultFontFamily", ui->fontComboBox->currentFont());
	config->setValue("DefaultFontSize", ui->spinBox_font_size->value());
	config->endGroup();
}
