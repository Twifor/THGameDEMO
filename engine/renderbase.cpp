#include "renderbase.h"
#include "shadermanager.h"
#include "itemmanager.h"
#include "openglgame.h"
#include "gamewidget.h"

RenderBase::RenderBase(QObject *parent) : QObject(parent)
{
	initializeOpenGLFunctions();
}

void RenderBase::offset(float &x, float &y)
{
	x += OpenGLGame::Instance->offsetX;
	y += OpenGLGame::Instance->offsetY;
}

TranslateRender2D::TranslateRender2D(TextureManager::TextureType type, QObject *parent) : RenderBase (parent), t(type)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 20];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

TranslateRender2D::~TranslateRender2D()
{
	delete VAO;
	delete VBO;
	delete IBO;

	delete []VBOOP;
	delete []IBOOP;
}

void TranslateRender2D::init()
{
	num = 0;
	offsetIBO = offsetVBO = 0;
	index = 0;
}

void TranslateRender2D::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(0)->bind();
	ShaderManager::INSTANCE()->getProgram(0)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(0)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(0)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(0)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);
	TextureManager::INSTANCE()->getTexture(t)->bind(0);
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);

	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(0)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void TranslateRender2D::setPos(float x, float y, float up, float left)
{
	offset(x, y);
	++num;
	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = y + up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;

	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = y - up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = y - up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 0.0f;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = y + up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 1.0f;

	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 1;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index + 3;
	index += 4;
}

void TranslateRender2D::changeMAX(int s)
{
	MAXNUMBER = s;
	delete VBOOP;
	delete IBOOP;

	VBOOP = new float[MAXNUMBER * 20];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

RotateRender2D::RotateRender2D(TextureManager::TextureType type, QObject *parent) : t(type), RenderBase (parent)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 32];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

RotateRender2D::~RotateRender2D()
{
	delete VAO;
	delete VBO;
	delete IBO;

	delete []VBOOP;
	delete []IBOOP;
}

void RotateRender2D::init()
{
	num = 0;
	offsetIBO = offsetVBO = 0;
	index = 0;
}

void RotateRender2D::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(1)->bind();
	ShaderManager::INSTANCE()->getProgram(1)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(1)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(1)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(1)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 8);

	ShaderManager::INSTANCE()->getProgram(1)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(1)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(1)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(1)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 8);

	ShaderManager::INSTANCE()->getProgram(1)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(1)->attributeLocation("rotate"));
	ShaderManager::INSTANCE()->getProgram(1)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(1)->attributeLocation("rotate"), GL_FLOAT, 5 * sizeof(float), 3, sizeof(float) * 8);
	TextureManager::INSTANCE()->getTexture(t)->bind(0);
	ShaderManager::INSTANCE()->getProgram(1)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(1)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	VBO->allocate(VBOOP, num * 128);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(1)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void RotateRender2D::setPos(float x, float y, float up, float left, float angle)
{
	offset(x, y);
	++num;

	VBOOP[offsetVBO++] = -left;
	VBOOP[offsetVBO++] = up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;

	VBOOP[offsetVBO++] = -left;
	VBOOP[offsetVBO++] = -up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;

	VBOOP[offsetVBO++] = left;
	VBOOP[offsetVBO++] = -up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;

	VBOOP[offsetVBO++] = left;
	VBOOP[offsetVBO++] = up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;

	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 1;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index + 3;
	index += 4;
}

