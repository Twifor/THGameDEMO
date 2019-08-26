#include "texturemanager.h"

TextureManager::~TextureManager()
{
	for(int i = 0; i < TEXTURE_NUMBER; i++) delete op[i];
}

TextureManager::TextureManager(QObject *parent) : QObject(parent)
{
}

QOpenGLTexture *TextureManager::getTexture(TextureManager::TextureType type)
{
	return op[type];
}

void TextureManager::setTexture(TextureManager::TextureType type, QOpenGLTexture *texture)
{
	op[type] = texture;
}

TextureManager *TextureManager::INSTANCE()
{
	if(in == nullptr) return in = new TextureManager;
	return in;
}

void TextureManager::destroy()
{
	if(in != nullptr) delete in;
}

TextureManager *TextureManager::in = nullptr;
