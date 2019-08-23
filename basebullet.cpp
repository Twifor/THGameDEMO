#include "basebullet.h"
#include "scene.h"
#include <QPainter>
#include "gametools.h"

BaseBullet::BaseBullet(QPointF p, QGraphicsItem *parent) : BaseItem  (p, parent)
{

}

MyBullet::MyBullet(QPointF p, QGraphicsItem *parent) : BaseBullet (p, parent)
{
	setZValue(-1);
	type = MY_PLANE_BULLET;
}

QRectF MyBullet::boundingRect() const
{
	return QRectF(-3, -10, 12, 10);
}

void MyBullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->beginNativePainting();
	if(flag) {
		init();
		flag = false;
	}
	Scene *s = static_cast<Scene*>(scene());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	s->getVAO1()->bind();
	s->getProgram1()->bind();
	s->getTexture(GameTexture::MARISA_BULLET)->bind(0);
	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
	s->getMatrix()->rotate(90, 0, 0, 1.0);
	s->getMatrix()->scale(0.055f, 0.035f);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	s->getProgram1()->setUniformValue("alpha", 0.3f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
	s->getTexture(GameTexture::MARISA_BULLET)->bind(0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	s->getTexture(GameTexture::MARISA_BULLET)->release();

	s->getVAO1()->release();
	s->getProgram1()->release();
	painter->endNativePainting();

}

void MyBullet::advance(int phase)
{
	pos.setY(pos.y() - 12);
	setPos(pos);
	if(pos.y() <= -10) static_cast<Scene*>(scene())->deleteItem(this);
}
