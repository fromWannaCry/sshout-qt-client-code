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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtCore/qglobal.h>
#if QT_VERSION < 0x050000
#include <QtGui/QDialog>
#else
#include <QtWidgets/QDialog>
#endif
#include <QtCore/QHash>

class QSettings;

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
	Q_OBJECT

public:
	explicit SettingsDialog(QWidget *, QSettings *);
	~SettingsDialog();
	void set_current_tab(int);
	void add_environment_variable(const QString &, const QString &);

private:
	Ui::SettingsDialog *ui;
	QSettings *config;
	QHash<QString, QString> language_code_to_name;

private slots:
	void use_internal_ssh_library_checked(bool);
	void add_environment_variable();
	void remove_environment_variable();
	void browse_ssh_program();
	void save_settings();
};

#endif // SETTINGSDIALOG_H
