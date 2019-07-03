#ifndef GAMERULE_H
#define GAMERULE_H

#include <QObject>

class GameRule : public QObject//单例模式，负责储存和指派游戏规则
{
	Q_OBJECT
public:
	static int bgmVolume;//bgm音量
	static bool isCheat;//是否作弊
	static bool isFullScreen;//是否全屏
	static void update();
private:

	explicit GameRule(QObject *parent = nullptr);


signals:

public slots:
};

#endif // GAMERULE_H
