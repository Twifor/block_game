#ifndef WIDGET_H
#define WIDGET_H

#include <QLabel>
#include <QWidget>
#include "gameview.h"

class MainWidget : public QWidget//主窗体
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);

	// QWidget interface
protected:
	void paintEvent(QPaintEvent *event);
	bool event(QEvent *event);
	void timerEvent(QTimerEvent *event);


private:
	GameView *gameView;
	QLabel *life;
	QLabel *score;
	QLabel *pauseLabel;
	int isPressing;//处理键盘长按
	int timerID;
	int sPoint;
	int stage;
	void getConfig();
	bool isPausing;//是否暂停

signals:
	void moveLineLeft();
	void moveLineRight();

public slots:
	void gameFail();
	void gameSuccess();
	void dealWithError();
	void lifeChange(int);
	void addScore(int);
};

#endif // WIDGET_H
