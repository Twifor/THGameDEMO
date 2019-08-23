#include "mainwidget.h"
#include <QApplication>
#include "windows.h"
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include "gamerule.h"
#include <QtOpenGL/QGLWidget>

void changeWindow(){
	DEVMODE DevMode;
	EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &DevMode);
	DevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;
	DevMode.dmPelsWidth = 800;
	DevMode.dmPelsHeight = 600;
	ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN);
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if((QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_3_3) == 0) {
		QMessageBox::critical(nullptr, "Missing OpenGL", "OpenGL3.3 or higher version is expected.\n"
							  "Please update the graphics card driver.");
		return 0;
	}
	QFile res("res.dat"), index("index.dat"), config("config.ini");
	if(!res.exists()) {
		QMessageBox::critical(nullptr, "Cannot find file", "Lack resource file res.dat");
		return 0;
	}
	if(!index.exists()) {
		QMessageBox::critical(nullptr, "Cannot find file", "Lack index file index.dat");
		return 0;
	}
	if(!config.exists()) {
		QMessageBox::critical(nullptr, "Cannot find file", "Lack config file config.ini");
		return 0;
	}

	QSettings set("config.ini", QSettings::IniFormat);
	set.beginGroup("Config");
	GameRule::defaultBgmVolume = GameRule::bgmVolume =  set.value("volume").toInt();
	GameRule::isCheat = set.value("isCheat").toBool();
	GameRule::isFullScreen = set.value("isFullScreen").toBool();

	if(GameRule::isFullScreen) changeWindow();
	MainWidget w;
	if(GameRule::isFullScreen) w.showFullScreen();
	else w.show();

	return a.exec();
}
