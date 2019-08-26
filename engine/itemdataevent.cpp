#include "itemdataevent.h"
#include "openglgame.h"

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
