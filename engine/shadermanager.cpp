#include "shadermanager.h"

#include <QFile>

ShaderManager::ShaderManager(QObject *parent) : QObject(parent)
{

}

void ShaderManager::setProgram(int id, const char *vsCode, const char *fsCode)
{
	QOpenGLShader *vs = new QOpenGLShader(QOpenGLShader::Vertex);
	vs->compileSourceFile(vsCode);

	QOpenGLShader *fs = new QOpenGLShader(QOpenGLShader::Fragment);
	fs->compileSourceFile(fsCode);

	QOpenGLShaderProgram *program = new QOpenGLShaderProgram;
	program->create();
	program->addShader(vs);
	program->addShader(fs);
	program->link();
	delete vs;
	delete fs;
	op[id] = program;
}

ShaderManager::~ShaderManager()
{
	for(int i = 0; i < SHADER_NUMBER; i++) delete op[i];
}

QOpenGLShaderProgram *ShaderManager::getProgram(int id)
{
	return op[id];
}

ShaderManager *ShaderManager::INSTANCE()
{
	if(in == nullptr) return in = new ShaderManager;
	return in;
}

void ShaderManager::destory()
{
	if(in != nullptr) delete in;
	in = nullptr;
}

ShaderManager* ShaderManager::in = nullptr;
