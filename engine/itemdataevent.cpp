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
	if(!OpenGLGame::Instance->isPause()) s += 0.08f;
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
	if(!OpenGLGame::Instance->isPause()) s += 0.08f * 1.5f;
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
	if(!OpenGLGame::Instance->isPause()) y += 0.08f;
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
		if(!OpenGLGame::Instance->isPause()) {
			x += 0.03f * cc;
			y += 0.03f * ss;
		}
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, angle + M_PI / 2);
	}else{
		if(!OpenGLGame::Instance->isPause()) y -= 0.01f;
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
		int score;
		if(y >= 0.7f) score = 1000;
		else if(y <= -0.4f) score = 0;
		else score = (y + 0.4) / 1.2f * 800;
		float ss = OpenGLGame::Instance->myPlaneX + 0.05f;
		GameWidget::Instance->addScore(score);
		for(int i = 0; i < 4; i++) {
			ItemManager::INSTANCE()->addNewItem(static_cast<ItemManager::ItemType>(ItemManager::W0 + score % 10), 1, new WhiteNumberEvent(ss, OpenGLGame::Instance->myPlaneY + 0.1f));
			score /= 10;
			ss -= 0.03f;
		}
		ItemManager::INSTANCE()->addNewItem(ItemManager::W_PLUS, 1, new WhiteNumberEvent(ss, OpenGLGame::Instance->myPlaneY + 0.1f));
		GameWidget::Instance->addScore(score);
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
		if(!OpenGLGame::Instance->isPause()) {
			x += 0.03f * cc;
			y += 0.03f * ss;
		}
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, angle + M_PI / 2);
	}else{
		if(!OpenGLGame::Instance->isPause()) y -= 0.01f;
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f, 0.035f, 0.0f);
	}
	return false;
}

ExtendEvent::ExtendEvent(float x, float y, QObject *parent) : ItemEvent (x, y, parent)
{

}

bool ExtendEvent::update(RenderBase *render)
{
	if(y < -1.1f) return true;
	if(isIn()) {
		GameWidget::Instance->addLife();
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
		if(!OpenGLGame::Instance->isPause()) {
			x += 0.03f * cc;
			y += 0.03f * ss;
		}
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.075f, 0.075f, angle + M_PI / 2);
	}else{
		if(!OpenGLGame::Instance->isPause()) y -= 0.01f;
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.075f, 0.075f, 0.0f);
	}
	return false;
}

SPExtendEvent::SPExtendEvent(float x, float y, QObject *parent) : ItemEvent (x, y, parent)
{

}

bool SPExtendEvent::update(RenderBase *render)
{
	if(y < -1.1f) return true;
	if(isIn()) {
		GameWidget::Instance->addSpell();
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
		if(!OpenGLGame::Instance->isPause()) {
			x += 0.03f * cc;
			y += 0.03f * ss;
		}
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.075f, 0.075f, angle + M_PI / 2);
	}else{
		if(!OpenGLGame::Instance->isPause()) y -= 0.01f;
		static_cast<RotateRender2D*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.075f, 0.075f, 0.0f);
	}
	return false;
}

ExtendTipEvent::ExtendTipEvent(QObject *parent) : ItemDataEventBase (parent)
{
	time = 0;
}

bool ExtendTipEvent::update(RenderBase *render)
{
	if(!OpenGLGame::Instance->isPause()) ++time;
	if(time > 90) return true;
	if(time <= 60) static_cast<TipRender*>(render)->setPos(0.05f, 0.5f * ItemManager::INSTANCE()->getDiv(), 0.1f, 0.3f, 1.0f);
	else static_cast<TipRender*>(render)->setPos(0.05f, 0.5f * ItemManager::INSTANCE()->getDiv(), 0.1f, 0.3f, (90 - time) / 30.0f);
	return false;
}

SPExtendTipEvent::SPExtendTipEvent(QObject *parent) : ItemDataEventBase (parent)
{
	time = 0;
}

bool SPExtendTipEvent::update(RenderBase *render)
{
	if(!OpenGLGame::Instance->isPause()) ++time;
	if(time > 90) return true;
	if(time <= 60) static_cast<TipRender*>(render)->setPos(0.05f, 0.5f * ItemManager::INSTANCE()->getDiv(), 0.0756f, 0.356f, 1.0f);
	else static_cast<TipRender*>(render)->setPos(0.05f, 0.5f * ItemManager::INSTANCE()->getDiv(), 0.0756f, 0.356f, (90 - time) / 30.0f);
	return false;
}

