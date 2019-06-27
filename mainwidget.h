#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
#include "loadingwidget.h"


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

private:
	LoadingOpenGLWidget *loadingWidget;
	LoadingThread *loadingThread;
};

#endif // MAINWIDGET_H
