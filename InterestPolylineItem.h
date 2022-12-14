#ifndef INTERESTPOLYLINEITEM_H
#define INTERESTPOLYLINEITEM_H

#include "GraphicsBaseItem.h"
class InterestPolylineItem: public GraphicsBaseItem
{
public:
    InterestPolylineItem(const QPolygonF& pts,bool drawFinished = false,GraphicsBaseItem *parent = Q_NULLPTR);
    virtual ShapeType getShapeType()const override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    virtual bool contains(const QPointF& pt)override;
    virtual QPainterPath shape() const override;
};

#endif // INTERESTPOLYLINEITEM_H
