#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::use_internet_ssh_library(bool t) {
	ui->lineEdit_ssh_program->setEnabled(!t);
	ui->lineEdit_ssh_args->setEnabled(!t);
	ui->environment_list->setEnabled(!t);
	ui->button_add_item->setEnabled(!t);
	ui->button_remove_item->setEnabled(!t);
}