void RotateRender2D::changeMAX(int s)
{
	MAXNUMBER = s;
	delete VBOOP;
	delete IBOOP;

	VBOOP = new float[MAXNUMBER * 32];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

BackGroundRender::BackGroundRender(TextureManager::TextureType type, QObject *parent) : TranslateRender2D (type, parent)
{
	changeMAX(1);
}

void BackGroundRender::setZ(float z)
{
	que.push(z);
}

void BackGroundRender::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(2)->bind();
	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("div", ItemManager::INSTANCE()->getDiv());
	TextureManager::INSTANCE()->getTexture(t)->bind(0);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while(!que.empty()) {
		float s = que.front();
		que.pop();
		QMatrix4x4 matrix, pr;
		matrix.setToIdentity();
		pr.setToIdentity();
		pr.perspective(45.0, 487.0f / 557.0f, 0.1f, 50.0f);
		matrix.translate(0, 0.0, s);
		matrix.rotate(-90, 1.0, 0, 0.0);
		matrix.scale(6.0f, 6.0f);
		camera.setPos(QVector3D(0, 1.2f, 6.0f));
		camera.setPitch(0);
		ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("alpha", 1.0f, 0.0f);
		ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("projection", pr * camera.getMatrix() * matrix);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	ShaderManager::INSTANCE()->getProgram(2)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void BackGroundRender::init()
{
	TranslateRender2D::init();
	while(!que.empty()) que.pop();
	setPos(0, 0, 1.0, 1.0);
}

StarBackGroundRender::StarBackGroundRender(TextureManager::TextureType type, QObject *parent) : TranslateRender2D (type, parent)
{
	changeMAX(1);
}

void StarBackGroundRender::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(2)->bind();
	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("div", ItemManager::INSTANCE()->getDiv());
	TextureManager::INSTANCE()->getTexture(t)->bind(0);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);

	QMatrix4x4 matrix;
	matrix.setToIdentity();

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("alpha", 1.0f, 0.0f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("projection", matrix);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(2)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void StarBackGroundRender::init()
{
	TranslateRender2D::init();
	setPos(0, 0, 1.0, 1.0);
}

PlaneRender::PlaneRender(QObject *parent) : RenderBase (parent)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 80];
	IBOOP = new unsigned int[MAXNUMBER * 24];

	t = TextureManager::MARISA1;
}

void PlaneRender::init()
{
	num = 0;
	offsetIBO = offsetVBO = 0;
	index = 0;
}

void PlaneRender::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(0)->bind();
	ShaderManager::INSTANCE()->getProgram(0)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(0)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(0)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(0)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(0)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);
	TextureManager::INSTANCE()->getTexture(t)->bind(0);
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);

	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(0)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void PlaneRender::setStatus(float x, float y, float up, float left, TextureManager::TextureType type)
{
	offset(x, y);
	++num;
	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = (y + up) * ItemManager::INSTANCE()->getDiv();
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;

	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = (y - up) * ItemManager::INSTANCE()->getDiv();
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = (y - up) * ItemManager::INSTANCE()->getDiv();
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 0.0f;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = (y + up) * ItemManager::INSTANCE()->getDiv();
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 1.0f;

	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 1;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index + 3;
	t = type;
	index += 4;
}

TreeRender::TreeRender(TextureManager::TextureType type, QObject *parent) : BackGroundRender(type, parent)
{
	delete VBOOP;
	delete IBOOP;
	VBOOP = new float[100];
	IBOOP = new unsigned int[50];
}

void TreeRender::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(2)->bind();
	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("div", ItemManager::INSTANCE()->getDiv());
	TextureManager::INSTANCE()->getTexture(t)->bind(0);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);
//	glBlendFunc(GL_ONE, GL_ZERO);

	while(!que.empty()) {
		float s = que.front();
		que.pop();
		QMatrix4x4 matrix, pr;
		matrix.setToIdentity();
		pr.setToIdentity();
		pr.perspective(45.0, 487.0f / 557.0f, 0.1f, 50.0f);
		matrix.translate(-3.8f, 2.0f, s);
		matrix.scale(2.8f, 2.8f);
		camera.setPos(QVector3D(0, 1.5f, 6.0f));
		camera.setPitch(0);
		ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("alpha", 1.3f, 0.0f);
		ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("projection", pr * camera.getMatrix() * matrix);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	ShaderManager::INSTANCE()->getProgram(2)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

TreeRender2::TreeRender2(TextureManager::TextureType type, QObject *parent) : BackGroundRender(type, parent)
{
	delete VBOOP;
	delete IBOOP;
	VBOOP = new float[100];
	IBOOP = new unsigned int[50];
}

void TreeRender2::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(2)->bind();
	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("div", ItemManager::INSTANCE()->getDiv());
	TextureManager::INSTANCE()->getTexture(t)->bind(0);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);
