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

#endif // ITEMDATAEVENT_H
