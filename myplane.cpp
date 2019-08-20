#include "myplane.h"
#include <QDebug>
#include <QOpenGLContext>
#include "scene.h"
#include <QPainter>
#include "gametexture.h"
#define MOVINGSPEED 6

MyPlane::MyPlane( QGraphicsItem *parent) : BaseItem (parent)
{
	pos.setX(247);
	pos.setY(510);
	setPos(pos);

	isShifting = false;
	status = 0;
	timeLine = 0;
	timeLine4 = 0;
	isZ = false;

	type = MY_PLANE;
}

void MyPlane::startLeft()
{
	status |= 1;
	timeLine = 0;
}

void MyPlane::endLeft()
{
	status &= ~1;
}

void MyPlane::startRight()
{
	status |= 2;
	timeLine = 0;
}

void MyPlane::endRight()
{
	status &= ~2;
}

void MyPlane::startUp()
{
	status |= 4;

}

void MyPlane::endUp()
{
	status &= ~4;
}

void MyPlane::startDown()
{
	status |= 8;

}

void MyPlane::endDown()
{
	status &= ~8;
}

void MyPlane::startShift()
{
	isShifting = true;
	timeLine2 = 0;
	timeLine3 = 45;
	timeLine4 = 0;
}

void MyPlane::endShift()
{
	isShifting = false;
}

void MyPlane::startZ()
{
	isZ = true;
	timeLine5 = 0;
}

void MyPlane::endZ()
{
	isZ = false;
}

QRectF MyPlane::boundingRect() const
{
	return QRectF(-20, -25, 35, 60);
}

QPainterPath MyPlane::shape() const
{

}

void MyPlane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(status & 1) {
		pos.setX(pos.x() - (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.x() <= 15) pos.setX(15);
		setPos(pos);
	}
	if(status & 2) {
		pos.setX(pos.x() + (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.x() >= 479) pos.setX(479);
		setPos(pos);
	}
	if(status & 4) {
		pos.setY(pos.y() - (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.y() <= 22) pos.setY(22);
		setPos(pos);
	}
	if(status & 8) {
		pos.setY(pos.y() + (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.y() >= 526) pos.setY(526);
		setPos(pos);
	}

	painter->beginNativePainting();
	static bool flag = true;
	if(flag) {
		init();
		flag = false;
	}

	Scene *s = static_cast<Scene*>(scene());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s->getVAO1()->bind();
	s->getProgram1()->bind();

	if(timeLine4) {
		s->getMatrix()->setToIdentity();
		s->getMatrix()->translate((pos.x() - 247) / 247.0, (278 - pos.y()) / 278.0);
		s->getMatrix()->rotate(timeLine2, 0, 0, 1.0);
		s->getMatrix()->scale(0.1f * 1.4f, 0.1f * 1.4f);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", 0.1f * (timeLine4 / 100.0f), 1.0f);
		s->getTexture(GameTexture::SLOWEFFECT2)->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(GameTexture::SLOWEFFECT2)->release();
		timeLine2 += 5;
		if(timeLine2 >= 360) timeLine2 = 0;

		s->getMatrix()->setToIdentity();
		s->getMatrix()->translate((pos.x() - 247) / 247.0, (278 - pos.y()) / 278.0);
		s->getMatrix()->rotate(timeLine3, 0, 0, 1.0);
		s->getMatrix()->scale(0.1f * 1.4f, 0.1f * 1.4f);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", 0.4f * (timeLine4 / 100.0f), 1.0f);
		s->getTexture(GameTexture::SLOWEFFECT1)->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(GameTexture::SLOWEFFECT1)->release();
		timeLine3 -= 5;
		if(timeLine3 <= 0) timeLine3 = 360;
	}

	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate((pos.x() - 247) / 247.0, (278 - pos.y()) / 278.0);
	s->getMatrix()->scale(0.08f, 0.095f);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);

	if((status & 1) && (status & 2));
	else if((status & 1) == 0 && ((status & 2) == 0)) {
		s->getTexture(static_cast<GameTexture::TextureType>(timeLine / 5))->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(static_cast<GameTexture::TextureType>(timeLine / 5))->release();
		++timeLine;
	}else if(status & 1) {
		s->getTexture(static_cast<GameTexture::TextureType>(8 + timeLine / 5))->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(static_cast<GameTexture::TextureType>(8 + timeLine / 5))->release();
		++timeLine;
		if(timeLine == 40) timeLine = 20;
	}else if(status & 2) {
		s->getTexture(static_cast<GameTexture::TextureType>(16 + timeLine / 5))->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(static_cast<GameTexture::TextureType>(16 + timeLine / 5))->release();
		++timeLine;
		if(timeLine == 40) timeLine = 20;
	}
	if(timeLine == 40) timeLine = 0;
	if(timeLine4) {
		s->getMatrix()->setToIdentity();
		s->getMatrix()->translate((pos.x() - 247) / 247.0, (278 - pos.y()) / 278.0);
		s->getMatrix()->rotate(timeLine3, 0, 0, 1.0);
		s->getMatrix()->scale(0.035f, 0.035f);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", timeLine4 / 100.0f, 1.0f);
		s->getTexture(GameTexture::CENTER)->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(GameTexture::CENTER)->release();
	}
	if(isShifting) {
		timeLine4 += 8;
		if(timeLine4 >= 100) timeLine4 = 100;
	}else{
		timeLine4 -= 8;
		if(timeLine4 <= 0) timeLine4 = 0;
	}
	s->getVAO1()->release();
	s->getProgram1()->release();

	painter->endNativePainting();

}
