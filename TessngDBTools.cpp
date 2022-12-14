#include "TessngDBTools.h"
#include "DbGlobal.h"

#include "gsignallamp.h"
#include "signallamp.h"
#include "signalphase.h"
#include "signalGroup.h"
#include "ISignalPhase.h"

#include "PassengerArriving.h"
#include "BusStationLine.h"
#include "GBusLine.h"
#include "GBusStation.h"

#include "GVehicleDrivInfoCollector.h"
#include "VehicleDrivInfoCollector.h"
#include "GVehicleQueueCounter.h"
#include "VehicleQueueCounter.h"
#include "GVehicleTravelDetector.h"
#include "VehicleTravelDetector.h"
#include "GGuideArrow.h"
#include "GuideArrow.h"
#include "ReduceSpeedInterval.h"
#include "ReduceSpeedVehiType.h"
#include "GReducespeedarea.h"
#include "ReduceSpeedArea.h"
#include "GDeparturepoint.h"
#include "DeparturePoint.h"

#include "RoutingFLowRatio.h"
#include "GRouting.h"
#include "Routing.h"
#include "GDecisionPoint.h"
#include "DecisionPoint.h"
#include "GLaneConnector.h"
#include "LaneConnector.h"
#include "GConnector.h"
#include "Connector.h"
#include "GVertex.h"
#include "Lane.h"
#include "Node.h"
#include "GLink.h"
#include "Link.h"
#include "ILink.h"
#include "Vehicletype.h"

#include <QException>
#include <QDebug>
TessngDBTools::TessngDBTools() :TessngDBToolsBase() {

}

//------------------------------------辅助函数----------------------------------
bool TessngDBTools::checkContansLane(const QList<GLink*>& list, long laneId)
{
	foreach(auto it, list) {
		foreach(auto lane, it->mlGLane)
		{
			if (laneId == lane->id()) return true;
		}
	}

	return false;
}

bool TessngDBTools::checkContansLane(const QList<GConnector*>& list, long fromlaneId, long tolaneId)
{
	foreach(auto it, list) {
		foreach(auto lane, it->laneConnectors())
		{
			if (fromlaneId == lane->fromLane()->id() &&
				tolaneId == lane->toLane()->id()) return true;
		}
	}
	return false;
}

bool TessngDBTools::checkContainsConnector(const QList<GConnector*>& list, long id)
{
	foreach(auto it, list) {
		if (id == it->id()) return true;
	}
	return false;
}

