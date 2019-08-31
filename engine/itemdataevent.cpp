#include "itemdataevent.h"
#include "openglgame.h"
#include "itemmanager.h"
#include "gamewidget.h"

ItemDataEventBase::ItemDataEventBase(QObject *parent) : QObject(parent)
{

}

BackGroundEvent::BackGroundEvent(float begin, QObject *parent) : ItemDataEventBase (parent)
{
	s = begin;
}

bool BackGroundEvent::update(RenderBase *render)
{
	s += 0.08f;
	static_cast<BackGroundRender*>(render)->setZ(s);
	if(s >= 9.5f) return true;
	return false;
}

NullEvent::NullEvent(QObject *parent) : ItemDataEventBase (parent)
{

}

bool NullEvent::update(RenderBase *render)
{
	return false;
}

MarisaEvent::MarisaEvent(QObject *parent) : ItemDataEventBase(parent)
{

}

bool MarisaEvent::update(RenderBase *render)
{
	static_cast<PlaneRender*>(render)->setStatus(OpenGLGame::Instance->myPlaneX, OpenGLGame::Instance->myPlaneY, 0.085f, 0.085f, static_cast<TextureManager::TextureType>(TextureManager::MARISA1 + OpenGLGame::Instance->myPlaneOffset));
	return false;
}

TreeEvent::TreeEvent(float begin, QObject *parent) : ItemDataEventBase (parent)
{
	s = begin;
}

bool TreeEvent::update(RenderBase *render)
{
	s += 0.08f * 1.5f;
	static_cast<TreeRender*>(render)->setZ(s);
	if(s >= 8.5f) return true;
	return false;
}

SlowEffectEvent1::SlowEffectEvent1(QObject *parent) : ItemDataEventBase(parent)
{

}

bool SlowEffectEvent1::update(RenderBase *render)
{
	if(OpenGLGame::Instance->slowEffectTimeLine == 0) return true;
	static_cast<SlowEffectRender1*>(render)->setPos(OpenGLGame::Instance->myPlaneX, OpenGLGame::Instance->myPlaneY, OpenGLGame::Instance->slowEffectRotate1, OpenGLGame::Instance->slowEffectAlpha1);
	return false;
}

SlowEffectEvent2::SlowEffectEvent2(QObject *parent) : ItemDataEventBase (parent)
{

}

bool SlowEffectEvent2::update(RenderBase *render)
{
	if(OpenGLGame::Instance->slowEffectTimeLine == 0) return true;
	static_cast<SlowEffectRender1*>(render)->setPos(OpenGLGame::Instance->myPlaneX, OpenGLGame::Instance->myPlaneY, OpenGLGame::Instance->slowEffectRotate2, OpenGLGame::Instance->slowEffectAlpha2);
	return false;
}

CenterEvent::CenterEvent(QObject *parent)
{

}

bool CenterEvent::update(RenderBase *render)
{
	if(OpenGLGame::Instance->slowEffectTimeLine == 0) return true;
	static_cast<CenterRender*>(render)->setPos(OpenGLGame::Instance->myPlaneX, OpenGLGame::Instance->myPlaneY, OpenGLGame::Instance->slowEffectRotate1, OpenGLGame::Instance->centerAlpha);
	return false;
}

BallEvent::BallEvent(int i, QObject *parent) : ItemDataEventBase(parent)
{
	id = i;
}

bool BallEvent::update(RenderBase *render)
{
	static_cast<BallRender*>(render)->setPos(OpenGLGame::Instance->ballX[id], OpenGLGame::Instance->ballY[id] * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, OpenGLGame::Instance->ballRotate);
	return false;
}

LineEvent::LineEvent(float x, float y, float limit, QObject *parent) : ItemDataEventBase (parent)
{
	this->x = x;
	this->y = y;
	this->limit = limit;
}

bool LineEvent::update(RenderBase *render)
{
	static_cast<LineRender*>(render)->setPos(x, y, 0.5f, 0.04f, limit);
	return true;
}

MyBulletEvent::MyBulletEvent(float x, float y, QObject *parent) : x(x), y(y), ItemDataEventBase (parent)
{
}

bool MyBulletEvent::update(RenderBase *render)
{
	if(y >= 1.2f) return true;
	y += 0.08f;
	static_cast<MyBulletRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.06f * 0.8f, 0.035f * 0.8f, 0.5f);
	return false;
}

EmiterEvent::EmiterEvent(float x, float y, QObject *parent) : ItemDataEventBase (parent)
{
	this->x = x;
	this->y = y;
}

ItemEvent::ItemEvent(float x, float y, QObject *parent) : ItemDataEventBase (parent)
{
	this->x = x;
	this->y = y;
	isActive = false;
}

bool ItemEvent::isColl()
{
	return dis2() <= 0.05f || OpenGLGame::Instance->activeItems;
}

bool ItemEvent::isIn()
{
	return dis2() <= 0.001f;
}

float ItemEvent::dis2()
{
	return (x - OpenGLGame::Instance->myPlaneX) * (x - OpenGLGame::Instance->myPlaneX) + (y - OpenGLGame::Instance->myPlaneY) * (y - OpenGLGame::Instance->myPlaneY);
}

PowerEvent::PowerEvent(float x, float y, QObject *parent) : ItemEvent (x, y, parent)
{

}

bool PowerEvent::update(RenderBase *render)
{
	if(y < -1.1f) return true;
	if(isIn()) {
		GameWidget::Instance->addPower();
		return true;
	}else if(isColl()) isActive = true;
	if(isActive) {
		float ss = (OpenGLGame::Instance->myPlaneY - y) / (sqrt(dis2()));
		float cc = (OpenGLGame::Instance->myPlaneX - x) / (sqrt(dis2()));
		float angle = asin(ss);
		if(cc < 0) {
			if(ss >= 0) angle = M_PI - angle;
			else angle = -M_PI - angle;
		};
		x += 0.03f * cc;
		y += 0.03f * ss;
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, angle + M_PI / 2);
	}else{
		y -= 0.01f;
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, 0.0f);
	}
	return false;
}

PointEvent::PointEvent(float x, float y, QObject *parent) : ItemEvent (x, y, parent)
{

}

bool PointEvent::update(RenderBase *render)
{
	if(y < -1.1f) return true;
	if(isIn()) {
		GameWidget::Instance->addPoint();
		return true;
	}else if(isColl()) isActive = true;
	if(isActive) {
		float ss = (OpenGLGame::Instance->myPlaneY - y) / (sqrt(dis2()));
		float cc = (OpenGLGame::Instance->myPlaneX - x) / (sqrt(dis2()));
		float angle = asin(ss);
		if(cc < 0) {
			if(ss >= 0) angle = M_PI - angle;
			else angle = -M_PI - angle;
		};
		x += 0.03f * cc;
		y += 0.03f * ss;
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, angle + M_PI / 2);
	}else{
		y -= 0.01f;
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, 0.0f);
	}
	return false;

}
