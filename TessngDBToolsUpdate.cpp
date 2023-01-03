#include "TessngDBToolsUpdate.h"
#include "netglobal.h"
#include "graphicsscene.h"
#include <QDebug>
#include "Link.h"
#include "glink.h"
#include "Node.h"
#include "GuideArrow.h"
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
bool TessngDBToolsUpdate::updateLink(const Link& link){
    bool result=updateLinkPtr(const_cast<Link*>(&link));
    if(!result) return false;
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
    return true;
}

///update lane
bool TessngDBToolsUpdate::updateLane(const Lane& lane){
    return false;
}

///updateGuide Arrow
bool TessngDBToolsUpdate::updateGuideArrow(const GuideArrow& arrow){
    return false;
}

///update Connector
bool TessngDBToolsUpdate::updateConnector(const Connector&){
    return false;
}

///update Routeing
bool TessngDBToolsUpdate::updateRouteing(const Routing&){
    return false;
}

///update DecisionPoint
bool TessngDBToolsUpdate::updateDecisionPoint(const DecisionPoint&){
    return false;
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
bool TessngDBToolsUpdate::updateVehicleDetector(const VehicleDetector&){
    return false;
}

///update ReduceSpeedArea
bool TessngDBToolsUpdate::updateReduceSpeedArea(const ReduceSpeedArea&){
    return false;
}

///update BusStation
bool TessngDBToolsUpdate::updateBustation(const BusStation&){
    return false;
}

///update BusLine
bool TessngDBToolsUpdate::updateBusLine(const BusLine&){
    return false;
}
