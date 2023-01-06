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
#include "PassengerArriving.h"
TessngDBToolsUpdate::TessngDBToolsUpdate()
{
}


TessngDBToolsUpdate::~TessngDBToolsUpdate()
{

}
void TessngDBToolsUpdate::gupdateNode(Node* dest, Node* it) {
	if (!it->nodeName.isEmpty()) dest->nodeName = it->nodeName;
	if (it->mpVertex->vertexID > 0) dest->mpVertex->vertexID = it->mpVertex->vertexID;
	if (0 == _isnan(it->mpVertex->x))dest->mpVertex->x = it->mpVertex->x;
	if (0 == _isnan(it->mpVertex->y))dest->mpVertex->y = it->mpVertex->y;
	if (0 == _isnan(it->mpVertex->z))dest->mpVertex->z = it->mpVertex->z;
}
void TessngDBToolsUpdate::gupdateVertex(Vertex* dest, const QList<Vertex*>& src) {
	foreach(auto it, src) {
		if (dest->vertexID == it->vertexID) {
			if (0 == _isnan(it->x))dest->x = it->x;
			if (0 == _isnan(it->y))dest->y = it->y;
			if (0 == _isnan(it->z))dest->z = it->z;
			break;
		}
	}
}
void TessngDBToolsUpdate::gupdateGuideArrow(GuideArrow* dest, const QList<GuideArrow*>& src) {
	foreach(auto it, src) {
		if (dest->guideArrowID == it->guideArrowID) {
			/* 车道编号 */
			if (it->laneID > 0)dest->laneID = it->laneID;
			/*导向箭头长度*/
			if (0 == _isnan(it->length))dest->length = it->length;
			/*中心点到末端距离 单位米*/
			if (0 == _isnan(it->distToTerminal))dest->distToTerminal = it->distToTerminal;

			dest->arrowType = it->arrowType;
			break;
		}
	}
}

void TessngDBToolsUpdate::gupdateLane(Lane* dest, const QList<Lane*>& src) {
	foreach(auto it, src) {
		/* 路段ID*/
		if (it->linkID > 0)dest->linkID = it->linkID;
		/* 车道序号 */
		if (it->serialNumber > 0)dest->serialNumber = it->serialNumber;
		/* 车道宽度 */
		if (0 == _isnan(it->width))dest->width = it->width;
		/* 希望行驶方向，L：左，R：右，A：任意 */
		if (!it->expectTravelDirection.isEmpty())dest->expectTravelDirection = it->expectTravelDirection;
		/* 行为类型 */
		if (!it->actionType.isEmpty())dest->actionType = it->actionType;
		/* 中心线断点json数据 */
		if (!it->centerLinePointsJson.isEmpty())dest->centerLinePointsJson = it->centerLinePointsJson;
		/* 左侧断点json数据 */
		if (!it->leftBreakPointsJson.isEmpty())dest->leftBreakPointsJson = it->leftBreakPointsJson;
		/* 右侧断点json数据 */
		if (!it->rightBreakPointsJson.isEmpty())dest->rightBreakPointsJson = it->rightBreakPointsJson;
		/* 其它属性json数据 */
		if (!it->otherAttrsJson.isEmpty())dest->otherAttrsJson = it->otherAttrsJson;
		/*导向箭头*/
		foreach(auto itt, dest->mlGuideArrow) {
			gupdateGuideArrow(itt, it->mlGuideArrow);
		}
		/*限制车型*/
		if (!it->mlVehicleType.isEmpty())dest->mlVehicleType = it->mlVehicleType;
	}
}
///update link
Link* TessngDBToolsUpdate::updateLink(const Link& link) {
	bool result = updateLinkPtr(const_cast<Link*>(&link));
	if (!result) return nullptr;
	GLink* gLink = gpScene->findGLinkByLinkID(link.linkID);

	assert(nullptr != gLink);


	if (!link.linkName.isEmpty())gLink->mpLink->linkName = link.linkName;
	/*路网ID*/
	if (link.netId > 0)gLink->mpLink->netId = link.netId;
	/*道路ID*/
	if (link.roadId > 0)gLink->mpLink->roadId = link.roadId;
	/* 车道数 */
	if (0 == _isnan(link.laneNumber))gLink->mpLink->laneNumber = link.laneNumber;
	/* 车道宽度 */
	if (0 == _isnan(link.laneWidth))gLink->mpLink->laneWidth = link.laneWidth;
	/* 车道颜色 */
	if (!link.laneColor.isEmpty())gLink->mpLink->laneColor = link.laneColor;
	/* 道路类型 */
	if (!link.linkType.isEmpty())gLink->mpLink->linkType = link.linkType;
	/* 路段长度 */
	if (0 == _isnan(link.length))gLink->mpLink->length = link.length;
	/* 曲率 */
	if (0 == _isnan(link.curvature))gLink->mpLink->curvature = link.curvature;
	/* 道路非直线系数 */
	if (0 == _isnan(link.nonLinearCoefficient))gLink->mpLink->nonLinearCoefficient = link.nonLinearCoefficient;
	/* 饱和流率 */
	if (0 == _isnan(link.linkSaturationFlow))gLink->mpLink->linkSaturationFlow = link.linkSaturationFlow;
	/* 路段流量 */
	if (0 == _isnan(link.linkTrafficFlow))gLink->mpLink->linkTrafficFlow = link.linkTrafficFlow;
	/* 期望速度(自由流速度) 千米/小时*/
	if (0 == _isnan(link.desiredSpeed))gLink->mpLink->desiredSpeed = link.desiredSpeed;
	/* 限速 千米/小时*/
	if (0 == _isnan(link.limitSpeed))gLink->mpLink->limitSpeed = link.limitSpeed;
	/* 最小速度 千米/小时*/
	if (0 == _isnan(link.minSpeed))gLink->mpLink->minSpeed = link.minSpeed;
	/* 附加值 */
	if (0 == _isnan(link.addValue))gLink->mpLink->addValue = link.addValue;
	/* 中心线断点json数组 */
	if (!link.centerLinePointsJson.isEmpty())gLink->mpLink->centerLinePointsJson = link.centerLinePointsJson;
	/* 左侧断点json数据 */
	if (!link.leftBreakPointsJson.isEmpty())gLink->mpLink->leftBreakPointsJson = link.leftBreakPointsJson;
	/* 右侧断点json数据 */
	if (!link.rightBreakPointsJson.isEmpty())gLink->mpLink->rightBreakPointsJson = link.rightBreakPointsJson;
	/* 其它属性json数据 */
	if (!link.otherAttrsJson.isEmpty())gLink->mpLink->otherAttrsJson = link.otherAttrsJson;

	gupdateNode(gLink->mpLink->startNode, link.startNode);
	gupdateNode(gLink->mpLink->endNode, link.endNode);

	foreach(auto it, gLink->mpLink->mlVertex) {
		gupdateVertex(it, link.mlVertex);
	}
	foreach(auto it, gLink->mpLink->mlLane) {
		gupdateLane(it, link.mlLane);
	}
	return gLink->mpLink;
}

