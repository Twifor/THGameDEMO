#include "musicfactory.h"
#include <QUrl>
#include <QTimer>
#include "gameresource.h"
#include "gamerule.h"

#define ABS(x) ((x) > 0 ? (x) : (-(x)))

MusicFactory::~MusicFactory()
{
}

MusicFactory *MusicFactory::getInstance()
{
	if(INSTANCE == nullptr) return INSTANCE = new MusicFactory;
	return INSTANCE;
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

void MusicFactory::stop()
{
	player[who]->stop();
	delete player[0];
	delete player[1];
	killTimer(timeID);
}

void MusicFactory::play(int s)
{
	newPlayer(0);
	newPlayer(1);
	who = false;
	if(buffer != nullptr) delete buffer;
	buffer = new QBuffer;
	now = s;

	if(s == 0) {
		setBack(101075);
		GameResource::getInstance()->load(BGM1_WAV);
		static_cast<GameResourceWAVData*>(GameResource::getInstance()->getData(BGM1_WAV))->loadData(buffer);
		player[0]->setMedia(QMediaContent(), buffer);
	}else if(s == 1) {
		setBack(385510);
		GameResource::getInstance()->load(BGM2_WAV);
		static_cast<GameResourceWAVData*>(GameResource::getInstance()->getData(BGM2_WAV))->loadData(buffer);
		player[0]->setMedia(QMediaContent(), buffer);
	}else if(s == 3) {
		setBack(222465);
		GameResource::getInstance()->load(BGM4_WAV);
		static_cast<GameResourceWAVData*>(GameResource::getInstance()->getData(BGM4_WAV))->loadData(buffer);
		player[0]->setMedia(QMediaContent(), buffer);
	}else if(s == 2) {
		setBack(417200);
		GameResource::getInstance()->load(BGM3_WAV);
		static_cast<GameResourceWAVData*>(GameResource::getInstance()->getData(BGM3_WAV))->loadData(buffer);
		player[0]->setMedia(QMediaContent(), buffer);
	}
	player[0]->setVolume(GameRule::bgmVolume);
	player[0]->play();
	timeID = startTimer(1, Qt::PreciseTimer);
}

int MusicFactory::getNow()
{
	return now;
}

void MusicFactory::quit()
{
	if(player[0]->state() == QMediaPlayer::PlayingState) player[0]->stop();
	if(player[1]->state() == QMediaPlayer::PlayingState) player[1]->stop();
	delete player[0];
	delete player[1];
	player[0] = nullptr;
	player[1] = nullptr;
}

void MusicFactory::setVolume(int volume)
{
	if(player[0] != nullptr) player[0]->setVolume(volume);
	if(player[1] != nullptr) player[0]->setVolume(volume);
}

MusicFactory *MusicFactory::INSTANCE = nullptr;

MusicFactory::MusicFactory(QObject *parent) : QObject(parent)
{
	now = -1;
	buffer = nullptr;
	player[0] = nullptr;
	player[1] = nullptr;
}

void MusicFactory::newPlayer(int pos)
{
	player[pos] = new QMediaPlayer(nullptr, QMediaPlayer::StreamPlayback);
}

void MusicFactory::timerEvent(QTimerEvent *)
{
//	qDebug() << player[0]->duration();
	qint64 ss = player[who]->position();
//	qDebug() << player[0]->duration();
	if(ABS(ss - b) <= 10) {//这个10是用来屏蔽加载时间误差(?)，猜的，反正加上就对了（试验无数次的结论）
		QTimer::singleShot(0, [ & ](){
			player[who ^ 1]->setMedia(QMediaContent(), buffer);
			player[who ^ 1]->setVolume(GameRule::bgmVolume);
			player[who = who ^ 1]->play();
		});
		qDebug() << "repeat BGM";
	}
}

