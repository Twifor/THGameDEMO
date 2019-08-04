#ifndef MUSICROOM_H
#define MUSICROOM_H

#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QTimer>
#include <QPainter>

class GreenParticle : public QObject {//随便起的名，不一定就是green（其实还是蓝的
	Q_OBJECT
public:
	float posX, posY, life, degree;
	bool lock;
	GreenParticle();

public slots:
	void draw(float s);

signals:
	void done(GreenParticle *p);
};

class MusicRoom : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT

public:
	enum MusicRoomStatus {
		QUIT,
		MAIN,
		PRE,
	};

	explicit MusicRoom(QWidget *parent = nullptr);
	void quit();
	void up();
	void down();
	void ok();
	~MusicRoom();
	static QMatrix4x4 *matrix;
	static QOpenGLShaderProgram *getProgram;
	static void init();
	static void destroy();
	MusicRoomStatus getStatus();

signals:
	void done();
	void draw(float s);

public slots:

	// QOpenGLWidget interface
protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	QOpenGLBuffer *VBO, *IBO, *VBO2;
	QOpenGLShader *vs, *fs, *vs2, *fs2;
	QOpenGLShaderProgram *program;
	QOpenGLTexture *texture, *texture2, *texture3;
	QOpenGLTexture *magic;
	QOpenGLVertexArrayObject *VAO, *VAO2;
	float totAlpha;
	float preTotAlpha;
	MusicRoomStatus status;
	QTimer *timer;
	int at;//指标
	int pos[4], pp;
	int now, labaNow;
	float degree;

	void initParticles();
	void newParticles();

	static QPixmap *pixMap[4], *pixMap_b[4], *laba[3], *song[4];

};

#endif // MUSICROOM_H
