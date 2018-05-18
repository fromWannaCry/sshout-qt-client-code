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

#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QtGui/QDialog>

namespace Ui {
class ConnectionWindow;
}

class ConnectionWindow : public QDialog
{
Q_OBJECT

public:
	explicit ConnectionWindow(QWidget *parent = 0);
	~ConnectionWindow();

private:
	Ui::ConnectionWindow *ui;

private slots:
	void browse_identity_file();
};

#endif // CONNECTIONWINDOW_H
