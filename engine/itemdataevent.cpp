#include "itemdataevent.h"
#include "openglgame.h"
#include "itemmanager.h"

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
