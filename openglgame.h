#ifndef OPENGLGAME_H
#define OPENGLGAME_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <deque>

class OpenGLGame : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	explicit OpenGLGame(QWidget *parent = nullptr);
	static OpenGLGame *Instance;

	int status, myPlaneTimeLine, myPlaneOffset;
	float myPlaneX, myPlaneY;


signals:

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

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void drawBackGround();
	void drawMyPlane();
	float backGroundMax, treeLMax, treeRMax;
};

#endif // OPENGLGAME_H
