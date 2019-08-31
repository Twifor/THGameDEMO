#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QObject>
#include <QOpenGLTexture>
#include <QVector>
#define TEXTURE_NUMBER 35

class TextureManager : public QObject
{
	Q_OBJECT
public:
	~TextureManager();
	enum TextureType {
		BACKGROUND,
		STAR_BACKGROUND,
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
		SLOWEFFECT1_PNG,
		SLOWEFFECT2_PNG,
		TREE,
		CENTER,
		BALL,
		MARISA_LINE,
		MY_BULLET,
		POWER,
		POINT
	};
	QOpenGLTexture *getTexture(TextureType type);
	void setTexture(TextureType type, QOpenGLTexture *texture);
	static TextureManager *INSTANCE();
	void destroy();

signals:

public slots:
private:
	static TextureManager *in;
	explicit TextureManager(QObject *parent = nullptr);
	QOpenGLTexture *op[TEXTURE_NUMBER];
};

#endif // TEXTUREMANAGER_H
