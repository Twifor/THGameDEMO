#ifndef GAMETEXTURE_H
#define GAMETEXTURE_H

#include <QObject>
#include <QOpenGLTexture>
const int TEXTURE_NUMBER = 36;
class GameTexture : public QObject//管理主游戏各种纹理
{
	Q_OBJECT

public:
	enum TextureType {
		MARISA1,
		MARISA2,
		MARISA3,
		MARISA4,
		MARISA5,
		MARISA6,
		MARISA7,
		MARISA8,
		MARISA9,
		MARISA10,
		MARISA11,
		MARISA12,
		MARISA13,
		MARISA14,
		MARISA15,
		MARISA16,
		MARISA17,
		MARISA18,
		MARISA19,
		MARISA20,
		MARISA21,
		MARISA22,
		MARISA23,
		MARISA24,
		SLOWEFFECT1,
		SLOWEFFECT2,
		CENTER,
		MARISA_BULLET,
		MARISA_BALL,
		MARISA_LINE,
		POWER,
		POINT,
		UP,
		SPELL,
		SPELL_EXTEND,
		EXTEND
	};

	explicit GameTexture(QObject *parent = nullptr);
	~GameTexture();
	void init();
	QOpenGLTexture *getTexure(TextureType type);

signals:

public slots:
private:
	QOpenGLTexture *op[TEXTURE_NUMBER];
};

#endif // GAMETEXTURE_H
