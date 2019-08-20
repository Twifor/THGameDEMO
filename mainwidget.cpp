#include "mainwidget.h"
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include "gameresource.h"
#include "gamerule.h"

void MainWidget::stopLoading()//这里做加载完成后的跳转动画
{
	delete loadingThread;
//	menuWidget->show();
//	menuWidget->update();
	loadingWidget->destroy();
	MusicFactory::getInstance()->play(0);
//	MusicFactory::getInstance()->setBack(101200);//101413
//	MusicFactory::getInstance()->play("res/bgm1.wav");//开始放标题曲，无缝循环播放逻辑真心头疼
	//	MusicFactory::getInstance()->seekTO(100000);
}

void MainWidget::gameStart()//开始游戏函数
{
	qDebug() << "Game Start";
	status = GameStart;//修改状态机状态
	gameWidget = new GameWidget(this);
	gameWidget->setGeometry(0, 0, 800, 600);
	gameWidget->show();

	MusicFactory::getInstance()->quit();//放bgm
	GameRule::bgmVolume = GameRule::defaultBgmVolume;
	GameRule::update();
	MusicFactory::getInstance()->play(1);
	QTimer::singleShot(1000, [&](){
		delete menuWidget;//释放菜单界面对象
	});
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

	menuWidget = new MenuWidget(this);
	gameWidget = nullptr;

	menuWidget->setGeometry(0, 0, 800, 600);
	menuWidget->hide();
	connect(menuWidget, &MenuWidget::close, this, &MainWidget::close);
	connect(menuWidget, &MenuWidget::start, this, &MainWidget::gameStart);

	loadingThread = new LoadingThread(this);
	connect(loadingThread, &LoadingThread::done, this, &MainWidget::stopLoading);
	connect(loadingWidget, &LoadingOpenGLWidget::done, [ = ](){
		loadingWidget->stop();
		QTimer::singleShot(0, [ = ](){
			delete loadingWidget;
			menuWidget->show();
			status = MENU;
		});
	});

	status = START_LOADING;

	loadingThread->start();
}

MainWidget::~MainWidget()
{
	GameResource::getInstance()->destroy();
	MusicRoom::destroy();
	MusicFactory::getInstance()->destroy();
}

void MainWidget::paintEvent(QPaintEvent *event)
{

}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_Shift && status == GameStart) {
		gameWidget->startShift();
	}else if(event->key() == Qt::Key_Up) {
		if(status == MENU && !event->isAutoRepeat()) menuWidget->up();
		else if(status == GameStart && !event->isAutoRepeat()) gameWidget->startUp();
	}else if(event->key() == Qt::Key_Down) {
		if(status == MENU && !event->isAutoRepeat()) menuWidget->down();
		else if(status == GameStart && !event->isAutoRepeat()) gameWidget->startDown();
	}else if(event->key() == Qt::Key_Z) {
		if(status == MENU && !event->isAutoRepeat()) menuWidget->ok();
		else if(status == GameStart && !event->isAutoRepeat()) gameWidget->startZ();
	}else if(event->key() == Qt::Key_Escape) {
		if(status == MENU && !event->isAutoRepeat()) menuWidget->quitWindow();
	}else if(event->key() == Qt::Key_Left) {
		if(status == MENU && !event->isAutoRepeat()) menuWidget->left();
		else if(status == GameStart && !event->isAutoRepeat()) gameWidget->startLeft();
	}else if(event->key() == Qt::Key_Right) {
		if(status == MENU && !event->isAutoRepeat()) menuWidget->right();
		else if(status == GameStart && !event->isAutoRepeat()) gameWidget->startRight();
	}
}

void MainWidget::closeEvent(QCloseEvent *event)
{
//	if(loadingThread->isRunning())loadingThread->exit(0);//这里还有问题，注意线程的及时退出[待修复]
	event->accept();
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
	if(event->isAutoRepeat()) {
		event->accept();
		return;
	}
	if(status == GameStart && event->key() == Qt::Key_Shift) gameWidget->endShift();
	else if(status == GameStart && event->key() == Qt::Key_Left) gameWidget->endLeft();
	else if(status == GameStart && event->key() == Qt::Key_Right) gameWidget->endRight();
	else if(status == GameStart && event->key() == Qt::Key_Up) gameWidget->endUp();
	else if(status == GameStart && event->key() == Qt::Key_Down) gameWidget->endDown();
	else if(status == GameStart && event->key() == Qt::Key_Z) gameWidget->endZ();
}

