#ifndef BASEBULLET_H
#define BASEBULLET_H

#include <QObject>
#include "baseitem.h"

class BaseBullet : public BaseItem//子弹基类
{
	Q_OBJECT
public:
	explicit BaseBullet(QPointF p, QGraphicsItem *parent = nullptr);

public slots:
};

class MyBullet : public BaseBullet//自机子弹
{
	Q_OBJECT
public:
	explicit MyBullet(QPointF p, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void advance(int phase) override;
};

#endif // BASEBULLET_H
