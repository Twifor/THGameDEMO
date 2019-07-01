#include "menuwidget.h"
#include <QFile>
#include <QDateTime>
#include <QSound>
#define SCALE 0.5f
#define MIN(x, y) ((x) < (y) ? (x) : (y))

QMatrix4x4 *MenuWidget::matrix = nullptr;
QOpenGLShaderProgram *MenuWidget::getProgram = nullptr;

MenuWidget::MenuWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	setFocusPolicy(Qt::NoFocus);
	qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

	matrix = new QMatrix4x4;
	timer = new QTimer;

	map = new QMap<int, Particle::Data>;
	loadData();

	now = 0;

	pos[0] = -1.5f;
	pos[1] = -1.5f;
	pos[2] = -1.5f;
	pos[3] = -1.5f;

	pos2[0] = -0.10f;
	pos2[1] = -0.10f;
	pos2[2] = -0.10f;
	pos2[3] = -0.10f;

	lock = true;
	at = 0;
	atAnimation = -1;
	sparkTime = -1;
	totAlpha = 1.0f;
	magicTime = 0;
	lock2 = true;
	degree = 0.0f;

	status = MAIN;
}

MenuWidget::~MenuWidget()
{
	makeCurrent();
	delete VBO;
	delete IBO;
	delete VBO2;

	delete vs;
	delete fs;
	delete vs2;
	delete fs2;

	delete program;
	delete program2;

	delete texture;
	delete texture2;

	delete VAO;
	delete VAO2;

	delete matrix;
	delete timer;

	delete gameStart;
	delete config;
	delete quit;
	delete musicRoom;
	delete menuMatrix;

	delete VBO3;
	delete VAO3;
	delete program3;

	delete gameStart_b;
	delete musicRoom_b;
	delete config_b;
	delete quit_b;

	delete p2;

	delete musicRoom_bg;
	delete magic;

	delete startK;
	delete startN;
	delete startF;
}

void MenuWidget::down()
{
	if(!lock) {
		if(status == MAIN) {
			atAnimation = at = (at + 1) % 4;
			QSound::play(":/std/select.wav");
			animationTime = 0;
		}else if(status == MUSICROOMING) {
			QSound::play(":/std/select.wav");
			musicRoomWidget->down();
		}
	}
}

void MenuWidget::up()
{
	if(!lock) {
		if(status == MAIN) {
			atAnimation = at = (at + 3) % 4;
			QSound::play(":/std/select.wav");
			animationTime = 0;
		}else if(status == MUSICROOMING) {
			QSound::play(":/std/select.wav");
			musicRoomWidget->up();

		}
	}
}

void MenuWidget::ok()
{
	if(!lock) {
		if(status == MAIN) {
			QSound::play(":/std/ok.wav");
			sparkTime = 0;
		}else if(status == MUSICROOMING) {
			QSound::play(":/std/ok.wav");
			musicRoomWidget->ok();
		}
	}
}

void MenuWidget::quitWindow()
{
	if(status == MAIN) {
		atAnimation = at = 3;
		animationTime = 0;
	}else if(status == MUSICROOMING) {
		musicRoomWidget->quit();
	}
	QSound::play(":/std/cancel.wav");
}

QOpenGLVertexArrayObject *MenuWidget::getBlueParticleVAO()
{
	return VAO2;
}

QOpenGLShaderProgram *MenuWidget::getBlueParticleProgram()
{
	return program2;
}

QOpenGLTexture *MenuWidget::getBlueParticleTexture()
{
	return p2;
}