//	glBlendFunc(GL_ONE, GL_ZERO);

	while(!que.empty()) {
		float s = que.front();
		que.pop();
		QMatrix4x4 matrix, pr;
		matrix.setToIdentity();
		pr.setToIdentity();
		pr.perspective(45.0, 487.0f / 557.0f, 0.1f, 50.0f);
		matrix.translate(3.8f, 2.0f, s);
		matrix.scale(2.8f, 2.8f);
		camera.setPos(QVector3D(0, 1.5f, 6.0f));
		camera.setPitch(0);
		ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("alpha", 1.3f, 0.0f);
		ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("projection", pr * camera.getMatrix() * matrix);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	ShaderManager::INSTANCE()->getProgram(2)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

SlowEffectRender1::SlowEffectRender1(QObject *parent) : RenderBase (parent)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->create();
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->create();
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	float Q[] = {
		-1.0, 1.0, 0.0, 0.0, 1.0,
		-1.0, -1.0, 0.0, 0.0, 0.0,
		1.0, -1.0, 0.0, 1.0, 0.0,
		1.0, 1.0, 0.0, 1.0, 1.0
	};
	unsigned int index[] = {
		0, 1, 2,
		0, 2, 3
	};
	VAO->bind();
	VBO->bind();
	VBO->allocate(Q, sizeof(Q));
	IBO->bind();
	IBO->allocate(index, sizeof(index));
	ShaderManager::INSTANCE()->getProgram(2)->bind();

	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 5);

	ShaderManager::INSTANCE()->getProgram(2)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(2)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(2)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 5);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	VAO->release();
	VBO->release();
	IBO->release();
	ShaderManager::INSTANCE()->getProgram(2)->release();
}

SlowEffectRender1::~SlowEffectRender1()
{
	delete VBO;
	delete IBO;
	delete VAO;
}

void SlowEffectRender1::setPos(float x, float y, float angle, float alpha)
{
	x0 = x;
	y0 = y;
	angle0 = angle;
	alpha0 = alpha;
}

void SlowEffectRender1::init()
{

}

void SlowEffectRender1::render()
{
	VAO->bind();
	ShaderManager::INSTANCE()->getProgram(2)->bind();
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("div", 1.0f);
	TextureManager::INSTANCE()->getTexture(TextureManager::SLOWEFFECT1_PNG)->bind(0);

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("alpha", alpha0, 0.0f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	QMatrix4x4 matrix;
	matrix.translate(x0, y0 * ItemManager::INSTANCE()->getDiv(), 0);
	matrix.rotate(angle0, 0.0, 0.0, 1.0);
	matrix.scale(0.1f * 1.4f, 0.1f * 1.4f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("projection", matrix);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	VAO->release();
	ShaderManager::INSTANCE()->getProgram(2)->release();
}

SlowEffectRender2::SlowEffectRender2(QObject *parent) : SlowEffectRender1 (parent)
{

}

void SlowEffectRender2::render()
{
	VAO->bind();
	ShaderManager::INSTANCE()->getProgram(2)->bind();
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("div", 1.0f);
	TextureManager::INSTANCE()->getTexture(TextureManager::SLOWEFFECT2_PNG)->bind(0);

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("alpha", alpha0, 0.0f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	QMatrix4x4 matrix;
	matrix.translate(x0, y0 * ItemManager::INSTANCE()->getDiv(), 0);
	matrix.rotate(angle0, 0.0, 0.0, 1.0);
	matrix.scale(0.1f * 1.4f, 0.1f * 1.4f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("projection", matrix);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	VAO->release();
	ShaderManager::INSTANCE()->getProgram(2)->release();
}

CenterRender::CenterRender(QObject *parent) : SlowEffectRender1 (parent)
{

}

void CenterRender::render()
{
	VAO->bind();
	ShaderManager::INSTANCE()->getProgram(2)->bind();
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("div", 1.0f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	TextureManager::INSTANCE()->getTexture(TextureManager::CENTER)->bind(0);

	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("alpha", alpha0, 0.0f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());
	QMatrix4x4 matrix;
	matrix.translate(x0, y0 * ItemManager::INSTANCE()->getDiv(), 0);
	matrix.rotate(angle0, 0.0, 0.0, 1.0);
	matrix.scale(0.035f, 0.035f);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	ShaderManager::INSTANCE()->getProgram(2)->setUniformValue("projection", matrix);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	VAO->release();
	ShaderManager::INSTANCE()->getProgram(2)->release();
}

BallRender::BallRender(QObject *parent) : RotateRender2D (TextureManager::BALL, parent)
{
	changeMAX(5);
}

LineRender::LineRender(QObject *parent) : RenderBase (parent)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 24];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

void LineRender::init()
{
	num = 0;
	offsetIBO = offsetVBO = 0;
	index = 0;
}

void LineRender::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(3)->bind();
	ShaderManager::INSTANCE()->getProgram(3)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(3)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(3)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(3)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(3)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(3)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(3)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(3)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(3)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(3)->attributeLocation("limit"));
	ShaderManager::INSTANCE()->getProgram(3)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(3)->attributeLocation("limit"), GL_FLOAT, 5 * sizeof(float), 1, sizeof(float) * 6);

	TextureManager::INSTANCE()->getTexture(TextureManager::MARISA_LINE)->bind(0);
	ShaderManager::INSTANCE()->getProgram(3)->setUniformValue("alpha", 0.6f, 0.0f);
	ShaderManager::INSTANCE()->getProgram(3)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(3)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);
	ShaderManager::INSTANCE()->getProgram(3)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	VBO->allocate(VBOOP, num * 96);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(3)->release();
	TextureManager::INSTANCE()->getTexture(TextureManager::MARISA_LINE)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void LineRender::setPos(float x, float y, float up, float left, float limit)
{
	offset(x, y);
	++num;
	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = y + up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = limit;

	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = y - up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = limit;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = y - up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = limit;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = y + up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = limit;

	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 1;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index + 3;
	index += 4;
}

NullRender::NullRender(QObject *parent) : RenderBase (parent)
{

}

void NullRender::init()
{

}

void NullRender::render()
{

}

TranslateAlphaRender2D::TranslateAlphaRender2D(TextureManager::TextureType type, QObject *parent) : t(type), RenderBase (parent)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 24];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

