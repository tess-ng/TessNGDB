#include "InterestElliseItem.h"
#include<QPainter>
InterestElliseItem::InterestElliseItem(const QPolygonF& pts,bool drawFinished ,GraphicsBaseItem *parent )
    :GraphicsBaseItem(pts, drawFinished, parent)
{

}

GraphicsBaseItem::ShapeType InterestElliseItem::getShapeType()const
{
    return ELLIPSE;
}

void InterestElliseItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget * )
{
    if (m_pts.isEmpty())
    {
        return;
    }
    painter->setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing, true);

    painter->setPen(QColor(0, 191, 255, 180));
    painter->setBrush(QBrush(QColor(0, 191, 255, 18)));
    painter->setOpacity(1.0);

    painter->drawPoint(m_mousePt);

    QPointF pt=m_pts[0];
    painter->drawEllipse(m_mousePt,fabs(m_mousePt.x()-pt.x()),fabs(m_mousePt.y()-pt.y()));
}
bool InterestElliseItem::contains(const QPointF & pt)
{
    if (m_pts.count() < 1) return false;
    QLineF line(m_mousePt,pt);
    QLineF line2(m_mousePt,m_pts[0]);
    return line2.length()>line.length();
}
QPainterPath InterestElliseItem::shape() const
{
    QPainterPath path;
    if (m_pts.count() < 1) return path;
    path.addEllipse(m_mousePt, m_pts[0].x(), m_pts[0].y());
    return path;
}