#ifndef BALLITEM_H
#define BALLITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPoint>
#define RADIUS 8

class BallItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	explicit BallItem(QPoint, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QPainterPath shape() const;
	bool isLocked();
	void advance(int phase);
	void lockBall();

signals:


public slots:
	void moveLeftWithLine();
	void moveRightWithLine();
	void launch();//发射

private:
	enum ReboundType {
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	double k, dx;//一次函数斜率，以及横坐标增长率
	bool isV;//斜率是否存在
	bool lock;//是否被锁
	ReboundType last;

protected:
	void reBound(ReboundType tp);
};

#endif // BALLITEM_H
