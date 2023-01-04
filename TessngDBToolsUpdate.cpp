#include "TessngDBToolsUpdate.h"
#include "netglobal.h"
#include "graphicsscene.h"
#include <QDebug>
#include "Link.h"
#include "glink.h"
#include "Node.h"
#include "GuideArrow.h"
#include "GGuideArrow.h"
#include "glane.h"
#include "Lane.h"
#include "gconnector.h"
#include "connector.h"
#include "glaneconnector.h"
#include "laneconnector.h"
#include "grouting.h"
#include "Routing.h"
#include "gdecisionpoint.h"
#include "DecisionPoint.h"

#include "GVehicleDetector.h"
#include "VehicleDetector.h"
#include "GReduceSpeedArea.h"
#include "ReduceSpeedArea.h"
#include "GBusStation.h"
#include "BusStation.h"
#include "GBusLine.h"
#include "BusLine.h"
TessngDBToolsUpdate::TessngDBToolsUpdate()
{

}


TessngDBToolsUpdate::~TessngDBToolsUpdate()
{

}
void TessngDBToolsUpdate::gupdateNode(Node* dest,Node* it){
    dest->nodeName=it->nodeName;
    dest->mpVertex->vertexID=it->mpVertex->vertexID;
    dest->mpVertex->x=it->mpVertex->x;
    dest->mpVertex->y=it->mpVertex->y;
    dest->mpVertex->z=it->mpVertex->z;
}
void TessngDBToolsUpdate::gupdateVertex(Vertex* dest,const QList<Vertex*>& src){
    foreach (auto it, src) {
        if(dest->vertexID==it->vertexID){
            dest->x=it->x;
            dest->y=it->y;
            dest->z=it->z;
            break;
        }
    }
}
void TessngDBToolsUpdate::gupdateGuideArrow(GuideArrow* dest,const QList<GuideArrow*>& src){
    foreach (auto it, src) {
        if(dest->guideArrowID==it->guideArrowID){
            /* 车道编号 */
            dest->laneID=it->laneID;
            /*导向箭头长度*/
            dest->length=it->length;
            /*中心点到末端距离 单位米*/
            dest->distToTerminal=it->distToTerminal;

            dest->arrowType=it->arrowType;
            break;
        }
    }
}

