#ifndef MUSICROOM_H
#define MUSICROOM_H

#include <QObject>
#include <QWidget>

class MusicRoom : public QObject
{
	Q_OBJECT
public:
	explicit MusicRoom(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MUSICROOM_H