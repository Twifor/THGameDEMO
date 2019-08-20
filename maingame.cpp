#include "gameresource.h"
#include "maingame.h"
#include <QPainter>

MainGame::MainGame(QWidget *parent) : QGraphicsView (parent)
{
	openGL = new QOpenGLWidget;
	scene = new Scene(nullptr);
	setViewport(openGL);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setScene(scene);

	setStyleSheet("border:0px solid red");
	timer = new QTimer;
	timer->setInterval(1000 / 60);
	connect(timer, &QTimer::timeout, [ = ](){
		scene->update();
	});
	timer->start();
	scene->setSceneRect(0, 0, 487, 557);
	setStyleSheet("padding: 0px; border: 0px;");
}

MainGame::~MainGame()
{
	delete scene;
	delete openGL;
	delete timer;
}

void MainGame::startLeft()
{
	scene->satrtLeft();
}

void MainGame::startRight()
{
	scene->startRight();
}

void MainGame::startUp()
{
	scene->startUp();
}

void MainGame::startDown()
{
	scene->startDown();
}

void MainGame::endtLeft()
{
	scene->endLeft();
}

void MainGame::endRight()
{
	scene->endRight();
}

void MainGame::endUp()
{
	scene->endUp();
}

void MainGame::endDown()
{
	scene->endDown();
}

void MainGame::startShift()
{
	scene->startShift();
}

void MainGame::endShift()
{
	scene->endShift();
}

void MainGame::startZ()
{
	scene->startZ();
}

void MainGame::endZ()
{
	scene->endZ();
}

