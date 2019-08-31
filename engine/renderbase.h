#ifndef RENDERBASE_H
#define RENDERBASE_H

#include <QObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include "texturemanager.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <queue>
#include "camera.h"

class RenderBase : public QObject, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	explicit RenderBase(QObject *parent = nullptr);
	virtual void init() = 0;
	virtual void render() = 0;

signals:

public slots:
};

class TranslateRender2D : public RenderBase {//位移渲染器
	Q_OBJECT
public:
	explicit TranslateRender2D(TextureManager::TextureType type, QObject *parent = nullptr);
	~TranslateRender2D();
	virtual void init() override;
	virtual void render() override;
	virtual void setPos(float x, float y, float up, float left);

protected:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLVertexArrayObject *VAO;
	int num, offsetVBO, offsetIBO;
	int MAXNUMBER = 1;
	float *VBOOP;
	unsigned int *IBOOP;
	unsigned int index;
	void changeMAX(int s);
	TextureManager::TextureType t;
};

class TranslateAlphaRender2D : public RenderBase {//支持alpha的位移渲染器
	Q_OBJECT
public:
	explicit TranslateAlphaRender2D(TextureManager::TextureType type, QObject *parent = nullptr);
	~TranslateAlphaRender2D();
	virtual void init() override;
	virtual void render() override;
	virtual void setPos(float x, float y, float up, float left, float alpha);

protected:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLVertexArrayObject *VAO;
	int num, offsetVBO, offsetIBO;
	int MAXNUMBER = 1;
	float *VBOOP;
	unsigned int *IBOOP;
	unsigned int index;
	void changeMAX(int s);
	TextureManager::TextureType t;
};

class RotateRender2D : public RenderBase {//旋转渲染器
	Q_OBJECT
public:
	explicit RotateRender2D(TextureManager::TextureType type, QObject *parent = nullptr);
	~RotateRender2D();
	void init() override;
	void render() override;
	void setPos(float x, float y, float up, float left, float angle);

protected:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLVertexArrayObject *VAO;
	int num, offsetVBO, offsetIBO;
	int MAXNUMBER = 1;
	float *VBOOP;
	unsigned int *IBOOP;
	unsigned int index;
	void changeMAX(int s);
	TextureManager::TextureType t;
};
class RotateAlphaRender2D : public RenderBase {
	Q_OBJECT
public:
	explicit RotateAlphaRender2D(TextureManager::TextureType type, QObject *parent = nullptr);
	~RotateAlphaRender2D();
	void init() override;
	void render() override;
	void setPos(float x, float y, float up, float left, float angle, float alpha);

protected:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLVertexArrayObject *VAO;
	int num, offsetVBO, offsetIBO;
	int MAXNUMBER = 1;
	float *VBOOP;
	unsigned int *IBOOP;
	unsigned int index;
	void changeMAX(int s);
	TextureManager::TextureType t;

};

class BackGroundRender : public TranslateRender2D {
	Q_OBJECT
public:
	BackGroundRender(TextureManager::TextureType type, QObject *parent = nullptr);
	void setZ(float z);
	void render() override;
	void init() override;

protected:
	std::queue<float> que;
	Camera camera;
};

class StarBackGroundRender : public TranslateRender2D {
	Q_OBJECT
public:
	StarBackGroundRender(TextureManager::TextureType type, QObject *parent = nullptr);
	void render() override;
	void init() override;
};

class PlaneRender : public RenderBase {
	Q_OBJECT
public:
	PlaneRender(QObject *parent = nullptr);
	void init() override;
	void render() override;
	void setStatus(float x, float y, float up, float left, TextureManager::TextureType type);

protected:
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLVertexArrayObject *VAO;
	int num, offsetVBO, offsetIBO;
	int MAXNUMBER = 1;
	float *VBOOP;
	unsigned int *IBOOP;
	unsigned int index;
	TextureManager::TextureType t;
};

class TreeRender : public BackGroundRender {
	Q_OBJECT
public:
	TreeRender(TextureManager::TextureType type, QObject *parent = nullptr);
	void render() override;
};

class SlowEffectRender1 : public RenderBase {
	Q_OBJECT
public:
	SlowEffectRender1(QObject *parent = nullptr);
	~SlowEffectRender1();
	void setPos(float x, float y, float angle, float alpha);
	void init() override;
	void render() override;

protected:
	float x0, y0, angle0, alpha0;
	QOpenGLBuffer *VBO, *IBO;
	QOpenGLVertexArrayObject *VAO;
};

class SlowEffectRender2 : public SlowEffectRender1 {
	Q_OBJECT
public:
	SlowEffectRender2(QObject *parent = nullptr);
	void render() override;
};

class TreeRender2 : public BackGroundRender {
	Q_OBJECT
public:
	TreeRender2(TextureManager::TextureType type, QObject *parent = nullptr);
	void render()override;
};

class CenterRender : public SlowEffectRender1 {
	Q_OBJECT
public:
	CenterRender(QObject *parent = nullptr);
	void render() override;
};

class BallRender : public RotateRender2D {
	Q_OBJECT
public:
	BallRender(QObject *parent = nullptr);
};

class LineRender : public RenderBase {
	Q_OBJECT
public:
	LineRender(QObject *parent = nullptr);
	void init() override;
	void render() override;
	void setPos(float x, float y, float up, float left, float limit);

protected:
	QOpenGLBuffer * VBO, *IBO;
	QOpenGLVertexArrayObject *VAO;
	int num, offsetVBO, offsetIBO;
	int MAXNUMBER = 25;
	float *VBOOP;
	unsigned int *IBOOP;
	unsigned int index;
};

class NullRender : public RenderBase {
	Q_OBJECT
public:
	NullRender(QObject *parent = nullptr);
	void init() override;
	void render() override;
};

class MyBulletRender : public TranslateAlphaRender2D {
	Q_OBJECT
public:
	MyBulletRender(QObject *parent = nullptr);
};

class ItemRender : public RotateRender2D {
	Q_OBJECT
public:
	ItemRender(TextureManager::TextureType type, QObject *parent = nullptr);
};

class SmallItemRender : public RotateRender2D {
	Q_OBJECT
public:
	SmallItemRender(TextureManager::TextureType type, QObject *parent = nullptr);
};

class TipRender : public TranslateAlphaRender2D {
	Q_OBJECT
public:
	explicit TipRender(TextureManager::TextureType type, QObject *parent = nullptr);
};

#endif // RENDERBASE_H
