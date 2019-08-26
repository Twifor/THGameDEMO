#ifndef MAINGAME_H
#define MAINGAME_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QGraphicsView>
#include <QOpenGLWidget>
#include <QTimer>
#include <QTime>

class MainGame : public QGraphicsView, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	MainGame(QWidget *parent);
	~MainGame();

public slots:
	void startLeft();
	void startRight();
	void startUp();
	void startDown();
	void endtLeft();
	void endRight();
	void endUp();
	void endDown();
	void startShift();
	void endShift();
	void startZ();
	void endZ();


private:
	QOpenGLWidget *openGL;
	Scene *scene;
	QTimer *timer;

	long long lastTime;
	QDateTime *time;

};

#endif // MAINGAME_H
