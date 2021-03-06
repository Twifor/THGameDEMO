#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QTimer>
#include <QMatrix4x4>
#include "openglgame.h"
#include <QThread>

class BackThread : public QThread {
	Q_OBJECT
public:
	BackThread(QObject *parent = nullptr);

	// QThread interface
protected:
	void run() override;
};

class GameWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core//游戏界面核心类
{
	Q_OBJECT
public:
	enum GameWidgetStatus {
		INIT,
		MAIN,
		PAUSE,
		BACK,
		BACK2
	};
	explicit GameWidget(QWidget *parent = nullptr);
	~GameWidget();
	void startUp();
	void startDown();
	void startLeft();
	void startRight();
	void endUp();
	void endDown();
	void endLeft();
	void endRight();
	void startShift();
	void endShift();
	void startZ();
	void endZ();
	void startX();
	static GameWidget *Instance;
	void addScore(int add);
	void addPower();
	void addPoint();
	void addSpell();
	void addLife();
	int getLevel();
	void quit();
	void reset();
	void preGame();
	void back();
	float getAlpha();

public slots:
signals:
	void done();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	int life, spellcard, score;//玩家信息
	int power, point, graze;//灵力，得点，擦弹
	int level;
	QOpenGLVertexArrayObject *bg_VAO, *ma_VAO;
	QOpenGLBuffer *bg_IBO, *bg_VBO, *ma_VBO;
	QOpenGLShader *bg_vs, *bg_fs, *ma_vs, *ma_fs;
	QOpenGLTexture *bg_texture, *bluestar_texture, *redstar_texture, *menu_texture;
	QOpenGLShaderProgram *bg_program, *ma_program;
	float totAlpha, loadingAlpha;
	QTimer timer;//控制游戏循环
	QPixmap *pixmap;//存放游戏元素贴图
	QMatrix4x4 *matrix;

	OpenGLGame *mainOpenGLGame;//主游戏界面对象
	GameWidgetStatus status;
	int numberOfFrames, ans;

	LARGE_INTEGER nEndTime, nBeginTime, nFrequency;
	double time;
	BackThread *backThread;
};

#endif // GAMEWIDGET_H
