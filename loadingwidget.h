#ifndef LOADINGWIDGET_H
#define LOADINGWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QMap>
#include <QFile>
#include <QThread>

class LoadingThread : public QThread {//游戏数据加载线程
	Q_OBJECT
public:
	explicit LoadingThread(QObject *parent = nullptr);

protected:
	void run() override;

signals:
	void done();
};

class Star : public QObject {
	Q_OBJECT

public:
	struct Data {
		double x, y;
		double size;
		double speed;
	};
	static void destroy();
	void setPos(double x, double y);
	void setSize(double ss);
	void setSpeed(double sp);
	static void init();
	void draw();
	void speed();
	explicit Star();
	~Star();

private:
	double alpha;//透明度
	double size;//大小
	double sp;
	static QMatrix4x4 *matrix;//变换矩阵
	double x, y;
	bool flag;
	static QOpenGLBuffer *VBO, *IBO;
	static QOpenGLShader *vs, *fs;
	static QOpenGLShaderProgram *program;
	static QOpenGLTexture *texture;
	static QOpenGLVertexArrayObject *VAO;
	static QMatrix4x4 *transform;
};

class LoadingOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	explicit LoadingOpenGLWidget(QWidget *parent = nullptr);
	~LoadingOpenGLWidget() override;
	void destroy();
	void stop();

signals:
	void drawStar();
	void done();

	// QOpenGLWidget interface
protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void loadFile();

private:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLShader *vs, *fs;
	QOpenGLShaderProgram *program;
	QOpenGLTexture *texture1, *texture2;
	QOpenGLVertexArrayObject *VAO;
	QMap<int, Star::Data> *map;
	double alpha, totAlpha;
	bool flag, isOK;
	QTimer *timer;
	Star *star;
	int timeID;
};

#endif // LOADINGWIDGET_H
