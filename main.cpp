#include "mainwidget.h"
#include <QApplication>
#include "windows.h"
#include <QFile>
#include <QMessageBox>

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
	QFile res("res.dat"), index("index.dat");
	if(!res.exists()) {
		QMessageBox::critical(nullptr, "找不到文件", "缺少游戏资源文件res.dat");
		return 0;
	}
	if(!index.exists()){
		QMessageBox::critical(nullptr, "找不到文件", "缺少游戏索引文件index.dat");
		return 0;
	}

//	changeWindow();
	MainWidget w;
	w.show();

	return a.exec();
}
