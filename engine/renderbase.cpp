#include "renderbase.h"
#include "shadermanager.h"
#include "itemmanager.h"

RenderBase::RenderBase(QObject *parent) : QObject(parent)
{
	initializeOpenGLFunctions();
}

TranslateRender2D::TranslateRender2D(TextureManager::TextureType type, QObject *parent) : RenderBase (parent), t(type)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 80];
	IBOOP = new unsigned int[MAXNUMBER * 24];
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
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("div", ItemManager::INSTANCE()->getDiv());
	TextureManager::INSTANCE()->getTexture(t)->bind(0);

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

RotateRender2D::RotateRender2D(TextureManager::TextureType type, QObject *parent) : t(type), RenderBase (parent)
{
	VAO = new QOpenGLVertexArrayObject;
	VAO->create();
	VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	VBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);
	IBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	IBO->setUsagePattern(QOpenGLBuffer::DynamicDraw);

	VBOOP = new float[MAXNUMBER * 128];
	IBOOP = new unsigned int[MAXNUMBER * 24];
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
	ShaderManager::INSTANCE()->getProgram(1)->setUniformValue("div", ItemManager::INSTANCE()->getDiv());
	TextureManager::INSTANCE()->getTexture(t)->bind(0);

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

BackGroundRender::BackGroundRender(TextureManager::TextureType type, QObject *parent) : TranslateRender2D (type, parent)
{
	delete VBOOP;
	delete IBOOP;
	VBOOP = new float[20];
	IBOOP = new unsigned int[6];
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
	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);

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
	delete VBOOP;
	delete IBOOP;
	VBOOP = new float[20];
	IBOOP = new unsigned int[6];
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
	ShaderManager::INSTANCE()->getProgram(0)->setUniformValue("div", ItemManager::INSTANCE()->getDiv());
	TextureManager::INSTANCE()->getTexture(t)->bind(0);

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
	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);

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
	VBO->allocate(VBOOP, num * 80);
	IBO->allocate(IBOOP, num * 24);

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
