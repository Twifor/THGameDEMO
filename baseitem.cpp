#include "baseitem.h"
#include "gametools.h"
#include "scene.h"
#include <QPainter>

BaseItem::BaseItem(QPointF p, QGraphicsItem *parent) : QGraphicsItem(parent)
{
	pos = p;
	setPos(p);
	flag = true;
	lock = false;
}

QPointF BaseItem::getPos()
{
	return pos;
}

void BaseItem::destroyItem()
{
	lock = true;
}

void BaseItem::init()
{
	initializeOpenGLFunctions();
	glViewport(0, 0, 487, 557);
}

PowerItem::PowerItem(QPointF p, QGraphicsItem *parent) : FunctionalItem (p, parent)
{
	type = BaseItem::POWER;
}

QRectF PowerItem::boundingRect() const
{
	return QRectF(-5, -5, 10, 10);
}

void PowerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->beginNativePainting();
	if(flag) {
		init();
		flag = false;
	}
	Scene* s = static_cast<Scene*>(scene());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s->getVAO1()->bind();
	s->getProgram1()->bind();
	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
	if(isTo) {
		if(fabs(hh->getPos().x() - pos.x()) <= 1e-6) {
			if(hh->getPos().y() > pos.y()) s->getMatrix()->rotate(180, 0.0, 0.0, 1.0);
		}else{
			float angle = atan(k) * 180.0 / M_PI;
			if(hh->getPos().y() < pos.y() && angle >= 0) s->getMatrix()->rotate(-90 - angle, 0.0, 0.0, 1.0);
			else if(hh->getPos().y() > pos.y() && angle >= 0) s->getMatrix()->rotate(-angle + 90, 0.0, 0.0, 1.0);
			else if(hh->getPos().y() < pos.y() && angle < 0) s->getMatrix()->rotate(-angle + 90, 0.0, 0.0, 1.0);
			else s->getMatrix()->rotate(-90 - angle, 0.0, 0.0, 1.0);
		}
	}
	s->getMatrix()->scale(0.035f, 0.035f);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
	s->getTexture(GameTexture::POWER)->bind(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	s->getTexture(GameTexture::POWER)->release();
	s->getVAO1()->release();
	s->getProgram1()->release();
	painter->endNativePainting();
}

FunctionalItem::FunctionalItem(QPointF p, QGraphicsItem *parent) : BaseItem (p, parent)
{
	isTo = false;
}

void FunctionalItem::makeTo(BaseItem *h)
{
	hh = h;
	isTo = true;
}

void FunctionalItem::advance(int phase)
{
	if(lock) static_cast<Scene*>(scene())->deleteItem(this);
	if(isTo) {
		if(fabs(hh->getPos().x() - pos.x()) <= 1e-6) {
			if(pos.y() > hh->getPos().y()) pos.setY(pos.y() - 5);
			else if(pos.y() < hh->getPos().y()) pos.setY(pos.y() + 5);
		}else{
			k = (hh->getPos().y() - pos.y()) / (hh->getPos().x() - pos.x());
			float dx = 5.0 / sqrt(1 + k * k);
			if(pos.x() > hh->getPos().x()) dx = -dx;
			pos.setX(pos.x() + dx);
			pos.setY(pos.y() + dx * k);
			setPos(pos);
		}
	}else{
		pos.setY(pos.y() + 1);
		setPos(pos);
		if(pos.y() >= 567) static_cast<Scene*>(scene())->deleteItem(this);
	}
}

PointItem::PointItem(QPointF p, QGraphicsItem *parent) : FunctionalItem (p, parent)
{
	type = POINT;
}

QRectF PointItem::boundingRect() const
{
	return QRectF(-5, -5, 10, 10);
}

void PointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->beginNativePainting();
	if(flag) {
		init();
		flag = false;
	}
	Scene* s = static_cast<Scene*>(scene());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s->getVAO1()->bind();
	s->getProgram1()->bind();
	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
	if(isTo) {
		if(fabs(hh->getPos().x() - pos.x()) <= 1e-6) {
			if(hh->getPos().y() > pos.y()) s->getMatrix()->rotate(180, 0.0, 0.0, 1.0);
		}else{
			float angle = atan(k) * 180.0 / M_PI;
			if(hh->getPos().y() < pos.y() && angle >= 0) s->getMatrix()->rotate(-90 - angle, 0.0, 0.0, 1.0);
			else if(hh->getPos().y() > pos.y() && angle >= 0) s->getMatrix()->rotate(-angle + 90, 0.0, 0.0, 1.0);
			else if(hh->getPos().y() < pos.y() && angle < 0) s->getMatrix()->rotate(-angle + 90, 0.0, 0.0, 1.0);
			else s->getMatrix()->rotate(-90 - angle, 0.0, 0.0, 1.0);
		}
	}
	s->getMatrix()->scale(0.035f, 0.035f);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
	s->getTexture(GameTexture::POINT)->bind(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	s->getTexture(GameTexture::POINT)->release();
	s->getVAO1()->release();
	s->getProgram1()->release();
	painter->endNativePainting();
}