TranslateAlphaRender2D::~TranslateAlphaRender2D()
{
	delete VAO;
	delete VBO;
	delete IBO;

	delete []VBOOP;
	delete []IBOOP;
}

void TranslateAlphaRender2D::init()
{
	num = 0;
	offsetIBO = offsetVBO = 0;
	index = 0;
}

void TranslateAlphaRender2D::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(4)->bind();
	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("alpha"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("alpha"), GL_FLOAT, 5 * sizeof(float), 1, sizeof(float) * 6);
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);

	TextureManager::INSTANCE()->getTexture(t)->bind(0);

	VBO->allocate(VBOOP, num * 96);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(4)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void TranslateAlphaRender2D::setPos(float x, float y, float up, float left, float alpha)
{
	offset(x, y);
	++num;
	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = y + up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = alpha;

	VBOOP[offsetVBO++] = x - left;
	VBOOP[offsetVBO++] = y - up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = alpha;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = y - up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = alpha;

	VBOOP[offsetVBO++] = x + left;
	VBOOP[offsetVBO++] = y + up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = alpha;

	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 1;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index + 3;
	index += 4;
}

void TranslateAlphaRender2D::changeMAX(int s)
{
	MAXNUMBER = s;
	delete VBOOP;
	delete IBOOP;

	VBOOP = new float[MAXNUMBER * 24];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

MyBulletRender::MyBulletRender(QObject *parent) : TranslateAlphaRender2D (TextureManager::MY_BULLET, parent)
{
	changeMAX(100);
}

RotateAlphaRender2D::RotateAlphaRender2D(TextureManager::TextureType type, QObject *parent) : RenderBase (parent)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 36];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

RotateAlphaRender2D::~RotateAlphaRender2D()
{
	delete VAO;
	delete VBO;
	delete IBO;

	delete []VBOOP;
	delete []IBOOP;
}

void RotateAlphaRender2D::init()
{
	num = 0;
	offsetIBO = offsetVBO = 0;
	index = 0;
}

void RotateAlphaRender2D::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(5)->bind();
	ShaderManager::INSTANCE()->getProgram(5)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(5)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 9);

	ShaderManager::INSTANCE()->getProgram(5)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(5)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 9);

	ShaderManager::INSTANCE()->getProgram(5)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("rotate"));
	ShaderManager::INSTANCE()->getProgram(5)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("rotate"), GL_FLOAT, 5 * sizeof(float), 3, sizeof(float) * 9);
	ShaderManager::INSTANCE()->getProgram(5)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("alpha"));
	ShaderManager::INSTANCE()->getProgram(5)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(5)->attributeLocation("alpha"), GL_FLOAT, 8 * sizeof(float), 1, sizeof(float) * 9);
	ShaderManager::INSTANCE()->getProgram(5)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(5)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());
	ShaderManager::INSTANCE()->getProgram(5)->setUniformValue("isBlack", OpenGLGame::Instance->isBlack);

	TextureManager::INSTANCE()->getTexture(t)->bind(0);

	VBO->allocate(VBOOP, num * 144);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(5)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

