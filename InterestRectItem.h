#ifndef INTERESTRECTITEM_H
#define INTERESTRECTITEM_H

#include "GraphicsBaseItem.h"

class InterestRectItem : public GraphicsBaseItem
{
public:
    explicit InterestRectItem(const QPolygonF& pts,bool drawFinished = false,GraphicsBaseItem *parent = Q_NULLPTR);

    virtual ShapeType getShapeType()const override;

    //绘图 在updata后调用 paint
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    virtual bool contains(const QPointF& pt)override;

    virtual QPainterPath shape() const override;
};

#endif // INTERESTRECTITEM_H
