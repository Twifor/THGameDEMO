#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <QObject>
#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#define SHADER_NUMBER 5

class ShaderManager : public QObject
{
	Q_OBJECT
public:
	void setProgram(int id, const char *vsCode, const char *fsCode);
	~ShaderManager();
	QOpenGLShaderProgram *getProgram(int id);
	static ShaderManager *INSTANCE();
	void destory();

signals:

public slots:
private:
	static ShaderManager* in;
	explicit ShaderManager(QObject *parent = nullptr);
	QOpenGLShaderProgram *op[SHADER_NUMBER];
};

#endif // SHADERMANAGER_H