void TessngDBToolsUpdate::gupdateLane(Lane* dest,const QList<Lane*>& src){
    foreach (auto it, src) {
        /* 路段ID*/
        dest->linkID=it->linkID;
        /* 车道序号 */
        dest->serialNumber=it->serialNumber;
        /* 车道宽度 */
        dest->width=it->width;
        /* 希望行驶方向，L：左，R：右，A：任意 */
        dest->expectTravelDirection=it->expectTravelDirection;
        /* 行为类型 */
        dest->actionType=it->actionType;
        /* 中心线断点json数据 */
        dest->centerLinePointsJson=it->centerLinePointsJson;
        /* 左侧断点json数据 */
        dest->leftBreakPointsJson=it->leftBreakPointsJson;
        /* 右侧断点json数据 */
        dest->rightBreakPointsJson=it->rightBreakPointsJson;
        /* 其它属性json数据 */
        dest->otherAttrsJson=it->otherAttrsJson;
        /*导向箭头*/
        foreach (auto itt, dest->mlGuideArrow) {
            gupdateGuideArrow(itt,it->mlGuideArrow);
        }
        /*限制车型*/
        dest->mlVehicleType=it->mlVehicleType;
    }
}
///update link
Link* TessngDBToolsUpdate::updateLink(const Link& link){
    bool result=updateLinkPtr(const_cast<Link*>(&link));
    if(!result) return nullptr;
    GLink* gLink=gpScene->findGLinkByLinkID(link.linkID);

    assert(nullptr!=gLink);


    gLink->mpLink->linkName=link.linkName;
    /*路网ID*/
    gLink->mpLink->netId=link.netId;
    /*道路ID*/
    gLink->mpLink->roadId=link.roadId;
    /* 车道数 */
    gLink->mpLink->laneNumber=link.laneNumber;
    /* 车道宽度 */
    gLink->mpLink->laneWidth=link.laneWidth;
    /* 车道颜色 */
    gLink->mpLink->laneColor=link.laneColor;
    /* 道路类型 */
    gLink->mpLink->linkType=link.linkType;
    /* 路段长度 */
    gLink->mpLink->length=link.length;
    /* 曲率 */
    gLink->mpLink->curvature=link.curvature;
    /* 道路非直线系数 */
    gLink->mpLink->nonLinearCoefficient=link.nonLinearCoefficient;
    /* 饱和流率 */
    gLink->mpLink->linkSaturationFlow=link.linkSaturationFlow;
    /* 路段流量 */
    gLink->mpLink->linkTrafficFlow=link.linkTrafficFlow;
    /* 期望速度(自由流速度) 千米/小时*/
    gLink->mpLink->desiredSpeed=link.desiredSpeed;
    /* 限速 千米/小时*/
    gLink->mpLink->limitSpeed=link.limitSpeed;
    /* 最小速度 千米/小时*/
    gLink->mpLink->minSpeed=link.minSpeed;
    /* 附加值 */
    gLink->mpLink->addValue=link.addValue;
    /* 中心线断点json数组 */
    gLink->mpLink->centerLinePointsJson=link.centerLinePointsJson;
    /* 左侧断点json数据 */
    gLink->mpLink->leftBreakPointsJson=link.leftBreakPointsJson;
    /* 右侧断点json数据 */
    gLink->mpLink->rightBreakPointsJson=link.rightBreakPointsJson;
    /* 其它属性json数据 */
    gLink->mpLink->otherAttrsJson=link.otherAttrsJson;

    gupdateNode(gLink->mpLink->startNode,link.startNode);
    gupdateNode(gLink->mpLink->endNode,link.endNode);

    foreach(auto it, gLink->mpLink->mlVertex) {
        gupdateVertex(it,link.mlVertex);
    }
    foreach(auto it, gLink->mpLink->mlLane) {
        gupdateLane(it,link.mlLane);
    }
    return gLink->mpLink;
}

///update lane
bool TessngDBToolsUpdate::updateLane(const Lane& lane){
    bool result=updateLanePtr(const_cast<Lane*>(&lane));
    if(!result) return false;
    GLane* glane=gpScene->findGLaneByLaneID(lane.laneID);

    assert(nullptr!=glane);
    /* 路段ID*/
    glane->mpLane->linkID=lane.linkID;
    /* 车道序号 */
    glane->mpLane->serialNumber=lane.serialNumber;
    /* 车道宽度 */
    glane->mpLane->width=lane.width;
    /* 希望行驶方向，L：左，R：右，A：任意 */
    glane->mpLane->expectTravelDirection=lane.expectTravelDirection;
    /* 行为类型 */
    glane->mpLane->actionType=lane.actionType;
    /* 中心线断点json数据 */
    glane->mpLane->centerLinePointsJson=lane.centerLinePointsJson;
    /* 左侧断点json数据 */
    glane->mpLane->leftBreakPointsJson=lane.leftBreakPointsJson;
    /* 右侧断点json数据 */
    glane->mpLane->rightBreakPointsJson=lane.rightBreakPointsJson;
    /* 其它属性json数据 */
    glane->mpLane->otherAttrsJson=lane.otherAttrsJson;

    /*导向箭头*/
    foreach (auto it, glane->mpLane->mlGuideArrow) {
        gupdateGuideArrow(it,lane.mlGuideArrow);
    }
    /*限制车型*/
    glane->mpLane->mlVehicleType=lane.mlVehicleType;
    return true;
}

