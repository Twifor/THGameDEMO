#include "gamerule.h"
#include "musicfactory.h"

void GameRule::update()
{
	MusicFactory::getInstance()->setVolume(bgmVolume);
}

GameRule::GameRule(QObject *parent) : QObject(parent)
{

}
int GameRule::bgmVolume = 30;
bool GameRule::isCheat = false;
bool GameRule::isFullScreen = true;
int GameRule::defaultBgmVolume = 30;
