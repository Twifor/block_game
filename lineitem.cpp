#include "lineitem.h"
#include <QPainter>
#include "common.h"

LineItem::LineItem(QPoint p, QGraphicsItem *parent) : QGraphicsItem(parent)
{
	setData(0, LINE_ITEM);
	setPos(p);
}

QRectF LineItem::boundingRect() const
{
	return QRectF(-LINE_LENGTH / 2, -LINE_HEIGHT / 2, LINE_LENGTH, LINE_HEIGHT);
}

QPainterPath LineItem::shape() const
{
	QPainterPath path;
	path.addRect(-LINE_LENGTH / 2, -LINE_HEIGHT / 2, LINE_LENGTH, LINE_HEIGHT );
	return path;
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	painter->drawPixmap(-LINE_LENGTH / 2, -LINE_HEIGHT / 2, LINE_LENGTH, LINE_HEIGHT, QPixmap(":/image/line"));
	painter->restore();
}

void LineItem::moveL()
{
	if (pos().x() - LINE_LENGTH / 2 > 0){
		setPos(pos().x() - MOVE_SPEED, pos().y());
	}
}

void LineItem::moveR()
{
	if (pos().x() + LINE_LENGTH / 2 < WIDTH){
		setPos(pos().x() + MOVE_SPEED, pos().y());
	}
}
