#include "gamerule.h"

GameRule::GameRule(QObject *parent) : QObject(parent)
{

}
int GameRule::bgmVolume = 10;
int GameRule::seVolume = 10;
bool GameRule::isCheat = false;
bool GameRule::isFullScreen = true;
