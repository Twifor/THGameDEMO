#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "camera.h"
#include "myplane.h"
#include <deque>
#include "gametexture.h"

class Scene : public QGraphicsScene, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	Scene(QObject *parent);
	~Scene();
	void satrtLeft();
	void endLeft();
	void startRight();
	void endRight();
	void startUp();
	void endUp();
	void startDown();
	void endDown();
	void startShift();
	void endShift();
	void startZ();
	void endZ();

	QOpenGLShaderProgram *getProgram1();//提供外部接口，以省资源
	QOpenGLVertexArrayObject *getVAO1();
	QOpenGLTexture *getTexture(GameTexture::TextureType type);
	QMatrix4x4 *getMatrix();

	// QGraphicsScene interface
protected:
	void drawBackground(QPainter *painter, const QRectF &rect) override;
	void init();
private:
	float totAlpha;
	QOpenGLVertexArrayObject *bg_VAO, *ma_VAO;
	QOpenGLBuffer *bg_IBO, *bg_VBO, *ma_VBO;
	QOpenGLShader *bg_vs, *bg_fs, *ma_vs, *ma_fs;
	QOpenGLTexture *bg_texture, *tree_texture, *star_texture, *redstar_texture;
	QOpenGLShaderProgram *bg_program, *ma_program;
	Camera *camera;//摄像机类
	QMatrix4x4 pr, matrix;

	std::deque<float> dq, tree_dq, tree2_dq;//双端队列，用于加载纹理
	MyPlane *myPlane;

	GameTexture *gameTexure;
};

#endif // SCENE_H
