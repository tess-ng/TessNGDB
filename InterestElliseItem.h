#ifndef INTERESTELLISEITEM_H
#define INTERESTELLISEITEM_H

#include "GraphicsBaseItem.h"
class InterestElliseItem: public GraphicsBaseItem
{
public:
    InterestElliseItem(const QPolygonF& pts,bool drawFinished = false,GraphicsBaseItem *parent = Q_NULLPTR);
    virtual ShapeType getShapeType()const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    virtual bool contains(const QPointF& pt)override;
    virtual QPainterPath shape() const override;
};

#endif // INTERESTELLISEITEM_H
