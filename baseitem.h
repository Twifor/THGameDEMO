#ifndef BASEITEM_H
#define BASEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QOpenGLFunctions_3_3_Core>
#include <QDebug>

class BaseItem : public QObject, public QGraphicsItem, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	enum ItemType {
		MY_PLANE
	};
	ItemType type;
	explicit BaseItem(QGraphicsItem *parent = nullptr);
protected:
	void init();

signals:

public slots:

};

#endif // BASEITEM_H
