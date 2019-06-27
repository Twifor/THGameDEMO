#include "loadingwidget.h"
#include <QPainter>

LoadingOpenGLWidget::LoadingOpenGLWidget(QWidget*parent) : QOpenGLWidget(parent)
{
	alpha = 0.0;
	flag = false;
	timer = new QTimer;
	timer->setInterval(20);
}

void LoadingOpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glViewport(0, 0, 800, 600);
	/*主体背景绘制*/
	float op1[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VAO->bind();

	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->create();
	VBO->bind();
	VBO->allocate(op1, sizeof(op1));


	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->create();
	IBO->bind();
	IBO->allocate(indices, sizeof(indices));

	QImage image(":/std/bg.png");
	image.scaled(800, 600);
	texture1 = new QOpenGLTexture(image.mirrored(false, true));
	texture1->bind();

	vs = new QOpenGLShader(QOpenGLShader::Vertex);
	vs->compileSourceCode("#version 330 core\n"
						  "layout (location = 0) in vec3 aPos;\n"
						  "layout (location = 1) in vec2 aTexCoord;\n"
						  "out vec2 TexCoord;\n"
						  "void main()\n"
						  "{\n"
						  "gl_Position = vec4(aPos, 1.0);\n"
						  "TexCoord = aTexCoord;\n"
						  "}");

	fs = new QOpenGLShader(QOpenGLShader::Fragment);
	fs->compileSourceCode("#version 330 core\n"
						  "out vec4 FragColor;\n"
						  "in vec2 TexCoord;\n"

						  "uniform sampler2D ourTexture;\n"

						  "void main()\n"
						  "{\n"
						  "FragColor = texture(ourTexture, TexCoord);\n"
						  "FragColor.a = 1.0;\n"
						  "}");
	program = new QOpenGLShaderProgram;
	program->addShader(vs);
	program->addShader(fs);
	program->link();
	program->bind();

	program->enableAttributeArray(program->attributeLocation("aPos"));
	program->setAttributeBuffer(program->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	program->enableAttributeArray(program->attributeLocation("aTexCoord"));
	program->setAttributeBuffer(program->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);

	program->setUniformValue("ourTexture", 0);

	VAO->release();
	VBO->release();
	IBO->release();
	program->release();
	texture1->release();

	timer->start();
	connect(timer, &QTimer::timeout, [ = ](){
		update();
	});
}

void LoadingOpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void LoadingOpenGLWidget::paintGL()
{
	QPainter painter(this);
	painter.beginNativePainting();
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	texture1->bind(0);
	VAO->bind();
	program->bind();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	texture1->release();
	VAO->release();
	program->release();
	painter.endNativePainting();
	alpha += 0.04;
	if(fabs(alpha - 1.0) <= 1e-5) {
		alpha = 0.0;
		flag = !flag;
	}
	if(!flag) painter.setOpacity(alpha);
	else painter.setOpacity(1.0 - alpha);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.drawPixmap(650, 520, 120, 60, QPixmap(":/std/loading.png"));
}