ItemGetLineEvent::ItemGetLineEvent(QObject *parent) : ItemDataEventBase (parent)
{
	time = 0;
}

bool ItemGetLineEvent::update(RenderBase *render)
{
	if(!OpenGLGame::Instance->isPause()) ++time;
	if(time > 180) return true;
	static_cast<TipRender*>(render)->setPos(0.0f, 0.6f * ItemManager::INSTANCE()->getDiv(), 0.065f, 1.2f, fabs(sin(time * M_PI / 90.0f)));
	return false;
}

WhiteNumberEvent::WhiteNumberEvent(float x, float y, QObject *parent) : ItemDataEventBase (parent)
{
	this->x = x;
	this->y = y;
	time = 0;
}

bool WhiteNumberEvent::update(RenderBase *render)
{
	if(time >= 40) return true;
	if(!OpenGLGame::Instance->isPause()) ++time;
	if(!OpenGLGame::Instance->isPause()) y += 0.001f;
	static_cast<WhiteNumberRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.02, 0.02, (40 - time) / 40.0);
	return false;
}

PauseMenuEvent::PauseMenuEvent(int id, float beginY, bool lock, QObject *parent) : ItemDataEventBase (parent)
{
	this->id = id;
	if(!lock) y = -0.2f;
	else y = beginY;
	if(!lock) x = -1.0f;
	else x = -0.5f;
	if(!lock) time = 0;
	else time = 100;
	lk = lock;
	offset = id * 10;
	k = (beginY + 0.2f) / 0.5f;
	by = beginY;
}

