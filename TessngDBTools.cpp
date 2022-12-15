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
#include "BusStation.h"
#include "VehicleConsDetail.h"

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
#include "GVehicleDetector.h"

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

bool TessngDBTools::deleteSignalColor(long id, QList<int> serialNnumbers)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();

		SignalPhase* dPhase = NULL;
		for(auto& group : gpScene->mlSignalGroup) {
			for (auto& phase : group->mlPhase) {
				if (phase->id() == id) dPhase = phase;
			}
		}
		if (!dPhase || dPhase->mlSignalColor.size() < 1) return false;

		//删除相位颜色
		result = removeSignalColor(dPhase, serialNnumbers);
		if (!result)goto failed;

		for (auto& serialNnumber : serialNnumbers) {
			dPhase->mlSignalColor.removeAt(serialNnumber);
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
		QList<GSignalLamp*> rmSignalLamp;
		for(SignalGroup * group : gpScene->mlSignalGroup) {
			for (SignalPhase* phase : group->mlPhase) {
				if (!ids.contains(phase->id())) continue;
				if (rmSignalPhases.contains(phase)) continue;
				rmSignalPhases.push_back(phase);
			}
		}
		for(GSignalLamp * it : gpScene->mlGSignalLamp) {
			if (!ids.contains(it->signalPhase()->id())) continue;
			if (rmSignalLamp.contains(it)) continue;
			rmSignalLamp.push_back(it);
		}

		//删除信号灯
		result = removeSignalLamp(rmSignalLamp);
		if (!result)goto failed;

		//删除相位
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
bool TessngDBTools::deleteBusStationPassengerArriving(QList<long> ids)
{
	bool result = true;
	try {
		gDB.transaction();
		//需要删除的乘客到站
		QList<PassengerArriving*> rmPassengerArriving;

		for (auto& busLine : gpScene->mlGBusLine) {
			for (auto& stationLine : busLine->mpBusLine->mlBusStationLine) {
				for (auto& passengerArriving : stationLine->mlPassengerArriving) {
					if (!ids.contains(passengerArriving->passengerArrivingID))continue;
					if (rmPassengerArriving.contains(passengerArriving))continue;

					rmPassengerArriving.push_back(passengerArriving);
				}
			}
		}

		///删除乘客到站
		result = removeBusStationPassengerArriving(rmPassengerArriving);
		if (!result) goto failed;

		///删除乘客到站内存
		for (auto& busLine : gpScene->mlGBusLine) {
			for (auto& stationLine : busLine->mpBusLine->mlBusStationLine) {
				for (int i = 0; i < stationLine->mlPassengerArriving.size();) {
					if (!ids.contains(stationLine->mlPassengerArriving[i]->passengerArrivingID)) {
						i++;
					}
					else {
						stationLine->mlPassengerArriving.removeOne(stationLine->mlPassengerArriving[i]);
					}
				}
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
		qWarning() << "remove BusStation failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteBusStationLine(QList<long> ids)
{
	bool result = true;
	try {
		gDB.transaction();
		//需要删除的公交站点线路
		QList<BusStationLine*> rmBusStationLine;

		for (auto& busLine : gpScene->mlGBusLine) {
			for (auto& stationLine : busLine->mpBusLine->mlBusStationLine) {
				if (!ids.contains(stationLine->id()))continue;
				if (rmBusStationLine.contains(stationLine))continue;

				rmBusStationLine.push_back(stationLine);
			}
		}

		///删除公交站点线路
		result = removeBusStationLine(rmBusStationLine);
		if (!result) goto failed;

		///删除公交站点线路内存
		for (auto& busLine : gpScene->mlGBusLine) {
			for (int i = 0; i < busLine->mpBusLine->mlBusStationLine.size();) {
				if (!ids.contains(busLine->mpBusLine->mlBusStationLine[i]->id())) {
					i++;
				}
				else {
					busLine->mpBusLine->mlBusStationLine.removeAt(i);
				}
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
		qWarning() << "remove BusStation failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteBusStation(QList<long> ids) {
	bool result = true;
	try {
		gDB.transaction();
		//需要删除的公交站
		QList<GBusStation*> rmBusTstation;
		//需要调整内存数据的公交线路
		QList<GBusLine*> updateBusLine;
		//需要删除的公交线路
		QList<BusStationLine*> rmBSLine;
		
		//得到所有需要删除的公交站
		foreach (auto& station, gpScene->mlGBusStation)
		{
			if (!ids.contains(station->id())) continue;
			if (rmBusTstation.contains(station)) continue;
			rmBusTstation.push_back(station);
		}

		//遍历所有场景中的公交站-线路关系
		foreach(auto bline, gpScene->mlGBusLine) {
			//检查该线路中是否存在要被删除的公交站，并记录该线路，等待修改内存数据
			foreach(auto& station, bline->stations()) {
				if (!ids.contains(station->id())) continue;
				if (updateBusLine.contains(bline)) continue;

				updateBusLine.push_back(bline);
			}
			//搜索该线路存在的所有公交站-线路关系中，所有与当前lineId相同，且stationId包含在ids里的关系记录
			foreach(auto& sline, bline->mpBusLine->mlBusStationLine) {
				if (rmBSLine.contains(sline)) continue;
				if(sline->lineId() != bline->id() || !ids.contains(sline->stationId())) continue;

				rmBSLine.push_back(sline);
			}
		}
		
		///删除公交站线路
		result = removeBusStationLine(rmBSLine);
		if (!result) goto failed;

		///删除公交站
		result = removeBusStation(rmBusTstation);
		if (!result) goto failed;

		foreach(auto busLine, gpScene->mlGBusLine) {
			for (int i = 0; i < busLine->mpBusLine->mlBusStationLine.size();) {
				if (busLine->mpBusLine->mlBusStationLine[i]->lineId() != busLine->id()
					|| !ids.contains(busLine->mpBusLine->mlBusStationLine[i]->stationId())) 
				{
					i++;
				}
				else 
				{
					busLine->mpBusLine->mlBusStationLine.removeAt(i);
				}
			}
		}
		foreach (auto it , rmBusTstation)
		{
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
		qWarning() << "remove BusStation failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteBusLine(QList<long> ids) {
	bool result = true;
	try {
		gDB.transaction();
		QList<GBusLine*> rmBusLine;
		QList<BusStationLine*> rmBSLine;
		foreach(auto bline, gpScene->mlGBusLine) {
			if (!ids.contains(bline->id())) continue;
			if (rmBusLine.contains(bline)) continue;
			rmBusLine.push_back(bline);
			rmBSLine.append(bline->mpBusLine->mlBusStationLine);
		}

		///删除公交站线路
		result = removeBusStationLine(rmBSLine);
		if (!result) goto failed;

		///删除公交线路
		result = removeBusLine(rmBusLine);
		if (!result) goto failed;

		foreach(auto& it, rmBusLine)
		{
			gpScene->removeGBusLine(it);
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
		qWarning() << "remove BusLine failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

//---------------------------------车辆运行及检测-------------------------------
bool TessngDBTools::deleteDrivInfoCollector(QList<long> ids) {
	bool result = true;
	try {
		gDB.transaction();
		QList<GVehicleDrivInfoCollector*> rmTemps;
		foreach(auto& it, gpScene->mlGVehicleDrivInfoCollector)
		{
			if (!ids.contains(it->id())) continue;
			if (rmTemps.contains(it)) continue;
			rmTemps.push_back(it);
		}

		result = removeDrivInfoCollector(rmTemps);
		if (!result) goto failed;

		foreach(auto& it, rmTemps) {
			gpScene->removeGCollector(it);
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
		qWarning() << "remove DrivInfoCollector failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteVehicleQueueCounter(QList<long> ids) {
	bool result = true;
	try {
		gDB.transaction();

		QList<GVehicleQueueCounter*> rmTemps;
		foreach(auto& it, gpScene->mlGVehicleQueueCounter)
		{
			if (!ids.contains(it->mpVehicleQueueCounter->queueCounterID)) continue;
			if (rmTemps.contains(it)) continue;
			rmTemps.push_back(it);
		}

		result = removeVehicleQueueCounter(rmTemps);
		if (!result) goto failed;

		foreach(auto& it, rmTemps) {
			gpScene->removeGQueueCounter(it);
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteVehicleTravelDetector(QList<long> ids) {
	bool result = true;
	try {
		gDB.transaction();
		QList<GVehicleTravelDetector*> rmTemps;
		foreach(auto& it, gpScene->mlGVehicleTravelDetector)
		{
			if (!ids.contains(it->mpVehicleTravelDetector->detectorId)) continue;
			if (rmTemps.contains(it)) continue;
			rmTemps.push_back(it);
		}

		result = removeVehicleTravelDetector(rmTemps);
		if (!result) goto failed;

		foreach(auto& it, rmTemps) {
			gpScene->removeGTravelDetector(it);
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
		qWarning() << "remove VehicleTravelDetector failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteGuideArrow(QList<long> ids) {
	bool result = true;
	try {
		gDB.transaction();
		QList<GGuideArrow*> rmTemps;
		foreach(auto& it, gpScene->mlGGuideArrow)
		{
			if (!ids.contains(it->mpGuideArrow->guideArrowID)) continue;
			if (rmTemps.contains(it)) continue;
			rmTemps.push_back(it);
		}

		result = removeGuideArrow(rmTemps);
		if (!result) goto failed;

		foreach(auto& it, rmTemps) {
			gpScene->removeGGuideArrow(it);
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
		qWarning() << "remove GuideArrow failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteReduceSpeedInterval(QList<long> ids)
{
	bool result = true;
	try {
		gDB.transaction();
		QList<GReduceSpeedArea*> updateArea;
		QList<ReduceSpeedInterval*> rmInterval;
		foreach(auto area, gpScene->mlGReduceSpeedArea)
		{
			bool find = false;
			//找到需要更改的限速区
			foreach(auto& it, area->mpReduceSpeedArea->mlReduceSpeedInterval) {
				if(!ids.contains(it->intervalID)) continue;
				if (updateArea.contains(area)) continue;

				find = true;
				updateArea.push_back(area);
				break;
			}

			if (!find) continue;

			//找到需要删除的时间间隔
			foreach(auto& it, area->mpReduceSpeedArea->mlReduceSpeedInterval) {
				if (!ids.contains(it->intervalID)) continue;
				if (rmInterval.contains(it)) continue;

				rmInterval.push_back(it);
			}
		}

		//删除时间间隔的数据库记录
		result = removeReduceSpeedInterval(rmInterval);
		if (!result) goto failed;

		//同步内存
		foreach(auto& area, updateArea) {
			for (int i = 0; i < area->mpReduceSpeedArea->mlReduceSpeedInterval.size();) {
				if (!ids.contains(area->mpReduceSpeedArea->mlReduceSpeedInterval[i]->intervalID)) {
					i++;
				}
				else {
					area->mpReduceSpeedArea->mlReduceSpeedInterval.removeAt(i);
				}
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

/**
	 * @brief 删除限速区车辆类型
	 * @param id					[限速区ID]
	 * @param departureIntervalDs	[车型编码]
	 *
	 * @return bool
*/
bool TessngDBTools::deleteReduceSpeedVehiType(long id, QList<long> vehicleTypeCodes)
{
	bool result = true;
	try {
		gDB.transaction();
		GReduceSpeedArea* updateArea = NULL;
		QList<ReduceSpeedVehiType*> rmReduceSpeedVehiTypes;
		foreach(auto& area, gpScene->mlGReduceSpeedArea)
		{
			if (id != area->mpReduceSpeedArea->reduceSpeedAreaID) continue;
			updateArea = area;

			foreach(auto& type, updateArea->mpReduceSpeedArea->mlReduceSpeedVehiType) {
				if (!vehicleTypeCodes.contains(type->vehicleTypeCode)) continue;

				rmReduceSpeedVehiTypes.push_back(type);
			}
		}
		if (!updateArea || vehicleTypeCodes.size() < 1) return false;

		//删除时间间隔的数据库记录
		result = removeReduceSpeedVehiType(rmReduceSpeedVehiTypes);
		if (!result) goto failed;

		//同步内存
		for (int i = 0; i < updateArea->mpReduceSpeedArea->mlReduceSpeedVehiType.size();) {
			if (!vehicleTypeCodes.contains(updateArea->mpReduceSpeedArea->mlReduceSpeedVehiType[i]->vehicleTypeCode)) {
				i++;
			}
			else {
				updateArea->mpReduceSpeedArea->mlReduceSpeedVehiType.removeAt(i);
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteReduceSpeedArea(QList<long> departureIntervalDs) {
	bool result = true;
	try {
		gDB.transaction();
		QList<GReduceSpeedArea*> rmArea;
		foreach(auto& it, gpScene->mlGReduceSpeedArea)
		{
			if (!departureIntervalDs.contains(it->mpReduceSpeedArea->reduceSpeedAreaID)) continue;
			if (rmArea.contains(it)) continue;
			rmArea.push_back(it);
		}

		result = removeReduceSpeedArea(rmArea);
		if (!result) goto failed;

		foreach(auto& it, rmArea) {
			gpScene->removeGReduceSpeedArea(it);
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

/**
	 * @brief 删除发车间隔
	 * @param id					[发车点ID]
	 * @param ids					[发车间隔ID]
	 *
	 * @return bool
*/
bool TessngDBTools::deleteDepartureInterval(long id, QList<long> departureIntervalDs)
{
	bool result = true;
	try {
		gDB.transaction();
		GDeparturePoint* updateTemp = NULL;
		QList<DepaInterval> rmTemps;
		foreach(auto& it, gpScene->mlGDeparturePoint)
		{
			if (id != it->id()) continue;
			updateTemp = it;

			for (auto& depaInterval : updateTemp->mpDeparturePoint->mlDepaInterval) {
				if (!departureIntervalDs.contains(depaInterval.departureIntervalD)) continue;
				rmTemps.push_back(depaInterval);
			}
		}
		if (!updateTemp || departureIntervalDs.size() < 1)return false;

		result = removeDepartureInterval(rmTemps);
		if (!result) goto failed;

		//同步内存
		for (int i = 0; i < updateTemp->mpDeparturePoint->mlDepaInterval.size();) {
			if (!departureIntervalDs.contains(updateTemp->mpDeparturePoint->mlDepaInterval.at(i).departureIntervalD)) {
				i++;
			}
			else {
				updateTemp->mpDeparturePoint->mlDepaInterval.removeAt(i);
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteDeparturePoint(QList<long> ids)
{
	bool result = true;
	try {
		gDB.transaction();
		QList<GDeparturePoint*> rmTemps;
		foreach(auto& it, gpScene->mlGDeparturePoint)
		{
			if (!ids.contains(it->mpDeparturePoint->departurePointID)) continue;
			if (rmTemps.contains(it)) continue;
			rmTemps.push_back(it);
		}

		result = removeDeparturePoint(rmTemps);
		if (!result) goto failed;

		foreach(auto& it, rmTemps) {
			gpScene->removeGDeparturePoint(it);
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

/**
	 * @brief 删除车型组成和占比
	 * @param id					[车型组成编号]
	 * @param vehicleTypeCodes		[车型编码]
	 *
	 * @return bool
*/
bool TessngDBTools::deleteVehicleConsDetail(long id, QList<long> vehicleTypeCodes)
{
	bool result = true;
	try {
		gDB.transaction();
		VehicleComposition updateVCposition;
		QList<VehicleConsDetail> rmTemps;
		foreach(auto& it, ghVehicleComposition)
		{
			if (id != it.vehicleConsCode) continue;
			updateVCposition = it;

			foreach(auto& detail, updateVCposition.mlVehicleConsDetail) {
				if (!vehicleTypeCodes.contains(detail.vehicleTypeCode)) continue;

				rmTemps.push_back(detail);
			}
		}
		if (updateVCposition.mlVehicleConsDetail.size() < 1 || vehicleTypeCodes.size() < 1) return false;

		result = removeVehicleConsDetail(rmTemps);
		if (!result) goto failed;

		//同步内存
		for (int i = 0; i < updateVCposition.mlVehicleConsDetail.size();) {
			if (!vehicleTypeCodes.contains(updateVCposition.mlVehicleConsDetail.at(i).vehicleTypeCode)) {
				i++;
			}
			else {
				updateVCposition.mlVehicleConsDetail.removeAt(i);
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

bool TessngDBTools::deleteVehicleConstitutent(QList<long> ids)
{
	bool result = true;
	try {
		gDB.transaction();
		QList<DepaInterval> rmDepaInterval;
		QList<VehicleComposition> rmTemps;
		
		foreach(auto& point, gpScene->mlGDeparturePoint)
		{
			foreach(auto& depaInterval, point->mpDeparturePoint->mlDepaInterval) {
				if(!ids.contains(depaInterval.vehiCons.vehicleConsCode)) continue;

				rmDepaInterval.push_back(depaInterval);
			}
		}
		foreach(auto it, ghVehicleComposition)
		{
			if (!ids.contains(it.vehicleConsCode)) continue;
			rmTemps.push_back(it);
		}

		//删除发车间隔
		result = removeDepartureInterval(rmDepaInterval);
		if (!result) goto failed;

		//删除车型组成
		result = removeVehicleConstitutent(rmTemps);
		if (!result) goto failed;

		//同步内存
		foreach(auto& point, gpScene->mlGDeparturePoint)
		{
			for (int i = 0; i < point->mpDeparturePoint->mlDepaInterval.size();) {
				if (!ids.contains(point->mpDeparturePoint->mlDepaInterval[i].vehiCons.vehicleConsCode)) {
					i++;
				}
				else {
					point->mpDeparturePoint->mlDepaInterval.removeAt(i);
				}
			}
		}
		foreach(auto it, rmTemps) {
			ghVehicleComposition.remove(it.vehicleConsCode);
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
		qWarning() << "remove ReduceSpeedArea failed! Unknow Error.";
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}
	return result;
}

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
