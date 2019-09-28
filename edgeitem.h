#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QObject>
#include <QGraphicsItem>

class EdgeItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	explicit EdgeItem(QPoint p, bool isV, QGraphicsItem *parent = nullptr);
	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void collaspe();

signals:
	void fail();//失败信号

public slots:

private:
	bool isV;

};

#endif // EDGEITEM_H