void MenuWidget::initializeGL()
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
						  "FragColor = mix(texture(texture2, TexCoord),texture(ourTexture, TexCoord),alpha.x);\n"
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
	program->setUniformValue("texture2", 2);
	program->setUniformValue("alpha", totAlpha, 0.0f);

	VAO->release();
	VBO->release();
	IBO->release();
	program->release();
	texture->release();

	initParticleOpenGL();

	/*菜单按钮绘制*/
	float vertices[] = {
		// 位置     // 纹理
		-0.28125f, 0.1f, 0.0f, 1.0f,
		0.28125f, -0.1f, 1.0f, 0.0f,
		-0.28125f, -0.1f, 0.0f, 0.0f,

		-0.28125f, 0.1f, 0.0f, 1.0f,
		0.28125f, 0.1f, 1.0f, 1.0f,
		0.28125f, -0.1f, 1.0f, 0.0f
	};

	gameStart = new QOpenGLTexture(QImage(":/std/gamestart.png").mirrored(false, true));
	musicRoom = new QOpenGLTexture(QImage(":/std/musicroom.png").mirrored(false, true));
	config = new QOpenGLTexture(QImage(":/std/config.png").mirrored(false, true));
	quit = new QOpenGLTexture(QImage(":std/quit.png").mirrored(false, true));
	gameStart_b = new QOpenGLTexture(QImage(":/std/gamestart_b.png").mirrored(false, true));
	musicRoom_b = new QOpenGLTexture(QImage(":/std/musicroom_b.png").mirrored(false, true));
	config_b = new QOpenGLTexture(QImage(":/std/config_b.png").mirrored(false, true));
	quit_b = new QOpenGLTexture(QImage(":/std/quit_b.png").mirrored(false, true));
	p2 = new QOpenGLTexture(QImage(":/std/p2.png"));
	magic = new QOpenGLTexture(QImage(":/std/magic2.png"));
	startK = new QOpenGLTexture(QImage(":/std/startK.png").mirrored(false, true));
	startF = new QOpenGLTexture(QImage(":/std/startF.png").mirrored(false, true));
	startN = new QOpenGLTexture(QImage(":/std/startN.png").mirrored(false, true));

	musicRoom_bg = new QOpenGLTexture(QImage(":/std/musicroom_bg.png").mirrored(false, true));
	menuMatrix = new QMatrix4x4;

	VAO3 = new QOpenGLVertexArrayObject;
	VAO3->create();
	VAO3->bind();

	VBO3 = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO3->create();
	VBO3->bind();
	VBO3->allocate(vertices, sizeof(vertices));

	program3 = new QOpenGLShaderProgram;
	program3->addShader(vs2);
	program3->addShader(fs2);
	program3->link();
	program3->bind();

	program3->enableAttributeArray(program3->attributeLocation("vertex"));
	program3->setAttributeBuffer(program3->attributeLocation("vertex"), GL_FLOAT, 0, 4, 4 * sizeof(float));

	program3->setUniformValue("sprite", 1);

	VAO3->release();
	VBO3->release();
	program3->release();

	timer->setInterval(1000 / 60);
	timer->start();
	connect(timer, &QTimer::timeout, [ = ](){
		update();
	});

}

void MenuWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void MenuWidget:: paintGL()
{
	if(map->find(now) != map->end() && status == MAIN) loadParticles(now); //加载粒子
	++now;
	now %= 30;
	if(!lock && now % 5 == 0 && status == MAIN) newBlueParticle();

	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VAO->bind();
	texture->bind(0);
	musicRoom_bg->bind(2);
	program->bind();
	program->setUniformValue("alpha", totAlpha, 0.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	texture->release();
	VAO->release();
	program->release();

	VAO2->bind();
	texture2->bind(1);
	program2->bind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	emit draw(totAlpha);
	texture2->release();
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	magic->bind(1);
	matrix->setToIdentity();
	matrix->translate(0.5f - MIN(magicTime, 30) / 150.0f, -0.1f);
	matrix->scale(0.75f * 1.8f, 1.8f);
	matrix->rotate(degree, 0.0f, 0.0f, 1.0f);
	matrix->translate(-0.5f, -0.5f, 0.0f);
	MenuWidget::getProgram->setUniformValue("projection", *MenuWidget::matrix);
	MenuWidget::getProgram->setUniformValue("alpha", totAlpha * MIN(magicTime, 30) / 60.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	magic->release();
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if(magicTime > 45) {
		startN->bind(1);
		matrix->setToIdentity();
		matrix->translate(-0.55f + (MIN(magicTime, 60) - 45.0f) / 150.0f, -0.5f, 0.0f);
		matrix->scale(0.75f * 1.2f, 1.2f);
		MenuWidget::getProgram->setUniformValue("projection", *MenuWidget::matrix);
		MenuWidget::getProgram->setUniformValue("alpha", totAlpha * (MIN(magicTime, 60) - 45.0f) / 25.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		startN->release();
	}

	if(magicTime > 30) {
		startK->bind(1);
		matrix->setToIdentity();
		matrix->translate(-0.2f, -0.6f + (MIN(magicTime, 45) - 30.0f) / 150.0f, 0.0f);
		matrix->scale(0.75f * 1.2f, 1.2f);
		MenuWidget::getProgram->setUniformValue("projection", *MenuWidget::matrix);
		MenuWidget::getProgram->setUniformValue("alpha", totAlpha * (MIN(magicTime, 45) - 30.0f) / 28.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		startK->release();
	}

	if(magicTime > 60) {
		startF->bind(1);
		matrix->setToIdentity();
		matrix->translate(0.22f - (MIN(magicTime, 75) - 60.0f) / 150.0f, -0.5f, 0.0f);
		matrix->scale(0.75f * 1.2f, 1.2f);
		MenuWidget::getProgram->setUniformValue("projection", *MenuWidget::matrix);
		MenuWidget::getProgram->setUniformValue("alpha", totAlpha *  (MIN(magicTime, 75) - 60.0f) / 25.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		startF->release();
	}

	if(lock2) {
		++magicTime;
		if(magicTime == 75) lock2 = false;
	}
	degree += 0.2f;
	if(degree > 360.0f) degree = 0.0f;

	VAO2->release();
	program2->release();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawMenu();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	p2->bind(1);
	program2->bind();
	VAO2->bind();
	emit draw2(totAlpha);
	p2->release();
	program2->release();
	VAO2->release();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	VAO2->release();
	texture2->release();
	program2->release();

	if(status == MUSICROOM) {
		if(totAlpha > 0.0f) totAlpha -= 0.01f;
		else{
			timer->stop();
			this->hide();
			musicRoomWidget->show();
			status = MUSICROOMING;
		}
	}
}

void MenuWidget::initParticleOpenGL()
{
	GLfloat vertices[] = {
		// 位置     // 纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	VAO2 = new QOpenGLVertexArrayObject;
	VAO2->create();
	VAO2->bind();

	VBO2 = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO2->create();
	VBO2->bind();
	VBO2->allocate(vertices, sizeof(vertices));

	texture2 = new QOpenGLTexture(QImage(":/std/p.png"));
	texture2->bind();

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

	program2 = new QOpenGLShaderProgram;
	program2->addShader(vs2);
	program2->addShader(fs2);
	program2->link();
	program2->bind();

	program2->enableAttributeArray(program2->attributeLocation("vertex"));
	program2->setAttributeBuffer(program2->attributeLocation("vertex"), GL_FLOAT, 0, 4, 4 * sizeof(float));

	program2->setUniformValue("sprite", 1);

	getProgram = program2;

	VAO2->release();
	VBO2->release();
	texture2->release();
	program2->release();
}

Particle *MenuWidget::newParticle()
{
	Particle *particle = new Particle;
	connect(particle, &Particle::done, [ = ](Particle *s){
		delete s;
	});
	connect(this, &MenuWidget::draw, particle, &Particle::draw);
	return particle;
}

void MenuWidget::newBlueParticle()
{
	BlueParticle *p = new BlueParticle;
	connect(p, &BlueParticle::done, [ = ](BlueParticle *p){
		delete p;
	});
	connect(this, &MenuWidget::draw2, p, &BlueParticle::draw);

	p->degree = (qrand() % 1000 + 500) / 50000.0f;
	if(at == 0 || at == 2) p->posX = -0.686331f + qrand() % 56250 / 100000.0f - 0.180625f;
	else p->posX = -0.70969f + qrand() % 56250 / 100000.0f - 0.180625f;
	if(at == 0) p->posY = -0.241709f + qrand() % 5625 / 100000.0f;
	else if(at == 1) p->posY = -0.386972f + qrand() % 5625 / 100000.0f;
	else if(at == 2) p->posY = -0.555014f + qrand() % 5625 / 100000.0f;
	else p->posY = -0.698439f + qrand() % 5625 / 100000.0f;
}

void MenuWidget::loadData()
{
	QFile file(":/data/particles");
	file.open(QIODevice::ReadOnly);
	QTextStream stream(&file);

	Particle::Data data;
	int time;
	while(!stream.atEnd()) {
		stream >> time >> data.num >> data.x >> data.y >> data.objX >> data.objY;
		(*map)[time] = data;
	}
	file.close();
}

void MenuWidget::loadParticles(int t)
{
	Particle::Data data = (*map)[t];
	for(int i = 0; i < data.num; i++) {
		Particle *p = newParticle();
		p->setPos(data.x + qrand() % 1000 / 1000.0f - 0.1f, data.y + qrand() % 1000 / 1000.0f - 0.1f);
		p->setObj(data.objX, data.objY);
		p->setV(qrand() % 1000 / 4500.0f * 0.1f);
		p->setDegree((qrand() % 1000 + 1) / 10000.0f);
		p->setSize(0.03f);
	}
}

void MenuWidget::drawMenu()
{
/*坐标位置
 * -0.686331 -0.201709
 * -0.70969 -0.346972
 * -0.669971 -0.515014
 * -0.702958 -0.658439
 *
 */
	float dx;
	VAO3->bind();
	program3->bind();

	if(lock) {//这里是菜单飞出动画部分
		gameStart->bind(1);
		dx = 0.02f  / 1.00778221f;
		menuMatrix->setToIdentity();
		if(pos[0] < -0.7f) {
			pos[0] += dx;
			pos2[0] -= dx * 0.125f;
		}
		menuMatrix->translate(pos[0], pos2[0]);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", 1.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		gameStart->release();

		musicRoom_b->bind(1);
		dx = 0.018f  / 1.04769091f;
		if(pos[1] < -0.72f) {
			pos[1] += dx;
			pos2[1] -= dx * 0.3125f;
		}
		menuMatrix->setToIdentity();
		menuMatrix->translate(pos[1], pos2[1]);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", 1.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		musicRoom_b->release();

		config_b->bind(1);
		dx = 0.016f  / 1.11803398f;
		if(pos[2] < -0.68f) {
			pos[2] += dx;
			pos2[2] -= dx * 0.5f;
		};
		menuMatrix->setToIdentity();
		menuMatrix->translate(pos[2], pos2[2]);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", 1.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		config_b->release();

		quit_b->bind(1);
		menuMatrix->setToIdentity();
		dx = 0.014f  / 1.1592830f;
		if(pos[3] < -0.715f) {
			pos[3] += dx;
			pos2[3] -= dx * 0.7006369f;
		}else lock = false; //注意这里控制锁
		menuMatrix->translate(pos[3], pos2[3]);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", 1.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		quit_b->release();

		program3->release();
		VAO3->release();
	}else{
		if(at == 0) {
			if(sparkTime == -1) gameStart->bind(1);
			else if(sparkTime % 4 < 2) gameStart->bind(1);
			else gameStart_b->bind(1);
		}else gameStart_b->bind(1);
		menuMatrix->setToIdentity();
		if(atAnimation == 0) menuMatrix->translate(-0.686331f + X[animationTime % 8] * ANIMATION, -0.201709f + Y[animationTime % 8] * ANIMATION);
		else menuMatrix->translate(-0.686331f, -0.201709f);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", totAlpha, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		if(at == 0) {
			if(sparkTime == -1) gameStart->release();
			else if(sparkTime % 4 < 2) gameStart->release();
			else gameStart_b->release();
		}else gameStart_b->release();

		if(at == 1) {
			if(sparkTime == -1) musicRoom->bind(1);
			else if(sparkTime % 4 < 2) musicRoom->bind(1);
			else musicRoom_b->bind(1);
		}else musicRoom_b->bind(1);
		menuMatrix->setToIdentity();
		if(atAnimation == 1) menuMatrix->translate(-0.70969f + X[animationTime % 8] * ANIMATION, -0.346972f + Y[animationTime % 8] * ANIMATION);
		else menuMatrix->translate(-0.70969f, -0.346972f);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", totAlpha, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		if(at == 1) {
			if(sparkTime == -1) musicRoom->release();
			else if(sparkTime % 4 < 2) musicRoom->release();
			else musicRoom_b->release();
		}else musicRoom_b->release();

		if(at == 2) {
			if(sparkTime == -1) config->bind(1);
			else if(sparkTime % 4 < 2) config->bind(1);
			else config_b->bind(1);
		}else config_b->bind(1);
		menuMatrix->setToIdentity();
		if(atAnimation == 2) menuMatrix->translate(-0.669971f + X[animationTime % 8] * ANIMATION, -0.515014f + Y[animationTime % 8] * ANIMATION);
		else menuMatrix->translate(-0.669971f, -0.515014f);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", totAlpha, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		if(at == 2) {
			if(sparkTime == -1) config->release();
			else if(sparkTime % 4 < 2) config->release();
			else config_b->release();
		}else config_b->release();

		if(at == 3) {
			if(sparkTime == -1) quit->bind(1);
			else if(sparkTime % 4 < 2) quit->bind(1);
			else quit_b->bind(1);
		}else quit_b->bind(1);
		menuMatrix->setToIdentity();
		if(atAnimation == 3) menuMatrix->translate(-0.702958f + X[animationTime % 8] * ANIMATION, -0.658439f + Y[animationTime % 8] * ANIMATION);
		else menuMatrix->translate(-0.702958f, -0.658439f);
		menuMatrix->scale(SCALE, SCALE);
		program3->setUniformValue("projection", *menuMatrix);
		program3->setUniformValue("alpha", totAlpha, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		if(at == 3) {
			if(sparkTime == -1) quit->release();
			else if(sparkTime % 4 < 2) quit->release();
			else quit_b->release();
		}else quit_b->release();

		if(atAnimation != -1) {
			++animationTime;
			if(animationTime == 15) {
				atAnimation = -1;
				animationTime = 0;
			}
		}
		if(sparkTime != -1) {
			++sparkTime;
			if(sparkTime == 35) {
				sparkTime = -1;
				solve();//动画播放完毕，执行操作
			}
		}
		program3->release();
		VAO3->release();
	}

}

void MenuWidget::solve()
{
	qDebug() << "Choose" << at;
	switch(at) {
		case 3: emit close(); break;//关闭，发射关闭信号
		case 1: {
			status = MUSICROOM;
			musicRoomWidget = new MusicRoom(parentWidget());
			musicRoomWidget->hide();
			connect(musicRoomWidget, &MusicRoom::done, [ & ](){
				QTimer::singleShot(0, [&](){
					lock = true;
					at = 1;
					atAnimation = -1;
					sparkTime = -1;
					totAlpha = 1.0f;
					now = 0;
					pos[0] = -1.5f;
					pos[1] = -1.5f;
					pos[2] = -1.5f;
					pos[3] = -1.5f;

					pos2[0] = -0.10f;
					pos2[1] = -0.10f;
					pos2[2] = -0.10f;
					pos2[3] = -0.10f;

					magicTime = 0;
					lock2 = true;
					degree = 0.0f;

					timer->setInterval(1000 / 60);
					timer->start();

					delete musicRoomWidget;
					this->show();
					status = MAIN;
				});
			});
			break;
		}
	}
}

Particle::Particle() : QObject (nullptr)
{
	posX = posY =  v = 0.0;
	life = 1.0;
	degree = 0.04f;
}

void Particle::setPos(float x, float y)
{
	posX = x;
	posY = y;
}

void Particle::setV(float v)
{
	this->v = v;
}

void Particle::setLife(float l)
{
	life = l;
}

void Particle::setSize(float s)
{
	size = s;
}

void Particle::setDegree(float d)
{
	degree = d;
}

void Particle::setObj(float x, float y)
{
	objX = x;
	objY = y;
	k = (this->posX - objY) / (this->posY - objX);//计算斜率
}

void Particle::draw(float s)
{
	MenuWidget::matrix->setToIdentity();
	MenuWidget::matrix->translate(posX, posY);
	MenuWidget::matrix->scale(size, size, size);

	MenuWidget::getProgram->setUniformValue("projection", *MenuWidget::matrix);
	MenuWidget::getProgram->setUniformValue("alpha", life * s, 0.0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

//	qDebug() << k;
	life -= degree;
	float ss = v * sqrt(2.0 / (k * k + 1));
	posX -= ss;
	posY -=  ss * k;

	if(life <= 0.0f) emit done(this); //粒子生命终结，发出信号
}

BlueParticle::BlueParticle() : QObject (nullptr)
{
	life = 1.0;
}

void BlueParticle::draw(float s)
{
	MenuWidget::matrix->setToIdentity();
	MenuWidget::matrix->translate(posX, posY);
	MenuWidget::matrix->scale(0.02f, 0.02f, 0.02f);

	MenuWidget::getProgram->setUniformValue("projection", *MenuWidget::matrix);
	MenuWidget::getProgram->setUniformValue("alpha", life * s, 0.0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	posY += 0.0010f;
	life -= degree;
	if(life <= 0.0f) emit done(this);
}
