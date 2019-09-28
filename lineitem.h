#ifndef LINEITEM_H
#define LINEITEM_H

#include <QObject>
#include <QGraphicsItem>
#define LINE_LENGTH 200
#define LINE_HEIGHT 10
#define MOVE_SPEED 3

class LineItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	explicit LineItem(QPoint, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void moveL();
	void moveR();

signals:

public slots:

private:


};

#endif // LINEITEM_H