///update lane
bool TessngDBToolsUpdate::updateLane(const Lane& lane) {
	bool result = updateLanePtr(const_cast<Lane*>(&lane));
	if (!result) return false;
	GLane* glane = gpScene->findGLaneByLaneID(lane.laneID);

	assert(nullptr != glane);
	/* 路段ID*/
	if (lane.linkID > 0)glane->mpLane->linkID = lane.linkID;
	/* 车道序号 */
	if (lane.serialNumber > 0)glane->mpLane->serialNumber = lane.serialNumber;
	/* 车道宽度 */
	if (0 == _isnan(lane.width))glane->mpLane->width = lane.width;
	/* 希望行驶方向，L：左，R：右，A：任意 */
	if (!lane.expectTravelDirection.isEmpty())glane->mpLane->expectTravelDirection = lane.expectTravelDirection;
	/* 行为类型 */
	if (!lane.actionType.isEmpty())glane->mpLane->actionType = lane.actionType;
	/* 中心线断点json数据 */
	if (!lane.centerLinePointsJson.isEmpty())glane->mpLane->centerLinePointsJson = lane.centerLinePointsJson;
	/* 左侧断点json数据 */
	if (!lane.leftBreakPointsJson.isEmpty())glane->mpLane->leftBreakPointsJson = lane.leftBreakPointsJson;
	/* 右侧断点json数据 */
	if (!lane.rightBreakPointsJson.isEmpty())glane->mpLane->rightBreakPointsJson = lane.rightBreakPointsJson;
	/* 其它属性json数据 */
	if (!lane.otherAttrsJson.isEmpty())glane->mpLane->otherAttrsJson = lane.otherAttrsJson;

	/*导向箭头*/
	foreach(auto it, glane->mpLane->mlGuideArrow) {
		gupdateGuideArrow(it, lane.mlGuideArrow);
	}
	/*限制车型*/
	if (!lane.mlVehicleType.isEmpty())glane->mpLane->mlVehicleType = lane.mlVehicleType;
	return true;
}

