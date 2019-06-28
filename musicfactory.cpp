#include "musicfactory.h"
#include <QUrl>

MusicFactory::~MusicFactory()
{
}

MusicFactory *MusicFactory::getInstance()
{
	if(INSTANCE == nullptr) return INSTANCE = new MusicFactory;
	return INSTANCE;
}

void MusicFactory::play(const char *data)
{
	newPlayer(0);
	newPlayer(1);
	who = false;
	player[0]->setMedia(QUrl::fromLocalFile(data));
	player[1]->setMedia(QUrl::fromLocalFile(data));


	player[0]->play();
}

void MusicFactory::setBack(qint64 p)
{
	b = p;
}

void MusicFactory::destroy()
{
	if(INSTANCE != nullptr) delete INSTANCE;
}

void MusicFactory::seekTO(qint64 pos)
{
	player[who]->setPosition(pos);
}

qint64 MusicFactory::getPostion()
{
	return player[who]->position();
}

MusicFactory *MusicFactory::INSTANCE = nullptr;

MusicFactory::MusicFactory(QObject *parent) : QObject(parent)
{
}

void MusicFactory::newPlayer(int pos)
{
	player[pos] = new QMediaPlayer;
	connect(player[pos], &QMediaPlayer::positionChanged, [ = ](qint64 s){
		if(s == b) {
				player[who = who ^ 1]->play();
				qDebug() << "repeat BGM";
		}
	});
	player[pos]->setNotifyInterval(1);
}

