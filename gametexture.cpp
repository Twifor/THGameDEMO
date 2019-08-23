#include "gametexture.h"
#include "gameresource.h"

GameTexture::GameTexture(QObject *parent) : QObject(parent)
{
}

GameTexture::~GameTexture()
{
	for(int i = 0; i < TEXTURE_NUMBER; i++) delete op[i];
}

void GameTexture::init()
{
	QImage image;
	for(int i = 0; i < 34; i++) {
		static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(static_cast<ResourceID>(i + MARISA1_PNG)))->loadData(image);
		op[i] = new QOpenGLTexture(image.mirrored(false, true));
	}
	for(int i = 34; i < TEXTURE_NUMBER; i++) {
		static_cast<GameResourcePNGData*>(GameResource::getInstance()->getData(static_cast<ResourceID>(i - 33 + +SCORE_PNG)))->loadData(image);
		op[i] = new QOpenGLTexture(image.mirrored(false, true));
	}
}

QOpenGLTexture *GameTexture::getTexure(GameTexture::TextureType type)
{
	return op[type];
}
