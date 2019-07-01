#include "musicfactory.h"
#include <QUrl>
#include <QTimer>
#define ABS(x) ((x) > 0 ? (x) : (-(x)))

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
	player[0]->setMedia(QUrl::fromLocalFile(data));//双播放器交替实现无缝循环衔接
	player[1]->setMedia(QUrl::fromLocalFile(data));

	player[0]->play();
	timeID = startTimer(1, Qt::PreciseTimer);
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
	now = s;
	if(s == 0) {
		setBack(101200);
		play("res/bgm1.wav");
	}else if(s == 1) {
		setBack(385510);
		play("res/bgm2.wav");
	}else if(s == 3) {
		setBack(222465);
		play("res/bgm4.wav");
	}else if(s == 2) {
		setBack(417200);
		play("res/bgm3.wav");
	}
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
}

MusicFactory *MusicFactory::INSTANCE = nullptr;

MusicFactory::MusicFactory(QObject *parent) : QObject(parent)
{
	now = -1;
}

void MusicFactory::newPlayer(int pos)
{
	player[pos] = new QMediaPlayer;
}

void MusicFactory::timerEvent(QTimerEvent *)
{
//	qDebug() << player[0]->duration();
	qint64 ss = player[who]->position();
//	qDebug() << player[0]->duration();
	if(ABS(ss - b) <= 10) {//这个10是用来屏蔽加载时间误差(?)，猜的，反正加上就对了（试验无数次的结论）
		player[who = who ^ 1]->play();
		QTimer::singleShot(2000, [ & ](){
			player[who ^ 1]->setMedia(player[who]->media());
		});
		qDebug() << "repeat BGM";
	}
}

