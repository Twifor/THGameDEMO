#ifndef GAMETOOLS_H
#define GAMETOOLS_H

#include <QObject>

class GameTools : public QObject
{
	Q_OBJECT
public:
	explicit GameTools(QObject *parent = nullptr);
	static float toGLX(float x);
	static float toGLY(float y);
	static int toSTDX(float x);
	static int toSTDY(float y);

signals:

public slots:
};

#endif // GAMETOOLS_H
