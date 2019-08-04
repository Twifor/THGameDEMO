#include "mainwidget.h"
#include <QApplication>
#include "windows.h"
#include <QFile>
#include <QMessageBox>
#include <QSettings>
#include "gamerule.h"

void changeWindow(){//改一下分辨率，我也不知道有啥用
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
	QFile res("res.dat"), index("index.dat"), config("config.ini");
	if(!res.exists()) {
		QMessageBox::critical(nullptr, "找不到文件", "缺少游戏资源文件res.dat");
		return 0;
	}
	if(!index.exists()) {
		QMessageBox::critical(nullptr, "找不到文件", "缺少游戏索引文件index.dat");
		return 0;
	}
	if(!config.exists()) {
		QMessageBox::critical(nullptr, "找不到文件", "缺少配置文件config.ini");
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
