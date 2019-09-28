#include "gameview.h"
#include "common.h"
#include <QGraphicsView>
#include "blockitem.h"
#include <QDebug>
#include <QSound>
#include <QMessageBox>
#include <QFile>
#include <QTime>

GameView::GameView(QWidget *parent) : QGraphicsView(parent)
{
	scene = new QGraphicsScene(this);
	scene->setSceneRect(0, 0, WIDTH, HEIGHT);
	QPixmap pixmap(width(), height());
	pixmap.load(":/image/gamebg");
	scene->setBackgroundBrush(pixmap);
	setCacheMode(QGraphicsView::CacheBackground);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setFocusPolicy(Qt::NoFocus);//禁止获取焦点，将事件发送至MainWidget处理
	setScene(scene);
	isGameingBool = false;

	connect(&timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
}

GameView::~GameView()
{
	delete scene;
}

bool GameView::isGameing()
{
	return isGameingBool;
}

void GameView::moveLineLeft()
{
	line->moveL();
	if (ball->isLocked()) ball->moveLeftWithLine();
}

void GameView::moveLineRight()
{
	line->moveR();
	if (ball->isLocked()) ball->moveRightWithLine();
}

void GameView::dealBallFail()
{
	emit lifeChange(--life);
	if (life == 0) emit fail(); //生命值用尽，发射失败信号
	else {
		scene->removeItem(ball);
		scene->removeItem(line);
		line = new LineItem(QPoint(WIDTH / 2, HEIGHT - 100));
		ball = new BallItem(QPoint(WIDTH / 2, HEIGHT - 100 - LINE_HEIGHT / 2 - RADIUS + 2));
		scene->addItem(line);
		scene->addItem(ball);
		endGame();
	}
}

void GameView::deleteBlock(BlockItem *item, BlockItem::BlockType type)
{
	if (type == BlockItem::KEY_BLOCK){
		emit addScore(life * 15);
		emit success();
		endGame();
	}
	switch (type){
		case BlockItem::NORMAL_BLOCK: emit addScore(1); break;
		case BlockItem::GREEN_BLOCK: emit addScore(3); break;
		case BlockItem::IRON_BLOCK: break;
		case BlockItem::RED_BLOCK: emit addScore(3); break;
		case BlockItem::KEY_BLOCK: emit addScore(30); break;
	}
	scene->removeItem(item);
}

void GameView::initBlocks(int s)
{
	char data[50];
	list.clear();
	QFile file(":/data/stage" + QString::number(s));
	if (!file.open(QIODevice::ReadOnly)){
		emit error();
		return;
	}
	while (!file.atEnd()){
		file.readLine(data, sizeof(data));
		QString str(data);
		int l, r, c;
		l = str.mid(4, 2).toInt();
		r = str.mid(7, 2).toInt();
		if (str[0] == 'B'){
			list.append(new BlockItem(QPoint(BLOCK_LENGTH * (l - 0.5), BLOCK_WIDTH * (r - 0.5)),
									  static_cast<BlockItem::BlockType>(str.mid(2, 1).toInt())));
		}else if (str[0] == 'L'){
			c = str.mid(10, 2).toInt();
			for (int i = l; i <= r; i++){
				list.append(new BlockItem(QPoint(BLOCK_LENGTH * (i - 0.5), BLOCK_WIDTH * (c - 0.5)),
										  static_cast<BlockItem::BlockType>(str.mid(2, 1).toInt())));
			}
		}else if (str[0] == 'C'){
			c = str.mid(10, 2).toInt();
			for (int i = l; i <= r; i++){
				list.append(new BlockItem(QPoint(BLOCK_LENGTH * (c - 0.5), BLOCK_WIDTH * (i - 0.5)),
										  static_cast<BlockItem::BlockType>(str.mid(2, 1).toInt())));
			}
		}
	}
	scene->clear();//清空所有图元
	for (BlockItem* p: list){
		scene->addItem(p);
		connect(p, &BlockItem::hasDestoryed, this, &GameView::deleteBlock);
	}
}

void GameView::startGame()
{
	isGameingBool = true;//标记游戏已经启动
	timer.start(4);//界面刷新
	ball->launch();//发射小球
}

void GameView::endGame()
{
	if (timer.isActive()) timer.stop();
	isGameingBool = false;
	ball->lockBall();//锁住小球
}

void GameView::pauseGame(bool k)
{
	if (!isGameingBool) return;
	if (k && timer.isActive()) timer.stop();
	else if (!timer.isActive()) timer.start(8);
}

void GameView::initStage(int s)
{
	h1 = new EdgeItem(QPoint(WIDTH / 2, 0), false);     //游戏界面边界线
	h2 = new EdgeItem(QPoint(WIDTH / 2, HEIGHT), false);
	v1 = new EdgeItem(QPoint(0, HEIGHT / 2), true);
	v2 = new EdgeItem(QPoint(WIDTH, HEIGHT / 2), true);
	line = new LineItem(QPoint(WIDTH / 2, HEIGHT - 100));
	ball = new BallItem(QPoint(WIDTH / 2, HEIGHT - 100 - LINE_HEIGHT / 2 - RADIUS + 2));

	endGame();
	initBlocks(s);

	scene->addItem(h1);
	scene->addItem(h2);
	scene->addItem(v1);
	scene->addItem(v2);
	scene->addItem(line);
	scene->addItem(ball);

	connect(h2, &EdgeItem::fail, this, &GameView::dealBallFail);
	life = GameRule::getInstance()->getMaxLife();
}