///updateGuide Arrow
bool TessngDBToolsUpdate::updateGuideArrow(const GuideArrow& arrow){
    bool result=updateGuideArrowByTransPtr(const_cast<GuideArrow*>(&arrow));
    if(!result) return false;
    foreach (auto it, gpScene->mlGGuideArrow) {
        if(it->mpGuideArrow->guideArrowID==arrow.guideArrowID){
            /* 车道编号 */
            it->mpGuideArrow->laneID=arrow.laneID;
            /*导向箭头长度*/
            it->mpGuideArrow->length=arrow.length;
            /*中心点到末端距离 单位米*/
            it->mpGuideArrow->distToTerminal=arrow.distToTerminal;

            it->mpGuideArrow->arrowType=arrow.arrowType;
            break;
        }
    }
    return true;
}
bool TessngDBToolsUpdate::updateLaneConnector(const LaneConnector& laneConn) {
    bool result = updateLaneConnectorPtr(const_cast<LaneConnector*>(&laneConn));
    if (!result) return false;
    GLaneConnector* glaneConn = gpScene->findGLaneConnectorByID(laneConn.mpFromLane->laneID,laneConn.mpToLane->laneID);
    assert(nullptr != glaneConn);
    if (glaneConn->mpLaneConnector->mpConnector->connID != laneConn.laneConnectorID) return true;

    updateLane(*laneConn.mpFromLane);
    updateLane(*laneConn.mpToLane);

    glaneConn->mpLaneConnector->mrLength= laneConn.mrLength;
    /* 权重 */
    glaneConn->mpLaneConnector->weight = laneConn.weight;
    /* 中心线断点json数据 */
    glaneConn->mpLaneConnector->centerLinePointsJson = laneConn.centerLinePointsJson;
    /* 左侧断点json数据 */
    glaneConn->mpLaneConnector->leftBreakPointsJson = laneConn.leftBreakPointsJson;
    /* 右侧断点json数据 */
    glaneConn->mpLaneConnector->rightBreakPointsJson = laneConn.rightBreakPointsJson;
    /* 其它属性json数据 */
    glaneConn->mpLaneConnector->otherAttrsJson = laneConn.otherAttrsJson;


    return true;
}
///update Connector
bool TessngDBToolsUpdate::updateConnector(const Connector& conn){
    bool result = updateConnectorsPtr(const_cast<Connector*>(&conn));
    if (!result) return false;
    GConnector* gConn = gpScene->findGConnectorByConnID(conn.connID);

    assert(nullptr != gConn);

    /* 道路ID */
    gConn->mpConnector->roadId=conn.roadId;
    /* 连接段名称 */
    gConn->mpConnector->connName = conn.connName;
    /* 连接段长度 */
    gConn->mpConnector->length = conn.length;
    /* 曲率 */
    gConn->mpConnector->curvature = conn.curvature;
    /* 连接段非直线系数 */
    gConn->mpConnector->nonLinearCoefficient = conn.nonLinearCoefficient;
    /* 路面颜色 */
    gConn->mpConnector->color = conn.color;
    /* 期望速度 */
    gConn->mpConnector->desiredSpeed = conn.desiredSpeed;
    /* 限速 */
    gConn->mpConnector->limitSpeed = conn.limitSpeed;
    /* 左侧断点json数据 */
    gConn->mpConnector->leftBreakPointsJson = conn.leftBreakPointsJson;
    /* 右侧断点json数据 */
    gConn->mpConnector->rightBreakPointsJson = conn.rightBreakPointsJson;
    /* 其它属性json数据 */
    gConn->mpConnector->otherAttrsJson = conn.otherAttrsJson;

    /* 起始路段 */
    updateLink(*gConn->mpConnector->mpFromLink);
    /* 目标路段 */
    updateLink(*gConn->mpConnector->mpToLink);

    /* 车道连接集*/
    foreach(auto it, conn.mlLaneConnector) {
        updateLaneConnector(*it);
    }
    
    /*限制车型*/
    gConn->mpConnector->mlVehicleType=conn.mlVehicleType;

    return true;
}
bool TessngDBToolsUpdate::findRoutingLink(int& m, int& n, int id, const QList<QList<Link*>>& list) {
    for(int i=0;i< list.size();i++)
    {
        m = i;
        for(int j = 0; j < list[i].size();j++) {
            if (id == list[i][j]->linkID) {
                n = j;
                return true;
            }
        }
    }
    return false;
}
///update Routeing
bool TessngDBToolsUpdate::updateRouteing(const Routing& rt){
    bool result = updateRouteingPtr(const_cast<Routing*>(&rt));
    if (!result) return false;
    GRouting* gRt = gpScene->findGRoutingByID(rt.routingID);

    assert(nullptr != gRt);

    gRt->getRouting()->routingName=rt.routingName;
    /* 流量比 */
    gRt->getRouting()->proportion=rt.proportion;

    //一条决策路径可以由多个最短路径组成
    QList<QPair<int, int>> rmList;
    for (int i = 0; i < gRt->getRouting()->mllLink.size();i++) {
        for (int j=0;j< gRt->getRouting()->mllLink[i].size(); j++)
        {
            int m, n;
            if (findRoutingLink(m, n, gRt->getRouting()->mllLink[i][j]->linkID, rt.mllLink)) {
                Link* link = updateLink(*rt.mllLink[m][n]);
                if (nullptr != link) {
                    gRt->getRouting()->mllLink[i][j] = link;
                }
            }
            else {
                rmList.append(QPair<int,int>(i,j));
            }
        }
    }
    while (!rmList.empty())
    {
        QPair<int, int> pair=rmList.takeFirst();
        gRt->getRouting()->mllLink[pair.first].removeAt(pair.second);
    }
    return true;
}

