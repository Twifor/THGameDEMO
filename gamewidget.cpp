#include "gamewidget.h"
#include "gameresource.h"
#include <QPainter>

GameWidget::GameWidget(QWidget *parent) : QOpenGLWidget (parent)
{
	totAlpha = 0.0f;
	pixmap = new QPixmap[30];
	life = 6;
	spellcard = 6;
	score = 0;
	power = 233;
	point = 6662333;
	graze = 100;

	status = INIT;
	mainGame = new MainGame(this);
	mainGame->setGeometry(41, 22, 487, 557);
}

GameWidget::~GameWidget()
{
	makeCurrent();
	delete bg_VBO;
	delete bg_VAO;
	delete bg_texture;
	delete bg_vs;
	delete bg_program;
	delete []pixmap;
	delete ma_fs;
	delete ma_vs;
	delete ma_program;
	delete bluestar_texture;
	delete redstar_texture;
	delete matrix;

	delete mainGame;
}

void GameWidget::initializeGL()
{
	initializeOpenGLFunctions();//全是套路
	glViewport(0, 0, 800, 600);

	matrix = new QMatrix4x4;
	float op1[] = {//背景渲染
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};
	bg_VAO = new QOpenGLVertexArrayObject;
	bg_VAO->create();
	bg_VAO->bind();

	bg_VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	bg_VBO->create();
	bg_VBO->bind();
	bg_VBO->allocate(op1, sizeof(op1));


	bg_IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	bg_IBO->create();
	bg_IBO->bind();
	bg_IBO->allocate(indices, sizeof(indices));

	QImage image(":/std/menubg.png");
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(GAME_BG_PNG))->loadData(image);
	image.scaled(800, 600);
	bg_texture = new QOpenGLTexture(image.mirrored(false, true));
	bg_texture->bind();

	bg_vs = new QOpenGLShader(QOpenGLShader::Vertex);
	bg_vs->compileSourceCode("#version 330 core\n"
							 "layout (location = 0) in vec3 aPos;\n"
							 "layout (location = 1) in vec2 aTexCoord;\n"
							 "out vec2 TexCoord;\n"
							 "void main()\n"
							 "{\n"
							 "gl_Position = vec4(aPos, 1.0);\n"
							 "TexCoord = aTexCoord;\n"
							 "}");

	bg_fs = new QOpenGLShader(QOpenGLShader::Fragment);
	bg_fs->compileSourceCode("#version 330 core\n"
							 "out vec4 FragColor;\n"
							 "in vec2 TexCoord;\n"

							 "uniform sampler2D ourTexture;\n"
							 "uniform vec2 alpha;\n"
							 "uniform sampler2D texture2;\n"

							 "void main()\n"
							 "{\n"
							 "FragColor = mix(texture(texture2, TexCoord),texture(ourTexture, TexCoord),alpha.x);\n"
							 "}");

	bg_program = new QOpenGLShaderProgram;
	bg_program->addShader(bg_vs);
	bg_program->addShader(bg_fs);
	bg_program->link();
	bg_program->bind();

	bg_program->enableAttributeArray(bg_program->attributeLocation("aPos"));
	bg_program->setAttributeBuffer(bg_program->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	bg_program->enableAttributeArray(bg_program->attributeLocation("aTexCoord"));
	bg_program->setAttributeBuffer(bg_program->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);

	bg_program->setUniformValue("ourTexture", 0);
	bg_program->setUniformValue("texture2", 1);
	bg_program->setUniformValue("alpha", totAlpha, 0.0f);


	bg_program->release();
	bg_texture->release();
	bg_VAO->release();
	bg_VBO->release();
	bg_IBO->release();


	/*-------------------------------------------------------------------------*/
	GLfloat vertices[] = {
		// 位置     // 纹理
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUESTAR_PNG))->loadData(image);
	bluestar_texture = new QOpenGLTexture(image.mirrored(false, true));
	bluestar_texture->bind();

	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(REDSTAR_PNG))->loadData(image);
	redstar_texture = new QOpenGLTexture(image.mirrored(false, true));
	redstar_texture->bind();

	ma_VAO = new QOpenGLVertexArrayObject;
	ma_VAO->create();
	ma_VAO->bind();

	ma_VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	ma_VBO->create();
	ma_VBO->bind();
	ma_VBO->allocate(vertices, sizeof(vertices));

	ma_vs = new QOpenGLShader(QOpenGLShader::Vertex);
	ma_vs->compileSourceCode("#version 330 core\n"
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

	ma_fs = new QOpenGLShader(QOpenGLShader::Fragment);
	ma_fs->compileSourceCode("#version 330 core\n"
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

	ma_program = new QOpenGLShaderProgram;
	ma_program->addShader(ma_vs);
	ma_program->addShader(ma_fs);
	ma_program->link();
	ma_program->bind();

	ma_program->enableAttributeArray(ma_program->attributeLocation("vertex"));
	ma_program->setAttributeBuffer(ma_program->attributeLocation("vertex"), GL_FLOAT, 0, 4, 4 * sizeof(float));

	ma_program->setUniformValue("sprite", 2);
	ma_program->setUniformValue("alpha", totAlpha);

	ma_VAO->release();
	ma_VBO->release();
	ma_program->release();
	bluestar_texture->release();
	redstar_texture->release();

	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(PLAYER_PNG))->loadData(image);
	pixmap[0].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(SPELLCARD_PNG))->loadData(image);
	pixmap[1].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(REDSTAR_PNG))->loadData(image);
	pixmap[2].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUESTAR_PNG))->loadData(image);
	pixmap[3].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(LINGLI_PNG))->loadData(image);
	pixmap[4].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(DEDIAN_PNG))->loadData(image);
	pixmap[5].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(GRAZE_PNG))->loadData(image);
	pixmap[6].convertFromImage(image);

	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED0_PNG))->loadData(image);
	pixmap[7].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED1_PNG))->loadData(image);
	pixmap[8].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED2_PNG))->loadData(image);
	pixmap[9].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED3_PNG))->loadData(image);
	pixmap[10].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED4_PNG))->loadData(image);
	pixmap[11].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED5_PNG))->loadData(image);
	pixmap[12].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED6_PNG))->loadData(image);
	pixmap[13].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED7_PNG))->loadData(image);
	pixmap[14].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED8_PNG))->loadData(image);
	pixmap[15].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(RED9_PNG))->loadData(image);
	pixmap[16].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(REDPOINT_PNG))->loadData(image);
	pixmap[17].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(REDXIE_PNG))->loadData(image);
	pixmap[18].convertFromImage(image);

	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE0_PNG))->loadData(image);
	pixmap[19].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE1_PNG))->loadData(image);
	pixmap[20].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE2_PNG))->loadData(image);
	pixmap[21].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE3_PNG))->loadData(image);
	pixmap[22].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE4_PNG))->loadData(image);
	pixmap[23].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE5_PNG))->loadData(image);
	pixmap[24].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE6_PNG))->loadData(image);
	pixmap[25].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE7_PNG))->loadData(image);
	pixmap[26].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE8_PNG))->loadData(image);
	pixmap[27].convertFromImage(image);
	static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(BLUE9_PNG))->loadData(image);
	pixmap[28].convertFromImage(image);

	timer.setInterval(1000 / 60);
	timer.start();
	connect(&timer, &QTimer::timeout, [ = ](){
		update();
	});
}

void GameWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void GameWidget::paintGL()//这里绘制游戏界面
{
	if(status == INIT) {
		if(totAlpha >= 1.0f) {
			totAlpha = 1.0f;
			status = MAIN;
		}
		else totAlpha += 0.05f;
	}
	QPainter painter(this);
	painter.beginNativePainting();

	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	bg_texture->bind(0);
	bg_VAO->bind();
	bg_program->bind();
	bg_program->setUniformValue("alpha", totAlpha, 0.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	bg_texture->release();
	bg_VAO->release();
	bg_program->release();

	bluestar_texture->bind(2);
	ma_VAO->bind();
	ma_program->bind();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	float be = 0.58f;
	for(int i = 1; i <= spellcard; i++) {
		matrix->setToIdentity();
		matrix->translate(be, 0.53f);
		matrix->scale(0.08f, 0.1f);
		ma_program->setUniformValue("projection", *matrix);
		ma_program->setUniformValue("alpha", totAlpha * 0.8f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		be += 0.065f;
	}
	bluestar_texture->release();
	be = 0.58f;
	redstar_texture->bind(2);
	for(int i = 1; i <= life; i++) {
		matrix->setToIdentity();
		matrix->translate(be, 0.65f);
		matrix->scale(0.08f, 0.1f);
		ma_program->setUniformValue("projection", *matrix);
		ma_program->setUniformValue("alpha", totAlpha * 0.8f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		be += 0.065f;
	}

	ma_program->release();
	ma_VAO->release();

	painter.endNativePainting();

	painter.setOpacity(totAlpha);
	painter.drawPixmap(545, 75, 94, 33, pixmap[0]);
	painter.drawPixmap(545, 110, 94, 33, pixmap[1]);
	painter.drawPixmap(545, 150, 86, 28, pixmap[4]);
	painter.drawPixmap(545, 183, 86, 28, pixmap[5]);
	painter.drawPixmap(545, 210, 107, 35, pixmap[6]);

	painter.drawPixmap(675, 150, 24, 28, pixmap[power / 100 + 7]);//绘制灵力数量
	painter.drawPixmap(688, 150, 24, 28, pixmap[17]);
	painter.drawPixmap(698, 150, 24, 28, pixmap[power % 100 / 10 + 7]);
	painter.drawPixmap(711, 150, 24, 28, pixmap[power % 10 + 7]);
	painter.drawPixmap(726, 150, 24, 28, pixmap[18]);
	painter.drawPixmap(737, 150, 24, 28, pixmap[11]);
	painter.drawPixmap(750, 150, 24, 28, pixmap[17]);
	painter.drawPixmap(760, 150, 24, 28, pixmap[7]);
	painter.drawPixmap(773, 150, 24, 28, pixmap[7]);

	int s = point, pp = 773;
	while(s) painter.drawPixmap(pp, 183, 24, 28, pixmap[19 + s % 10]), s /= 10, pp -= 13;
	s = graze, pp = 773;
	while(s) painter.drawPixmap(pp, 210, 24, 28, pixmap[19 + s % 10]), s /= 10, pp -= 13;
}
