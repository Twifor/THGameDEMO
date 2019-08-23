#ifndef BULLETMAKERBASE_H
#define BULLETMAKERBASE_H

#include <QObject>
#include "baseitem.h"
#include "basebullet.h"

class BulletMakerBase : public BaseItem//子弹发射器基类
{
	Q_OBJECT
public:
	explicit BulletMakerBase(QPointF p, QGraphicsItem *parent = nullptr);

signals:
	void makeBullet(BaseBullet *bullet);

public slots:
};

#endif // BULLETMAKERBASE_H
