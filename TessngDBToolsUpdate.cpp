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
#include "ReduceSpeedInterval.h"
#include "ReduceSpeedVehiType.h"
#include "GBusStation.h"
#include "BusStation.h"
#include "GBusLine.h"
#include "BusLine.h"
#include "RoutingFLowRatio.h"
#include "gdeparturepoint.h"
#include "DeparturePoint.h"
#include "signalgroup.h"
#include "signallamp.h"
#include "gsignallamp.h"
#include "gvehicledrivinfocollector.h"
#include "VehicleDrivInfoCollector.h"
#include "gvehiclequeuecounter.h"
#include "VehicleQueueCounter.h"
#include "VehicleTravelDetector.h"
#include "gvehicletraveldetector.h"
#include "VehicleDetector.h"
#include "GVehicleDetector.h"
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
Routing* TessngDBToolsUpdate::updateRouteing(const Routing& rt){
    bool result = updateRouteingPtr(const_cast<Routing*>(&rt));
    if (!result) return nullptr;
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
    return gRt->getRouting();
}
void TessngDBToolsUpdate::gupdateRoutingFlowByInterval(QList<RoutingFLowRatio*>& dest, const QList<RoutingFLowRatio*>& src) {
    bool result = false;
    for (int i = 0; i < dest.size(); i++) {
        for (int j = 0; j < src.size(); j++) {
            if (dest[i]->RoutingFLowRatioID != src[j]->RoutingFLowRatioID) continue;

            dest[i]->routingID=src[j]->routingID;
            dest[i]->startDateTime = src[j]->startDateTime;
            dest[i]->endDateTime = src[j]->endDateTime;
            dest[i]->ratio = src[j]->ratio;
            dest[i]->vehiCount = src[j]->vehiCount;
            break;
        }
    }
}
///update DecisionPoint
bool TessngDBToolsUpdate::updateDecisionPoint(const DecisionPoint& dp){
    bool result = updateDecisionPointPtr(const_cast<DecisionPoint*>(&dp));
    if (!result) return false;
    GDecisionPoint* gdP = gpScene->findGDecisionPointByID(dp.deciPointID);

    assert(nullptr != gdP);
   
    gdP->mpDecisionPoint->deciPointName=dp.deciPointName;
    /* 坐标X */
    gdP->mpDecisionPoint->X = dp.X;
    /* 坐标Y */
    gdP->mpDecisionPoint->Y = dp.Y;
    /* 高程 */
    gdP->mpDecisionPoint->Z = dp.Z;

    gdP->mpDecisionPoint->mFromPoint = dp.mFromPoint;
    gdP->mpDecisionPoint->mToPoint = dp.mToPoint;

    for (int i = 0; i < gdP->mpDecisionPoint->mlRouting.size();i++)
    {
        for (int j = 0; j < dp.mlRouting.size();j++) {
            if (gdP->mpDecisionPoint->mlRouting[i]->routingID != dp.mlRouting[j]->routingID) continue;

            Routing*  rt=updateRouteing(*dp.mlRouting[j]);
            if(nullptr!=rt) gdP->mpDecisionPoint->mlRouting[i] = rt;
        }
    }


    for (int i = 0; i < gdP->mpDecisionPoint->mlRoutingFlowByInterval.size(); i++) {
        for (int j = 0; j < dp.mlRoutingFlowByInterval.size(); j++) {

            if (gdP->mpDecisionPoint->mlRoutingFlowByInterval[i]->endDateTime != dp.mlRoutingFlowByInterval[j]->endDateTime) continue;
            gupdateRoutingFlowByInterval(gdP->mpDecisionPoint->mlRoutingFlowByInterval[i]->mlRoutingFlowRatio, dp.mlRoutingFlowByInterval[j]->mlRoutingFlowRatio);
        }
    }
    return true;
}
///update DeparturePoint
bool TessngDBToolsUpdate::updateDeparturePoint(const DeparturePoint& dp){
    bool result = updateDeparturePointPtr(const_cast<DeparturePoint*>(&dp));
    if (!result) return false;
    GDeparturePoint* gdP = gpScene->findGDeparturePointByID(dp.departurePointID);

    assert(nullptr != gdP);
    
    /*发车点名称*/
    gdP->mpDeparturePoint->name=dp.name;

    gdP->mpDeparturePoint->mlDepaInterval=dp.mlDepaInterval;
    gdP->mpDeparturePoint->mlDepaPossion=dp.mlDepaPossion;

    /*路段*/
    Link* mpLink;
    return true;
}

