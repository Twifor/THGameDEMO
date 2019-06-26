#include "mainwidget.h"
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>

void MainWidget::stopLoading()
{
	timer->stop();
}

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	setCursor(Qt::BlankCursor);

	loadingWidget = new LoadingWidget(this);
	loadingWidget->setGeometry(620, 500, 150, 80);

	setWindowTitle("THGAMEV1.0");
	setFocusPolicy(Qt::StrongFocus);
	showFullScreen();

	timer = new QTimer;
	timer->setInterval(20);
	timer->start();

	connect(timer, &QTimer::timeout, [ = ](){
		update();
	});
}

MainWidget::~MainWidget()
{
	delete timer;
}

void MainWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, 800, 600, QPixmap(":/std/bg.bmp"));
//	loadingAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
	if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_F4) {//Alt+F4强行退出游戏
		close();
	}
}

LoadingWidget::LoadingWidget(QWidget *parent) : QWidget (parent)
{

}

void LoadingWidget::paintEvent(QPaintEvent *)
{
	static int steps = 0;
	static bool flags = false;
	steps += 20;
	steps %= 500;
	if(steps == 0) flags = !flags;
	QPainter painter(this);
	if(!flags) painter.setOpacity(steps / 500.0);
	else painter.setOpacity(1.0 - steps / 500.0);
	painter.drawPixmap(0, 0, width(), height(), QPixmap(":/std/loading.png"));
}
