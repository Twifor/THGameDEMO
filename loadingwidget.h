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

class LoadingOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	explicit LoadingOpenGLWidget(QWidget *parent = nullptr);

signals:

public slots:

	// QOpenGLWidget interface
protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLShader *vs, *fs;
	QOpenGLShaderProgram *program;
	QOpenGLTexture *texture1;
	QOpenGLVertexArrayObject *VAO;

private:
	double alpha;
	bool flag;
	QTimer *timer;
};

#endif // LOADINGWIDGET_H
