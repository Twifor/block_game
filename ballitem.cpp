#include "ballitem.h"
#include "lineitem.h"
#include <QPainter>
#include <QDebug>
#include "common.h"
#include "edgeitem.h"
#include <cmath>
#include "blockitem.h"
#include <QSound>

BallItem::BallItem(QPoint f, QGraphicsItem *parent) : QGraphicsItem(parent)
{
	setData(0, BALL_ITEM);
	isV = false;
	lock = true;
	setPos(f);//传入圆心
	last = static_cast<ReboundType>(-1);
}

QRectF BallItem::boundingRect() const
{
	return QRectF( -RADIUS, -RADIUS, RADIUS * 2, RADIUS * 2);
}

void BallItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPixmap(-RADIUS, -RADIUS, RADIUS * 2, RADIUS * 2, QPixmap(":/image/ball"));
	painter->restore();
}

QPainterPath BallItem::shape() const
{
	QPainterPath path;
	path.addEllipse(0, 0, RADIUS, RADIUS);
	return path;
}

bool BallItem::isLocked()
{
	return lock;
}

void BallItem::advance(int phase)
{
	if (phase == 0 || lock) return;
	QList<QGraphicsItem*> list = collidingItems(Qt::IntersectsItemBoundingRect);
	if (!list.isEmpty()) {//执行反弹操作
		QGraphicsItem* p = list[0];//一次只能碰撞一个
		QSound::play(":/music/col");
		int tp = p->data(0).toInt();
		if (tp == EDGE_ITEM) {    //触碰边界屏障
			EdgeItem *p1 = static_cast<EdgeItem*>(p);
			p1->collaspe();    //可能触发失败信号
			if (!p1->data(1).toBool()) reBound(DOWN);
			else reBound(LEFT);
		}else if (tp == LINE_ITEM) {
			LineItem *p2 = static_cast<LineItem*>(p);
			if (fabs(p2->pos().x() - pos().x()) <= 1e-6) {//正好在中间，执行高精度的反射
				isV = true;
				dx = -MOVE_STEP;
			}else{//否则根据长度比例调整反弹斜率
				if(fabs(fabs(pos().x() - p2->pos().x()) - LINE_LENGTH / 2.0) <= 1) return;
				reBound(UP); //先进行精确反射
				if (pos().x() < p2->pos().x()) {//再调整
					if (isV) {
						isV = false;
						dx = -MOVE_STEP;
						k = 1.0 * (LINE_LENGTH - p2->pos().x() + pos().x()) / (LINE_LENGTH / 2.0);
					}else k += 0.02 * (p2->pos().x() - pos().x()) / (LINE_LENGTH / 2.0) * k;
				}else{
					if (isV) {
						isV = false;
						dx = MOVE_STEP;
						k = -1.0 * (LINE_LENGTH - pos().x() + p2->pos().x()) / (LINE_LENGTH / 2.0);
					}else k -= 0.02 * (pos().x() - p2->pos().x()) / (LINE_LENGTH / 2.0) * k;
				}
			}
		}else if (tp == BLOCK_ITEM) {
			BlockItem *p3 = static_cast<BlockItem*>(p);
			qDebug() << dx << k << p3->pos() << pos();
			if(isV) {
				if(dx < 0) reBound(DOWN);
				else reBound(UP);
			}
			else if(dx > 0) {
				if(p3->pos().x() - BLOCK_LENGTH / 2.0 + 3 >= pos().x()
				   && pos().y() >= p3->pos().y() - BLOCK_WIDTH / 2.0 - 3
				   && pos().y() <= p3->pos().y() + BLOCK_WIDTH / 2.0 + 3) reBound(LEFT);
				else if(p3->pos().y() - BLOCK_WIDTH / 2.0 >= pos().y()) reBound(UP);
				else reBound(DOWN);
			}else{
				if(p3->pos().x() + BLOCK_LENGTH / 2.0 - 3 <= pos().x()
				   && pos().y() >= p3->pos().y() - BLOCK_WIDTH / 2.0 - 3
				   && pos().y() <= p3->pos().y() + BLOCK_WIDTH / 2.0 + 3) reBound(RIGHT);
				else if(p3->pos().y() - BLOCK_WIDTH / 2.0 >= pos().y()) reBound(UP);
				else reBound(DOWN);
			}
			if(p3->getType() == BlockItem::RED_BLOCK) {
				if(isV) {
					k = (qrand() % 2000 + 500) / 1000.0;
					isV = false;
				}else{
					int s = qrand() % 25 + 1;
					k = k > 0 ? s / 10.0 : -s / 10.0;
				}
			}
			p3->breakBlock();
		}
		if (isV) setPos(pos().x(), pos().y() + dx);
		else {
			if(dx < 0) dx = -MOVE_STEP / sqrt(k * k + 1);
			else dx = MOVE_STEP / sqrt(k * k + 1);
			setPos(pos().x() + dx, pos().y() + k * dx); //一次函数线性移动
		}
	}else{
		last = static_cast<ReboundType>(-1);
		if (isV) setPos(pos().x(), pos().y() + dx);
		else {
			if(dx < 0) dx = -MOVE_STEP / sqrt(k * k + 1);
			else dx = MOVE_STEP / sqrt(k * k + 1);
			setPos(pos().x() + dx, pos().y() + k * (dx)); //一次函数线性移动
		}
	}
}

void BallItem::lockBall()
{
	lock = true;
}

void BallItem::moveLeftWithLine()
{
	if (pos().x() - LINE_LENGTH / 2.0 > 0) {
		setPos(pos().x() - MOVE_SPEED, pos().y());
	}
}

void BallItem::moveRightWithLine()
{
	if (pos().x() + LINE_LENGTH / 2.0 < WIDTH) {
		setPos(pos().x() + MOVE_SPEED, pos().y());
	}
}

void BallItem::launch()
{
	if (lock) {
		lock = false;
		isV = true;
		k = 0;
		dx = -MOVE_STEP;
	}
}

void BallItem::reBound(BallItem::ReboundType tp)//反弹函数
{
	if(last == tp) return;
	else if(last == UP && tp == DOWN) return;
	else if(last == DOWN && tp == UP) return;
	else if(last == LEFT && tp == RIGHT) return;
	else if(last == RIGHT && tp == LEFT) return;
	last = tp;
	qDebug() << tp << dx << k << pos().x() << pos().y();
	switch (tp) {
		case DOWN:
			if (isV) dx = MOVE_STEP; //方向扭转
			else k = -k;
			break;
		case UP:
			if (isV) dx = -MOVE_STEP; //方向扭转
			else k = -k;
			break;
		case LEFT: case RIGHT:
			dx = -dx;
			k = -k;
			break;
	}
}