///update signalGroup
bool TessngDBToolsUpdate::updateSignalGroup(const SignalGroup& sg){
    bool result = updateSignalGroupPtr(const_cast<SignalGroup*>(&sg));
    if (!result) return false;
    SignalGroup* gdP = gpScene->findSignalGroupById(sg.signalGroupID);

    assert(nullptr != gdP);

    /* 名称 */
    gdP->name=sg.name;
    /* 时长(秒) */
    gdP->timeLength = sg.timeLength;
    /* 起始时间，单位：秒 */
    gdP->startTime = sg.startTime;
    /* 结束时间 单位：秒 */
    gdP->endTime = sg.endTime;

    QList<SignalPhase*> mlPhase;
    return true;
}

///update SignalLamp
bool TessngDBToolsUpdate::updateSignalLamp(const SignalLamp& pSignalLamp){
    bool result = updateSignalLampPtr(const_cast<SignalLamp*>(&pSignalLamp));
    if (!result) return false;

    GSignalLamp* gdP = nullptr;
    foreach(auto it, gpScene->mlGSignalLamp) {
        if (it->mpSignalLamp->signalLampID != pSignalLamp.signalLampID) continue;
        gdP = it;
        break;
    }
    assert(nullptr != gdP);

    gdP->mpSignalLamp->name= pSignalLamp.name;

    /* 车道*/
    gdP->mpSignalLamp->laneID = pSignalLamp.laneID;
    /* 目标车道ID， 如果不在连接连接上， 目标车道ID为0 */
    gdP->mpSignalLamp->toLaneID = pSignalLamp.toLaneID;

    gdP->mpSignalLamp->x = pSignalLamp.x;
    gdP->mpSignalLamp->y = pSignalLamp.y;
    gdP->mpSignalLamp->z = pSignalLamp.z;

    SignalPhase* mpSignalPhase;

    return true;
}

///update VehicleDrivInfoCollector
bool TessngDBToolsUpdate::updateVehicleDrivInfoCollector(const VehicleDrivInfoCollector& vdc){
    bool result = updateVehicleDrivInfoCollectorPtr(const_cast<VehicleDrivInfoCollector*>(&vdc));
    if (!result) return false;

    GVehicleDrivInfoCollector* gdP = gpScene->findGVehicleDrivInfoCollectorByID(vdc.collecterID);

    assert(nullptr != gdP);


    gdP->mpVehicleDrivInfoCollector->name=vdc.name;
    gdP->mpVehicleDrivInfoCollector->roadID = vdc.roadID;
    gdP->mpVehicleDrivInfoCollector->laneNumber = vdc.laneNumber;
    gdP->mpVehicleDrivInfoCollector->toLaneNumber = vdc.toLaneNumber;
    gdP->mpVehicleDrivInfoCollector->distance = vdc.distance;
    gdP->mpVehicleDrivInfoCollector->x = vdc.x;
    gdP->mpVehicleDrivInfoCollector->y = vdc.y;
    gdP->mpVehicleDrivInfoCollector->z = vdc.z;
    gdP->mpVehicleDrivInfoCollector->startTime = vdc.startTime;
    gdP->mpVehicleDrivInfoCollector->endTime = vdc.endTime;
    gdP->mpVehicleDrivInfoCollector->dataInterval = vdc.dataInterval;
    return true;
}

///update VehicleQueueCounter
bool TessngDBToolsUpdate::updateVehicleQueueCounter(const VehicleQueueCounter& vqc){
    bool result = updateVehicleQueueCounterPtr(const_cast<VehicleQueueCounter*>(&vqc));
    if (!result) return false;

    GVehicleQueueCounter* gdP = gpScene->findGVehicleQueueCounterByID(vqc.queueCounterID);

    assert(nullptr != gdP);

    /* 计数器名称 */
    gdP->mpVehicleQueueCounter->name=vqc.name;
    /* 道路(路段或连接段)ID */
    gdP->mpVehicleQueueCounter->roadID = vqc.roadID;
    /* 采集器所在车道序号 */
    gdP->mpVehicleQueueCounter->laneNumber = vqc.laneNumber;
    /* 目标车道*/
    gdP->mpVehicleQueueCounter->toLaneNumber = vqc.toLaneNumber;
    gdP->mpVehicleQueueCounter->x = vqc.x;
    /* 位置Y */
    gdP->mpVehicleQueueCounter->y = vqc.y;
    /* 高程*/
    gdP->mpVehicleQueueCounter->z = vqc.z;
    /* 速度下限 */
    gdP->mpVehicleQueueCounter->speedLowLimit = vqc.speedLowLimit;
    /* 速度上限 */
    gdP->mpVehicleQueueCounter->speedUpLimit = vqc.speedUpLimit;
    /* 最大车辆间距 */
    gdP->mpVehicleQueueCounter->maxDistInterval = vqc.maxDistInterval;
    /* 最大排队长度 */
    gdP->mpVehicleQueueCounter->maxQueueLength = vqc.maxQueueLength;
    /* 距起点距离 */
    gdP->mpVehicleQueueCounter->distance = vqc.distance;
    /* 起始时间 */
    gdP->mpVehicleQueueCounter->startTime = vqc.startTime;
    /* 结束时间 */
    gdP->mpVehicleQueueCounter->endTime = vqc.endTime;
    /* 数据间隔*/
    gdP->mpVehicleQueueCounter->dataInterval = vqc.dataInterval;
    /* 采集间隔*/
    gdP->mpVehicleQueueCounter->countInterval = vqc.countInterval;

    return true;
}

