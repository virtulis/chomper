#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMouseEvent>
#include <QScreen>
#include <QWindow>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{

	ui->setupUi(this);
	setWindowIcon(QIcon(":/icons/chomper.png"));
	setWindowTitle("Chomper");
	setAttribute(Qt::WA_QuitOnClose);

	shoot();

	setMouseTracking(true);
	centralWidget()->setMouseTracking(true);
	ui->view->setMouseTracking(true);

}

MainWindow::~MainWindow()
{
	delete ui;

}

void MainWindow::shoot()
{

	hide();

	QScreen *screen = QGuiApplication::primaryScreen();
	if (const QWindow *window = windowHandle())
		screen = window->screen();
	if (!screen)
		return;

	pixmap = screen->grabWindow(0);
	auto ow = pixmap.width();
	auto oh = pixmap.height();

	showFullScreen();

	ui->view->setGeometry(0, 0, ow, oh);
	ui->view->setPixmap(pixmap);

	show();
	state = ChompState::init;

}

void MainWindow::setRect(QMouseEvent *ev)
{
	x1 = ev->x();
	y1 = ev->y();
	rect.setCoords(
		std::min(x0, x1),
		std::min(y0, y1),
		std::max(x0, x1),
		std::max(y0, y1)
	);
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
	if (state == ChompState::init) {
		ui->frame->setGeometry(ev->x(), ev->y(), 16, 16);
	}
	else if (state == ChompState::drag) {
		setRect(ev);
		ui->frame->setGeometry(rect);
	}
	else if (state == ChompState::dragWindow) {
		move(
			x() + (ev->globalX() - x0),
			y() + (ev->globalY() - y0)
		);
		x0 = ev->globalX();
		y0 = ev->globalY();
	}
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
	if (state == ChompState::init) {
		state = ChompState::drag;
		x0 = ev->x();
		y0 = ev->y();
		ui->frame->setGeometry(rect);
	}
	else if (state == ChompState::done) {
		state = ChompState::dragWindow;
		x0 = ev->globalX();
		y0 = ev->globalY();
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *ev)
{

	if (state == ChompState::dragWindow) {
		state = ChompState::done;
		return;
	}

	if (state != ChompState::drag) return;

	state = ChompState::done;
	setRect(ev);

	centralWidget()->setMouseTracking(false);
	ui->view->setMouseTracking(false);
	setMouseTracking(false);
	showNormal();

	pixmap = pixmap.copy(rect);
	ui->view->setPixmap(pixmap);
	ui->view->setGeometry(0, 0, rect.width(), rect.height());
	ui->frame->hide();

	setFixedSize(rect.width(), rect.height());
	setWindowFlag(Qt::WindowStaysOnTopHint);
	show();
	move(rect.x() + rect.width() / 2, rect.y() + rect.width() / 2);

	setWindowIcon(QIcon(pixmap.scaled(64, 64, Qt::KeepAspectRatio)));

}


