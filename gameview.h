#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QTimer>
#include "lineitem.h"
#include "ballitem.h"
#include "edgeitem.h"
#include "blockitem.h"

class GameView : public QGraphicsView//游戏界面显示类
{
	Q_OBJECT
public:
	explicit GameView(QWidget *parent = nullptr);
	~GameView();
	bool isGameing();//判断游戏进行状态
	void startGame();//开始游戏
	void endGame();//结束游戏
	void pauseGame(bool);//暂停/取消暂停
	void initStage(int s);//加载一个关卡，s关卡编号


signals:
	void fail();
	void success();//成功
	void error();//异常信号
	void lifeChange(int);
	void addScore(int);

public slots:
	void moveLineLeft();
	void moveLineRight();
	void dealBallFail();
	void deleteBlock(BlockItem *, BlockItem::BlockType);


private:
	void initBlocks(int s);//初始化关卡对应的方块
	bool isGameingBool;
	QGraphicsScene *scene;
	LineItem *line;
	BallItem *ball;
	QTimer timer;
	EdgeItem *h1, *h2, *v1, *v2;
	QList<BlockItem*> list;
	int life;//剩余生命值
};

#endif // GAMEVIEW_H
