#ifndef MYPLANE_H
#define MYPLANE_H

#include <QObject>
#include <QGraphicsItem>
#include "baseitem.h"

class MyPlane : public BaseItem//控制自机
{
	Q_OBJECT
public:
	explicit MyPlane(QGraphicsItem *parent = nullptr);
	void startLeft();
	void endLeft();
	void startRight();
	void endRight();
	void startUp();
	void endUp();
	void startDown();
	void endDown();
	void startShift();
	void endShift();
	void startZ();
	void endZ();

signals:

public slots:

	// QGraphicsItem interface
public:
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	QPoint pos;
	bool isShifting, isZ;
	int status;
	int timeLine, timeLine2, timeLine3, timeLine4, timeLine5;//时间线，用于控制动画
};

#endif // MYPLANE_H