//------------------------------------信号控制----------------------------------
bool TessngDBTools::deleteSignalLamp(QList<long> ids)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();

		QList<GSignalLamp*> rmSignalLamp;
		foreach(GSignalLamp * it, gpScene->mlGSignalLamp) {
			if (!ids.contains(it->id())) continue;
			if (rmSignalLamp.contains(it)) continue;
			rmSignalLamp.push_back(it);
		}

		//删除信号灯
		result = removeSignalLamp(rmSignalLamp);
		if (!result)goto failed;

		foreach(auto it, rmSignalLamp)
		{
			gpScene->removeGSignalLamp(it);
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove SignalGroups failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

bool TessngDBTools::deleteSignalColor(long id, QList<int> colors)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();

		SignalPhase* dPhase;
		for(auto& group : gpScene->mlSignalGroup) {
			for (auto& phase : group->mlPhase) {
				if (phase->id() == id) dPhase = phase;
			}
		}
		//删除相位颜色
		result = removeSignalColor(dPhase, colors);
		if (!result)goto failed;

		/*for (auto& dColor : colors) {
			for (int i = 0; i < dPhase->mlSignalColor.size();) {
				if (dColor.color == dPhase->mlSignalColor[i].color) {
					dPhase->mlSignalColor.removeAt(i);
					break;
				}
				else {
					i++;
				}
			}
		}*/
		for (auto& color : colors) {
			dPhase->mlSignalColor.removeAt(color);
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove SignalGroups failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

bool TessngDBTools::deleteSignalPhase(QList<long> ids)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();

		QList<SignalPhase*> rmSignalPhases;
		for(SignalGroup * group : gpScene->mlSignalGroup) {
			for (SignalPhase* phase : group->mlPhase) {
				if (!ids.contains(phase->id())) continue;
				if (rmSignalPhases.contains(phase)) continue;
				rmSignalPhases.push_back(phase);
			}
		}
		
		//删除信号灯
		result = removeSignalPhase(rmSignalPhases);
		if (!result)goto failed;

		for (int i = 0; i < gpScene->mlSignalGroup.size(); i++) {
			SignalGroup* group = gpScene->mlSignalGroup[i];
			for (int j = 0; j < group->mlPhase.size(); ) {
				SignalPhase* phase = group->mlPhase[j];
				if (!rmSignalPhases.contains(phase)) {
					j++; 
					continue;
				}

				group->mlPhase.removeOne(phase);
			}
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove SignalGroups failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

bool TessngDBTools::deleteSignalGroup(QList<long> ids) 
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();

		QList<SignalGroup*> rmSignalGroup;
		QList<GSignalLamp*> rmSignalLamp;
		foreach(SignalGroup * pGSl, gpScene->mlSignalGroup) {
			if (!ids.contains(pGSl->signalGroupID)) continue;
			if (rmSignalGroup.contains(pGSl)) continue;
			rmSignalGroup.push_back(pGSl);
		}
		foreach(GSignalLamp* it, gpScene->mlGSignalLamp) {
			if (!ids.contains(it->signalGroup()->id())) continue;
			if (rmSignalLamp.contains(it)) continue;
			rmSignalLamp.push_back(it);
		}

		//删除信号灯
		result = removeSignalLamp(rmSignalLamp);
		if (!result)goto failed;

		//删除信号灯组
		result = removeSignalGroup(rmSignalGroup) && result;
		if (!result)goto failed;

		foreach(auto it, rmSignalLamp)
		{
			gpScene->removeGSignalLamp(it);
		}
		foreach(auto it, rmSignalGroup)
		{
			gpScene->removeSignalGroup(it);
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove SignalGroups failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

//--------------------------------------公交------------------------------------

//---------------------------------车辆运行及检测-------------------------------

//-----------------------------------道路及连接---------------------------------
bool TessngDBTools::deleteRouting(QList<long> ids) 
{
	bool result = true;
	try {
		QList<GRouting*> rmRouteings;
		foreach(auto it, gpScene->mlGRouting)
		{
			if (!ids.contains(it->id())) continue;
			if (rmRouteings.contains(it)) continue;
			rmRouteings.push_back(it);
		}

		result = removeRouting(rmRouteings) && result;
		if (!result)goto failed;
		foreach(auto it, rmRouteings)
		{
			gpScene->removeGRouting(it);
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove Routes failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}
bool TessngDBTools::deleteVehicleType(QList<long> ids) {
	bool result = true;
	try {

	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove Routes failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}
bool TessngDBTools::deleteConnectors(QList<long> ids) 
{
	bool result = true;
	try {
		QList<GConnector*> rmConnects;
		foreach(GConnector * ptrConn, gpScene->mlGConnector) {
			if (rmConnects.contains(ptrConn)) continue;
			if (ids.contains(ptrConn->mpConnector->connID)) {
				rmConnects.push_back(ptrConn);
			}
		}

		foreach(GDecisionPoint * pGDecisionPoint, gpScene->mlGDecisionPoint) {
			foreach(GRouting * pGRouting, pGDecisionPoint->mlGRouting) {
				foreach(OneRouting oneRouting, pGRouting->mlOneRouting) {
					if (oneRouting.mlGLink.size() == 0) break;

					for (int i = 0, size = oneRouting.mlGLink.size(); i < size - 1; ++i) {
						GLink* pGLink1 = oneRouting.mlGLink.at(i);
						GLink* pGLink2 = oneRouting.mlGLink.at(i + 1);

						foreach(GConnector * pGConnector, pGLink1->mlToGConnector)
						{
							if (pGConnector->mpToGLink != pGLink2) continue;
							if (!rmConnects.contains(pGConnector)) continue;

							QList<LCStruct*> lLC = pGRouting->mhLCStruct.values(pGConnector->mpConnector->connID);
							foreach(LCStruct * pLc, lLC)
							{
								result = removeRoutingLaneConnector(pGRouting->routingID, pGConnector->mpConnector->connID, pLc->fromLaneId, pLc->toLaneId) && result;
								if (!result) goto failed;
							}
							break;
						}
					}

				}
			}
		}

		result = removeConnector(rmConnects) && result;
		if (!result)goto failed;
		foreach(auto it, rmConnects)
		{
			gpScene->removeGConnector(it);
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove Connectors failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteLane(QList<long> ids)
{
	bool result = true;
	try {
		QList<GGuideArrow*> rmGGuideArrows;
		QList<GSignalLamp*> rmGSignalLamps;
		QList<GLane*> rmLanes;
		foreach(GLink * pGLink, gpScene->mlGLink) {
			foreach(auto lane, pGLink->mlGLane) {
				if (!ids.contains(lane->id())) continue;
				if (rmLanes.contains(lane)) continue;
				rmLanes.push_back(lane);
			}
		}
		foreach(GGuideArrow * pOne, gpScene->mlGGuideArrow) {
			if (!pOne->validate()) continue;
			if (rmGGuideArrows.contains(pOne)) continue;
			if (!ids.contains(pOne->mpGLane->id())) continue;
			rmGGuideArrows.push_back(pOne);
		}
		foreach(GSignalLamp * pGSl, gpScene->mlGSignalLamp) {
			if (ids.contains(pGSl->mpSignalLamp->laneID) ||
				ids.contains(pGSl->mpSignalLamp->toLaneID)) {
				rmGSignalLamps.push_back(pGSl);
			}
		}
		foreach(GDecisionPoint * pGDecisionPoint, gpScene->mlGDecisionPoint) {
			foreach(GRouting * pGRouting, pGDecisionPoint->mlGRouting) {
				foreach(OneRouting oneRouting, pGRouting->mlOneRouting) {
					if (oneRouting.mlGLink.size() == 0) break;

					for (int i = 0, size = oneRouting.mlGLink.size(); i < size - 1; ++i) {
						GLink* pGLink1 = oneRouting.mlGLink.at(i);
						GLink* pGLink2 = oneRouting.mlGLink.at(i + 1);

						foreach(GConnector * pGConnector, pGLink1->mlToGConnector)
						{
							if (pGConnector->mpToGLink != pGLink2) continue;
							QList<LCStruct*> lLC = pGRouting->mhLCStruct.values(pGConnector->mpConnector->connID);
							foreach(LCStruct * pLc, lLC)
							{
								if (ids.contains(pLc->fromLaneId) || ids.contains(pLc->toLaneId)) {
									result = removeRoutingLaneConnector(pGRouting->routingID, pGConnector->mpConnector->connID, pLc->fromLaneId, pLc->toLaneId) && result;
									if (!result) goto failed;
								}
							}
							break;
						}
					}

				}
			}
		}
		///车道连接
		foreach(GConnector * ptrConn, gpScene->mlGConnector) {
			result = removeLaneConnector(ptrConn->mlGLaneConnector) && result;
			if (!result) goto failed;
		}

		///删除导向箭头
		result = removeGuideArrow(rmGGuideArrows) && result;
		if (!result)goto failed;

		///删除信号灯
		result = removeSignalLamp(rmGSignalLamps) && result;
		if (!result)goto failed;

		result = removeLane(rmLanes) && result;
		if (!result)goto failed;

		foreach(auto it, rmGGuideArrows)
		{
			gpScene->removeGGuideArrow(it);
		}

		foreach(auto it, rmGSignalLamps) {
			gpScene->removeGSignalLamp(it);
		}

		foreach(auto it, rmLanes) {
			gpScene->removeGLane(it);
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove Lanes failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteLink(QList<long> ids) 
{
	bool result = true;
	try
	{
		QList<GLink*> rmLinks;
		QList<GConnector*> rmConnects;
		QList<GDecisionPoint*> rmDecisionPoints;
		QList<GGuideArrow*> rmGGuideArrows;
		QList<SignalGroup*> rmSignalGroups;
		QList<GSignalLamp*> rmGSignalLamps;
		QList<GDeparturePoint*> rmGDeparturePoints;
		QList<GVehicleDrivInfoCollector*> rmGVehicleDrivInfoCollectors;
		QList<GVehicleQueueCounter*> rmGVehicleQueueCounters;
		QList<GVehicleTravelDetector*> rmGVehicleTravelDetectors;
		QList<GReduceSpeedArea*> rmGReduceSpeedAreas;
		QList<GBusStation*> rmGBusStations;
		foreach(GLink * pGLink, gpScene->mlGLink) {
			if (!ids.contains(pGLink->mpLink->linkID)) continue;
			rmLinks.push_back(pGLink);
		}
		foreach(GConnector * ptrConn, gpScene->mlGConnector) {
			if (rmConnects.contains(ptrConn)) continue;
			if (ids.contains(ptrConn->fromLink()->id()) || ids.contains(ptrConn->toLink()->id())) {
				rmConnects.push_back(ptrConn);
			}
		}
		foreach(GDecisionPoint * pGDecisionPoint, gpScene->mlGDecisionPoint) {
			if (!ids.contains(pGDecisionPoint->link()->id())) continue;
			rmDecisionPoints.push_back(pGDecisionPoint);
		}
		foreach(GGuideArrow * pOne, gpScene->mlGGuideArrow) {
			if (!pOne->validate()) continue;
			if (!ids.contains(pOne->mpGLane->link()->id())) continue;
			rmGGuideArrows.push_back(pOne);
		}
		foreach(GSignalLamp * pGSl, gpScene->mlGSignalLamp) {
			bool flag = false;
			if (0 == pGSl->mpSignalLamp->toLaneID) {
				flag = checkContansLane(rmLinks, pGSl->mpSignalLamp->laneID);
			}
			else {
				flag = checkContansLane(rmConnects, pGSl->mpSignalLamp->laneID, pGSl->mpSignalLamp->toLaneID);
			}
			if (!flag) continue;
			SignalGroup* group = dynamic_cast<SignalGroup*>(pGSl->signalGroup());
			if (nullptr != group && !rmSignalGroups.contains(group)) {
				rmSignalGroups.append(group);
			}
			rmGSignalLamps.push_back(pGSl);
		}
		foreach(GDeparturePoint * pDp, gpScene->mlGDeparturePoint) {
			if (!ids.contains(pDp->link()->id())) continue;
			rmGDeparturePoints.push_back(pDp);
		}
		foreach(GVehicleDrivInfoCollector * pGCollecter, gpScene->mlGVehicleDrivInfoCollector) {
			if (!ids.contains(pGCollecter->link()->id())) continue;
			rmGVehicleDrivInfoCollectors.push_back(pGCollecter);
		}
		foreach(GVehicleQueueCounter * pGQueue, gpScene->mlGVehicleQueueCounter) {
			if (!ids.contains(pGQueue->mpGLane->link()->id())) continue;
			rmGVehicleQueueCounters.push_back(pGQueue);
		}
		foreach(GVehicleTravelDetector * pOne, gpScene->mlGVehicleTravelDetector) {
			if (pOne->mbStarted)continue;
			bool flag = true;
			if (0 == pOne->mpVehicleTravelDetector->start_toLaneNumber) {
				flag = ids.contains(pOne->mpVehicleTravelDetector->startRoadId) && flag;
			}
			else {
				flag = checkContainsConnector(rmConnects, pOne->mpVehicleTravelDetector->startRoadId) && flag;
			}

			if (0 == pOne->mpVehicleTravelDetector->teminal_toLaneNumber) {
				flag = ids.contains(pOne->mpVehicleTravelDetector->teminalRoadId) && flag;
			}
			else {
				flag = checkContainsConnector(rmConnects, pOne->mpVehicleTravelDetector->teminalRoadId) && flag;
			}
			if (!flag) continue;
			rmGVehicleTravelDetectors.push_back(pOne);
		}
		foreach(GReduceSpeedArea * pOne, gpScene->mlGReduceSpeedArea) {
			bool flag = false;
			if (0 == pOne->mpReduceSpeedArea->toLaneNumber) {
				flag = ids.contains(pOne->mpReduceSpeedArea->roadID);
			}
			else {
				flag = checkContainsConnector(rmConnects, pOne->mpReduceSpeedArea->roadID);
			}
			if (!flag) continue;
			rmGReduceSpeedAreas.push_back(pOne);
		}
		foreach(GBusStation * pGBusStation, gpScene->mlGBusStation) {
			if (!ids.contains(pGBusStation->link()->id())) continue;
			rmGBusStations.push_back(pGBusStation);
		}
		///删除路段相关数据
		result = removeLink(rmLinks) && result;
		if (!result)goto failed;

		///删除连接段相关数据
		result = removeConnector(rmConnects) && result;
		if (!result)goto failed;

		///删除决策点相关数据
		result = removeDecisionPoint(rmDecisionPoints) && result;
		if (!result)goto failed;

		///删除导向箭头
		result = removeGuideArrow(rmGGuideArrows) && result;
		if (!result)goto failed;

		///删除信号灯
		result = removeSignalLamp(rmGSignalLamps) && result;
		if (!result)goto failed;

		///删除信号灯组
		result = removeSignalGroup(rmSignalGroups) && result;
		if (!result)goto failed;

		///删除发车点
		result = removeDeparturePoint(rmGDeparturePoints) && result;
		if (!result)goto failed;

		///删除采集器
		result = removeDrivInfoCollector(rmGVehicleDrivInfoCollectors) && result;
		if (!result)goto failed;

		///删除排队计数器
		result = removeVehicleQueueCounter(rmGVehicleQueueCounters) && result;
		if (!result)goto failed;

		///删除行程时间检测器
		result = removeVehicleTravelDetector(rmGVehicleTravelDetectors) && result;
		if (!result)goto failed;

		///删除限速区
		result = removeReduceSpeedArea(rmGReduceSpeedAreas) && result;
		if (!result)goto failed;

		///删除公交站
		result = removeBusStation(rmGBusStations) && result;
		if (!result)goto failed;

		foreach(auto it, rmLinks)
		{
			gpScene->removeGLink(it);
		}

		foreach(auto it, rmConnects)
		{
			gpScene->removeGConnector(it);
		}

		foreach(auto it, rmDecisionPoints)
		{
			gpScene->removeGDecisionPoint(it);
		}

		foreach(auto it, rmGGuideArrows)
		{
			gpScene->removeGGuideArrow(it);
		}

		foreach(auto it, rmSignalGroups)
		{
			gpScene->removeSignalGroup(it);
		}

		foreach(auto it, rmGSignalLamps) {
			gpScene->removeGSignalLamp(it);
		}

		foreach(auto it, rmGDeparturePoints) {
			gpScene->removeGDeparturePoint(it);
		}

		foreach(auto it, rmGVehicleDrivInfoCollectors)
		{
			gpScene->removeGCollector(it);
		}
		foreach(auto it, rmGVehicleQueueCounters) {
			gpScene->removeGQueueCounter(it);
		}
		foreach(auto it, rmGVehicleTravelDetectors) {
			gpScene->removeGTravelDetector(it);
		}
		foreach(auto it, rmGReduceSpeedAreas) {
			gpScene->removeGReduceSpeedArea(it);
		}
		foreach(auto it, rmGBusStations) {
			gpScene->removeGBusStation(it);
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what();
		result = false;
	}
	catch (const std::exception& exc)
	{
		qWarning() << exc.what();
		result = false;
	}
	catch (...) {
		qWarning() << "remove link failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}
