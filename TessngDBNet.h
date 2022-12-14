#ifndef TESSNGDBNET_H
#define TESSNGDBNET_H

#include "customernet.h"
#include "InterestRectItem.h"
#include "InterestPolylineItem.h"
#include "InterestElliseItem.h"
#include <QObject>
class QMenu;
class GLink;
class GConnector;
class TessngDBNet :public QObject, public CustomerNet
{
    Q_OBJECT
public:
    TessngDBNet();
    //==========以下是接口方法重新实现==========
    //加载路网前的准备
    void beforeLoadNet() override;
    //加载完路网后的行为
    void afterLoadNet() override;
    //
    bool isPermitForCustDraw() override { return true; }
    //写标签，按照给定的属性名和字体大小（米）
    void labelNameAndFont(int itemType, long itemId, int &outPropName, qreal &outFontSize) override;
    //------------------------------------------
    void setDrawRectangleModule();

    void setDrawPolylineModule();

    void setDrawElliseModule();
protected:
    //QGraphicsView的mouseMoveEvent事件后的行为
    virtual void afterViewMouseMoveEvent(QMouseEvent *event)override;
    //QGraphicsView的mousePressEvent事件后的行为
    virtual void afterViewMousePressEvent(QMouseEvent *event)override;
    //QGraphicsView的mouseReleaseEvent事件后的行为
    virtual void afterViewMouseReleaseEvent(QMouseEvent *event)override;
private Q_SLOTS:
    void onCustomContextMenuRequested(const QPoint &pos);
    void enableDrawing();
    void removeGraphicsItem();
    void chooseItemTypeChanged(QAction*);
    void saveScenesData();
private:
    bool checkContansLane(QHash<int, GLink*>& hash, long laneId);
    bool checkContansLane(QHash<int, GConnector*>& hash, long fromlaneId,long tolaneId);
    void initMenu();
    bool isSamePoint(QPointF p1, QPointF p2);
    enum ProcessMode
    {
        DEFAULT = 0, // 默认模式
        DRAW, //绘图模式
        MOVE  //移动模式
    };
    QPointF m_pPressPoint;
    InterestRectItem* m_ptrInterRect=nullptr;
    GraphicsBaseItem*  m_graphicsBaseItem = nullptr;
    InterestPolylineItem *	m_PolylineItem = nullptr;
    InterestElliseItem* m_ElliseItem=nullptr;
    ProcessMode m_processMode= DEFAULT;
    QMenu* m_menu=nullptr;
    bool m_bEnableDrawing=false;
};

#endif // TESSNGDBNET_H