///updateGuide Arrow
bool TessngDBToolsUpdate::updateGuideArrow(const GuideArrow& arrow) {
	bool result = updateGuideArrowByTransPtr(const_cast<GuideArrow*>(&arrow));
	if (!result) return false;
	foreach(auto it, gpScene->mlGGuideArrow) {
		if (it->mpGuideArrow->guideArrowID == arrow.guideArrowID) {
			/* 车道编号 */
			if (arrow.laneID > 0)it->mpGuideArrow->laneID = arrow.laneID;
			/*导向箭头长度*/
			if (0 == _isnan(arrow.length))it->mpGuideArrow->length = arrow.length;
			/*中心点到末端距离 单位米*/
			if (0 == _isnan(arrow.distToTerminal))it->mpGuideArrow->distToTerminal = arrow.distToTerminal;

			it->mpGuideArrow->arrowType = arrow.arrowType;
			break;
		}
	}
	return true;
}
bool TessngDBToolsUpdate::updateLaneConnector(const LaneConnector& laneConn) {
	bool result = updateLaneConnectorPtr(const_cast<LaneConnector*>(&laneConn));
	if (!result) return false;
	GLaneConnector* glaneConn = gpScene->findGLaneConnectorByID(laneConn.mpFromLane->laneID, laneConn.mpToLane->laneID);
	assert(nullptr != glaneConn);
	if (glaneConn->mpLaneConnector->mpConnector->connID != laneConn.laneConnectorID) return true;

	updateLane(*laneConn.mpFromLane);
	updateLane(*laneConn.mpToLane);

	if (0 == _isnan(laneConn.mrLength))glaneConn->mpLaneConnector->mrLength = laneConn.mrLength;
	/* 权重 */
	if (laneConn.weight > 0)glaneConn->mpLaneConnector->weight = laneConn.weight;
	/* 中心线断点json数据 */
	if (!laneConn.centerLinePointsJson.isEmpty())glaneConn->mpLaneConnector->centerLinePointsJson = laneConn.centerLinePointsJson;
	/* 左侧断点json数据 */
	if (!laneConn.leftBreakPointsJson.isEmpty())glaneConn->mpLaneConnector->leftBreakPointsJson = laneConn.leftBreakPointsJson;
	/* 右侧断点json数据 */
	if (!laneConn.rightBreakPointsJson.isEmpty())glaneConn->mpLaneConnector->rightBreakPointsJson = laneConn.rightBreakPointsJson;
	/* 其它属性json数据 */
	if (!laneConn.otherAttrsJson.isEmpty())glaneConn->mpLaneConnector->otherAttrsJson = laneConn.otherAttrsJson;


	return true;
}
///update Connector
bool TessngDBToolsUpdate::updateConnector(const Connector& conn) {
	bool result = updateConnectorsPtr(const_cast<Connector*>(&conn));
	if (!result) return false;
	GConnector* gConn = gpScene->findGConnectorByConnID(conn.connID);

	assert(nullptr != gConn);

	/* 道路ID */
	if (conn.roadId > 0)gConn->mpConnector->roadId = conn.roadId;
	/* 连接段名称 */
	if (!conn.connName.isEmpty())gConn->mpConnector->connName = conn.connName;
	/* 连接段长度 */
	if (0 == _isnan(conn.length))gConn->mpConnector->length = conn.length;
	/* 曲率 */
	if (0 == _isnan(conn.curvature))gConn->mpConnector->curvature = conn.curvature;
	/* 连接段非直线系数 */
	if (0 == _isnan(conn.nonLinearCoefficient))gConn->mpConnector->nonLinearCoefficient = conn.nonLinearCoefficient;
	/* 路面颜色 */
	if (!conn.color.isEmpty())gConn->mpConnector->color = conn.color;
	/* 期望速度 */
	if (0 == _isnan(conn.desiredSpeed))gConn->mpConnector->desiredSpeed = conn.desiredSpeed;
	/* 限速 */
	if (0 == _isnan(conn.limitSpeed))gConn->mpConnector->limitSpeed = conn.limitSpeed;
	/* 左侧断点json数据 */
	if (!conn.leftBreakPointsJson.isEmpty())gConn->mpConnector->leftBreakPointsJson = conn.leftBreakPointsJson;
	/* 右侧断点json数据 */
	if (!conn.rightBreakPointsJson.isEmpty())gConn->mpConnector->rightBreakPointsJson = conn.rightBreakPointsJson;
	/* 其它属性json数据 */
	if (!conn.otherAttrsJson.isEmpty())gConn->mpConnector->otherAttrsJson = conn.otherAttrsJson;

	/* 起始路段 */
	updateLink(*gConn->mpConnector->mpFromLink);
	/* 目标路段 */
	updateLink(*gConn->mpConnector->mpToLink);

	/* 车道连接集*/
	foreach(auto it, conn.mlLaneConnector) {
		updateLaneConnector(*it);
	}

	/*限制车型*/
	if (!conn.mlVehicleType.isEmpty())gConn->mpConnector->mlVehicleType = conn.mlVehicleType;

	return true;
}
bool TessngDBToolsUpdate::findRoutingLink(int& m, int& n, int id, const QList<QList<Link*>>& list) {
	for (int i = 0; i < list.size(); i++)
	{
		m = i;
		for (int j = 0; j < list[i].size(); j++) {
			if (id == list[i][j]->linkID) {
				n = j;
				return true;
			}
		}
	}
	return false;
}
///update Routeing
Routing* TessngDBToolsUpdate::updateRouteing(const Routing& rt) {
	bool result = updateRouteingPtr(const_cast<Routing*>(&rt));
	if (!result) return nullptr;
	GRouting* gRt = gpScene->findGRoutingByID(rt.routingID);

	assert(nullptr != gRt);

	if (!rt.routingName.isEmpty())gRt->getRouting()->routingName = rt.routingName;
	/* 流量比 */
	if (0 == _isnan(rt.proportion))gRt->getRouting()->proportion = rt.proportion;

	//一条决策路径可以由多个最短路径组成
	QList<QPair<int, int>> rmList;
	for (int i = 0; i < gRt->getRouting()->mllLink.size(); i++) {
		for (int j = 0; j < gRt->getRouting()->mllLink[i].size(); j++)
		{
			int m, n;
			if (findRoutingLink(m, n, gRt->getRouting()->mllLink[i][j]->linkID, rt.mllLink)) {
				Link* link = updateLink(*rt.mllLink[m][n]);
				if (nullptr != link) {
					gRt->getRouting()->mllLink[i][j] = link;
				}
			}
			else {
				rmList.append(QPair<int, int>(i, j));
			}
		}
	}
	while (!rmList.empty())
	{
		QPair<int, int> pair = rmList.takeFirst();
		gRt->getRouting()->mllLink[pair.first].removeAt(pair.second);
	}
	return gRt->getRouting();
}
void TessngDBToolsUpdate::gupdateRoutingFlowByInterval(QList<RoutingFLowRatio*>& dest, const QList<RoutingFLowRatio*>& src) {
	bool result = false;
	for (int i = 0; i < dest.size(); i++) {
		for (int j = 0; j < src.size(); j++) {
			if (dest[i]->RoutingFLowRatioID != src[j]->RoutingFLowRatioID) continue;

			if (src[j]->routingID > 0)dest[i]->routingID = src[j]->routingID;
			if (src[j]->startDateTime > 0)dest[i]->startDateTime = src[j]->startDateTime;
			if (src[j]->endDateTime > 0)dest[i]->endDateTime = src[j]->endDateTime;
			if (0 == _isnan(src[j]->ratio))dest[i]->ratio = src[j]->ratio;
			if (src[j]->vehiCount > 0)dest[i]->vehiCount = src[j]->vehiCount;
			break;
		}
	}
}
///update DecisionPoint
bool TessngDBToolsUpdate::updateDecisionPoint(const DecisionPoint& dp) {
	bool result = updateDecisionPointPtr(const_cast<DecisionPoint*>(&dp));
	if (!result) return false;
	GDecisionPoint* gdP = gpScene->findGDecisionPointByID(dp.deciPointID);

	assert(nullptr != gdP);

	if (!dp.deciPointName.isEmpty())gdP->mpDecisionPoint->deciPointName = dp.deciPointName;
	/* 坐标X */
	if (0 == _isnan(dp.X)) gdP->mpDecisionPoint->X = dp.X;
	/* 坐标Y */
	if (0 == _isnan(dp.Y))gdP->mpDecisionPoint->Y = dp.Y;
	/* 高程 */
	if (0 == _isnan(dp.Z))gdP->mpDecisionPoint->Z = dp.Z;

	if (!dp.mFromPoint.isNull()) gdP->mpDecisionPoint->mFromPoint = dp.mFromPoint;
	if (!dp.mToPoint.isNull())gdP->mpDecisionPoint->mToPoint = dp.mToPoint;

	for (int i = 0; i < gdP->mpDecisionPoint->mlRouting.size(); i++)
	{
		for (int j = 0; j < dp.mlRouting.size(); j++) {
			if (gdP->mpDecisionPoint->mlRouting[i]->routingID != dp.mlRouting[j]->routingID) continue;

			Routing* rt = updateRouteing(*dp.mlRouting[j]);
			if (nullptr != rt) gdP->mpDecisionPoint->mlRouting[i] = rt;
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
bool TessngDBToolsUpdate::updateDeparturePoint(const DeparturePoint& dp) {
	bool result = updateDeparturePointPtr(const_cast<DeparturePoint*>(&dp));
	if (!result) return false;
	GDeparturePoint* gdP = gpScene->findGDeparturePointByID(dp.departurePointID);

	assert(nullptr != gdP);

	/*发车点名称*/
	if (!dp.name.isEmpty())gdP->mpDeparturePoint->name = dp.name;

	if (!dp.mlDepaInterval.isEmpty())gdP->mpDeparturePoint->mlDepaInterval = dp.mlDepaInterval;
	if (!dp.mlDepaPossion.isEmpty())gdP->mpDeparturePoint->mlDepaPossion = dp.mlDepaPossion;

	return true;
}

///update signalGroup
bool TessngDBToolsUpdate::updateSignalGroup(const SignalGroup& sg) {
	bool result = updateSignalGroupPtr(const_cast<SignalGroup*>(&sg));
	if (!result) return false;
	SignalGroup* gdP = gpScene->findSignalGroupById(sg.signalGroupID);

	assert(nullptr != gdP);

	/* 名称 */
	if (!sg.name.isEmpty())gdP->name = sg.name;
	/* 时长(秒) */
	if (sg.timeLength > 0)gdP->timeLength = sg.timeLength;
	/* 起始时间，单位：秒 */
	if (sg.startTime > 0)gdP->startTime = sg.startTime;
	/* 结束时间 单位：秒 */
	if (sg.endTime > 0)gdP->endTime = sg.endTime;

	for (int i = 0; i < gdP->mlPhase.size(); i++) {

		for (int j = 0; j < sg.mlPhase.size(); j++) {
			if (gdP->mlPhase[i]->signalPhaseID != sg.mlPhase[j]->signalPhaseID) continue;
			/* 序号 */
			if (sg.mlPhase[j]->serialNumber > 0)gdP->mlPhase[i]->serialNumber = sg.mlPhase[j]->serialNumber;
			/* 名称 */
			if (!sg.mlPhase[j]->name.isEmpty())gdP->mlPhase[i]->name = sg.mlPhase[j]->name;

			if (!sg.mlPhase[j]->mlSignalColor.isEmpty())gdP->mlPhase[i]->mlSignalColor = sg.mlPhase[j]->mlSignalColor;

			break;
		}
	}
	return true;
}

///update SignalLamp
bool TessngDBToolsUpdate::updateSignalLamp(const SignalLamp& pSignalLamp) {
	bool result = updateSignalLampPtr(const_cast<SignalLamp*>(&pSignalLamp));
	if (!result) return false;

	GSignalLamp* gdP = nullptr;
	foreach(auto it, gpScene->mlGSignalLamp) {
		if (it->mpSignalLamp->signalLampID != pSignalLamp.signalLampID) continue;
		gdP = it;
		break;
	}
	assert(nullptr != gdP);

	if (!pSignalLamp.name.isEmpty())gdP->mpSignalLamp->name = pSignalLamp.name;

	/* 车道*/
	if (pSignalLamp.laneID > 0)gdP->mpSignalLamp->laneID = pSignalLamp.laneID;
	/* 目标车道ID， 如果不在连接连接上， 目标车道ID为0 */
	if (pSignalLamp.toLaneID > 0)gdP->mpSignalLamp->toLaneID = pSignalLamp.toLaneID;

	if (0 == _isnan(pSignalLamp.x))gdP->mpSignalLamp->x = pSignalLamp.x;
	if (0 == _isnan(pSignalLamp.y))gdP->mpSignalLamp->y = pSignalLamp.y;
	if (0 == _isnan(pSignalLamp.z))gdP->mpSignalLamp->z = pSignalLamp.z;

	return true;
}

///update VehicleDrivInfoCollector
bool TessngDBToolsUpdate::updateVehicleDrivInfoCollector(const VehicleDrivInfoCollector& vdc) {
	bool result = updateVehicleDrivInfoCollectorPtr(const_cast<VehicleDrivInfoCollector*>(&vdc));
	if (!result) return false;

	GVehicleDrivInfoCollector* gdP = gpScene->findGVehicleDrivInfoCollectorByID(vdc.collecterID);

	assert(nullptr != gdP);


	if (!vdc.name.isEmpty())gdP->mpVehicleDrivInfoCollector->name = vdc.name;
	if (vdc.roadID > 0)gdP->mpVehicleDrivInfoCollector->roadID = vdc.roadID;
	if (vdc.laneNumber > 0)gdP->mpVehicleDrivInfoCollector->laneNumber = vdc.laneNumber;
	if (vdc.toLaneNumber > 0)gdP->mpVehicleDrivInfoCollector->toLaneNumber = vdc.toLaneNumber;
	if (0 == _isnan(vdc.distance))gdP->mpVehicleDrivInfoCollector->distance = vdc.distance;
	if (0 == _isnan(vdc.x))gdP->mpVehicleDrivInfoCollector->x = vdc.x;
	if (0 == _isnan(vdc.y))gdP->mpVehicleDrivInfoCollector->y = vdc.y;
	if (0 == _isnan(vdc.z))gdP->mpVehicleDrivInfoCollector->z = vdc.z;
	if (vdc.startTime > 0)gdP->mpVehicleDrivInfoCollector->startTime = vdc.startTime;
	if (vdc.endTime > 0)gdP->mpVehicleDrivInfoCollector->endTime = vdc.endTime;
	if (vdc.dataInterval > 0)gdP->mpVehicleDrivInfoCollector->dataInterval = vdc.dataInterval;
	return true;
}

///update VehicleQueueCounter
bool TessngDBToolsUpdate::updateVehicleQueueCounter(const VehicleQueueCounter& vqc) {
	bool result = updateVehicleQueueCounterPtr(const_cast<VehicleQueueCounter*>(&vqc));
	if (!result) return false;

	GVehicleQueueCounter* gdP = gpScene->findGVehicleQueueCounterByID(vqc.queueCounterID);

	assert(nullptr != gdP);

	/* 计数器名称 */
	if (!vqc.name.isEmpty())gdP->mpVehicleQueueCounter->name = vqc.name;
	/* 道路(路段或连接段)ID */
	if (vqc.roadID > 0)gdP->mpVehicleQueueCounter->roadID = vqc.roadID;
	/* 采集器所在车道序号 */
	if (vqc.laneNumber > 0)gdP->mpVehicleQueueCounter->laneNumber = vqc.laneNumber;
	/* 目标车道*/
	if (vqc.toLaneNumber > 0)gdP->mpVehicleQueueCounter->toLaneNumber = vqc.toLaneNumber;
	if (0 == _isnan(vqc.x))gdP->mpVehicleQueueCounter->x = vqc.x;
	/* 位置Y */
	if (0 == _isnan(vqc.y))gdP->mpVehicleQueueCounter->y = vqc.y;
	/* 高程*/
	if (0 == _isnan(vqc.z))gdP->mpVehicleQueueCounter->z = vqc.z;
	/* 速度下限 */
	if (0 == _isnan(vqc.speedLowLimit))gdP->mpVehicleQueueCounter->speedLowLimit = vqc.speedLowLimit;
	/* 速度上限 */
	if (0 == _isnan(vqc.speedUpLimit))gdP->mpVehicleQueueCounter->speedUpLimit = vqc.speedUpLimit;
	/* 最大车辆间距 */
	if (0 == _isnan(vqc.maxDistInterval))gdP->mpVehicleQueueCounter->maxDistInterval = vqc.maxDistInterval;
	/* 最大排队长度 */
	if (0 == _isnan(vqc.maxQueueLength))gdP->mpVehicleQueueCounter->maxQueueLength = vqc.maxQueueLength;
	/* 距起点距离 */
	if (0 == _isnan(vqc.distance))gdP->mpVehicleQueueCounter->distance = vqc.distance;
	/* 起始时间 */
	if (vqc.startTime > 0)gdP->mpVehicleQueueCounter->startTime = vqc.startTime;
	/* 结束时间 */
	if (vqc.endTime > 0)gdP->mpVehicleQueueCounter->endTime = vqc.endTime;
	/* 数据间隔*/
	if (vqc.dataInterval > 0)gdP->mpVehicleQueueCounter->dataInterval = vqc.dataInterval;
	/* 采集间隔*/
	if (vqc.countInterval > 0)gdP->mpVehicleQueueCounter->countInterval = vqc.countInterval;

	return true;
}

///update VehicleTravelDetector
bool TessngDBToolsUpdate::updateVehicleTravelDetector(const VehicleTravelDetector& vtd) {
	bool result = updateVehicleTravelDetectorPtr(const_cast<VehicleTravelDetector*>(&vtd));
	if (!result) return false;

	QList<GVehicleTravelDetector*> list = gpScene->findGVehicleTravelDetectorById(vtd.detectorId);
	GVehicleTravelDetector* gdP = nullptr;
	foreach(auto it, list) {
		if (it->mpVehicleTravelDetector->detectorId == vtd.detectorId) {
			gdP = it;
			break;
		}
	}
	if (nullptr == gdP) return true;
	/* 检测器名称 */
	if (!vtd.name.isEmpty())gdP->mpVehicleTravelDetector->name = vtd.name;
	/* 起始路段编号 */
	if (vtd.startRoadId > 0)gdP->mpVehicleTravelDetector->startRoadId = vtd.startRoadId;
	/* 起始车道连接的起始车道 */
	if (vtd.start_laneNumber > 0)gdP->mpVehicleTravelDetector->start_laneNumber = vtd.start_laneNumber;
	/* 起始车道连接的目标车道 */
	if (vtd.start_toLaneNumber > 0)gdP->mpVehicleTravelDetector->start_toLaneNumber = vtd.start_toLaneNumber;
	/* 起始路段距离 */
	if (0 == _isnan(vtd.startDist))gdP->mpVehicleTravelDetector->startDist = vtd.startDist;
	/* 起始位置X */
	if (0 == _isnan(vtd.startX))gdP->mpVehicleTravelDetector->startX = vtd.startX;
	/* 起始位置Y */
	if (0 == _isnan(vtd.startY))gdP->mpVehicleTravelDetector->startY = vtd.startY;
	/* 终点路段编号 */
	if (vtd.teminalRoadId > 0)gdP->mpVehicleTravelDetector->teminalRoadId = vtd.teminalRoadId;
	/* 终点车道连接的起始车道 */
	if (vtd.teminal_laneNumber > 0)gdP->mpVehicleTravelDetector->teminal_laneNumber = vtd.teminal_laneNumber;
	/* 终点车道连接的目标车道 */
	if (vtd.teminal_toLaneNumber > 0)gdP->mpVehicleTravelDetector->teminal_toLaneNumber = vtd.teminal_toLaneNumber;
	/* 终点路段距离 */
	if (0 == _isnan(vtd.teminalDist))gdP->mpVehicleTravelDetector->teminalDist = vtd.teminalDist;
	/* 终点位置X */
	if (0 == _isnan(vtd.teminalX))gdP->mpVehicleTravelDetector->teminalX = vtd.teminalX;
	/* 终点位置Y */
	if (0 == _isnan(vtd.teminalY))gdP->mpVehicleTravelDetector->teminalY = vtd.teminalY;
	/* 仿真起始时间 */
	if (vtd.startTime > 0)gdP->mpVehicleTravelDetector->startTime = vtd.startTime;
	/* 仿真结束时间 */
	if (vtd.endTime > 0)gdP->mpVehicleTravelDetector->endTime = vtd.endTime;
	/* 数据间隔 */
	if (vtd.dataInterval > 0)gdP->mpVehicleTravelDetector->dataInterval = vtd.dataInterval;
	/* 最短距离 */
	if (0 == _isnan(vtd.shortestDistance))gdP->mpVehicleTravelDetector->shortestDistance = vtd.shortestDistance;
	return true;
}
///update VehicleDetector
bool TessngDBToolsUpdate::updateVehicleDetector(const VehicleDetector& vd) {
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
	if (!vd.name.isEmpty())gdP->mpVehicleDetector->name = vd.name;
	/* 信号灯组ID */
	if (vd.signalGroupID > 0)gdP->mpVehicleDetector->signalGroupID = vd.signalGroupID;
	/* 相位序号 */
	if (vd.phaseNumber > 0)gdP->mpVehicleDetector->phaseNumber = vd.phaseNumber;
	/* 道路ID */
	if (vd.roadId > 0)gdP->mpVehicleDetector->roadId = vd.roadId;
	/* 道路类型 */
	if (!vd.roadType.isEmpty())gdP->mpVehicleDetector->roadType = vd.roadType;
	/* 车道序号 */
	if (vd.laneNumber > 0)gdP->mpVehicleDetector->laneNumber = vd.laneNumber;
	/* 目标车道序号 */
	if (vd.toLaneNumber > 0)gdP->mpVehicleDetector->toLaneNumber = vd.toLaneNumber;
	/* 到终点距离 */
	if (0 == _isnan(vd.distToTerminal))gdP->mpVehicleDetector->distToTerminal = vd.distToTerminal;
	/* 长度 */
	if (0 == _isnan(vd.length))gdP->mpVehicleDetector->length = vd.length;
	/*检测器类型*/
	if (!vd.type.isEmpty())gdP->mpVehicleDetector->type = vd.type;
	/* 最大绿时 */
	if (vd.maxGreen > 0)gdP->mpVehicleDetector->maxGreen = vd.maxGreen;

	if (!vd.mlVehicleType.isEmpty())gdP->mpVehicleDetector->mlVehicleType = vd.mlVehicleType;

	return true;
}

///update ReduceSpeedArea
bool TessngDBToolsUpdate::updateReduceSpeedArea(const ReduceSpeedArea& reduceSpeedArea) {
	bool result = updateReduceSpeedAreaPtr(const_cast<ReduceSpeedArea*>(&reduceSpeedArea));
	if (!result) return false;

	GReduceSpeedArea* gReduceSpeedArea = nullptr;
	foreach(GReduceSpeedArea * it, gpScene->mlGReduceSpeedArea) {
		if (reduceSpeedArea.reduceSpeedAreaID == it->mpReduceSpeedArea->reduceSpeedAreaID) gReduceSpeedArea = it;
	}
	assert(nullptr != gReduceSpeedArea);

	if (!reduceSpeedArea.name.isEmpty())gReduceSpeedArea->mpReduceSpeedArea->name = reduceSpeedArea.name;
	if (0 == _isnan(reduceSpeedArea.location))gReduceSpeedArea->mpReduceSpeedArea->location = reduceSpeedArea.location;
	if (0 == _isnan(reduceSpeedArea.areaLength))gReduceSpeedArea->mpReduceSpeedArea->areaLength = reduceSpeedArea.areaLength;
	if (reduceSpeedArea.roadID > 0)gReduceSpeedArea->mpReduceSpeedArea->roadID = reduceSpeedArea.roadID;
	if (reduceSpeedArea.laneNumber > 0)gReduceSpeedArea->mpReduceSpeedArea->laneNumber = reduceSpeedArea.laneNumber;
	if (reduceSpeedArea.toLaneNumber > 0)gReduceSpeedArea->mpReduceSpeedArea->toLaneNumber = reduceSpeedArea.toLaneNumber;

	foreach(auto newItem, reduceSpeedArea.mlReduceSpeedInterval) {
		foreach(auto oldItem, gReduceSpeedArea->mpReduceSpeedArea->mlReduceSpeedInterval) {
			if (newItem->intervalID == oldItem->intervalID && newItem->reduceSpeedAreaID == oldItem->reduceSpeedAreaID) {
				if (newItem->startTime > 0)oldItem->startTime = newItem->startTime;
				if (newItem->endTime > 0)oldItem->endTime = newItem->endTime;
			}
		}
	}
	foreach(auto newItem, reduceSpeedArea.mlReduceSpeedVehiType) {
		foreach(auto oldItem, gReduceSpeedArea->mpReduceSpeedArea->mlReduceSpeedVehiType) {
			if (newItem->vehicleTypeCode == oldItem->vehicleTypeCode && newItem->reduceSpeedAreaID == oldItem->reduceSpeedAreaID) {
				if (0 == _isnan(newItem->avgSpeed))oldItem->avgSpeed = newItem->avgSpeed;
				if (0 == _isnan(newItem->speedSD))oldItem->speedSD = newItem->speedSD;
			}
		}
	}

	return true;
}

///update BusStation: BusStationLine仅能对已有元素进行更新
bool TessngDBToolsUpdate::updateBustation(const BusStation& busStation) {
	bool result = updateBustationPtr(const_cast<BusStation*>(&busStation));
	if (!result) return false;

	GBusStation* gBusStation = gpScene->findGBusStationByID(busStation.busStationID);
	assert(nullptr != gBusStation);

	if (busStation.busStationID > 0)gBusStation->mpBusStation->busStationID = busStation.busStationID;
	if (!busStation.name.isEmpty())gBusStation->mpBusStation->name = busStation.name;
	if (busStation.laneNumber > 0)gBusStation->mpBusStation->laneNumber = busStation.laneNumber;
	if (0 == _isnan(busStation.x))gBusStation->mpBusStation->x = busStation.x;
	if (0 == _isnan(busStation.y))gBusStation->mpBusStation->y = busStation.y;
	if (0 == _isnan(busStation.length))gBusStation->mpBusStation->length = busStation.length;
	if (!busStation.type.isEmpty())gBusStation->mpBusStation->type = busStation.type;

	foreach(auto newItem, busStation.mlBusStationLine) {
		foreach(auto oldItem, gBusStation->mpBusStation->mlBusStationLine) {
			if (newItem->id() == oldItem->id()) {
				if (newItem->busStationID > 0)oldItem->busStationID = newItem->busStationID;
				if (newItem->busStationID > 0)oldItem->linkID = newItem->linkID;
				if (newItem->busStationID > 0)oldItem->busLineID = newItem->busLineID;
				if (newItem->busStationID > 0)oldItem->parkingTime = newItem->parkingTime;
				if (0 == _isnan(newItem->leavingPercent))oldItem->leavingPercent = newItem->leavingPercent;
				if (0 == _isnan(newItem->getOnTimePerson))oldItem->getOnTimePerson = newItem->getOnTimePerson;
				if (0 == _isnan(newItem->getOutTimePerson))oldItem->getOutTimePerson = newItem->getOutTimePerson;

				foreach(auto newArriving, newItem->mlPassengerArriving) {
					foreach(auto oldArriving, oldItem->mlPassengerArriving) {
						if (newArriving->passengerArrivingID == oldArriving->passengerArrivingID) {
							if (newArriving->startTime > 0)oldArriving->startTime = newArriving->startTime;
							if (newArriving->endTime > 0)oldArriving->endTime = newArriving->endTime;
							if (newArriving->passengerCount > 0)oldArriving->passengerCount = newArriving->passengerCount;
						}
					}
				}

			}
		}
	}

	return true;
}

bool TessngDBToolsUpdate::updateBusLine(const BusLine& busLine) {
	bool result = updateBusLinePtr(const_cast<BusLine*>(&busLine));
	if (!result) return false;

	GBusLine* gBusLine = gpScene->findGBusLineByID(busLine.busLineID);
	assert(nullptr != gBusLine);

	if (!busLine.name.isEmpty())gBusLine->mpBusLine->name = busLine.name;
	if (0 == _isnan(busLine.length))gBusLine->mpBusLine->length = busLine.length;
	if (busLine.dischargeFreq > 0)gBusLine->mpBusLine->dischargeFreq = busLine.dischargeFreq;
	if (busLine.dischargeStartTime > 0)gBusLine->mpBusLine->dischargeStartTime = busLine.dischargeStartTime;
	if (busLine.dischargeEndTime > 0)gBusLine->mpBusLine->dischargeEndTime = busLine.dischargeEndTime;
	if (0 == _isnan(busLine.startX))gBusLine->mpBusLine->startX = busLine.startX;
	if (0 == _isnan(busLine.startY))gBusLine->mpBusLine->startY = busLine.startY;
	if (0 == _isnan(busLine.endX))gBusLine->mpBusLine->endX = busLine.endX;
	if (0 == _isnan(busLine.endY))gBusLine->mpBusLine->endY = busLine.endY;
	if (0 == _isnan(busLine.desirSpeed))gBusLine->mpBusLine->desirSpeed = busLine.desirSpeed;
	if (0 == _isnan(busLine.speedSD))gBusLine->mpBusLine->speedSD = busLine.speedSD;
	if (busLine.passCountAtStartTime > 0)gBusLine->mpBusLine->passCountAtStartTime = busLine.passCountAtStartTime;
	if (!busLine.mSpecialApp.isEmpty())gBusLine->mpBusLine->mSpecialApp = busLine.mSpecialApp;

	foreach(auto newItem, busLine.mlBusStationLine) {
		foreach(auto oldItem, gBusLine->mpBusLine->mlBusStationLine) {
			if (newItem->id() == oldItem->id()) {
				if (newItem->busStationID > 0)oldItem->busStationID = newItem->busStationID;
				if (newItem->linkID > 0)oldItem->linkID = newItem->linkID;
				if (newItem->busLineID > 0)oldItem->busLineID = newItem->busLineID;
				if (newItem->parkingTime > 0)oldItem->parkingTime = newItem->parkingTime;
				if (0 == _isnan(newItem->leavingPercent))oldItem->leavingPercent = newItem->leavingPercent;
				if (0 == _isnan(newItem->getOnTimePerson))oldItem->getOnTimePerson = newItem->getOnTimePerson;
				if (0 == _isnan(newItem->getOutTimePerson))oldItem->getOutTimePerson = newItem->getOutTimePerson;

				foreach(auto newArriving, newItem->mlPassengerArriving) {
					foreach(auto oldArriving, oldItem->mlPassengerArriving) {
						if (newArriving->passengerArrivingID == oldArriving->passengerArrivingID) {
							if (newArriving->startTime > 0)oldArriving->startTime = newArriving->startTime;
							if (newArriving->endTime > 0)oldArriving->endTime = newArriving->endTime;
							if (newArriving->passengerCount > 0)oldArriving->passengerCount = newArriving->passengerCount;
						}
					}
				}
			}
		}
	}

	return true;
}