bool PauseMenuEvent::update(RenderBase *render)
{
	if(OpenGLGame::Instance->pauseStatus > 4 || OpenGLGame::Instance->pauseStatus <= 0) {
		if(x >= -1.0f) {
			x -= 0.02f  / sqrt(1 + k * k);
			y -= 0.02f * k / sqrt(1 + k * k);
			static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
			return false;
		}else return true;
	}
	if(OpenGLGame::Instance->pauseStatus & (1 << id)) {
		if(time <= offset) {
			++time;
			return false;
		}
		if(x < -0.5f) {
			x += 0.02f / sqrt(1 + k * k);
			y += 0.02f * k / sqrt(1 + k * k);
		}else x = -0.5f, y = by;
		if(lk) {
			if(time <= 130) {
				static_cast<PauseMenuRender*>(render)->setPos(x + X[time % 8] / 300.0f, (y + Y[time % 8] / 300.0f) * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
				++time;
			}
			else static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
		}
		else static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
		return false;
	}else return true;
}

PauseMenuEndEvent::PauseMenuEndEvent(int id, float beginY, bool lock, QObject *parent) : ItemDataEventBase (parent)
{
	this->id = id;
	if(!lock) y = -0.2f;
	else y = beginY;
	if(!lock) x = -1.0f;
	else x = -0.5f;
	if(!lock) time = 0;
	else time = 100;
	lk = lock;
	offset = id * 10;
	k = (beginY + 0.2f) / 0.5f;
	by = beginY;
}

bool PauseMenuEndEvent::update(RenderBase *render)
{
	if(OpenGLGame::Instance->pauseStatus > 4 || OpenGLGame::Instance->pauseStatus <= 0) {
		if(x >= -1.0f) {
			x -= 0.02f  / sqrt(1 + k * k);
			y -= 0.02f * k / sqrt(1 + k * k);
			static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
			return false;
		}else return true;
	}
	if((OpenGLGame::Instance->pauseStatus & (1 << id)) == 0) {
		++time;
		if(time <= offset) return false;
		else --time;
		if(x < -0.5f) {
			x += 0.02f  / sqrt(1 + k * k);
			y += 0.02 * k / sqrt(1 + k * k);
		}else x = -0.5f, y = by;
		static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
		return false;
	}else return true;
}

PauseMenu2Event::PauseMenu2Event(int id, float beginY, bool lock, QObject *parent) : PauseMenuEvent (id, beginY, lock, parent)
{

}

bool PauseMenu2Event::update(RenderBase *render)
{
	if(OpenGLGame::Instance->pauseStatus <= 4) {
		if(x >= -1.0f) {
			x -= 0.02f  / sqrt(1 + k * k);
			y -= 0.02f * k / sqrt(1 + k * k);
			static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
			return false;
		}else return true;
	}
	if(OpenGLGame::Instance->pauseStatus & (1 << id)) {
		++time;
		if(time <= offset) return false;
		else --time;
		if(x < -0.5f) {
			x += 0.02f / sqrt(1 + k * k);
			y += 0.02f * k / sqrt(1 + k * k);
		}else x = -0.5f, y = by;
		if(lk) {
			if(time <= 130) {
				static_cast<PauseMenuRender*>(render)->setPos(x + X[time % 8] / 300.0f, (y + Y[time % 8] / 300.0f) * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
				++time;
			}
			else static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
		}
		else static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
		return false;
	}else return true;
}

PauseMenu2EndEvent::PauseMenu2EndEvent(int id, float beginY, bool lock, QObject *parent) : PauseMenuEndEvent (id, beginY, lock, parent)
{

}

bool PauseMenu2EndEvent::update(RenderBase *render)
{
	if(OpenGLGame::Instance->pauseStatus <= 4) {
		if(x >= -1.0f) {
			x -= 0.02f  / sqrt(1 + k * k);
			y -= 0.02f * k / sqrt(1 + k * k);
			static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
			return false;
		}else return true;
	}
	if((OpenGLGame::Instance->pauseStatus & (1 << id)) == 0) {
		++time;
		if(time <= offset) return false;
		else --time;
		if(x < -0.5f) {
			x += 0.02f  / sqrt(1 + k * k);
			y += 0.02 * k / sqrt(1 + k * k);
		}else x = -0.5f, y = by;
		static_cast<PauseMenuRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.04f, 0.2857f * ItemManager::INSTANCE()->getDiv(), (x + 1.0f) / 0.5f);
		return false;
	}else return true;
}

SPPEvent::SPPEvent(float x, float y, QObject *parent) : ItemDataEventBase (parent)
{
	this->x = x;
	this->y = y;
}

bool SPPEvent::update(RenderBase *render)
{
	if(isIn()) return true;
	float ss = (OpenGLGame::Instance->myPlaneY - y) / (sqrt(dis2()));
	float cc = (OpenGLGame::Instance->myPlaneX - x) / (sqrt(dis2()));
	if(!OpenGLGame::Instance->isPause()) {
		x += 0.01f * cc;
		y += 0.01f * ss;
	}
	static_cast<SPPRender*>(render)->setPos(x, y * ItemManager::INSTANCE()->getDiv(), 0.035f * (dis2() + 0.02) / 0.12f, 0.035f * (dis2() + 0.02) / 0.12f);
	return false;
}

bool SPPEvent::isIn()
{
	return dis2() <= 0.001f;
}

double SPPEvent::dis2()
{
	return (x - OpenGLGame::Instance->myPlaneX) * (x - OpenGLGame::Instance->myPlaneX) + (y - OpenGLGame::Instance->myPlaneY) * (y - OpenGLGame::Instance->myPlaneY);
}

MasterEvent::MasterEvent(QObject *parent) : ItemDataEventBase (parent)
{
	time = -20;
}

bool MasterEvent::update(RenderBase *render)
{
	if(time <= 0) {
		if(!OpenGLGame::Instance->isPause()) ++time;
		if(time < -10) static_cast<MasterRender*>(render)->setPos(OpenGLGame::Instance->myPlaneX, ItemManager::INSTANCE()->getDiv() * (OpenGLGame::Instance->myPlaneY + (time + 20) / 10.0f * 1.4f), (time + 20) / 10.0f * 2.0f, 0.1f, 0.75f);
		else static_cast<MasterRender*>(render)->setPos(OpenGLGame::Instance->myPlaneX, ItemManager::INSTANCE()->getDiv() * (OpenGLGame::Instance->myPlaneY + 1.4f), 2.0f, 1.21f * (time + 10) / 10.0f, 0.75f);
		return false;
	}
	if(time >= 300) {
		if(time == 330) return true;
		static_cast<MasterRender*>(render)->setPos(OpenGLGame::Instance->myPlaneX, ItemManager::INSTANCE()->getDiv() * (OpenGLGame::Instance->myPlaneY + 1.4f), 2.0f, 1.21f, 0.75f * (330 - time) / 30.0f);
	}else{
		static_cast<MasterRender*>(render)->setPos(OpenGLGame::Instance->myPlaneX, ItemManager::INSTANCE()->getDiv() * (OpenGLGame::Instance->myPlaneY + 1.4f), 2.0f, 1.21f, 0.75f);
	}
	if(!OpenGLGame::Instance->isPause()) ++time;
	return false;
}
