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
#include <deque>

class Scene : public QGraphicsScene, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	Scene(QObject *parent);
	~Scene();

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

	std::deque<float> dq, tree_dq, tree2_dq;//双端队列，用于加载纹理
};

#endif // SCENE_H
