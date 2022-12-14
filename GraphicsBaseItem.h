#ifndef GRAPHICSBASEITEM_H
#define GRAPHICSBASEITEM_H

#include <QGraphicsItem>

class GraphicsBaseItem : public QGraphicsItem
{
public:
    enum ShapeType
    {
        UNDEFINED = 0,
        CIRCLE,	            // 圆形
        ELLIPSE,	        // 椭圆形
        RECTANGLE,	        // 矩形
        POLYGON     //多边形
    };

    explicit GraphicsBaseItem(const QPolygonF& pts,
                              bool drawFinished = false,
                              QGraphicsItem *parent = Q_NULLPTR);

    ~GraphicsBaseItem();

    // 查看图形类型，比如矩形，多边形
    virtual ShapeType getShapeType()const = 0;

    // 查看是否包含这个点
    virtual bool contains(const QPointF& pt) = 0;

    //是否完成了绘图（鼠标拖动场景下使用，或者多边形绘制）
    virtual void finishDrawing();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) =0;

    //添加点，获取点
    void addPoint(const QPointF& point);

    int getPointCounts();

    bool getPoint(int id, QPointF& pt);

    QPolygonF getPoints();

    void removeLastPoint();

    void removeFirstPoint();

    int getNearestVertex(const QPointF& pt);

    //设置当前鼠标位置
    void setMousePt(const QPointF& mousePt);

    //重置
    void reset();

    //是否被选中
    void setSelected(bool isSelected);
    virtual QPainterPath shape() const =0;
    virtual QRectF boundingRect() const override;
protected:
    QPolygonF m_pts;//闭合曲线首尾点无需重复存储
    QPointF m_mousePt;//画图时鼠标当前位置点
    bool    m_selected;//是否被选中
    bool    m_drawFinished;//是否完成绘图
};

#endif // GRAPHICSBASEITEM_H
