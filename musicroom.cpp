#include "menuwidget.h"
#include "musicroom.h"
#include <musicfactory.h>

MusicRoom::MusicRoom(QWidget *parent) : QOpenGLWidget(parent)
{
	totAlpha = 1.0f;
	setGeometry(0, 0, 800, 600);
	setFocusPolicy(Qt::NoFocus);

	status = PRE;
	timer = new QTimer;
	timer->setInterval(1000 / 60);

	at = MusicFactory::getInstance()->getNow();

	pos[0] = -100;
	pos[1] = -120;
	pos[2] = -140;
	pos[3] = -160;

	pp = 0;
	now = 0;
	preTotAlpha = 0.0f;
	degree = 0.0f;
	labaNow = 0;
}

void MusicRoom::quit()
{
	status = QUIT;
}

void MusicRoom::up()
{
	at = (at + 3) % 4;
	pp = 0;
}

void MusicRoom::down()
{
	at = (at + 1) % 4;
	pp = 0;
}

void MusicRoom::ok()
{
	if(MusicFactory::getInstance()->getNow() == at) return;
	MusicFactory::getInstance()->quit();

	MusicFactory::getInstance()->play(at);

}
QOpenGLShaderProgram *MusicRoom::getProgram = nullptr;
QMatrix4x4 *MusicRoom::matrix = nullptr;

MusicRoom::~MusicRoom()
{
	timer->stop();
	delete VAO;
	delete VBO;
	delete program;
	delete vs;
	delete fs;
	delete IBO;
	delete texture;
	delete texture2;
	delete timer;

	delete VAO2;
	delete VBO2;
	delete vs2;
	delete fs2;
	delete texture3;
	delete getProgram;
	delete matrix;

	delete magic;
}

void MusicRoom::initializeGL()
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

	QImage image(":/std/menubg.png");
	image.scaled(800, 600);
	texture = new QOpenGLTexture(image.mirrored(false, true));
	texture->bind();
	texture2 = new QOpenGLTexture(QImage(":/std/musicroom_bg.png").mirrored(false, true));
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
						  "uniform sampler2D texture2;\n"
						  "uniform vec2 alpha;\n"

						  "void main()\n"
						  "{\n"
						  "FragColor = mix(texture(ourTexture, TexCoord),texture(texture2, TexCoord),alpha.x);\n"
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
	program->setUniformValue("texture2", 1);
	program->setUniformValue("alpha", totAlpha, 0.0f);

	VAO->release();
	VBO->release();
	IBO->release();
	program->release();
	texture->release();
	texture2->release();

	magic = new QOpenGLTexture(QImage(":/std/magic.png"));

	initParticles();

	timer->start();
	connect(timer, &QTimer::timeout, [ = ](){
		update();
	});
}

