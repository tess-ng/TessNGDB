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
bool TessngDBNet::checkContansLane(QHash<int, GLink*>& hash, long laneId) {
    auto it = hash.constBegin();
    while (it != hash.constEnd())
    {
        foreach(auto lane, it.value()->mlGLane)
        {
            if (laneId == lane->id()) return true;
        }
        ++it;
    }
    return false;
}
bool TessngDBNet::checkContansLane(QHash<int, GConnector*>& hash, long fromlaneId, long tolaneId) {
    auto it = hash.constBegin();
    while (it != hash.constEnd())
    {
        foreach(auto lane, it.value()->laneConnectors())
        {
            if (fromlaneId == lane->fromLane()->id() &&
                tolaneId == lane->toLane()->id()) return true;
        }
        ++it;
    }
    return false;
}
void TessngDBNet::saveScenesData()
{
    if(nullptr==m_graphicsBaseItem) return;

    QPainterPath pathShape=m_graphicsBaseItem->shape();
    QHash<int, GLink*> hashLink;
    QHash<int, GLink*> allGLinkCatch;
    QList<BusLine*> bsList;
    QList<SignalGroup*> listSignalGroup;
    foreach(GLink * pGLink, gpScene->mlGLink) {
        allGLinkCatch.insert(pGLink->id(), pGLink);
        QPainterPath temp = pGLink->shape();
        if (!temp.intersects(pathShape)) continue;
        hashLink[pGLink->id()] = pGLink;
    }
    QHash<int, GConnector*> hashConnector;
    foreach(GConnector* pConnector, gpScene->mlGConnector) {
        QPainterPath temp = pConnector->shape();
        if (!temp.intersects(pathShape)) continue;
        hashConnector[pConnector->id()] = pConnector;

        if (!hashLink.contains(pConnector->fromLink()->id())) {
            hashLink.insert(pConnector->fromLink()->id(), allGLinkCatch[pConnector->fromLink()->id()]);
        }
        if (!hashLink.contains(pConnector->toLink()->id())) {
            hashLink.insert(pConnector->toLink()->id(), allGLinkCatch[pConnector->toLink()->id()]);
        }
    }

    QMainWindow *pWin = gpTessInterface->guiInterface()->mainWindow();
    if(hashLink.empty()){
        QMessageBox::warning(pWin, tr("Waning"), tr("nothing need to do!"));
        return;
    }
    QString path=QFileDialog::getSaveFileName(pWin,tr("Save File"),"","*.tess");
    if(path.isEmpty()) return;
    bool result = true;
    result = TessngDBCopy::getInstance()->copyDb(path);
    if (!result) return;
    TessngDBCopy::getInstance()->getSqlDatabase()->transaction();
    result =TessngDBCopy::getInstance()->saveConfiguration();
    if (!result) goto exit;
    result =TessngDBCopy::getInstance()->insertNet();
    if (!result) goto exit;
    ///保存路段相关数据
    foreach (auto it, hashLink) {
        result = TessngDBCopy::getInstance()->insertLink(it->mpLink);
        if (!result) goto exit;
    }
    ///保存连接段相关数据
    foreach (auto it, hashConnector) {
        result = TessngDBCopy::getInstance()->insertConnector(it->mpConnector);
        if (!result) goto exit;
    }
    ///保存决策点
    foreach(GDecisionPoint* pGDecisionPoint, gpScene->mlGDecisionPoint){
        if (!hashLink.contains(pGDecisionPoint->link()->id())) continue;
        result = TessngDBCopy::getInstance()->insertDecisionPoint(pGDecisionPoint->mpDecisionPoint);
        if (!result) goto exit;
        foreach(GRouting * pGRouting, pGDecisionPoint->mlGRouting) {
            foreach(OneRouting oneRouting, pGRouting->mlOneRouting) {
                if (oneRouting.mlGLink.size() == 0)
                {
                    break;
                }
                for (int i = 0, size = oneRouting.mlGLink.size(); i < size - 1; ++i) {
                    GLink* pGLink1 = oneRouting.mlGLink.at(i);
                    GLink* pGLink2 = oneRouting.mlGLink.at(i + 1);
                    foreach(GConnector * pGConnector, pGLink1->mlToGConnector)
                    {
                        if (pGConnector->mpToGLink == pGLink2)
                        {
                            QList<LCStruct*> lLC = pGRouting->mhLCStruct.values(pGConnector->mpConnector->connID);
                            foreach(LCStruct * pLc, lLC)
                            {
                                result = TessngDBCopy::getInstance()->insertRoutingLaneConnector(pGRouting->routingID,pGConnector->mpConnector->connID, pLc->fromLaneId, pLc->toLaneId);
                                if (!result) goto exit;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    ///保存导向箭头
    foreach(GGuideArrow* pOne, gpScene->mlGGuideArrow)
    {
        if (!pOne->validate()) continue;
        if (!hashLink.contains(pOne->mpGLane->link()->id())) continue;
        result = TessngDBCopy::getInstance()->insertGuideArrow(pOne->mpGuideArrow);
        if (!result) goto exit;
    }
    
    ///保存信号灯
    foreach(GSignalLamp* pGSl, gpScene->mlGSignalLamp){
        bool flag = false;
        if (0 == pGSl->mpSignalLamp->toLaneID) {
            flag = checkContansLane(hashLink, pGSl->mpSignalLamp->laneID);
        }else {
            flag = checkContansLane(hashConnector, pGSl->mpSignalLamp->laneID, pGSl->mpSignalLamp->toLaneID);
        }
        if (!flag) continue;
        SignalGroup* group = dynamic_cast<SignalGroup*>(pGSl->signalGroup());
        if (nullptr != group && !listSignalGroup.contains(group)) {
            listSignalGroup.append(group);
        }
        result = TessngDBCopy::getInstance()->insertSignalLamp(pGSl->mpSignalLamp);
        if (!result) goto exit;
    }
    ///保存信号灯组
    foreach(SignalGroup* pSg, listSignalGroup) {
        result = TessngDBCopy::getInstance()->insertSignalGroup(pSg);
        if (!result) goto exit;
    }
    ///保存车型
    foreach(const VehicleType& vt, ghVehicleType) {
        result = TessngDBCopy::getInstance()->insertVehicleType(vt);
        if (!result) goto exit;
    }
    foreach(const VehicleComposition& vc, ghVehicleComposition) {
        result = TessngDBCopy::getInstance()->insertVehicleConstitutent(vc);
        if (!result) goto exit;
    }
    ///保存发车点
    foreach(GDeparturePoint* pDp, gpScene->mlGDeparturePoint) {
        if (!hashLink.contains(pDp->link()->id())) continue;
        result = TessngDBCopy::getInstance()->insertDeparturePoint(*pDp->mpDeparturePoint);
        if (!result) goto exit;
    }
    ///保存采集器
    foreach(GVehicleDrivInfoCollector* pGCollecter, gpScene->mlGVehicleDrivInfoCollector) {
        if (!hashLink.contains(pGCollecter->link()->id())) continue;
        result = TessngDBCopy::getInstance()->insertDrivInfoCollector(*pGCollecter->mpVehicleDrivInfoCollector);
        if (!result) goto exit;
    }
    ///保存排队计数器
    foreach(GVehicleQueueCounter* pGQueue, gpScene->mlGVehicleQueueCounter) {
        if (!hashLink.contains(pGQueue->mpGLane->link()->id())) continue;
        result = TessngDBCopy::getInstance()->insertVehicleQueueCounter(pGQueue->mpVehicleQueueCounter);
        if (!result) goto exit;
    }
    ///保存行程时间检测器
    foreach(GVehicleTravelDetector* pOne, gpScene->mlGVehicleTravelDetector){
        if (pOne->mbStarted)continue;
        bool flag = true;
        if (0 == pOne->mpVehicleTravelDetector->start_toLaneNumber) {
            flag = flag && hashLink.contains(pOne->mpVehicleTravelDetector->startRoadId);
        }
        else {
            flag = flag && hashConnector.contains(pOne->mpVehicleTravelDetector->startRoadId);
        }

        if (0 == pOne->mpVehicleTravelDetector->teminal_toLaneNumber) {
            flag = flag && hashLink.contains(pOne->mpVehicleTravelDetector->teminalRoadId);
        }
        else {
            flag = flag && hashConnector.contains(pOne->mpVehicleTravelDetector->teminalRoadId);
        }
        if (!flag) continue;
        result =  TessngDBCopy::getInstance()->insertVehicleTravelDetector(pOne->mpVehicleTravelDetector);
        if (!result) goto exit;
    }
    ///保存限速区
    foreach(GReduceSpeedArea* pOne, gpScene->mlGReduceSpeedArea) {
        bool flag = false;
        if (0 == pOne->mpReduceSpeedArea->toLaneNumber) {
            flag = hashLink.contains(pOne->mpReduceSpeedArea->roadID);
        }
        else {
            flag = hashConnector.contains(pOne->mpReduceSpeedArea->roadID);
        }
        if (!flag) continue;
        result =  TessngDBCopy::getInstance()->insertReduceSpeedArea(pOne->mpReduceSpeedArea);
        if (!result) goto exit;
    }
    ///保存公交站
    foreach(GBusStation* pGBusStation, gpScene->mlGBusStation) {
        if (!hashLink.contains(pGBusStation->link()->id())) continue;
        result = TessngDBCopy::getInstance()->insertBusStation(pGBusStation->mpBusStation);
        if (!result) goto exit;
    }
    ///保存公交线路及相关站点线路，以及乘客到站
    foreach(GBusLine* pGBusLine, gpScene->mlGBusLine) {
        BusLine* bline = pGBusLine->mpBusLine;
        for (int i = 0, size = bline->mlLink.size(); i < size; ++i){
            Link* pLink = bline->mlLink.at(i);
            if (!hashLink.contains(pLink->linkID)) continue;
            if (bsList.contains(bline)) continue;
            bsList.push_back(bline);
        }
    }
    foreach(auto it, bsList) {
        result = TessngDBCopy::getInstance()->insertBusLine(it);
        if (!result) goto exit;
    }
    result = TessngDBCopy::getInstance()->updateIds();
exit:
    result =TessngDBCopy::getInstance()->getSqlDatabase()->commit()&&result;
    if (!result)
    {
        TessngDBCopy::getInstance()->getSqlDatabase()->rollback();
        QMessageBox::warning(pWin, tr("Waning"), tr("save failed!"));
    }
    else {
        QMessageBox::warning(pWin, tr("Notify"), tr("save succed."));
    }
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
