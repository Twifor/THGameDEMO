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
	~OpenGLGame();
	void pause();
	void endPause();
	bool isPause();
	bool isPauseResponse();
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
	float offsetX, offsetY;
	bool activeItems, isBlack;//激活自动吸取道具
	int pauseStatus;
	short int spellCard;

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
	void startX();
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
	void dealSomeThing();
	void drawSpellCard();
	float backGroundMax, treeLMax, treeRMax;
	float ballLine;
	bool pauseLock;
	int pauseTimeOut;
};

#endif // OPENGLGAME_H
