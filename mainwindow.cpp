#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/QSettings>
#include <QtGui/QRubberBand>

MainWindow::MainWindow(QWidget *parent, QSettings *config, const QString &host, quint16 port, const QString &identify_file) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	//setMouseTracking(true);
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::eventFilter(QObject *o, QEvent *e) {
	qDebug("function: MainWindow::eventFilter(%p, %p)", o, e);
	return true;
}