UpItem::UpItem(QPointF p, QGraphicsItem *parent) : FunctionalItem (p, parent)
{
	type = UP;
}

QRectF UpItem::boundingRect() const
{
	return QRectF(-10, -10, 20, 20);
}

void UpItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->beginNativePainting();
	if(flag) {
		init();
		flag = false;
	}
	Scene* s = static_cast<Scene*>(scene());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s->getVAO1()->bind();
	s->getProgram1()->bind();
	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
	s->getMatrix()->scale(0.0756, 0.0756);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
	s->getTexture(GameTexture::UP)->bind(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	s->getTexture(GameTexture::UP)->release();
	s->getVAO1()->release();
	s->getProgram1()->release();
	painter->endNativePainting();
}

SpellItem::SpellItem(QPointF p, QGraphicsItem *parent) : FunctionalItem (p, parent)
{
	type = SPELL;
}

QRectF SpellItem::boundingRect() const
{
	return QRectF(-10, -10, 20, 20);
}

void SpellItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
	if(flag) {
		init();
		flag = false;
	}
	Scene* s = static_cast<Scene*>(scene());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s->getVAO1()->bind();
	s->getProgram1()->bind();
	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
	s->getMatrix()->scale(0.0756, 0.0756);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
	s->getTexture(GameTexture::SPELL)->bind(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	s->getTexture(GameTexture::SPELL)->release();
	s->getVAO1()->release();
	s->getProgram1()->release();
}

SpellExtendItem::SpellExtendItem(QPointF p, QGraphicsItem *parent) : BaseItem (p, parent)
{
	type = SPELL_EXTEND;
	timeLine = 250;
	setZValue(5);//具有很高的优先级
}

QRectF SpellExtendItem::boundingRect() const
{
	return QRectF(-1, -1, 2, 2);//反正又不用于判定，矩阵设小一点
}

void SpellExtendItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->beginNativePainting();
	if(flag) {
		init();
		flag = false;
	}
	Scene* s = static_cast<Scene*>(scene());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s->getVAO1()->bind();
	s->getProgram1()->bind();
	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
	s->getMatrix()->scale(0.356, 0.0756);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	if(timeLine <= 30) s->getProgram1()->setUniformValue("alpha", timeLine / 30.0f, 1.0f);
	else s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
	s->getTexture(GameTexture::SPELL_EXTEND)->bind(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	s->getTexture(GameTexture::SPELL_EXTEND)->release();
	s->getVAO1()->release();
	s->getProgram1()->release();

	painter->endNativePainting();
}

void SpellExtendItem::advance(int)
{
	if(timeLine == 0) static_cast<Scene*>(scene())->deleteItem(this);
	--timeLine;
}

ExtendItem::ExtendItem(QPointF p, QGraphicsItem *parent) : BaseItem (p, parent)
{
	type = EXTEND;
	timeLine = 250;
	setZValue(5);
}

QRectF ExtendItem::boundingRect() const
{
	return QRectF(-1, -1, 2, 2);
}

void ExtendItem::paint(QPainter *, const QStyleOptionGraphicsItem*, QWidget*)
{
	if(flag) {
		init();
		flag = false;
	}
	Scene* s = static_cast<Scene*>(scene());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s->getVAO1()->bind();
	s->getProgram1()->bind();
	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
	s->getMatrix()->scale(0.300f, 0.1f);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	if(timeLine <= 30) s->getProgram1()->setUniformValue("alpha", timeLine / 30.0f, 1.0f);
	else s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
	s->getTexture(GameTexture::EXTEND)->bind(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	s->getTexture(GameTexture::EXTEND)->release();
	s->getVAO1()->release();
	s->getProgram1()->release();
}

void ExtendItem::advance(int)
{
	if(timeLine == 0) static_cast<Scene*>(scene())->deleteItem(this);
	--timeLine;
}
