#include "TessngDBNet.h"
#include "_netitem.h"
#include "_netitemtype.h"
#include "netinterface.h"
#include <QMouseEvent>
#include <QGraphicsView>
#include "ilink.h"
#include "IConnector.h"
#include "gconnector.h"
#include "TessngDBCopy.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "glink.h"
#include "IDecisionPoint.h"
#include "gdecisionpoint.h"
#include "connector.h"
#include "signalgroup.h"
#include "isignallamp.h"
#include "signallamp.h"
#include "netglobal.h"
#include "GGuideArrow.h"
#include "gsignallamp.h"
#include "VehicleComposition.h"
#include "gdeparturepoint.h"
#include "gvehicledrivinfocollector.h"
#include "gvehiclequeuecounter.h"
#include "gvehicletraveldetector.h"
#include "GVehicleDetector.h"
#include "greducespeedarea.h"
#include "gbusstation.h"
#include "gbusline.h"
#include "CustomerGraphicsObject.h"
#include "VehicleTravelDetector.h"
#include "ReduceSpeedArea.h"
TessngDBNet::TessngDBNet()
{
    initMenu();
}
void TessngDBNet::initMenu(){
    QWidget* wnd=gpTessInterface->netInterface()->graphicsView();
    wnd->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(wnd,&QWidget::customContextMenuRequested,this,&TessngDBNet::onCustomContextMenuRequested);

    m_menu=new QMenu(wnd);
    QAction* action=m_menu->addAction(tr("Eanble Drawing"));
    action->setCheckable(true);
    QObject::connect(action,&QAction::toggled,this,&TessngDBNet::enableDrawing);

    action=m_menu->addAction(tr("Remove Item"));
    QObject::connect(action,&QAction::triggered,this,&TessngDBNet::removeGraphicsItem);

    QActionGroup* group=new QActionGroup(this);
    QObject::connect(group,&QActionGroup::triggered,this,&TessngDBNet::chooseItemTypeChanged);
    QMenu* menuType=m_menu->addMenu(tr("Item Type Choose"));
    action=menuType->addAction(tr("Rectangle"));
    action->setCheckable(true);
    action->setChecked(true);
    action->setData(QVariant::fromValue<GraphicsBaseItem::ShapeType>(GraphicsBaseItem::RECTANGLE));
    group->addAction(action);
    action=menuType->addAction(tr("PolyRect"));
    action->setCheckable(true);
    action->setData(QVariant::fromValue<GraphicsBaseItem::ShapeType>(GraphicsBaseItem::POLYGON));
    group->addAction(action);
    action=menuType->addAction(tr("Ellipse"));
    action->setCheckable(true);
    action->setData(QVariant::fromValue<GraphicsBaseItem::ShapeType>(GraphicsBaseItem::ELLIPSE));
    group->addAction(action);
    action=m_menu->addAction(tr("Save Net"));
    QObject::connect(action,&QAction::triggered,this,&TessngDBNet::saveScenesData);
}

bool TessngDBNet::isSamePoint(QPointF p1, QPointF p2)
{
    QPointF dif = p1 - p2;
    float distance = dif.manhattanLength();
    if (distance <= 5) return true;
    return false;
}
void TessngDBNet::beforeLoadNet()
{
}

void TessngDBNet::afterLoadNet()
{
}
void TessngDBNet::chooseItemTypeChanged(QAction* action){
    if(!m_bEnableDrawing) return;
    GraphicsBaseItem::ShapeType type=action->data().value<GraphicsBaseItem::ShapeType>();
    if(GraphicsBaseItem::RECTANGLE==type){
        setDrawRectangleModule();
    }else if(GraphicsBaseItem::POLYGON==type){
        setDrawPolylineModule();
    }else if(GraphicsBaseItem::ELLIPSE==type){
        setDrawElliseModule();
    }
}
void TessngDBNet::saveScenesData()
{
    if(nullptr==m_graphicsBaseItem) return;

    QPainterPath pathShape=m_graphicsBaseItem->shape();
    QMainWindow* pWin = gpTessInterface->guiInterface()->mainWindow();
    QString path = QFileDialog::getSaveFileName(pWin, tr("Save File"), "", "*.tess");
    if (path.isEmpty()) return;

    TessngDBCopy::getInstance()->clippingDB(pathShape, path);
}

void TessngDBNet::enableDrawing(){
    QAction *action=qobject_cast<QAction *>(sender());
    m_bEnableDrawing=action->isChecked();
    if(m_bEnableDrawing&&nullptr==m_graphicsBaseItem){
        setDrawRectangleModule();
    }
}
void TessngDBNet::removeGraphicsItem(){
    if(nullptr==m_graphicsBaseItem) return;
    m_processMode=DEFAULT;
    m_graphicsBaseItem->reset();
    m_graphicsBaseItem->finishDrawing();
    gpTessInterface->netInterface()->graphicsScene()->update();
}
void TessngDBNet::labelNameAndFont(int itemType, long itemId, int& outPropName, qreal& outFontSize)
{
    outPropName = GraphicsItemPropName::Id;
    outFontSize = 6;
    if (itemType == NetItemType::GConnectorType) {
        outPropName = GraphicsItemPropName::Name;
    }
    else if (itemType == NetItemType::GLinkType) {
        if (itemId == 1 || itemId == 5 || itemId == 6) {
            outPropName = GraphicsItemPropName::Name;
        }
    }
}

