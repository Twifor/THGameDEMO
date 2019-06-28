#include "mainwidget.h"
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>

void MainWidget::stopLoading()//这里做加载完成后的跳转动画
{
	delete loadingThread;
//	menuWidget->show();
//	menuWidget->update();
	loadingWidget->destroy();
	MusicFactory::getInstance()->setBack(101200);//101413
	MusicFactory::getInstance()->play("bgm1.wav");//开始放标题曲吧
//	MusicFactory::getInstance()->seekTO(50000);
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

	menuWidget = new MenuWidget(this);
	menuWidget->setGeometry(0, 0, 800, 600);
	menuWidget->hide();

	connect(loadingThread, &LoadingThread::done, this, &MainWidget::stopLoading);
	connect(loadingWidget, &LoadingOpenGLWidget::done, [ = ](){
		loadingWidget->stop();
		QTimer::singleShot(1000, [ = ](){
			delete loadingWidget;
			menuWidget->show();
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
}

void MainWidget::closeEvent(QCloseEvent *event)
{
//	if(loadingThread->isRunning())loadingThread->exit(0);//这里还有问题，注意线程的及时退出[待修复]
	event->accept();
}

