#include "myplane.h"
#include <QDateTime>
#include <QDebug>
#include <QOpenGLContext>
#include "scene.h"
#include <QPainter>
#include "gametexture.h"
#include "basebullet.h"
#include <QSound>
#include "musicfactory.h"
#include "gametools.h"
#include "gamewidget.h"

#define MOVINGSPEED 3

MyPlane::MyPlane( QGraphicsItem *parent) : BulletMakerBase (QPointF(247, 510), parent)
{
	isShifting = false;
	status = 0;
	timeLine = 0;
	timeLine4 = 0;
	isZ = false;

	type = MY_PLANE;
	timeLine6 = 0;
	setBall(5);

	ballLine = 0.0f;
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

void MyPlane::advance(int phase)
{
	bool isOk = true;
	if(status & 1) {
		pos.setX(pos.x() - (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.x() <= 15) {
			pos.setX(15);
		}else{
			isOk = false;
			setPos(pos);
		}
	}
	if(status & 2) {
		pos.setX(pos.x() + (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.x() >= 479) {
			pos.setX(479);
		}else{
			isOk = false;
			setPos(pos);
		}
	}
	if(status & 4) {
		pos.setY(pos.y() - (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.y() <= 22) {
			pos.setY(22);
		}else{
			setPos(pos);
			isOk = false;
		}
	}
	if(status & 8) {
		pos.setY(pos.y() + (isShifting ? MOVINGSPEED >> 1 : MOVINGSPEED));
		if(pos.y() >= 526) {
			pos.setY(526);
		}else{
			setPos(pos);
			isOk = false;
		}
	}
	if(pos.y() <= 100) emit collect(this);
	if(isOk) update();
}

QRectF MyPlane::boundingRect() const
{
	return QRectF(-20, -20, 40, 40);
}

void MyPlane::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QList<QGraphicsItem *>l = collidingItems(Qt::IntersectsItemBoundingRect);        //碰撞检测
	for(QGraphicsItem *s:l) {
		BaseItem* ss = static_cast<BaseItem*>(s);
		if(ss->type == POWER || ss->type == POINT || ss->type == UP || ss->type == SPELL) {
			if(fabs(ss->getPos().x() - pos.x()) + fabs(ss->getPos().y() - pos.y()) <= 15) {
				if(ss->type == POWER || ss->type == POINT) MusicFactory::getInstance()->playItem();
				else MusicFactory::getInstance()->playExtend();
				ss->destroyItem();
				if(ss->type == POWER) GameWidget::Instance->addPower();
				else if(ss->type == POINT) GameWidget::Instance->addPoint();
				else if(ss->type == SPELL) {
					static_cast<Scene*>(scene())->addBaseItem(new SpellExtendItem());
					GameWidget::Instance->addSpell();
				}else{
					static_cast<Scene*>(scene())->addBaseItem(new ExtendItem());
					GameWidget::Instance->addLife();
				}
			}else static_cast<FunctionalItem*>(ss)->makeTo(this);
		}
	}

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

	if(timeLine4) {
		s->getMatrix()->setToIdentity();
		s->getMatrix()->translate(GameTools::toGLX(pos.x()), GameTools::toGLY(pos.y()));
		s->getMatrix()->rotate(timeLine2, 0, 0, 1.0);
		s->getMatrix()->scale(0.1f * 1.4f, 0.1f * 1.4f);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", 0.1f * (timeLine4 / 100.0f), 1.0f);
		s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
		s->getTexture(GameTexture::SLOWEFFECT2)->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(GameTexture::SLOWEFFECT2)->release();
		timeLine2 += 5;
		if(timeLine2 >= 360) timeLine2 = 0;

		s->getMatrix()->setToIdentity();
		s->getMatrix()->translate(GameTools::toGLX(pos.x()),  GameTools::toGLY(pos.y()));
		s->getMatrix()->rotate(timeLine3, 0, 0, 1.0);
		s->getMatrix()->scale(0.1f * 1.4f, 0.1f * 1.4f);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", 0.4f * (timeLine4 / 100.0f), 1.0f);
		s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
		s->getTexture(GameTexture::SLOWEFFECT1)->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(GameTexture::SLOWEFFECT1)->release();
		timeLine3 -= 5;
		if(timeLine3 <= 0) timeLine3 = 360;
	}

	s->getMatrix()->setToIdentity();
	s->getMatrix()->translate(GameTools::toGLX(pos.x()),  GameTools::toGLY(pos.y()));
	s->getMatrix()->scale(0.08f, 0.095f);
	s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
	s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
	s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);

	if(((status & 1) == 0 && ((status & 2) == 0)) || ((status & 1) && (status & 2))) {
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
		s->getMatrix()->translate(GameTools::toGLX(pos.x()),  GameTools::toGLY(pos.y()));
		s->getMatrix()->rotate(timeLine3, 0, 0, 1.0);
		s->getMatrix()->scale(0.035f, 0.035f);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", timeLine4 / 100.0f, 1.0f);
		s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);
		s->getTexture(GameTexture::CENTER)->bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		s->getTexture(GameTexture::CENTER)->release();
	}
	if(isShifting) {
		timeLine4 += 8;
		if(timeLine4 >= 100) timeLine4 = 100;
		for(int i = 1; i <= numOfBalls; i++) {
			if(ballPos[i].y() + moveY[numOfBalls][i] < objY[numOfBalls][i]) {
				ballPos[i].setY(ballPos[i].y() + moveY[numOfBalls][i]);
				if(ballPos[i].x() < 0) ballPos[i].setX(ballPos[i].x() + moveX[numOfBalls][i]);
				else ballPos[i].setX(ballPos[i].x() - moveX[numOfBalls][i]);
			}
			drawLine(QPointF(pos.x() + ballPos[i].x(), pos.y() + ballPos[i].y()));
		}
	}else{
		timeLine4 -= 8;
		if(timeLine4 <= 0) timeLine4 = 0;
		for(int i = 1; i <= numOfBalls; i++) {
			if(ballPos[i].y() - moveY[numOfBalls][i] > offsetY[numOfBalls][i]) {
				ballPos[i].setY(ballPos[i].y() - moveY[numOfBalls][i]);
				if(ballPos[i].x() < 0) ballPos[i].setX(ballPos[i].x() - moveX[numOfBalls][i]);
				else ballPos[i].setX(ballPos[i].x() + moveX[numOfBalls][i]);
			}
		}
	}
	if(isZ) { //产生新子弹
		if(timeLine5 == 3) {
			MusicFactory::getInstance()->playFire();
			if(numOfBalls == 1) emit makeBullet(new MyBullet(QPointF(pos.x(), pos.y() - 5)));
			else if(numOfBalls == 2) {
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() - 10, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() + 10, pos.y() - 5)));
			}else if(numOfBalls == 3) {
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() - 10, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x(), pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() + 10, pos.y() - 5)));
			}else if(numOfBalls == 4) {
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() - 21, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() - 7, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() + 7, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() + 21, pos.y() - 5)));
			}else{
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() - 20, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() - 10, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x(), pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() + 10, pos.y() - 5)));
				Scene::Instance->addBaseItem(new MyBullet(QPointF(pos.x() + 20, pos.y() - 5)));
			}
		}
		++timeLine5;
		if(timeLine5 == 4) timeLine5 = 0;
	};
	s->getTexture(GameTexture::MARISA_BALL)->bind(0);
	for(int i = 1; i <= numOfBalls; i++) {
		s->getMatrix()->setToIdentity();
		s->getMatrix()->translate(GameTools::toGLX(pos.x() + ballPos[i].x()),  GameTools::toGLY(pos.y() + ballPos[i].y()));
		s->getMatrix()->rotate(timeLine6, 0, 0, 1.0);
		s->getMatrix()->scale(0.035f, 0.035f);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", 1.0f, 1.0f);
		s->getProgram1()->setUniformValue("limit", 0.0f, -100.0f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	s->getTexture(GameTexture::MARISA_BALL)->release();
	timeLine6 += 3;
	if(timeLine6 >= 360) timeLine6 = 0;


	s->getVAO1()->release();
	s->getProgram1()->release();

	painter->endNativePainting();
}

void MyPlane::drawLine(QPointF begin)
{
	Scene *s = static_cast<Scene*>(scene());
	float ss = GameTools::toGLY(begin.y());
	float p =  ss + ballLine;
	s->getTexture(GameTexture::MARISA_LINE)->bind(0);
	while(p <= 1.5) {
		s->getMatrix()->setToIdentity();
		s->getMatrix()->translate(GameTools::toGLX(begin.x()), p);
		s->getMatrix()->scale(0.035f, 0.5f);
		s->getMatrix()->rotate(90, 0, 0, 1.0);
		s->getProgram1()->setUniformValue("matrix", *s->getMatrix());
		s->getProgram1()->setUniformValue("alpha", 0.6f, 1.0f);
		s->getProgram1()->setUniformValue("limit", 0.0f, ss);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		p += 1.0f;
	}
	s->getTexture(GameTexture::MARISA_LINE)->release();
	ballLine += 0.02f;
	if(ballLine >= 0.5f) ballLine = 0.0f;
}

void MyPlane::setBall(int num)
{
	numOfBalls = num;
	for(int i = 1; i <= numOfBalls; i++) {
		ballPos[i].setX(offsetX[numOfBalls][i]);
		ballPos[i].setY(offsetY[numOfBalls][i]);
	}
}
