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

#endif // ITEMDATAEVENT_H
