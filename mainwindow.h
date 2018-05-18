#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *, const QString &, int, const QString &);
	~MainWindow();

protected:
	bool eventFilter(QObject *, QEvent *);

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
