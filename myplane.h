#ifndef MYPLANE_H
#define MYPLANE_H

#include <QObject>

class MyPlane : public QObject
{
	Q_OBJECT
public:
	explicit MyPlane(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MYPLANE_H