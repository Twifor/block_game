#include "widget.h"
#include "common.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <ctime>
#include <QSettings>
#include <QFileInfo>

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	getConfig();//读取游戏规则配置文件
	qsrand(time(nullptr));

	setWindowTitle(QString("Block Game[Stage %1]").arg(GameRule::getInstance()->getStartStage()));
	setFixedSize(610, 900);

	gameView = new GameView(this);
	gameView->setGeometry(50, 20, WIDTH, HEIGHT);
	this->setFocusPolicy(Qt::StrongFocus);

	isPressing = 0;
	isPausing = false;
	sPoint = 0;
	stage = GameRule::getInstance()->getStartStage();

	QFont font;
	font.setPointSize(16); // 设置字号

	life = new QLabel(this);
	score = new QLabel(this);
	pauseLabel = new QLabel(this);
	life->setGeometry(160, 800, 200, 50);
	life->setText(QString("Life: %1").arg(GameRule::getInstance()->getMaxLife()));
	life->setFont(font);
	score->setGeometry(360, 800, 200, 50);
	score->setText(QString("Score: 0"));
	score->setFont(font);
	pauseLabel->setGeometry(65, 400, 800, 50);
	pauseLabel->setFont(font);
	pauseLabel->setText("Game has paused, press B to continue..");
	pauseLabel->setVisible(false);

	connect(this, &MainWidget::moveLineLeft, gameView, &GameView::moveLineLeft);
	connect(this, &MainWidget::moveLineRight, gameView, &GameView::moveLineRight);
	connect(gameView, &GameView::fail, this, &MainWidget::gameFail);
	connect(gameView, &GameView::success, this, &MainWidget::gameSuccess, Qt::QueuedConnection);
	connect(gameView, &GameView::error, this, &MainWidget::dealWithError);
	connect(gameView, &GameView::lifeChange, this, &MainWidget::lifeChange);
	connect(gameView, &GameView::addScore, this, &MainWidget::addScore);

	gameView->initStage(stage);//初始化关卡
}

void MainWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, width(), height(), QPixmap(":/image/bg"));
}

bool MainWidget::event(QEvent *event)//处理所有外部事件，负责与用户交互
{
	if (event->type() == QEvent::KeyPress) {
		if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Left) {
			if (isPressing != 1 && !isPausing) {
				if (!isPressing) timerID = startTimer(5);
				isPressing = 1;
				return true;
			}
		}else if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Right) {
			if (isPressing != 2 && !isPausing) {
				if (!isPressing) timerID = startTimer(5);
				isPressing = 2;
				return true;
			}
		}else if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Space) {
			if (!gameView->isGameing() && !isPausing) {
				gameView->startGame(); //开始游戏
				return true;
			}
		}else if(static_cast<QKeyEvent*>(event)->key() == Qt::Key_B) {
			if(isPausing) {
				gameView->pauseGame(false);
				isPausing = false;
				pauseLabel->setVisible(false);
			}else{
				gameView->pauseGame(true);
				isPausing = true;
				pauseLabel->setVisible(true);
			}
			return true;
		}
	}else if (event->type() == QEvent::KeyRelease) {
		if (isPressing != 0 && !static_cast<QKeyEvent*>(event)->isAutoRepeat()) {
			isPressing = 0;
			killTimer(timerID);
			return true;
		}
	}
	return QWidget::event(event);
}

void MainWidget::timerEvent(QTimerEvent *)
{
	if (isPressing == 1) emit moveLineLeft(); //发射移动底板信号
	else emit moveLineRight();
}

void MainWidget::getConfig()
{
	QSettings set("config.ini", QSettings::IniFormat);
	QFileInfo file("config.ini");
	if(!file.exists()) {
		set.setValue("config/maxLife", 3);
		set.setValue("config/breakPoint", 1);
		set.setValue("config/startStage", 1);
	}else{
		GameRule::getInstance()->setMaxLife(set.value("config/maxLife").toInt());
		GameRule::getInstance()->setBreakRule(set.value("config/breakPoint").toInt());
		GameRule::getInstance()->setStartStage(set.value("config/startStage").toInt());
	}
}

void MainWidget::gameFail()
{
	QMessageBox::information(this, "Fail", "You Lose.");
	gameView->endGame();
	close();
}

void MainWidget::gameSuccess()
{
	QMessageBox::information(this, "Success", "You have finished this stage!");
	gameView->initStage(++stage);
	life->setText(QString("Life: %1").arg(GameRule::getInstance()->getMaxLife()));
	setWindowTitle(QString("Block Game[Stage %1]").arg(stage));
}

void MainWidget::dealWithError()
{
	QMessageBox::critical(this, "Error", QString("Lack data file for stage %1.").arg(stage));
	gameView->endGame();
	close();
}

void MainWidget::lifeChange(int x)
{
	life->setText(QString("Life: %1").arg(x));
}

void MainWidget::addScore(int x)
{
	sPoint += x;
	score->setText(QString("Score: %1").arg(sPoint));
}