///update VehicleTravelDetector
bool TessngDBToolsUpdate::updateVehicleTravelDetector(const VehicleTravelDetector& vtd){
    bool result = updateVehicleTravelDetectorPtr(const_cast<VehicleTravelDetector*>(&vtd));
    if (!result) return false;

    QList<GVehicleTravelDetector*> list = gpScene->findGVehicleTravelDetectorById(vtd.detectorId);
    GVehicleTravelDetector* gdP = nullptr;
    foreach(auto it, list) {
        if (it->mpVehicleTravelDetector->detectorId == vtd.detectorId) continue;

        gdP = it;
        break;
    }
    if (nullptr == gdP) return true;
    /* 检测器名称 */
    gdP->mpVehicleTravelDetector->name=vtd.name;
    /* 起始路段编号 */
    gdP->mpVehicleTravelDetector->startRoadId = vtd.startRoadId;
    /* 起始车道连接的起始车道 */
    gdP->mpVehicleTravelDetector->start_laneNumber = vtd.start_laneNumber;
    /* 起始车道连接的目标车道 */
    gdP->mpVehicleTravelDetector->start_toLaneNumber = vtd.start_toLaneNumber;
    /* 起始路段距离 */
    gdP->mpVehicleTravelDetector->startDist = vtd.startDist;
    /* 起始位置X */
    gdP->mpVehicleTravelDetector->startX = vtd.startX;
    /* 起始位置Y */
    gdP->mpVehicleTravelDetector->startY = vtd.startY;
    /* 终点路段编号 */
    gdP->mpVehicleTravelDetector->teminalRoadId = vtd.teminalRoadId;
    /* 终点车道连接的起始车道 */
    gdP->mpVehicleTravelDetector->teminal_laneNumber = vtd.teminal_laneNumber;
    /* 终点车道连接的目标车道 */
    gdP->mpVehicleTravelDetector->teminal_toLaneNumber = vtd.teminal_toLaneNumber;
    /* 终点路段距离 */
    gdP->mpVehicleTravelDetector->teminalDist = vtd.teminalDist;
    /* 终点位置X */
    gdP->mpVehicleTravelDetector->teminalX = vtd.teminalX;
    /* 终点位置Y */
    gdP->mpVehicleTravelDetector->teminalY = vtd.teminalY;
    /* 仿真起始时间 */
    gdP->mpVehicleTravelDetector->startTime = vtd.startTime;
    /* 仿真结束时间 */
    gdP->mpVehicleTravelDetector->endTime = vtd.endTime;
    /* 数据间隔 */
    gdP->mpVehicleTravelDetector->dataInterval = vtd.dataInterval;
    /* 最短距离 */
    gdP->mpVehicleTravelDetector->shortestDistance = vtd.shortestDistance;
    return true;
}
///update VehicleDetector
bool TessngDBToolsUpdate::updateVehicleDetector(const VehicleDetector& vd){
    bool result = updateVehicleDetectorPtr(const_cast<VehicleDetector*>(&vd));
    if (!result) return false;
    GVehicleDetector* gdP = nullptr;
    foreach(auto it, gpScene->mlGVehicleDetector) {
        if (it->mpVehicleDetector->vehicleDetectorId != vd.vehicleDetectorId) continue;
        gdP = it;
        break;
    }
    if (nullptr == gdP) return true;

    /* 检测器名 */
    gdP->mpVehicleDetector->name=vd.name;
    /* 信号灯组ID */
    gdP->mpVehicleDetector->signalGroupID = vd.signalGroupID;
    /* 相位序号 */
    gdP->mpVehicleDetector->phaseNumber = vd.phaseNumber;
    /* 道路ID */
    gdP->mpVehicleDetector->roadId = vd.roadId;
    /* 道路类型 */
    gdP->mpVehicleDetector->roadType = vd.roadType;
    /* 车道序号 */
    gdP->mpVehicleDetector->laneNumber = vd.laneNumber;
    /* 目标车道序号 */
    gdP->mpVehicleDetector->toLaneNumber = vd.toLaneNumber;
    /* 到终点距离 */
    gdP->mpVehicleDetector->distToTerminal = vd.distToTerminal;
    /* 长度 */
    gdP->mpVehicleDetector->length = vd.length;
    /*检测器类型*/
    gdP->mpVehicleDetector->type = vd.type;
    /* 最大绿时 */
    gdP->mpVehicleDetector->maxGreen = vd.maxGreen;

    gdP->mpVehicleDetector->mlVehicleType = vd.mlVehicleType;

    return true;
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
    gReduceSpeedArea->mpReduceSpeedArea->mlReduceSpeedVehiType = reduceSpeedArea.mlReduceSpeedVehiType;
    foreach(auto newItem, reduceSpeedArea.mlReduceSpeedInterval) {
        foreach(auto oldItem, gReduceSpeedArea->mpReduceSpeedArea->mlReduceSpeedInterval) {
            if (newItem->intervalID == oldItem->intervalID && newItem->reduceSpeedAreaID == oldItem->reduceSpeedAreaID) {
                oldItem->startTime = newItem->startTime;
                oldItem->endTime = newItem->endTime;
            }
        }
    }
    foreach(auto newItem, reduceSpeedArea.mlReduceSpeedVehiType) {
        foreach(auto oldItem, gReduceSpeedArea->mpReduceSpeedArea->mlReduceSpeedVehiType) {
            if (newItem->vehicleTypeCode == oldItem->vehicleTypeCode && newItem->reduceSpeedAreaID == oldItem->reduceSpeedAreaID) {
                oldItem->avgSpeed = newItem->avgSpeed;
                oldItem->speedSD = newItem->speedSD;
            }
        }
    }

    return true;
}

