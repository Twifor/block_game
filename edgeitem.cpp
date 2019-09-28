#include "edgeitem.h"
#include "common.h"
#include <QDebug>

EdgeItem::EdgeItem(QPoint p, bool isV, QGraphicsItem *parent) : QGraphicsItem(parent)
{
	setPos(p);
	setData(0, EDGE_ITEM);
	setData(1, isV);
	this->isV = isV;
}

QRectF EdgeItem::boundingRect() const
{
	if (isV) return QRectF(-2, -375, 4, 750);
	else return QRectF(-250, -2, 500, 4);
}

QPainterPath EdgeItem::shape() const
{
	QPainterPath path;
	if (isV) path.addRect(-2, -375, 4, 750);
	else path.addRect(-250, -2, 500, 4);
	return path;
}

void EdgeItem::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{

}

void EdgeItem::collaspe()
{
	emit fail();
}
