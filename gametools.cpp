#include "gametools.h"

GameTools::GameTools(QObject *parent) : QObject(parent)
{

}

float GameTools::toGLX(float x)
{
	return (x - 243.0f) / 243.0f;
}

float GameTools::toGLY(float y)
{
	return (278.0f - y) / 278.0f;
}

int GameTools::toSTDX(float x)
{

}

int GameTools::toSTDY(float y)
{

}
