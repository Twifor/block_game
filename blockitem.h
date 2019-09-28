#ifndef BLOCKITEM_H
#define BLOCKITEM_H

#include <QGraphicsItem>
#define BLOCK_LENGTH 30
#define BLOCK_WIDTH 15

class BlockItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	enum BlockType {//方块形状
		NORMAL_BLOCK,
		KEY_BLOCK,
		GREEN_BLOCK,
		IRON_BLOCK,
		RED_BLOCK
	};
	explicit BlockItem(QPoint, BlockType t = NORMAL_BLOCK,QGraphicsItem *parent = nullptr);
	BlockType getType();
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QPainterPath shape() const;
	void breakBlock();//破坏这个方块

signals:
	void hasDestoryed(BlockItem *, BlockType);//已经被破坏了

public slots:

private:
	BlockType type;
	int life;//剩余生命

};

#endif // BLOCKITEM_H
