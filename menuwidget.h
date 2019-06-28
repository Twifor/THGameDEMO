#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

class MenuWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	explicit MenuWidget(QWidget *parent = nullptr);

signals:

public slots:

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLShader *vs, *fs;
	QOpenGLShaderProgram *program;
	QOpenGLTexture *texture;
	QOpenGLVertexArrayObject *VAO;


};

#endif // MENUWIDGET_H
