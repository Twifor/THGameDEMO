#include "loadingwidget.h"
#include <QPainter>
#include <QTextStream>
#include <gameresource.h>
#include "musicroom.h"

LoadingOpenGLWidget::LoadingOpenGLWidget(QWidget*parent) : QOpenGLWidget(parent)
{
	alpha = 0.0;
	flag = false;
	timer = new QTimer;
	timer->setInterval(20);
	map = new QMap<int, Star::Data>;
	loadFile();

	timeID = 0;
	totAlpha = 1.0;

	isOK = true;
}

LoadingOpenGLWidget::~LoadingOpenGLWidget()
{
	makeCurrent();
	delete VBO;
	delete IBO;
	delete vs;
	delete fs;
	delete program;
	delete texture1;
	delete texture2;
	delete VAO;
	delete transform;

	delete map;

	Star::destroy();
}

void LoadingOpenGLWidget::destroy()
{
	isOK = false;
}

void LoadingOpenGLWidget::stop()
{
	timer->stop();
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

	texture2 = new QOpenGLTexture(QImage(":/std/menubg.png").mirrored(false, true));
	texture2->bind();

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
						  "uniform sampler2D ourTexture2;\n"
						  "uniform vec2 alpha;\n"

						  "void main()\n"
						  "{\n"
						  "FragColor = mix(texture(ourTexture, TexCoord),texture(ourTexture2, TexCoord),alpha.y);\n"
//						  "FragColor.a *= alpha.x;\n"
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
	program->setUniformValue("ourTexture2", 2);

	VAO->release();
	VBO->release();
	IBO->release();
	program->release();
	texture1->release();

	Star::init();

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	texture1->bind(0);
	texture2->bind(2);
	VAO->bind();
	program->bind();

	program->setUniformValue("alpha", 0.0, 1.0 - totAlpha);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	texture1->release();
	texture2->release();
	VAO->release();
	program->release();

	emit drawStar();

	if(map->find(timeID) != map->end() && isOK) {
		Star::Data data = (*map)[timeID];
		Star *star = new Star;
		star->setPos(data.x, data.y);
		star->setSize(data.size);
		star->setSpeed(data.speed);
		connect(this, &LoadingOpenGLWidget::drawStar, star, &Star::draw);
	}
	if(!isOK) totAlpha -= 0.015;

	++timeID;
	timeID %= 50;
	painter.endNativePainting();
	alpha += 0.04;
	if(fabs(alpha - 1.0) <= 1e-5) {
		alpha = 0.0;
		flag = !flag;
	}
	if(!flag) painter.setOpacity((alpha / 1.25 + 0.2) * totAlpha);
	else painter.setOpacity(((1.0 - alpha) / 1.25 + 0.2) * totAlpha);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
	painter.drawPixmap(650, 520, 120, 60, QPixmap(":/std/loading.png"));
	painter.end();

	//	QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);//正宗的刷新界面方法
	if(totAlpha <= 0.0) emit done();
}

void LoadingOpenGLWidget::loadFile()
{
	int at;
	QFile file(":/data/stars");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);
	while(!stream.atEnd()) {
		QString str = stream.readLine();
		QTextStream stream0(&str);
		Star::Data data;
		stream0 >> at >> data.x >> data.y >> data.size >> data.speed;
		(*map)[at] = data;
	}
}

void Star::destroy()
{
	delete matrix;
	delete VBO;
	delete IBO;
	delete vs;
	delete fs;
	delete program;
	delete texture;
	delete VAO;
}

void Star::setPos(double x, double y)
{
	this->x = x;
	this->y = y;
}

void Star::setSize(double ss)
{
	this->size = ss;
}

void Star::setSpeed(double sp)
{
	this->sp = sp;
}

void Star::init()
{
	float op2[] = {
		-0.375f, -0.5f, 0.0f, 0.0f, 0.0f,
		-0.375f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.375f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.375f, 0.5f, 0.0f, 1.0f, 1.0f
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
	VBO->allocate(op2, sizeof(op2));

	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->create();
	IBO->bind();
	IBO->allocate(indices, sizeof(indices));

	texture = new QOpenGLTexture(QImage(":/std/Star.png"));
	texture->bind();

	vs = new QOpenGLShader(QOpenGLShader::Vertex);
	vs->compileSourceCode("#version 330 core\n"
						  "layout (location = 0) in vec3 aPos;\n"
						  "layout (location = 1) in vec2 aTexCoord;\n"
						  "out vec2 TexCoord;\n"
						  "uniform mat4 transform;\n"
						  "void main()\n"
						  "{\n"
						  "gl_Position = transform*vec4(aPos, 1.0);\n"
						  "TexCoord = aTexCoord;\n"
						  "}");

	fs = new QOpenGLShader(QOpenGLShader::Fragment);
	fs->compileSourceCode("#version 330 core\n"
						  "out vec4 FragColor;\n"
						  "in vec2 TexCoord;\n"

						  "uniform sampler2D ourTexture;\n"
						  "uniform vec2 alpha;"

						  "void main()\n"
						  "{\n"
						  "FragColor = texture(ourTexture, TexCoord);\n"
						  "FragColor.a *= alpha.x;\n"
						  "}");

	program = new QOpenGLShaderProgram;
	program->addShader(vs);
	program->addShader(fs);
	program->link();
	program->bind();

	program->setUniformValue("ourTexture", 1);

	program->enableAttributeArray(program->attributeLocation("aPos"));
	program->setAttributeBuffer(program->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	program->enableAttributeArray(program->attributeLocation("aTexCoord"));
	program->setAttributeBuffer(program->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);

	VAO->release();
	VBO->release();
	program->release();
	texture->release();
	IBO->release();
}

void Star::draw()
{
	matrix->setToIdentity();
	matrix->translate(x, y, 0.0);
	matrix->scale(static_cast<float>(size), static_cast<float>(size));

	VAO->bind();
	program->bind();
	texture->bind(1);

	program->setUniformValue("alpha", alpha, 0.0);
	program->setUniformValue("transform", *matrix);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	VAO->release();
	program->release();
	texture->release();

	if(!flag) {
		alpha += sp;
		if(alpha >= 1.0) {
			alpha = 1.0;
			flag = true;
		}
	}else{
		alpha -= sp;
		if(alpha <= 0.0) this->deleteLater();
	}
}

Star::Star() : QObject (nullptr)
{
	alpha = 0.0;
	matrix = new QMatrix4x4;
	flag = false;
}

Star::~Star()
{

}

LoadingThread::LoadingThread(QObject *parent) : QThread (parent)
{

}

void LoadingThread::run()//加载过程
{
	msleep(2000);//提供播放动画的时间
	qDebug() << "Begin loading...";
	GameResource::getInstance()->changeStatus(GameResource::MENU);//初始化游戏资源
	MusicRoom::init();
	qDebug() << "Loading successfully";
	emit done();
}

QMatrix4x4 *Star::matrix = nullptr;//变换矩阵
QOpenGLBuffer *Star::VBO = nullptr, *Star::IBO = nullptr;
QOpenGLShader *Star::vs = nullptr, *Star::fs = nullptr;
QOpenGLShaderProgram *Star::program = nullptr;
QOpenGLTexture *Star::texture = nullptr;
QOpenGLVertexArrayObject *Star::VAO = nullptr;
QMatrix4x4 *Star::transform = nullptr;

