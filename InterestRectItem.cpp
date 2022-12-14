#include "InterestRectItem.h"
#include <QPainter>
#include <QDebug>
namespace
{
    bool IsGreaterThan(double d1, double d2)//比较d1是否大于d2
    {
        double eps = 1.0e-6;
        double delta = d1 - d2;
        if (delta > eps)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    QRectF makeRect(const QPointF& pt1, const QPointF& pt2)
    {
        double topLeftX = IsGreaterThan(pt1.x(), pt2.x()) ? pt2.x() : pt1.x();
        double topLeftY = IsGreaterThan(pt1.y(), pt2.y()) ? pt2.y() : pt1.y();
        double width = qAbs(pt1.x() - pt2.x());
        double height = qAbs(pt1.y() - pt2.y());
        return QRectF(topLeftX, topLeftY, width, height);
    }
}


InterestRectItem::InterestRectItem(const QPolygonF& pts,bool drawFinished ,GraphicsBaseItem *parent )
    :GraphicsBaseItem(pts, drawFinished, parent)
{

}

GraphicsBaseItem::ShapeType InterestRectItem::getShapeType()const
{
    return RECTANGLE;
}

void InterestRectItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget *)
{
    if (m_pts.isEmpty()) return;
    painter->setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing, true);

    painter->setPen(QColor(0, 191, 255, 180));
    painter->setBrush(QBrush(QColor(0, 191, 255, 18)));
    painter->setOpacity(1.0);

    QRectF rect = makeRect(m_pts[0], m_mousePt);
    painter->drawRect(rect);
}

bool InterestRectItem::contains(const QPointF & pt)
{
    if (m_pts.count() != 1)
    {
        return false;
    }

    QRectF rect(m_mousePt,m_pts[0]);
    bool ret = rect.contains(pt);
    return ret;
}
QPainterPath InterestRectItem::shape() const
{
    QPainterPath path;
    if (m_pts.count() < 1) return path;
    QRectF rect = makeRect(m_pts[0], m_mousePt);
    path.addRect(rect);
    return path;
}