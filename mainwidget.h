#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include "loadingwidget.h"
#include "menuwidget.h"
#include "musicfactory.h"

class MainWidget : public QWidget
{
	Q_OBJECT
public slots:
	void stopLoading();

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget()override;

	// QWidget interface
protected:
	void paintEvent(QPaintEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;
	void closeEvent(QCloseEvent *event) override;

private:
	LoadingOpenGLWidget *loadingWidget;
	LoadingThread *loadingThread;
	MenuWidget *menuWidget;
};

#endif // MAINWIDGET_H
