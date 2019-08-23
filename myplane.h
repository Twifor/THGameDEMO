#ifndef MYPLANE_H
#define MYPLANE_H

#include <QObject>
#include <QGraphicsItem>
#include "bulletmakerbase.h"

class MyPlane : public BulletMakerBase//自机
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
	void advance(int phase) override;

signals:
	void collect(MyPlane* p);


public slots:
	void setBall(int num);
	// QGraphicsItem interface
public:
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
//	long long lastTime;
	void drawLine(QPointF begin);
	bool isShifting, isZ;
	int status, numOfBalls;
	int timeLine, timeLine2, timeLine3, timeLine4, timeLine5;//时间线，用于控制动画
	int timeLine6;
	float ballLine;
	const int offsetX[6][6] = {
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, -20, 20, 0, 0, 0},
		{0, -20, 0, 20, 0, 0},
		{0, -25, -11, 11, 25, 0},
		{0, -30, -15, 0, 15, 30}
	};
	const int offsetY[6][6] = {
		{0, 0, 0, 0, 0, 0},
		{0, -45, 0, 0, 0, 0},
		{0, -45, -45, 0, 0, 0},
		{0, -40, -45, -40, 0, 0},
		{0, -35, -48, -48, -35, 0},
		{0, -30, -42, -48, -42, -30}
	};
	const int objX[6][6] = {
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, -10, 10, 0, 0, 0},
		{0, -10, 0, 10, 0, 0},
		{0, -15, -12, 12, 15, 0},
		{0, -14, -8, 0, 8, 14}

	};
	const int objY[6][6] = {
		{0, 0, 0, 0, 0, 0},
		{0, -30, 0, 0, 0, 0},
		{0, -30, -30, 0, 0, 0},
		{0, -25, -30, -25, 0, 0},
		{0, -25, -35, -35, -25, 0},
		{0, -20, -30, -35, -30, -20}
	};
	const int moveX[6][6] = {
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 1, 1, 0, 0, 0},
		{0, 1, 0, 1, 0, 0},
		{0, 2, 1, 1, 2, 0},
		{0, 3, 1, 0, 1, 3}
	};
	const int moveY[6][6] = {
		{0, 0, 0, 0, 0, 0},
		{0, 2, 0, 0, 0, 0},
		{0, 2, 2, 0, 0, 0},
		{0, 2, 2, 2, 0, 0},
		{0, 2, 2, 2, 2, 0},
		{0, 2, 2, 2, 2, 2}
	};
	QPointF ballPos[6];
};

#endif // MYPLANE_H
