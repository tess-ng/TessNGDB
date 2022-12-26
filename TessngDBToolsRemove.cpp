#include "TessngDBToolsRemove.h"
#include "gitemtypes.hpp"
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
#include "TessngDBToolsUpdate.h"
#include <QException>
#include <QDebug>
TessngDBToolsRemove::TessngDBToolsRemove() :TessngDBToolsRemoveBase() {

}

//------------------------------------辅助函数----------------------------------
bool TessngDBToolsRemove::checkContansLane(const QList<GLink*>& list, long laneId)
{
    foreach(auto it, list) {
        foreach(auto lane, it->mlGLane)
        {
            if (laneId == lane->id()) return true;
        }
    }

    return false;
}

bool TessngDBToolsRemove::checkContansLane(const QList<GConnector*>& list, long fromlaneId, long tolaneId)
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

bool TessngDBToolsRemove::checkContainsConnector(const QList<GConnector*>& list, long id)
{
    foreach(auto it, list) {
        if (id == it->id()) return true;
    }
    return false;
}

//------------------------------------信号控制----------------------------------
bool TessngDBToolsRemove::deleteSignalLamp(QList<long> ids)
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
        if (!result)goto exitPoint;

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
        qWarning() << "remove SignalGroups exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsRemove::deleteSignalColor(long id, QList<int> serialNnumbers)
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
        if (!dPhase || dPhase->mlSignalColor.size() < 1 || serialNnumbers.size() < 1) return false;

        //删除相位颜色
        result = removeSignalColor(dPhase, serialNnumbers);
        if (!result)goto exitPoint;

        for (auto& serialNnumber : serialNnumbers) {
            dPhase->mlSignalColor.removeAt(serialNnumber - 1);
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
        qWarning() << "remove SignalGroups exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsRemove::deleteSignalPhase(QList<long> ids)
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
        if (!result)goto exitPoint;

        //删除相位
        result = removeSignalPhase(rmSignalPhases);
        if (!result)goto exitPoint;

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
        qWarning() << "remove SignalGroups exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsRemove::deleteSignalGroup(QList<long> ids)
{
    bool result = true;
    try {
        //开启事务
        gDB.transaction();

        QList<SignalGroup*> rmSignalGroup;
        QList<GSignalLamp*> rmSignalLamp;
        foreach(SignalGroup* pGSl, gpScene->mlSignalGroup) {
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
        if (!result)goto exitPoint;

        //删除信号灯组
        result = removeSignalGroup(rmSignalGroup) && result;
        if (!result)goto exitPoint;

        foreach(auto& it, rmSignalLamp)
        {
            gpScene->removeGSignalLamp(it);
        }
        foreach(auto& it, rmSignalGroup)
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
        qWarning() << "remove SignalGroups exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

//--------------------------------------公交------------------------------------
bool TessngDBToolsRemove::deleteBusStationPassengerArriving(QList<long> ids)
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
        if (!result) goto exitPoint;

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
        qWarning() << "remove BusStation exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteBusStationLine(QList<long> ids)
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
        if (!result) goto exitPoint;

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
        qWarning() << "remove BusStation exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteBusStation(QList<long> ids) {
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
        if (!result) goto exitPoint;

        ///删除公交站
        result = removeBusStation(rmBusTstation);
        if (!result) goto exitPoint;

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
        qWarning() << "remove BusStation exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteBusLine(QList<long> ids) {
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
        if (!result) goto exitPoint;

        ///删除公交线路
        result = removeBusLine(rmBusLine);
        if (!result) goto exitPoint;

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
        qWarning() << "remove BusLine exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

//---------------------------------车辆运行及检测-------------------------------
bool TessngDBToolsRemove::deleteDrivInfoCollector(QList<long> ids) {
    bool result = true;
    try {
        gDB.transaction();
        QList<GVehicleDrivInfoCollector*> rmVDICollector;
        foreach(auto& it, gpScene->mlGVehicleDrivInfoCollector)
        {
            if (!ids.contains(it->id())) continue;
            if (rmVDICollector.contains(it)) continue;
            rmVDICollector.push_back(it);
        }

        result = removeDrivInfoCollector(rmVDICollector);
        if (!result) goto exitPoint;

        foreach(auto& it, rmVDICollector) {
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
        qWarning() << "remove DrivInfoCollector exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteVehicleQueueCounter(QList<long> ids) {
    bool result = true;
    try {
        gDB.transaction();

        QList<GVehicleQueueCounter*> rmVQCounter;
        foreach(auto& it, gpScene->mlGVehicleQueueCounter)
        {
            if (!ids.contains(it->mpVehicleQueueCounter->queueCounterID)) continue;
            if (rmVQCounter.contains(it)) continue;
            rmVQCounter.push_back(it);
        }

        result = removeVehicleQueueCounter(rmVQCounter);
        if (!result) goto exitPoint;

        foreach(auto& it, rmVQCounter) {
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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteVehicleTravelDetector(QList<long> ids) {
    bool result = true;
    try {
        gDB.transaction();
        QList<GVehicleTravelDetector*> rmVTDetector;
        foreach(auto& it, gpScene->mlGVehicleTravelDetector)
        {
            if (!ids.contains(it->mpVehicleTravelDetector->detectorId)) continue;
            if (rmVTDetector.contains(it)) continue;
            rmVTDetector.push_back(it);
        }

        result = removeVehicleTravelDetector(rmVTDetector);
        if (!result) goto exitPoint;

        foreach(auto& it, rmVTDetector) {
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
        qWarning() << "remove VehicleTravelDetector exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteGuideArrow(QList<long> ids) {
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
        if (!result) goto exitPoint;

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
        qWarning() << "remove GuideArrow exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteReduceSpeedInterval(QList<long> ids)
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

        //删除时间间隔的
        result = removeReduceSpeedInterval(rmInterval);
        if (!result) goto exitPoint;

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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteReduceSpeedVehiType(long id, QList<long> vehicleTypeCodes)
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

        //删除时间间隔的
        result = removeReduceSpeedVehiType(rmReduceSpeedVehiTypes);
        if (!result) goto exitPoint;

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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteReduceSpeedArea(QList<long> departureIntervalDs) {
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
        if (!result) goto exitPoint;

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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteDepartureInterval(long id, QList<long> departureIntervalDs)
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
        if (!result) goto exitPoint;

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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteDeparturePoint(QList<long> ids)
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
        if (!result) goto exitPoint;

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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteVehicleConsDetail(long id, QList<long> vehicleTypeCodes)
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
        if (!result) goto exitPoint;

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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteVehicleConstitutent(QList<long> ids)
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
        if (!result) goto exitPoint;

        //删除车型组成
        result = removeVehicleConstitutent(rmTemps);
        if (!result) goto exitPoint;

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
        qWarning() << "remove ReduceSpeedArea exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

//-----------------------------------道路及连接---------------------------------
bool TessngDBToolsRemove::deleteRoutingFLowRatio(QList<long> ids)
{
    bool result = true;
    try {
        gDB.transaction();
        QList<RoutingFLowRatio*> rmTemps;
        //决策点
        foreach(auto& point, gpScene->mlGDecisionPoint) {
            //时间段
            foreach(auto& routingFlowByInterval, point->mpDecisionPoint->mlRoutingFlowByInterval) {
                //流量分配
                foreach(auto& routingFLowRatio, routingFlowByInterval->mlRoutingFlowRatio) {
                    if (!ids.contains(routingFLowRatio->RoutingFLowRatioID)) continue;
                    if (rmTemps.contains(routingFLowRatio)) continue;

                    rmTemps.push_back(routingFLowRatio);
                }
            }
        }

        //删除路径流量分配的
        result = removeRoutingFLowRatio(rmTemps);
        if (!result) goto exitPoint;

        //同步内存
        foreach(auto& point, gpScene->mlGDecisionPoint) {
            foreach(auto& routingFlowByInterval, point->mpDecisionPoint->mlRoutingFlowByInterval) {
                for (int i = 0; i < routingFlowByInterval->mlRoutingFlowRatio.size();) {
                    if (!ids.contains(routingFlowByInterval->mlRoutingFlowRatio[i]->RoutingFLowRatioID)) {
                        i++;
                    }
                    else {
                        routingFlowByInterval->mlRoutingFlowRatio.removeAt(i);
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
        qWarning() << "remove DrivInfoCollector exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteRoutingLaneConnector(long routingID, long connID, long fromLaneId, long toLaneId, bool isFix)
{
    bool result = true;
    try {
        gDB.transaction();

        for (auto& routing : gpScene->mlGRouting) {
            if (routing->id() != routingID) continue;
            //目标连接段的所有路径车道连接
            QList<LCStruct*> lcStructs = routing->mhLCStruct.values(connID);

            //仅有一条，且需要使用其他车道连接修复路径
            if (isFix && lcStructs.size() == 1) {
                for (auto& connector : gpScene->mlGConnector) {
                    if (connector->id() == connID && !connector->mlGLaneConnector.isEmpty())
                    {
                        //如果该连接段只有1条车道连接，则不对路径车道连接进行处理
                        if (connector->mlGLaneConnector.size() <= 1) break;

                        //如果该连接段有多条车道连接，则寻找From最小车道序号
                        GLaneConnector* laneConMin = connector->mlGLaneConnector[0];
                        for (auto& laneConnector : connector->mlGLaneConnector)
                        {
                            if (laneConMin->fromLane()->number() > laneConnector->fromLane()->number())
                            {
                                laneConMin = laneConnector;
                            }
                        }

                        // 修改路径车道连接
                        result = removeRoutingLaneConnector(routingID, connID, fromLaneId, toLaneId);
                        if (!result) goto exitPoint;
                        result = insertRoutingLaneConnector(routingID, connID, laneConMin->fromLane()->id(), laneConMin->toLane()->id());
                        if (!result) goto exitPoint;

                        //修改路径车道连接,同步内存数据一致
                        routing->mhLCStruct.value(connID)->fromLaneId = laneConMin->fromLane()->id();
                        routing->mhLCStruct.value(connID)->toLaneId = laneConMin->toLane()->id();
                    }
                }
            }
            //仅有一条，不需要修复，删除后续序列
            else if (lcStructs.size() == 1) {
                /*
                //寻找要删除的车道连接所在的连接段
                //TODO-仿真时mlRoad才有数据
                foreach(auto & item, routing->mlRoad) {
                    if (item->type() == GConnectorType && qgraphicsitem_cast<GConnector*>(item)->id() == connID) {
                        rmBeginItem = item;
                        rmRoutingConnectors.push_back(qgraphicsitem_cast<GConnector*>(item));
                        break;
                    }
                }

                //找到要清除的路径的连接段序列，路段序列
                //TODO-仿真时nextRoad才有数据
                bool isEndLink = false;
                while (!isEndLink) {
                    QGraphicsItem* nextRoad = routing->nextRoad(rmBeginConnector);

                    if (nextRoad->type() == GConnectorType) {
                        rmRoutingConnectors.push_back(qgraphicsitem_cast<GConnector*>(nextRoad));
                    }
                    else if (nextRoad->type() == GLinkType) {
                        GLink* nextLink = qgraphicsitem_cast<GLink*>(nextRoad);
                        rmRoutingLinks.push_back(nextLink);

                        if (nextLink->id() == routing->mpLastGLink->id()) {
                            isEndLink = true;
                        }
                    }

                    rmRoutingRoads.push_back(nextRoad);
                }*/
                //1.找到目标连接段
                GConnector* rmBeginConnector = NULL;
                QList<GConnector*> rmRoutingConnectors;
                foreach(auto& item, gpScene->mlGConnector) {
                    if (item->id() == connID) {
                        rmBeginConnector = item;
                        rmRoutingConnectors.push_back(item);
                        break;
                    }
                }
                if (!rmBeginConnector) break;

                //2.在路段序列中，从目标连接段的下游路段开始，到末尾，即要删除的序列
                QList<ILink*> rmRoutingLinks = routing->getLinks();
                for (int i = 0; i < rmRoutingLinks.size(); i++) {
                    if (rmBeginConnector->toLink()->id() != rmRoutingLinks[i]->id()) {
                        rmRoutingLinks.removeAt(i);
                    }
                    else {
                        break;
                    }
                }
                //3.场景里，上游路段和下游路段分别与两个需要删除的相邻序列路段相同的连接段，为需要删除的路径连接段序列
                foreach(auto& con, gpScene->mlGConnector) {
                    for (int i = 1; i < rmRoutingLinks.size(); i++) {
                        if (con->fromLink()->id() == rmRoutingLinks[i-1]->id() && con->toLink()->id() == rmRoutingLinks[i]->id()) {
                            rmRoutingConnectors.push_back(con);
                        }
                    }
                }

                //删除后续路径路段序列和路径车道连接的
                for (auto& connector : rmRoutingConnectors) {
                    result = removeRoutingLaneConnector(routingID, connector->id());
                    if (!result) goto exitPoint;
                }
                result = removeRoutingLink(routingID, rmRoutingLinks);
                if (!result) goto exitPoint;

                //同步内存
                //删除连接段序列
                for (auto& connector : rmRoutingConnectors) {
                    routing->mhLCStruct.remove(connector->id());
                }
                //删除路段序列
                for(int i = 0; i < routing->mlOneRouting.size();) {
                    //该最短序列的所有路段
                    foreach(auto& link, routing->mlOneRouting[i].mlGLink) {
                        foreach(auto& rmLink, rmRoutingLinks) {
                            if (rmLink->id() == link->id()) {
                                routing->mlOneRouting[i].mlGLink.removeOne(link);
                                break;
                            }
                        }
                    }
                    //如果删除后最短序列不存在路段
                    if (routing->mlOneRouting[i].mlGLink.isEmpty()) {
                        routing->mlOneRouting.removeAt(i);
                    }
                    else {
                        i++;
                    }
                }
                //TODO
                //是否判断序列里只存在一个路段，此时路径是否需要删除
            }
            //有多条，无需其他处理
            else {
                //删除路径车道连接
                result = removeRoutingLaneConnector(routingID, connID, fromLaneId, toLaneId);
                if (!result) goto exitPoint;

                for (auto& lcStruct : lcStructs) {
                    if (lcStruct->fromLaneId == fromLaneId && lcStruct->toLaneId == toLaneId) {
                        routing->mhLCStruct.remove(connID, lcStruct);
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
        qWarning() << "remove DrivInfoCollector exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteRouting(QList<long> ids)
{
    bool result = true;
    try {
        gDB.transaction();

        QList<GRouting*> rmRouteings;
        foreach(auto& it, gpScene->mlGRouting)
        {
            if (!ids.contains(it->id())) continue;
            if (rmRouteings.contains(it)) continue;
            rmRouteings.push_back(it);
        }

        result = removeRouting(rmRouteings);
        if (!result)goto exitPoint;

        foreach(auto& it, rmRouteings)
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
        qWarning() << "remove Routes exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteDecisionPoint(QList<long> ids)
{
    bool result = true;
    try {
        gDB.transaction();

        QList<GDecisionPoint*> rmDecisionPoint;
        foreach(auto& it, gpScene->mlGDecisionPoint)
        {
            if (!ids.contains(it->id())) continue;
            if (rmDecisionPoint.contains(it)) continue;
            rmDecisionPoint.push_back(it);
        }

        result = removeDecisionPoint(rmDecisionPoint);
        if (!result)goto exitPoint;

        foreach(auto & it, rmDecisionPoint)
        {
            gpScene->removeGDecisionPoint(it);
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
        qWarning() << "remove Routes exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteLaneConnector(long connID, long fromLaneId, long toLaneId)
{
    bool result = true;
    try {
        gDB.transaction();

        GConnector* connector = gpScene->findGConnectorByConnID(connID);
        GLaneConnector* laneConnector = gpScene->findGLaneConnectorByID(fromLaneId, toLaneId);
        GLane* fromLane = gpScene->findGLaneByLaneID(fromLaneId);
        GLane* toLane = gpScene->findGLaneByLaneID(toLaneId);
        int fromLaneNumber = fromLane->number();
        int toLaneNumber = toLane->number();
        if(!fromLane || !toLane || !connector)goto exitPoint;

        //需要删除的限速区
        QList<GReduceSpeedArea*> rmArea = gpScene->lGReduceSpeedAreaOnLaneObject(qgraphicsitem_cast<GraphicsObject*>(laneConnector));
        //需要删除的行程时间检测器
        QList<GVehicleTravelDetector*> rmVTDetector;
        foreach(auto& it, gpScene->mlGVehicleTravelDetector)
        {
            if (it->mpVehicleTravelDetector->startRoadId == connector->fromLink()->id() &&
                it->mpVehicleTravelDetector->start_laneNumber == fromLaneNumber && 
                it->mpVehicleTravelDetector->start_toLaneNumber == toLaneNumber 
                ||
                it->mpVehicleTravelDetector->teminalRoadId == connector->toLink()->id() &&
                it->mpVehicleTravelDetector->teminal_laneNumber == fromLaneNumber &&
                it->mpVehicleTravelDetector->teminal_toLaneNumber == toLaneNumber) {
                rmVTDetector.push_back(it);
            }
        }
        //需要删除的排队计数器
        QList<GVehicleQueueCounter*> rmVQCounter;
        foreach(auto& it, gpScene->mlGVehicleQueueCounter)
        {
            if (it->mpVehicleQueueCounter->roadID == connID && 
                it->mpVehicleQueueCounter->laneNumber == fromLaneNumber && 
                it->mpVehicleQueueCounter->toLaneNumber) {
                rmVQCounter.push_back(it);
            }
        }
        //需要删除的车辆信息采集器
        QList<GVehicleDrivInfoCollector*> rmVDICollector;
        foreach(auto& it, gpScene->mlGVehicleDrivInfoCollector)
        {
            if (it->mpVehicleDrivInfoCollector->roadID == connID &&
                it->mpVehicleDrivInfoCollector->laneNumber == fromLaneNumber &&
                it->mpVehicleDrivInfoCollector->toLaneNumber) {
                rmVDICollector.push_back(it);
            }
        }
        //需要删除的信号灯
        QList<GSignalLamp*> rmSignalLamp;
        foreach(auto& it, gpScene->mlGSignalLamp)
        {
            if (it->mpSignalLamp->laneID == fromLaneId && it->mpSignalLamp->toLaneID == toLaneId) {
                rmSignalLamp.push_back(it);
            }
        }
        //需要删除的车道连接
        QList<GLaneConnector*> rmLaneConnector;
        rmLaneConnector.push_back(laneConnector);

        //删除限速区
        result = removeReduceSpeedArea(rmArea);
        if (!result) goto exitPoint;
        //删除行程时间检测器
        result = removeVehicleTravelDetector(rmVTDetector);
        if (!result) goto exitPoint;
        //删除排队计数器
        result = removeVehicleQueueCounter(rmVQCounter);
        if (!result) goto exitPoint;
        //删除车辆信息采集器
        result = removeDrivInfoCollector(rmVDICollector);
        if (!result) goto exitPoint;
        //删除信号灯
        result = removeSignalLamp(rmSignalLamp);
        if (!result) goto exitPoint;
        //删除车道连接
        result = removeLaneConnector(rmLaneConnector);
        if (!result) goto exitPoint;

        //删除限速区内存
        foreach(auto& it, rmArea)
        {
            gpScene->removeGReduceSpeedArea(it);
        }
        //删除行程时间检测器内存
        foreach(auto& it, rmVTDetector)
        {
            gpScene->removeGTravelDetector(it);
        }
        //删除排队计数器内存
        foreach(auto& it, rmVQCounter)
        {
            gpScene->removeGQueueCounter(it);
        }
        //删除车辆信息采集器内存
        foreach(auto& it, rmVDICollector)
        {
            gpScene->removeGCollector(it);
        }
        //删除信号灯内存
        foreach(auto& it, rmSignalLamp)
        {
            gpScene->removeGSignalLamp(it);
        }
        //删除车道连接内存
        foreach(auto& it, gpScene->mlGConnector)
        {
            if (it->id() == connID) {
                connector = it;
            }
        }
        foreach(auto& it, rmLaneConnector)
        {
            for (int i = 0; i < connector->mpConnector->mlLaneConnector.size();) {
                if (connector->mpConnector->mlLaneConnector[i]->mpFromLane->laneID == fromLaneId &&
                    connector->mpConnector->mlLaneConnector[i]->mpToLane->laneID == toLaneId) {
                    connector->mpConnector->mlLaneConnector.removeAt(i);
                }
                else {
                    i++;
                }
            }
            for (int i = 0; i < connector->mlGLaneConnector.size();) {
                if (connector->mlGLaneConnector[i]->fromLane()->id() == fromLaneId &&
                    connector->mlGLaneConnector[i]->toLane()->id() == toLaneId) {
                    connector->mlGLaneConnector.removeAt(i);
                }
                else {
                    i++;
                }
            }
            for (int i = 0; i < gpScene->mlGLaneConnector.size();) {
                if (gpScene->mlGLaneConnector[i]->fromLane()->id() == fromLaneId && gpScene->mlGLaneConnector[i]->toLane()->id() == toLaneId) {
                    gpScene->mlGLaneConnector.removeAt(i);
                }
                else {
                    i++;
                }
            }
        }

        //删除并保护路径车道连接
        for (int i = 0; i < gpScene->mlGRouting.size(); i++)
        {
            QList<LCStruct*> lcStructs = gpScene->mlGRouting[i]->mhLCStruct.values(connID);
            for (auto& lcStruct : lcStructs)
            {
                if (lcStruct->fromLaneId == fromLaneId && lcStruct->toLaneId == toLaneId)
                {
                    //删除路径车道连接
                    result = removeRoutingLaneConnector(gpScene->mlGRouting[i]->id(), connID, fromLaneId, toLaneId);
                    if (!result) goto exitPoint;

                    if (lcStructs.size() == 1) {
                        if (!connector->mlGLaneConnector.isEmpty())
                        {
                            //寻找From最小车道序号
                            GLaneConnector* laneConMinFrom = connector->mlGLaneConnector[0];
                            for (auto& laneConnector : connector->mlGLaneConnector)
                            {
                                if (laneConMinFrom->fromLane()->number() > laneConnector->fromLane()->number())
                                {
                                    laneConMinFrom = laneConnector;
                                }
                            }

                            GLane* laneFromMin = gpScene->findGLaneByLaneID(laneConMinFrom->fromLane()->id());
                            QList<GLaneConnector*> laneCons = gpScene->findGLaneConnectorByFromGLaneAndToGLink(laneFromMin, GLink::iToGLink(connector->toLink()));

                            GLaneConnector* laneConMinTo = laneCons[0];
                            for (auto& laneConnector : laneCons) {
                                if (laneConMinTo->fromLane()->number() > laneConnector->fromLane()->number())
                                {
                                    laneConMinTo = laneConnector;
                                }
                            }

                            //插入路径车道连接
                            //result = insertRoutingLaneConnector(gpScene->mlGRouting[i]->id(), connID, laneConMinFrom->fromLane()->id(), laneConMinTo->toLane()->id());

                            //修改路径车道连接
                            gpScene->mlGRouting[i]->mhLCStruct.value(connID)->fromLaneId = laneConMinFrom->fromLane()->id();
                            gpScene->mlGRouting[i]->mhLCStruct.value(connID)->toLaneId = laneConMinTo->toLane()->id();
                        }
                        else {
                            //同步内存
                            gpScene->mlGRouting[i]->mhLCStruct.remove(connID);
                        }
                    }
                    else {
                        //同步内存
                        gpScene->mlGRouting[i]->mhLCStruct.remove(connID, lcStruct);
                    }
                }
            }
        }
        

        //判断连接段是否还存在连接并处理
        if (connector->mlGLaneConnector.isEmpty())
        {
            //删除连接段之前要删除经过这个连接段的所有路径的一部分序列
            for (int i = 0; i < gpScene->mlGRouting.size(); i++) {
                QList<ILink*> rmLinks = gpScene->mlGRouting[i]->getLinks();

                for (int j = 0; j < rmLinks.size() - 1;) {
                    if (rmLinks[i]->id() == connector->fromLink()->id() && rmLinks[i + 1]->id() == connector->toLink()->id()) {
                        rmLinks.removeAt(j);
                        break;
                    }
                    else {
                        rmLinks.removeAt(j);
                    }
                }

                //删除数据库记录
                result = removeRoutingLink(gpScene->mlGRouting[i]->id(), rmLinks);
                if (!result) goto exitPoint;

                //处理内存
                for (int j = 0; j < gpScene->mlGRouting[i]->mlOneRouting.size(); j++) {
                    foreach(auto& link, gpScene->mlGRouting[i]->mlOneRouting[j].mlGLink) {
                        foreach(auto & rmLink, rmLinks) {
                            if (rmLink->id() == link->id()) {
                                gpScene->mlGRouting[i]->mlOneRouting[j].mlGLink.removeOne(link);
                                break;
                            }
                        }
                    }
                    //如果删除后最短序列不存在路段
                    if (gpScene->mlGRouting[i]->mlOneRouting[j].mlGLink.isEmpty()) {
                        gpScene->mlGRouting[i]->mlOneRouting.removeAt(j);
                    }
                    else {
                        j++;
                    }
                }
                
            }

            //删除连接段之前要删除经过这个连接段的所有公交线路的一部分序列
            for (int i = 0; i < gpScene->mlGBusLine.size(); i++) {
                QList<GLink*> tempBusLineLinks = gpScene->mlGBusLine[i]->mlGLink;

                for (int j = 0; j < tempBusLineLinks.size() - 1;) {
                    if (tempBusLineLinks[i]->id() == connector->fromLink()->id() && tempBusLineLinks[i + 1]->id() == connector->toLink()->id()) {
                        tempBusLineLinks.removeAt(j);
                        break;
                    }
                    else {
                        tempBusLineLinks.removeAt(j);
                    }
                }

                //删除数据库记录
                result = removeBusLineLink(gpScene->mlGRouting[i]->id(), tempBusLineLinks);
                if (!result) goto exitPoint;

                //处理内存
                for (auto& rm : tempBusLineLinks) {
                    gpScene->mlGBusLine[i]->mlGLink.removeOne(rm);
                }
            }


            QList<GConnector*> rmCon;
            rmCon.push_back(connector);
            result = removeConnector(rmCon);
            if (!result) goto exitPoint;

            gpScene->removeGConnector(connector);
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
        qWarning() << "remove DrivInfoCollector exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteConnectors(QList<long> ids)
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
                                if (!result) goto exitPoint;
                            }
                            break;
                        }
                    }

                }
            }
        }

        result = removeConnector(rmConnects) && result;
        if (!result)goto exitPoint;

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
        qWarning() << "remove Connectors exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteLane(QList<long> ids,bool fixed)
{
    bool result = true;
    try {
        /// <summary>
        /// 车道删除步骤
        /// 1.根据提供的车道id 删除信号灯(SignalLamp)
        /// 2.根据车道id 删除公交站点(BusStation)
        /// 3.根据车道id 删除限速区(ReduceSpeedArea)
        /// 4.根据车道id 删除行程时间检测器(VehicleTravelDetector)
        /// 5.根据车道id 删除排队计数器(VehicleQueueCounter)
        /// 6.根据车道id 删除车辆信息采集器(VehicleDrivInfoCollecter)
        /// 7.根据车道id 删除导向箭头(GuideArrow)
        /// 8.根据车道id 删除车道连接(LaneConnector)
        /// 9.根据车道id 删除路径车道连接(RoutingLaneConnector) 如果当前路径车道连接条数<=1 需要建立一条路径车道连接
        /// 10.根据车道id 删除指定车道 同时车道序号重新排序 如果当前路段车道全部删除干净则还需要删除该路段
        /// </summary>
        /// <param name="ids"></param>
        /// <returns></returns>
        QList<GGuideArrow*> rmGGuideArrows;
        QList<GSignalLamp*> rmGSignalLamps;
        QList<GLane*> rmLanes;
        QList<GBusStation*> rmBusStations;
        QList<GReduceSpeedArea*> rmGReduceSpeedAreas;
        QList<GVehicleTravelDetector*> rmVehicleTravelDetector;
        QList<GVehicleQueueCounter*> rmVehicleQueueCounters;
        QList<GVehicleDrivInfoCollector*> rmVehicleDrivInfoCollectors;
        QList<LaneConnector*> rmLaneConnectors;
        QList<GLink*> changLinks;
        QList<long> rmLinks;
        foreach(auto link, gpScene->mlGLink) {
            QList<GLane*> lanes=link->mlGLane;
            foreach (auto lane, link->mlGLane) {
                if (!ids.contains(lane->id())) continue;

                lanes.removeOne(lane);

                if (rmLanes.contains(lane)) continue;
                rmLanes.push_back(lane);

                if(!changLinks.contains(lane->mpGLink))changLinks.append(lane->mpGLink);
            }
            if(lanes.isEmpty()) {
                rmLinks.append(link->mpLink->linkID);
                if(changLinks.contains(link)) changLinks.removeOne(link);
            }
        }
        foreach(auto bs, gpScene->mlGBusStation) {
            if (rmBusStations.contains(bs)) continue;
            if (!rmLanes.contains(bs->mpGLane)) continue;
            int index=rmLanes.indexOf(bs->mpGLane);
            if (index < 0) continue;
            if (rmLanes[index]->mpGLink->mpLink->linkID != bs->mpGLink->mpLink->linkID) continue;
            rmBusStations.append(bs);
        }


        foreach(auto lane, rmLanes)
        {
            foreach(auto it, gpScene->mlGReduceSpeedArea)
            {
                if (rmGReduceSpeedAreas.contains(it))continue;
                if (it->mpReduceSpeedArea->roadID != lane->mpGLink->mpLink->linkID) continue;
                if (it->mpReduceSpeedArea->laneNumber == lane->mpLane->serialNumber ||
                    it->mpReduceSpeedArea->toLaneNumber == lane->mpLane->serialNumber) {
                    rmGReduceSpeedAreas.push_back(it);
                    break;
                }
            }
        }
        foreach(auto lane, rmLanes)
        {
            foreach(auto it, gpScene->mlGVehicleTravelDetector)
            {
                if (rmVehicleTravelDetector.contains(it))continue;
                if (it->mpVehicleTravelDetector->startRoadId == lane->mpGLink->mpLink->linkID && (it->mpVehicleTravelDetector->start_laneNumber ==
                    lane->mpLane->serialNumber || it->mpVehicleTravelDetector->start_toLaneNumber == lane->mpLane->serialNumber)) {
                    rmVehicleTravelDetector.push_back(it);
                    break;
                }

                if (it->mpVehicleTravelDetector->teminalRoadId == lane->mpGLink->mpLink->linkID && (it->mpVehicleTravelDetector->teminal_laneNumber ==
                    lane->mpLane->serialNumber || it->mpVehicleTravelDetector->teminal_toLaneNumber == lane->mpLane->serialNumber)) {
                    rmVehicleTravelDetector.push_back(it);
                    break;
                }
            }

        }
        foreach(auto lane, rmLanes)
        {
            foreach(auto it, gpScene->mlGVehicleQueueCounter)
            {
                if (rmVehicleQueueCounters.contains(it))continue;
                if (it->mpGLane != lane) continue;
                rmVehicleQueueCounters.push_back(it);
            }
        }

        foreach(auto lane, rmLanes)
        {
            foreach(auto it, gpScene->mlGVehicleDrivInfoCollector)
            {
                if (rmVehicleDrivInfoCollectors.contains(it))continue;
                if (it->mpGLane != lane) continue;
                rmVehicleDrivInfoCollectors.push_back(it);
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
        ///车道连接
        foreach(auto lane, rmLanes)
        {
            foreach(GConnector * ptrConn, gpScene->mlGConnector) {
                if (ptrConn->mpFromGLink != lane->mpGLink && ptrConn->mpToGLink != lane->mpGLink) continue;
                foreach(auto lc, ptrConn->mpConnector->mlLaneConnector) {
                    if (rmLaneConnectors.contains(lc)) continue;
                    if (lc->mpFromLane != lane->mpLane && lc->mpToLane != lane->mpLane) continue;
                    rmLaneConnectors.push_back(lc);
                }
            }
        }
        foreach(GRouting* pGRouting, gpScene->mlGRouting) {
        }

        result =removeBusStation(rmBusStations);
        if (!result)goto exitPoint;

        result=removeReduceSpeedArea(rmGReduceSpeedAreas);
        if (!result)goto exitPoint;

        result =removeVehicleTravelDetector(rmVehicleTravelDetector);
        if (!result)goto exitPoint;

        result =removeVehicleQueueCounter(rmVehicleQueueCounters);
        if (!result)goto exitPoint;

        result =removeDrivInfoCollector(rmVehicleDrivInfoCollectors);
        if (!result)goto exitPoint;

        result =removeLaneConnector(rmLaneConnectors);
        if (!result)goto exitPoint;
        ///删除导向箭头
        result = removeGuideArrow(rmGGuideArrows) && result;
        if (!result)goto exitPoint;

        ///删除信号灯
        result = removeSignalLamp(rmGSignalLamps) && result;
        if (!result)goto exitPoint;

        result = removeLane(rmLanes) && result;
        if (!result)goto exitPoint;

        result =deleteLink(rmLinks,false);
        if (!result)goto exitPoint;

        foreach (auto it, changLinks) {
            QList<GLane*> lans=it->mlGLane;
            int nun=1;
            foreach (auto lane, lans) {
                lane->mpLane->serialNumber=nun;
                nun++;
            }
            //TessngDBToolsUpdate::getInstance()->updateLanes(lans);
        }

        foreach (auto it, rmBusStations) {
            gpScene->removeGBusStation(it);
        }

        foreach (auto it, rmGReduceSpeedAreas) {
            gpScene->removeGReduceSpeedArea(it);
        }

        foreach (auto it, rmVehicleTravelDetector) {
            gpScene->removeGTravelDetector(it);
        }

        foreach (auto it, rmVehicleQueueCounters) {
            gpScene->removeGQueueCounter(it);
        }

        foreach (auto it, rmVehicleDrivInfoCollectors) {
            gpScene->removeGCollector(it);
        }

        foreach (auto it, rmLaneConnectors) {
            GConnector* gc=gpScene->findGConnectorByConnID(it->mpConnector->connID);
            gc->mlGLaneConnector.removeOne(gpScene->findGLaneConnectorByID(it->mpFromLane->laneID,it->mpToLane->laneID));
        }

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
        qWarning() << "remove Lanes exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}

bool TessngDBToolsRemove::deleteLinkVertex(long id, QList<int> nums)
{
    bool result = true;
    try {
        //开启事务
        gDB.transaction();

        QList<GSignalLamp*> rmSignalLamp;
        foreach(GSignalLamp * it, gpScene->mlGSignalLamp) {
        }

        //删除顶点
        //result = removeSignalLamp(rmSignalLamp);
        if (!result)goto exitPoint;

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
        qWarning() << "remove SignalGroups exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsRemove::deleteLink(QList<long> ids,bool clearCache)
{
    bool result = true;
    try
    {
        gDB.transaction();

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
        if (!result)goto exitPoint;

        ///删除连接段相关数据
        result = removeConnector(rmConnects) && result;
        if (!result)goto exitPoint;

        ///删除决策点相关数据
        result = removeDecisionPoint(rmDecisionPoints) && result;
        if (!result)goto exitPoint;

        ///删除导向箭头
        result = removeGuideArrow(rmGGuideArrows) && result;
        if (!result)goto exitPoint;

        ///删除信号灯
        result = removeSignalLamp(rmGSignalLamps) && result;
        if (!result)goto exitPoint;

        ///删除信号灯组
        result = removeSignalGroup(rmSignalGroups) && result;
        if (!result)goto exitPoint;

        ///删除发车点
        result = removeDeparturePoint(rmGDeparturePoints) && result;
        if (!result)goto exitPoint;

        ///删除采集器
        result = removeDrivInfoCollector(rmGVehicleDrivInfoCollectors) && result;
        if (!result)goto exitPoint;

        ///删除排队计数器
        result = removeVehicleQueueCounter(rmGVehicleQueueCounters) && result;
        if (!result)goto exitPoint;

        ///删除行程时间检测器
        result = removeVehicleTravelDetector(rmGVehicleTravelDetectors) && result;
        if (!result)goto exitPoint;

        ///删除限速区
        result = removeReduceSpeedArea(rmGReduceSpeedAreas) && result;
        if (!result)goto exitPoint;

        ///删除公交站
        result = removeBusStation(rmGBusStations) && result;
        if (!result)goto exitPoint;

        if(!clearCache) goto exitPoint;

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
        qWarning() << "remove link exitPoint! Unknow Error.";
        result = false;
    }
exitPoint:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }
    return result;
}
