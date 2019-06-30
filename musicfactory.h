#ifndef MUSICFACTORY_H
#define MUSICFACTORY_H

#include <QObject>
#include <QMediaPlayer>
#include <QTimer>
#include <QMediaPlaylist>

class MusicFactory : public QObject//单例模式，调控游戏全局的BGM
{
	Q_OBJECT
public:
	~MusicFactory();
	static MusicFactory *getInstance();
	void play(const char* data);
	void setBack(qint64 p);
	void destroy();
	void seekTO(qint64 pos);
	qint64 getPostion();
	void stop();
	void play(int s);
	int getNow();
	void quit();

signals:

public slots:

private:
	qint64 b;
	bool who;
	static MusicFactory *INSTANCE;
	QMediaPlayer *player[2];
	explicit MusicFactory(QObject *parent = nullptr);
	void newPlayer(int pos);
	QMediaPlaylist s;
	int timeID;
	int now;

	// QObject interface
protected:
	void timerEvent(QTimerEvent *event) override;
};

#endif // MUSICFACTORY_H
