#include "mainwidget.h"
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>

void MainWidget::stopLoading()//这里做加载完成后的跳转动画
{
	delete loadingThread;
	loadingWidget->destroy();
}

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	loadingWidget = new LoadingOpenGLWidget(this);

	setFixedSize(800, 600);
	setCursor(Qt::BlankCursor);

	loadingWidget->setGeometry(0, 0, 800, 600);

	setWindowTitle("THGAMEV1.0");
	setFocusPolicy(Qt::StrongFocus);
//	showFullScreen();
//	show();

	loadingThread = new LoadingThread(this);
	connect(loadingThread, &LoadingThread::done, this, &MainWidget::stopLoading);
	connect(loadingWidget, &LoadingOpenGLWidget::done, [ = ](){
		loadingWidget->stop();
		QTimer::singleShot(1000, [ = ](){
			delete loadingWidget;
		});
	});
	loadingThread->start();
}

MainWidget::~MainWidget()
{

}

void MainWidget::paintEvent(QPaintEvent *event)
{

}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
	if(event->modifiers() == Qt::AltModifier && event->key() == Qt::Key_F4) {//Alt+F4强行退出游戏
		if(loadingThread->isRunning())loadingThread->exit();
		close();
	}
}