void RotateAlphaRender2D::setPos(float x, float y, float up, float left, float angle, float alpha)
{
	offset(x, y);
	++num;

	VBOOP[offsetVBO++] = -left;
	VBOOP[offsetVBO++] = up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;
	VBOOP[offsetVBO++] = alpha;

	VBOOP[offsetVBO++] = -left;
	VBOOP[offsetVBO++] = -up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;
	VBOOP[offsetVBO++] = alpha;

	VBOOP[offsetVBO++] = left;
	VBOOP[offsetVBO++] = -up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;
	VBOOP[offsetVBO++] = alpha;

	VBOOP[offsetVBO++] = left;
	VBOOP[offsetVBO++] = up;
	VBOOP[offsetVBO++] = 0.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = 1.0f;
	VBOOP[offsetVBO++] = x;
	VBOOP[offsetVBO++] = y;
	VBOOP[offsetVBO++] = angle;
	VBOOP[offsetVBO++] = alpha;

	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 1;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index;
	IBOOP[offsetIBO++] = index + 2;
	IBOOP[offsetIBO++] = index + 3;
	index += 4;
}

void RotateAlphaRender2D::changeMAX(int s)
{
	MAXNUMBER = s;
	delete VBOOP;
	delete IBOOP;

	VBOOP = new float[MAXNUMBER * 36];
	IBOOP = new unsigned int[MAXNUMBER * 6];
}

ItemRender::ItemRender(TextureManager::TextureType type, QObject *parent) : RotateRender2D (type, parent)
{
	changeMAX(5000);
}

SmallItemRender::SmallItemRender(TextureManager::TextureType type, QObject *parent) : RotateRender2D (type, parent)
{
	changeMAX(2);
}

TipRender::TipRender(TextureManager::TextureType type, QObject *parent) : TranslateAlphaRender2D (type, parent)
{
	changeMAX(2);
}

WhiteNumberRender::WhiteNumberRender(TextureManager::TextureType type, QObject *parent) : TranslateAlphaRender2D (type, parent)
{
	changeMAX(100);
}

PauseMenuRender::PauseMenuRender(TextureManager::TextureType type, QObject *parent) : TranslateAlphaRender2D (type, parent)
{
	changeMAX(1);
}

void PauseMenuRender::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(4)->bind();
	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("alpha"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("alpha"), GL_FLOAT, 5 * sizeof(float), 1, sizeof(float) * 6);
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("isBlur", false);
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("isBlack", false);
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());

	TextureManager::INSTANCE()->getTexture(t)->bind(0);

	VBO->allocate(VBOOP, num * 96);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(4)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

SPPRender::SPPRender(TextureManager::TextureType type, QObject *parent) : TranslateRender2D (type, parent)
{
	changeMAX(15);
}

MasterRender::MasterRender(TextureManager::TextureType type, QObject *parent) : TranslateAlphaRender2D (type, parent)
{
	changeMAX(1);
}

void MasterRender::render()
{
	VBO->destroy();
	VAO->destroy();
	IBO->destroy();
	VAO->create();
	VAO->bind();
	VBO->create();
	VBO->bind();
	IBO->create();
	IBO->bind();

	ShaderManager::INSTANCE()->getProgram(4)->bind();
	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aPos"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aPos"), GL_FLOAT, 0, 3, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aTexCoord"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("aTexCoord"), GL_FLOAT, 3 * sizeof(float), 2, sizeof(float) * 6);

	ShaderManager::INSTANCE()->getProgram(4)->enableAttributeArray(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("alpha"));
	ShaderManager::INSTANCE()->getProgram(4)->setAttributeBuffer(ShaderManager::INSTANCE()->getProgram(4)->attributeLocation("alpha"), GL_FLOAT, 5 * sizeof(float), 1, sizeof(float) * 6);
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("isBlur", OpenGLGame::Instance->isPause());
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("totAlpha", GameWidget::Instance->getAlpha());
	ShaderManager::INSTANCE()->getProgram(4)->setUniformValue("isBlack", false);

	TextureManager::INSTANCE()->getTexture(t)->bind(0);

	VBO->allocate(VBOOP, num * 96);
	IBO->allocate(IBOOP, num * 24);

	glDrawElements(GL_TRIANGLES, num * 6, GL_UNSIGNED_INT, 0);

	ShaderManager::INSTANCE()->getProgram(4)->release();
	TextureManager::INSTANCE()->getTexture(t)->release();
	VAO->release();
	VBO->release();
	IBO->release();
}

