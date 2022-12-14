#include "GraphicsBaseItem.h"
GraphicsBaseItem::GraphicsBaseItem(const QPolygonF& pts,bool drawFinished,QGraphicsItem *parent): QGraphicsItem{parent}, m_pts(pts), m_drawFinished(drawFinished)
{
    if (m_pts.count() > 0)
    {
        m_mousePt = m_pts[0];
    }
}

GraphicsBaseItem::~GraphicsBaseItem()
{

}
void GraphicsBaseItem::finishDrawing()
{
    m_drawFinished = true;
}
void GraphicsBaseItem::addPoint(const QPointF & point)
{
    if (m_drawFinished)
    {
        return;
    }

    if (m_pts.count() > 0)
    {
        bool hasPt = m_pts.contains(point);
        if (hasPt)
        {
            return;
        }
    }
    m_pts.push_back(point);
}
int GraphicsBaseItem::getPointCounts()
{
    return m_pts.count();
}

bool GraphicsBaseItem::getPoint(int id, QPointF& pt)
{
    if (id >= 0 && id < m_pts.size())
    {
        pt = m_pts[id];
        return true;
    }
    return false;
}

QPolygonF GraphicsBaseItem::getPoints()
{
    return m_pts;
}

void GraphicsBaseItem::removeLastPoint()
{
    if(!m_pts.empty())m_pts.removeLast();
}

void GraphicsBaseItem::removeFirstPoint(){
    if (!m_pts.empty())m_pts.removeFirst();
}
int GraphicsBaseItem::getNearestVertex(const QPointF & pt)
{
    int id = -1;
    double minDistance = 999999.99;
    double eps = 2.0;
    for (int ii = 0; ii < m_pts.size(); ++ii)
    {
        QPointF ptII = m_pts[ii];
        QPointF p = ptII - pt;
        double dis = std::sqrt(p.x()*p.x() + p.y()*p.y());
        if (dis < eps && dis < minDistance)
        {
            minDistance = dis;
            id = ii;
        }
    }
    return id;
}


void GraphicsBaseItem::setMousePt(const QPointF & mousePt)
{
    m_mousePt = mousePt;
}

void GraphicsBaseItem::reset()
{
    m_pts.clear();
    m_drawFinished = false;
}

void GraphicsBaseItem::setSelected(bool isSelected)
{
    m_selected = isSelected;
}

QRectF GraphicsBaseItem::boundingRect() const
{
    QRectF rect;
    if (m_pts.size() > 1)
    {
        QPointF leftTop = m_pts[0];
        QPointF rightBottom = m_pts[0];
        for (int ii = 1; ii < m_pts.size(); ++ii)
        {
            QPointF pt = m_pts[ii];
            if (pt.x() < leftTop.x())
            {
                leftTop.setX(pt.x());
            }

            if (pt.y() < leftTop.y())
            {
                leftTop.setY(pt.y());
            }

            if (pt.x() > rightBottom.x())
            {
                rightBottom.setX(pt.x());
            }

            if (pt.y() > rightBottom.y())
            {
                rightBottom.setY(pt.y());
            }
        }
        rect=QRectF(leftTop, rightBottom);
    }
    return rect;

}