void MusicRoom::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void MusicRoom::paintGL()
{
	if(status == MAIN) ++now;
	now %= 30;
	if(now % 5 == 0) newParticles();
	if(now == 29) {
		++labaNow;
		if(labaNow == 3) labaNow = 0;
	}
	QPainter painter(this);
	painter.beginNativePainting();
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VAO->bind();
	texture->bind(0);
	texture2->bind(1);
	program->bind();
	program->setUniformValue("alpha", totAlpha, 0.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	texture2->release();
	texture->release();
	VAO->release();
	program->release();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	VAO2->bind();
	getProgram->bind();
	texture3->bind(1);
	emit draw(totAlpha);

	texture3->release();
	magic->bind(1);
	matrix->setToIdentity();
//	glViewport(0.0, 0.0f, 600, 600);
	matrix->translate(0.78f, 0.78f, 0.0f);
	matrix->perspective(45.0f, 8.0f / 6.0f, 0.1f, 100.0f);
	matrix->translate(0.0f, 0.0f, -3.0f);
	matrix->rotate(-45.0f, 0.0f, 1.0f, 0.0f);
	matrix->rotate(45.0f, 1.0f, 0.0f, 0.0f);
	matrix->rotate(degree, 0.0f, 0.0f, 1.0f);//绕z轴旋转
	MusicRoom::getProgram->setUniformValue("projection", *matrix);
	MusicRoom::getProgram->setUniformValue("alpha", preTotAlpha * totAlpha * 0.9f, 0.0);
	if(preTotAlpha < 1.0f) preTotAlpha += 0.04f;
	if(pp == 192 || status == PRE) degree += 0.5f;
	else degree += 2.0f;
	if(degree > 360.0f) degree = 0.0f;

	glDrawArrays(GL_TRIANGLES, 0, 6);
//	glViewport(0.0, 0.0f, 800, 600);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	magic->release();
	VAO2->release();
	getProgram->release();


	painter.endNativePainting();
	if(status == PRE) {
		painter.drawPixmap(120, pos[0], 550, 75, QPixmap(":/std/e1.png"));
		painter.drawPixmap(120, pos[1], 550, 75, QPixmap(":/std/e2b.png"));
		painter.drawPixmap(120, pos[2], 550, 75, QPixmap(":/std/e3b.png"));
		painter.drawPixmap(120, pos[3], 550, 75, QPixmap(":/std/e4b.png"));
		if(pos[0] < 100) pos[0] += 10;
		if(pos[1] < 150) pos[1] += 10;
		if(pos[2] < 200) pos[2] += 10;
		if(pos[3] < 250) pos[3] += 10;
		else status = MAIN;
	}else  {
		painter.setOpacity(totAlpha);
		if(at == 0) painter.drawPixmap(120, 100, 550, 75, QPixmap(":/std/e1.png"));
		else painter.drawPixmap(120, 100, 550, 75, QPixmap(":/std/e1b.png"));
		if(at == 1) painter.drawPixmap(120, 150, 550, 75, QPixmap(":/std/e2.png"));
		else painter.drawPixmap(120, 150, 550, 75, QPixmap(":/std/e2b.png"));
		if(at == 2) painter.drawPixmap(120, 200, 550, 75, QPixmap(":/std/e3.png"));
		else painter.drawPixmap(120, 200, 550, 75, QPixmap(":/std/e3b.png"));
		if(at == 3) painter.drawPixmap(120, 250, 550, 75, QPixmap(":/std/e4.png"));
		else painter.drawPixmap(120, 250, 550, 75, QPixmap(":/std/e4b.png"));

		QPixmap laba;
		if(labaNow == 0) laba.convertFromImage(QImage(":/std/laba3.png").mirrored(true, false));
		else if(labaNow == 1) laba.convertFromImage(QImage(":/std/laba2.png").mirrored(true, false));
		else laba.convertFromImage(QImage(":/std/laba.png").mirrored(true, false));
		if(MusicFactory::getInstance()->getNow() == 0) painter.drawPixmap(90, 115, 45, 45, laba);
		else if(MusicFactory::getInstance()->getNow() == 1) painter.drawPixmap(90, 165, 45, 45, laba);
		else if(MusicFactory::getInstance()->getNow() == 2) painter.drawPixmap(90, 215, 45, 45, laba);
		else painter.drawPixmap(90, 265, 45, 45, laba);

		if(at == 0) painter.drawPixmap(20, 400, 768, pp, QPixmap(":/std/song1.png"));
		else if(at == 1) painter.drawPixmap(20, 400, 768, pp, QPixmap(":/std/song2.png"));
		else if(at == 2) painter.drawPixmap(20, 400, 768, pp, QPixmap(":/std/song3.png"));
		else if(at == 3) painter.drawPixmap(20, 400, 768, pp, QPixmap(":/std/song4.png"));
		if(pp < 192) pp += 11;
		else pp = 192;
	}

	if(status == QUIT) {
		if(totAlpha > 0.0f) totAlpha -= 0.01f;
		else {
			timer->stop();
			emit done();
		}
	}
}


void MusicRoom::initParticles()
{
	GLfloat vertices[] = {
		// 位置     // 纹理
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,

		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f
	};
	VAO2 = new QOpenGLVertexArrayObject;
	VAO2->create();
	VAO2->bind();

	VBO2 = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO2->create();
	VBO2->bind();
	VBO2->allocate(vertices, sizeof(vertices));

	texture3 = new QOpenGLTexture(QImage(":/std/p2.png"));
	texture3->bind();

	vs2 = new QOpenGLShader(QOpenGLShader::Vertex);
	vs2->compileSourceCode("#version 330 core\n"
						   "layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>\n"
						   "\n"
						   "out vec2 TexCoords;\n"
						   "out vec4 ParticleColor;\n"
						   "\n"
						   "uniform mat4 projection;\n"
						   "\n"
						   "void main()\n"
						   "{\n"
						   "TexCoords = vertex.zw;\n"
						   "gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
						   "}");

	fs2 = new QOpenGLShader(QOpenGLShader::Fragment);
	fs2->compileSourceCode("#version 330 core\n"
						   "in vec2 TexCoords;\n"
						   "out vec4 color;\n"
						   "\n"
						   "uniform sampler2D sprite;"
						   "uniform vec2 alpha;\n"
						   "\n"
						   "void main()\n"
						   "{\n"
						   "color = texture(sprite, TexCoords);\n"
						   "color.a *= alpha.x;\n"
						   "}");

	getProgram = new QOpenGLShaderProgram;
	getProgram->addShader(vs2);
	getProgram->addShader(fs2);
	getProgram->link();
	getProgram->bind();

	getProgram->enableAttributeArray(getProgram->attributeLocation("vertex"));
	getProgram->setAttributeBuffer(getProgram->attributeLocation("vertex"), GL_FLOAT, 0, 4, 4 * sizeof(float));

	getProgram->setUniformValue("sprite", 1);


	VAO2->release();
	VBO2->release();
	texture3->release();
	getProgram->release();

	matrix = new QMatrix4x4;
}

void MusicRoom::newParticles()
{
	GreenParticle *p = new GreenParticle;
	connect(p, &GreenParticle::done, [ = ](GreenParticle *p){
		delete p;
	});
	connect(this, &MusicRoom::draw, p, &GreenParticle::draw);

	p->degree = (qrand() % 1000 + 500) / 100000.0f;
	p->posX = qrand() % 20000 / 10000.0f - 1.0f;
	p->posY = qrand() % 20000 / 10000.0f - 1.0f;
}

GreenParticle::GreenParticle() : QObject (nullptr)
{
	life = 0.0f;
	lock = true;
}

void GreenParticle::draw(float s)
{
	MusicRoom::matrix->setToIdentity();
	MusicRoom::matrix->translate(posX, posY);
	MusicRoom::matrix->scale(0.02f, 0.02f, 0.02f);

	MusicRoom::getProgram->setUniformValue("projection", *MusicRoom::matrix);
	MusicRoom::getProgram->setUniformValue("alpha", life * s, 0.0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	posY += 0.0010f;
	if(lock) {//淡出
		life += 0.04f;
		if(life > 1.0f) lock = false;
	}else life -= degree;
	if(!lock && life <= 0.0f) emit done(this);
}
