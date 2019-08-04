#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QThread>
#include "musicroom.h"

#define PARTICLE_MAX_NUM 1000
#define ANIMATION 0.005f

const float X[] = {1, -1, 1, -1, 1, -1, 1, -1};//从竞赛里学的写法
const float Y[] = {1, 1, -1, -1, 1, 1, -1, -1};

class MainGameLoadingThread : public QThread {//Game Start游戏资源加载线程
	Q_OBJECT
public:
	explicit MainGameLoadingThread(QObject *parent = nullptr);
protected:
	void run() override;
};

class Particle : public QObject {//用于产生粒子特效
	Q_OBJECT
private:
	float posX, posY, v, life, size, k;//k: 斜率，一定存在
	float degree;//生命值消减速度
	float objX, objY;

signals:
	void done(Particle *);

public:
	explicit Particle();
	void setPos(float x, float y);
	void setV(float v);
	void setLife(float l);
	void setSize(float s);
	void setDegree(float d);
	void setObj(float x, float y);
	struct Data {
		int num;//生成粒子数量，生成时间
		float x, y;//初始坐标
		float objX, objY;//目的坐标
	};

public slots:
	void draw(float s);

};

class BlueParticle : public QObject {
	Q_OBJECT
public:
	float posX, posY, life, degree;
	BlueParticle();

public slots:
	void draw(float s);

signals:
	void done(BlueParticle *p);
};

class MenuWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core//超级简单粗暴的标题菜单界面
{
	Q_OBJECT
	enum MenuStatus {
		MAIN,
		MUSICROOM,
		MUSICROOMING,
		CONFIG,
		GAMESTART//游戏开始加载的状态
	};

public:
	explicit MenuWidget(QWidget *parent = nullptr);
	static QMatrix4x4 *matrix;//变换矩阵，公开给粒子使用，省不少内存
	static QOpenGLShaderProgram *getProgram;
	~MenuWidget() override;
	void down();//菜单选项移动接口
	void up();
	void ok();//确认接口
	void quitWindow();
	void left();
	void right();

	QOpenGLVertexArrayObject *getBlueParticleVAO();
	QOpenGLShaderProgram *getBlueParticleProgram();
	QOpenGLTexture *getBlueParticleTexture();

signals:
	void draw(float s);
	void draw2(float s);
	void close();
	void start();//游戏正式开始信号

public slots:
	void dealWithLoading();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	bool lock, lock2;//加载动画过场播放锁
	int at, atAnimation, animationTime;//现在选的是哪一个，现在哪一个处于动画状态（鬼畜的抽动），没有就是-1，抖动动画调控时间
	int sparkTime;//选中闪烁动画
	int magicTime;//控制魔法阵显示动画
	QOpenGLBuffer *VBO, *IBO, *VBO2, *VBO3;
	QOpenGLShader *vs, *fs, *vs2, *fs2;
	QOpenGLShaderProgram *program, *program2, *program3;
	QOpenGLTexture *texture, *texture2;
	QOpenGLVertexArrayObject *VAO, *VAO2, *VAO3;
	QOpenGLTexture *musicRoom, *quit, *gameStart, *config;//需要加载一堆纹理
	QOpenGLTexture *musicRoom_b, *quit_b, *gameStart_b, *config_b, *title, *bgmvolume;
	QOpenGLTexture *p2, *musicRoom_bg, *magic, *startK, *startF, *startN;
	QOpenGLTexture *num[10];
	void initParticleOpenGL();
	QTimer *timer;//用于产生游戏循环,很简单，很粗暴
	Particle *newParticle();
	void newBlueParticle();
	QMap<int, Particle::Data> *map;
	int now;//用于控制粒子循环
	void loadData();
	void loadParticles(int t);
	void drawMenu();
	float pos[4];
	float pos2[4];
	float degree;//旋转角度
	QMatrix4x4 *menuMatrix;
	void solve();
	float totAlpha;//用于实现渐隐动画
	float loadingAlpha;

	MenuStatus status;//菜单状态，其实想好好写的，但是想起状态机模型时已经基本写完了:P
	int configStatus;//控制config时的动画过程
	float posConfigX, posConfigY;//控制config选项的飞入

	MusicRoom *musicRoomWidget;

	MainGameLoadingThread *thread;
};

#endif // MENUWIDGET_H