///update DecisionPoint
bool TessngDBToolsUpdate::updateDecisionPoint(const DecisionPoint& dp){
    bool result = updateDecisionPointPtr(const_cast<DecisionPoint*>(&dp));
    if (!result) return false;
    GDecisionPoint* gdP = gpScene->findGDecisionPointByID(dp.deciPointID);

    assert(nullptr != gdP);
   
    gdP->mpDecisionPoint->deciPointName=dp.deciPointID;
    /* 坐标X */
    gdP->mpDecisionPoint->X = dp.X;
    /* 坐标Y */
    gdP->mpDecisionPoint->Y = dp.Y;
    /* 高程 */
    gdP->mpDecisionPoint->Z = dp.Z;

    gdP->mpDecisionPoint->mFromPoint = dp.mFromPoint;
    gdP->mpDecisionPoint->mToPoint = dp.mToPoint;

    QList<Routing*> mlRouting;
    Link* mpLink;

    QList<RoutingFlowByInterval*> mlRoutingFlowByInterval;


    return true;
}
///update DeparturePoint
bool TessngDBToolsUpdate::updateDeparturePoint(const DeparturePoint&){
    return false;
}

///update signalGroup
bool TessngDBToolsUpdate::updateSignalGroup(const SignalGroup& ){
    return false;
}

///update SignalLamp
bool TessngDBToolsUpdate::updateSignalLamp(const SignalLamp& pSignalLamp){
    return false;
}

///update VehicleDrivInfoCollector
bool TessngDBToolsUpdate::updateVehicleDrivInfoCollector(const VehicleDrivInfoCollector&){
    return false;
}

///update VehicleQueueCounter
bool TessngDBToolsUpdate::updateVehicleQueueCounter(const VehicleQueueCounter&){
    return false;
}

///update VehicleTravelDetector
bool TessngDBToolsUpdate::updateVehicleTravelDetector(const VehicleTravelDetector&){
    return false;
}
///update VehicleDetector
bool TessngDBToolsUpdate::updateVehicleDetector(const VehicleDetector& vehicleDetector){
    return false;
}

