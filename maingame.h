#ifndef MAINGAME_H
#define MAINGAME_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QGraphicsView>
#include <QOpenGLWidget>
#include <QTimer>
#include "scene.h"

class MainGame : public QGraphicsView, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	MainGame(QWidget *parent);
	~MainGame();

	// QOpenGLWidget interface


private:
	QOpenGLWidget *openGL;
	Scene *scene;
	QTimer *timer;

};

#endif // MAINGAME_H
