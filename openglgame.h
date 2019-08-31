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
	void pause();
	void endPause();
	bool isPause();
	static OpenGLGame *Instance;

	//以下是数据接口，供event使用
	int status, myPlaneTimeLine, myPlaneOffset;
	float myPlaneX, myPlaneY;
	float slowEffectRotate1, slowEffectAlpha1, slowEffectRotate2, slowEffectAlpha2;
	float centerAlpha;
	float ballRotate;
	float ballX[5], ballY[5];
	int slowEffectTimeLine;
	int ballTimeLine, bulletTime;
	bool activeItems;//激活自动吸取道具

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
	void levelUp();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void drawBackGround();
	void drawMyPlane();
	void drawSlowEffect();
	void drawBalls();
	void drawLines();
	void drawBullets();
	float backGroundMax, treeLMax, treeRMax;
	float ballLine;
	bool pauseLock;
};

#endif // OPENGLGAME_H
