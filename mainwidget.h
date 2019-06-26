#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPropertyAnimation>
#include <QLabel>
class LoadingWidget : public QWidget {
	Q_OBJECT;
public:
	LoadingWidget(QWidget *parent = nullptr);
protected:
	void paintEvent(QPaintEvent *event) override;
};

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
	QTimer *timer;
	LoadingWidget *loadingWidget;
};

#endif // MAINWIDGET_H
