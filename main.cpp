#include "mainwidget.h"
#include <QApplication>
#include "windows.h"

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
//	changeWindow();
	MainWidget w;
	w.show();

	return a.exec();
}