///update BusStation: BusStationLine仅能对已有元素进行更新
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
    gBusStation->mpBusStation->mlBusStationLine = busStation.mlBusStationLine;
    foreach(auto newItem, busStation.mlBusStationLine) {
        foreach(auto oldItem, gBusStation->mpBusStation->mlBusStationLine) {
            if (newItem->id() == oldItem->id()) {
                oldItem->busStationID = newItem->busStationID;
                oldItem->linkID = newItem->linkID;
                oldItem->busLineID = newItem->busLineID;
                oldItem->parkingTime = newItem->parkingTime;
                oldItem->leavingPercent = newItem->leavingPercent;
                oldItem->getOnTimePerson = newItem->getOnTimePerson;
                oldItem->getOutTimePerson = newItem->getOutTimePerson;
                oldItem->mlPassengerArriving = newItem->mlPassengerArriving;
                foreach(auto newArriving, newItem->mlPassengerArriving) {
                    foreach(auto oldArriving, oldItem->mlPassengerArriving) {
                        if (newArriving->passengerArrivingID == oldArriving->passengerArrivingID) {
                            oldArriving->startTime = newArriving->startTime;
                            oldArriving->endTime = newArriving->endTime;
                            oldArriving->passengerCount = newArriving->passengerCount;
                        }
                    }
                }
                oldItem->mlPassenger = newItem->mlPassenger;
                oldItem->mlPassengerLeaved = newItem->mlPassengerLeaved;
            }
        }
    }
    gBusStation->mpBusStation->mpLink = busStation.mpLink;

    return true;
}

///update BusLine: 可对BusLineRoads进行新增删除级别的更新，其他如BusStationLine仅能对已有元素进行更新
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
    gBusLine->mpBusLine->mlLink = busLine.mlLink;
    gBusLine->mpBusLine->mlLinkId = busLine.mlLinkId;
    gBusLine->mpBusLine->mlBusStationLine = busLine.mlBusStationLine;
    foreach(auto newItem, busLine.mlBusStationLine) {
        foreach(auto oldItem, gBusLine->mpBusLine->mlBusStationLine) {
            if (newItem->id() == oldItem->id()) {
                oldItem->busStationID = newItem->busStationID;
                oldItem->linkID = newItem->linkID;
                oldItem->busLineID = newItem->busLineID;
                oldItem->parkingTime = newItem->parkingTime;
                oldItem->leavingPercent = newItem->leavingPercent;
                oldItem->getOnTimePerson = newItem->getOnTimePerson;
                oldItem->getOutTimePerson = newItem->getOutTimePerson;
                oldItem->mlPassengerArriving = newItem->mlPassengerArriving;
                foreach(auto newArriving, newItem->mlPassengerArriving) {
                    foreach(auto oldArriving, oldItem->mlPassengerArriving) {
                        if (newArriving->passengerArrivingID == oldArriving->passengerArrivingID) {
                            oldArriving->startTime = newArriving->startTime;
                            oldArriving->endTime = newArriving->endTime;
                            oldArriving->passengerCount = newArriving->passengerCount;
                        }
                    }
                }
                oldItem->mlPassenger = newItem->mlPassenger;
                oldItem->mlPassengerLeaved = newItem->mlPassengerLeaved;
            }
        }
    }
    gBusLine->mpBusLine->mSpecialApp = busLine.mSpecialApp;

    return true;
}
