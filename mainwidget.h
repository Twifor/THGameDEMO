#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include "loadingwidget.h"
#include "menuwidget.h"
#include "musicfactory.h"
#include "gamewidget.h"

class MainWidget : public QWidget//主控件，负责与用户交互，并向各部件发送数据
{
	Q_OBJECT
	enum GameStatus {//设置状态，状态机模型的体现
		START_LOADING,//一开始的加载状态
		MENU,//菜单状态
		GameStart//游戏进行中状态
	};

public slots:
	void stopLoading();
	void gameStart();

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget()override;

	// QWidget interface
protected:
	void paintEvent(QPaintEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private:
	LoadingOpenGLWidget *loadingWidget;
	LoadingThread *loadingThread;
	MenuWidget *menuWidget;
	GameStatus status;//游戏状态
	GameWidget *gameWidget;
};

#endif // MAINWIDGET_H