///update ReduceSpeedArea
bool TessngDBToolsUpdate::updateReduceSpeedArea(const ReduceSpeedArea& reduceSpeedArea){
    bool result = updateReduceSpeedAreaPtr(const_cast<ReduceSpeedArea*>(&reduceSpeedArea));
    if (!result) return false;

    GReduceSpeedArea* gReduceSpeedArea = nullptr;
    foreach(GReduceSpeedArea * it, gpScene->mlGReduceSpeedArea) {
        if (reduceSpeedArea.reduceSpeedAreaID == it->mpReduceSpeedArea->reduceSpeedAreaID) gReduceSpeedArea = it;
    }
    assert(nullptr != gReduceSpeedArea);

    gReduceSpeedArea->mpReduceSpeedArea->name = reduceSpeedArea.name;
    gReduceSpeedArea->mpReduceSpeedArea->location = reduceSpeedArea.location;
    gReduceSpeedArea->mpReduceSpeedArea->areaLength = reduceSpeedArea.areaLength;
    gReduceSpeedArea->mpReduceSpeedArea->roadID = reduceSpeedArea.roadID;
    gReduceSpeedArea->mpReduceSpeedArea->laneNumber = reduceSpeedArea.laneNumber;
    gReduceSpeedArea->mpReduceSpeedArea->toLaneNumber = reduceSpeedArea.toLaneNumber;
    /*gReduceSpeedArea->mpReduceSpeedArea->mlReduceSpeedInterval = reduceSpeedArea.mlReduceSpeedInterval;
    gReduceSpeedArea->mpReduceSpeedArea->mlReduceSpeedVehiType = reduceSpeedArea.mlReduceSpeedVehiType;*/

    return true;
}

///update BusStation
bool TessngDBToolsUpdate::updateBustation(const BusStation& busStation){
    bool result = updateBustationPtr(const_cast<BusStation*>(&busStation));
    if (!result) return false;

    GBusStation* gBusStation = gpScene->findGBusStationByID(busStation.busStationID);
    assert(nullptr != gBusStation);

    gBusStation->mpBusStation->busStationID = busStation.busStationID;
    gBusStation->mpBusStation->name = busStation.name;
    gBusStation->mpBusStation->laneNumber = busStation.laneNumber;
    gBusStation->mpBusStation->x = busStation.x;
    gBusStation->mpBusStation->y = busStation.y;
    gBusStation->mpBusStation->length = busStation.length;
    gBusStation->mpBusStation->type = busStation.type;
    //gBusStation->mpBusStation->mlBusStationLine = busStation.mlBusStationLine;
    gBusStation->mpBusStation->mpLink = busStation.mpLink;

    return true;
}

///update BusLine
bool TessngDBToolsUpdate::updateBusLine(const BusLine& busLine){
    bool result = updateBusLinePtr(const_cast<BusLine*>(&busLine));
    if (!result) return false;

    GBusLine* gBusLine = gpScene->findGBusLineByID(busLine.busLineID);
    assert(nullptr != gBusLine);

    gBusLine->mpBusLine->name = busLine.name;
    gBusLine->mpBusLine->length = busLine.length;
    gBusLine->mpBusLine->dischargeFreq = busLine.dischargeFreq;
    gBusLine->mpBusLine->dischargeStartTime = busLine.dischargeStartTime;
    gBusLine->mpBusLine->dischargeEndTime = busLine.dischargeEndTime;
    gBusLine->mpBusLine->startX = busLine.startX;
    gBusLine->mpBusLine->startY = busLine.startY;
    gBusLine->mpBusLine->endX = busLine.endX;
    gBusLine->mpBusLine->endY = busLine.endY;
    gBusLine->mpBusLine->desirSpeed = busLine.desirSpeed;
    gBusLine->mpBusLine->speedSD = busLine.speedSD;
    gBusLine->mpBusLine->passCountAtStartTime = busLine.passCountAtStartTime;
    /*gBusLine->mpBusLine->mlLink = busLine.mlLink;
    gBusLine->mpBusLine->mlLinkId = busLine.mlLinkId;
    gBusLine->mpBusLine->mlBusStationLine = busLine.mlBusStationLine;*/
    gBusLine->mpBusLine->mSpecialApp = busLine.mSpecialApp;

    return true;
}
