#ifndef ITEMDATAEVENT_H
#define ITEMDATAEVENT_H

#include <QObject>
#include "renderbase.h"

class ItemDataEventBase : public QObject
{
	Q_OBJECT
public:
	explicit ItemDataEventBase(QObject *parent = nullptr);
	virtual bool update(RenderBase *render) = 0;

signals:

public slots:
};

class NullEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit NullEvent(QObject *parent = nullptr);
	bool update(RenderBase *render) override;
};

class BackGroundEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit BackGroundEvent(float begin, QObject *parent = nullptr);
	bool update(RenderBase *render) override;

private:
	float s;
};

class MarisaEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit MarisaEvent(QObject *parent = nullptr);
	bool update(RenderBase *render)override;
};

class TreeEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit TreeEvent(float begin, QObject *parent = nullptr);
	bool update(RenderBase *render)override;

protected:
	float s;
};

class SlowEffectEvent1 : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit SlowEffectEvent1 (QObject *parent = nullptr);
	bool update(RenderBase *render)override;
};

class SlowEffectEvent2 : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit SlowEffectEvent2 (QObject *parent = nullptr);
	bool update(RenderBase *render)override;
};

class CenterEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit CenterEvent(QObject *parent = nullptr);
	bool update(RenderBase *render)override;
};

class BallEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit BallEvent(int id, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
private:
	int id;
};

class LineEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit LineEvent(float x, float y, float limit, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
private:
	float x, y, limit;
};

class EmiterEvent : public ItemDataEventBase {//发射器事件[抽象类]
	Q_OBJECT
public:
	explicit EmiterEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render) = 0;
private:
	float x, y;
};

class MyBulletEvent : public ItemDataEventBase {//自机子弹事件
	Q_OBJECT
public:
	explicit MyBulletEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
private:
	float x, y;
};

class ItemEvent : public ItemDataEventBase {//道具事件类[抽象类]
	Q_OBJECT
public:
	explicit ItemEvent(float x, float y, QObject *parent = nullptr);
	bool isColl();//与自机碰撞事件
	bool isIn();//自机吸收事件
	bool update(RenderBase *render) = 0;
	float x, y;
	bool isActive;
	float dis2();
};

class PowerEvent : public ItemEvent {
	Q_OBJECT
public:
	explicit PowerEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render) override;
};

class PointEvent : public ItemEvent {
	Q_OBJECT
public:
	explicit PointEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render) override;
};
class ExtendEvent : public ItemEvent {
	Q_OBJECT
public:
	explicit ExtendEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render) override;
};

class SPExtendEvent : public ItemEvent {
	Q_OBJECT
public:
	explicit SPExtendEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
};

class ExtendTipEvent : public ItemDataEventBase {
public:
	explicit ExtendTipEvent(QObject *parent = nullptr);
	bool update(RenderBase *render)override;
private:
	int time;
};

class SPExtendTipEvent : public ItemDataEventBase {
public:
	explicit SPExtendTipEvent(QObject *parent = nullptr);
	bool update(RenderBase *render)override;
private:
	int time;
};

class ItemGetLineEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit ItemGetLineEvent(QObject *parent = nullptr);
	bool update(RenderBase *render)override;
private:
	int time;
};

class WhiteNumberEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit WhiteNumberEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
private:
	int time;
	float x, y;
};

const float X[] = {1, -1, 1, -1, 1, -1, 1, -1};//颤抖动画
const float Y[] = {1, 1, -1, -1, 1, 1, -1, -1};

class PauseMenuEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit PauseMenuEvent(int id, float beginY, bool lock, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
protected:
	int time, id, offset;
	float x, y, k, by;
	bool lk;
};

class PauseMenuEndEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit PauseMenuEndEvent(int id, float beginY, bool lock, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
protected:
	int time, id, offset;
	float x, y, k, by;
	bool lk;
};

class PauseMenu2Event : public PauseMenuEvent {
	Q_OBJECT
public:
	explicit PauseMenu2Event(int id, float beginY, bool lock, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
};

class PauseMenu2EndEvent : public PauseMenuEndEvent {
	Q_OBJECT
public:
	explicit PauseMenu2EndEvent(int id, float beginY, bool lock, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
};
class SPPEvent : public ItemDataEventBase {
	Q_OBJECT
public:
	explicit SPPEvent(float x, float y, QObject *parent = nullptr);
	bool update(RenderBase *render)override;
protected:
	bool isIn();
	double dis2();

private:
	float x, y;
};

class MasterEvent : public ItemDataEventBase {
public:
	explicit MasterEvent(QObject *parent = nullptr);
	bool update(RenderBase *render)override;

private:
	int time;
};

#endif
