#include "InterestPolylineItem.h"
#include<QPainter>
InterestPolylineItem::InterestPolylineItem(const QPolygonF& pts,bool drawFinished ,GraphicsBaseItem *parent )
    :GraphicsBaseItem(pts, drawFinished, parent)
{

}

GraphicsBaseItem::ShapeType InterestPolylineItem::getShapeType()const
{
    return POLYGON;
}

void InterestPolylineItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget * )
{
    if (m_pts.isEmpty())
    {
        return;
    }

    painter->setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing, true);

    painter->setPen(QColor(0, 191, 255, 180));
    painter->setBrush(QBrush(QColor(0, 191, 255, 18)));
    painter->setOpacity(1.0);

    if (m_drawFinished)
    {
        painter->drawPolygon(m_pts);
    }
    else
    {
        painter->drawPolyline(m_pts);
        painter->drawLine(m_pts.last(), m_mousePt);
    }
    for (int i = 0; i < m_pts.size(); i++)
    {
        if(0==i){
            painter->setPen(QColor(255,20,147, 180));
        }else{
            painter->setPen(QColor(0, 191, 255, 180));
        }
        painter->drawEllipse(m_pts.at(i), 2, 2);
    }
}

bool InterestPolylineItem::contains(const QPointF & pt)
{
    if (m_pts.count() < 3)
    {
        return false;
    }

    bool ret = m_pts.containsPoint(pt, Qt::WindingFill);
    return ret;
}
QPainterPath InterestPolylineItem::shape() const
{
    QPainterPath path;

    if (m_drawFinished)
    {
        path.addPolygon(m_pts);
    }
    else
    {
        for (auto& pt : m_pts)
        {
            path.lineTo(pt);
        }
    }

    return path;
}