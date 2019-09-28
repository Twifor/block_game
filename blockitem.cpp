#include "blockitem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "common.h"
#include <QDebug>

BlockItem::BlockItem(QPoint s, BlockType t, QGraphicsItem *parent) : QGraphicsItem(parent)
{
	setData(0, BLOCK_ITEM);
	type = t;
	setPos(s);
	switch (type) {
	case NORMAL_BLOCK: case KEY_BLOCK: case RED_BLOCK:
		life = 1; break;
	case GREEN_BLOCK:
		life = 3; break;
	case IRON_BLOCK:
		life = 50; break;
	}
}

BlockItem::BlockType BlockItem::getType()
{
	return type;
}

QRectF BlockItem::boundingRect() const
{
	return QRectF(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH);
}

void BlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->save();
	painter->setRenderHint(QPainter::Antialiasing);
	if (type == NORMAL_BLOCK) painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
											  QPixmap(":/image/yellow_block"));
	else if (type == KEY_BLOCK) painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
												QPixmap(":/image/key"));
	else if (type == GREEN_BLOCK) {
		if(life == 3) painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
									  QPixmap(":/image/green"));
		else if(life == 2) painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
										   QPixmap(":/image/green1"));
		else painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
							 QPixmap(":/image/green2"));
	}else if(type == IRON_BLOCK) {
		if(life >= 35) painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
									   QPixmap(":/image/iron"));
		else if(life >= 15) painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
											QPixmap(":/image/iron1"));
		else painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
							 QPixmap(":/image/iron2"));
	}else if(type == RED_BLOCK) {
		painter->drawPixmap(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH,
							QPixmap(":/image/red"));
	}
	painter->restore();
}

QPainterPath BlockItem::shape() const
{
	QPainterPath path;
	path.addRect(-BLOCK_LENGTH / 2, -BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH);
	return path;
}

void BlockItem::breakBlock()
{
	life -= GameRule::getInstance()->getBreakRule();
	if (life <= 0) emit hasDestoryed(this, type);
	else update();
}
