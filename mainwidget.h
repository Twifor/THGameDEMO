#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include "loadingwidget.h"
#include "menuwidget.h"
#include "musicfactory.h"

class MainWidget : public QWidget//主控件，负责与用户交互，并向各部件发送数据
{
	Q_OBJECT
	enum GameStatus{
		START_LOADING,
		MENU
	};

public slots:
	void stopLoading();

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
};

#endif // MAINWIDGET_H
