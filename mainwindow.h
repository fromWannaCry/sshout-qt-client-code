#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QSettings;
class QProcess;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *, QSettings *, const QString &, quint16, const QString &);
	~MainWindow();

protected:
	bool eventFilter(QObject *, QEvent *);

private:
	Ui::MainWindow *ui;
	QProcess *ssh_process;
};

#endif // MAINWINDOW_H
