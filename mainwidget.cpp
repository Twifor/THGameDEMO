#include "mainwidget.h"
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>

void MainWidget::stopLoading()
{
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
		close();
	}
}

