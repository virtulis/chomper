#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

enum class ChompState {
	init,
	drag,
	dragWindow,
	done
};

namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void mouseMoveEvent(QMouseEvent *ev);
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);

private:
	Ui::MainWindow *ui;
	QPixmap pixmap;
	ChompState state = ChompState::init;
	int x0 = 0;
	int y0 = 0;
	int x1 = 0;
	int y1 = 0;
	QRect rect;
	void shoot();
	void setRect(QMouseEvent *ev);

};

#endif // MAINWINDOW_H