void TessngDBNet::afterViewMouseMoveEvent(QMouseEvent *event)
{
    if(!m_bEnableDrawing) return;
    if (!m_graphicsBaseItem) return;
    GraphicsBaseItem::ShapeType shapeType = m_graphicsBaseItem->getShapeType();
    switch (m_processMode){
    case DEFAULT:break;
    case DRAW:
        if (shapeType == GraphicsBaseItem::RECTANGLE)
        {
            m_graphicsBaseItem->removeFirstPoint();
            m_graphicsBaseItem->addPoint(gpTessInterface->netInterface()->graphicsView()->mapToScene(event->pos()));
            gpTessInterface->netInterface()->graphicsScene()->update();
        }else if(shapeType == GraphicsBaseItem::ELLIPSE){
            m_graphicsBaseItem->removeFirstPoint();
            m_graphicsBaseItem->addPoint(gpTessInterface->netInterface()->graphicsView()->mapToScene(event->pos()));
            gpTessInterface->netInterface()->graphicsScene()->update();
        }
        break;
    case MOVE:break;
    }
}

void TessngDBNet::afterViewMousePressEvent(QMouseEvent *event)
{
    if(!m_bEnableDrawing) return;
    if (event->button() != Qt::LeftButton || !m_graphicsBaseItem) return;
    m_pPressPoint= gpTessInterface->netInterface()->graphicsView()->mapToScene(event->pos());
    GraphicsBaseItem::ShapeType shapeType = m_graphicsBaseItem->getShapeType();
    switch (m_processMode){
    case DEFAULT:
    {
        m_processMode = DRAW;
        if (shapeType == GraphicsBaseItem::RECTANGLE)
        {
            m_graphicsBaseItem->reset();
            m_graphicsBaseItem->setMousePt(m_pPressPoint);
        }else if(shapeType == GraphicsBaseItem::POLYGON){
            m_graphicsBaseItem->reset();
            m_graphicsBaseItem->setMousePt(m_pPressPoint);
        }else if (shapeType == GraphicsBaseItem::ELLIPSE){
            m_graphicsBaseItem->reset();
            m_graphicsBaseItem->setMousePt(m_pPressPoint);
        }
    }
        break;
    case DRAW:break;
    case MOVE:break;
    }
}

void TessngDBNet::afterViewMouseReleaseEvent(QMouseEvent *event)
{
    if(!m_bEnableDrawing) return;
    if (event->button() != Qt::LeftButton || !m_graphicsBaseItem) return;
    GraphicsBaseItem::ShapeType shapeType = m_graphicsBaseItem->getShapeType();
    QPointF releasePos = gpTessInterface->netInterface()->graphicsView()->mapToScene(event->pos());
    switch (m_processMode){
    case DEFAULT: break;
    case MOVE:break;
    case DRAW:
    {
        if (shapeType == GraphicsBaseItem::RECTANGLE){
            m_graphicsBaseItem->addPoint(releasePos);
            m_graphicsBaseItem->finishDrawing();
            m_processMode = DEFAULT;
        }else if(shapeType == GraphicsBaseItem::POLYGON){
            QPointF firstPt;
            if (!m_graphicsBaseItem->getPoint(0, firstPt))
            {
                m_graphicsBaseItem->addPoint(releasePos);
            }
            if (m_graphicsBaseItem->getPointCounts() > 1 && isSamePoint(firstPt, releasePos))
            {
                m_graphicsBaseItem->finishDrawing();
                m_processMode = DEFAULT;
            }
            else
            {
                m_graphicsBaseItem->addPoint(releasePos);
            }
        }else if(shapeType == GraphicsBaseItem::ELLIPSE){
            m_graphicsBaseItem->addPoint(releasePos);
            m_graphicsBaseItem->finishDrawing();
            m_processMode = DEFAULT;
        }

    }
        break;
    }
    gpTessInterface->netInterface()->graphicsScene()->update();
}

void TessngDBNet::onCustomContextMenuRequested(const QPoint &)
{
    m_menu->exec(QCursor::pos());
}
void TessngDBNet::setDrawPolylineModule()
{
    if (m_graphicsBaseItem)
    {
        m_graphicsBaseItem->reset();
        m_graphicsBaseItem = nullptr;
    }
    if (!m_PolylineItem)
    {
        QVector<QPointF> pts;
        m_PolylineItem = new InterestPolylineItem(pts, false);
        gpTessInterface->netInterface()->graphicsScene()->addItem(m_PolylineItem);
    }
    m_graphicsBaseItem = m_PolylineItem;
    m_processMode = DEFAULT;
}
void TessngDBNet::setDrawRectangleModule()
{
    if (m_graphicsBaseItem)
    {
        m_graphicsBaseItem->reset();
        m_graphicsBaseItem = nullptr;
    }
    if (!m_ptrInterRect)
    {
        QVector<QPointF> rpts;
        m_ptrInterRect=new InterestRectItem(rpts,false);
        gpTessInterface->netInterface()->graphicsScene()->addItem(m_ptrInterRect);
    }
    m_graphicsBaseItem =m_ptrInterRect;

    m_processMode = DEFAULT;
}
void TessngDBNet::setDrawElliseModule(){
    if (m_graphicsBaseItem)
    {
        m_graphicsBaseItem->reset();
        m_graphicsBaseItem = nullptr;
    }
    if (!m_ElliseItem)
    {
        QVector<QPointF> pts;
        m_ElliseItem = new InterestElliseItem(pts, false);
        gpTessInterface->netInterface()->graphicsScene()->addItem(m_ElliseItem);
    }
    m_graphicsBaseItem = m_ElliseItem;
    m_processMode = DEFAULT;
}
