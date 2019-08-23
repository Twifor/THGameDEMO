#ifndef BASEITEM_H
#define BASEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QOpenGLFunctions_3_3_Core>
#include <QDebug>

class BaseItem : public QObject, public QGraphicsItem, protected QOpenGLFunctions_3_3_Core//物品基类
{
	Q_OBJECT
public:
	enum ItemType {//物品种类
		MY_PLANE,
		MY_PLANE_BULLET,
		POWER,
		POINT,
		UP,
		SPELL,
		SPELL_EXTEND,
		EXTEND
	};
	ItemType type;
	explicit BaseItem(QPointF p, QGraphicsItem *parent = nullptr);
	QPointF getPos();
	void destroyItem();

protected:
	void init();
	QPointF pos;
	bool flag;
	bool lock;

signals:
	void destory(BaseItem *p);

public slots:

};

class FunctionalItem : public BaseItem {//功能性道具
	Q_OBJECT

public:
	explicit FunctionalItem(QPointF p, QGraphicsItem *parent = nullptr);

public:
	void makeTo(BaseItem *h);
	bool isTo;
	BaseItem *hh;
	void advance(int phase) override;

protected:
	float k;
};

class PowerItem : public FunctionalItem {//power
	Q_OBJECT
public:
	explicit PowerItem(QPointF p, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class PointItem : public FunctionalItem {
	Q_OBJECT
public:
	explicit PointItem(QPointF p, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class UpItem : public FunctionalItem {
	Q_OBJECT
public:
	explicit UpItem(QPointF p, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

class SpellItem : public FunctionalItem {
	Q_OBJECT
public:
	explicit SpellItem(QPointF p, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

};

class SpellExtendItem : public BaseItem {
	Q_OBJECT

public:
	explicit SpellExtendItem(QPointF p = QPointF(255, 150), QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void advance(int phase) override;
private:
	int timeLine;

};

class ExtendItem : public BaseItem {
	Q_OBJECT

public:
	explicit ExtendItem(QPointF p = QPointF(255, 150), QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void advance(int phase) override;
private:
	int timeLine;

};

#endif // BASEITEM_H
