#include "TessngDBToolsRemoveBase.h"
#include "DbGlobal.h"
#include "exception.h"

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
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSet>
#include <QSqlDatabase>
TessngDBToolsRemoveBase::TessngDBToolsRemoveBase()
{

}

TessngDBToolsRemoveBase::~TessngDBToolsRemoveBase()
{

}

//------------------------------------信号控制----------------------------------
/// 信号灯
bool TessngDBToolsRemoveBase::removeSignalLamp(const QList<GSignalLamp*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeSignalLamp(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeSignalLamp(GSignalLamp* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from SignalLamp where signalLampID=%1;)").arg(it->mpSignalLamp->signalLampID);
    result = slQuery.exec(deleteSql);
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

///  相位信号颜色
bool TessngDBToolsRemoveBase::removeSignalColor(const SignalPhase* signalPhase, QList<int> serialNumbers)
{
    bool result = true;
    for (auto& it : serialNumbers) {
        result = removeSignalColor(signalPhase, it);
        if (!result) return false;
    }

    QSqlQuery slQuery(gDB);
    QString updateSql = QString(R"(update SignalColor set serialNumber=serialNumber-%1 where signalPhaseID=%2 and serialNumber>%3;)")
        .arg(serialNumbers.size()).arg(signalPhase->signalPhaseID).arg(serialNumbers.back());
    result = slQuery.exec(updateSql);
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsRemoveBase::removeSignalColor(const SignalPhase* signalPhase, int serialNumber)
{
    QSqlQuery slQuery(gDB);
    QString deleteSql = QString(R"(delete from SignalColor where signalPhaseID=%1 and serialNumber=%2;)").arg(signalPhase->signalPhaseID).arg(serialNumber);
    bool result = slQuery.exec(deleteSql);
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 相位
bool TessngDBToolsRemoveBase::removeSignalPhase(const QList<SignalPhase*>& list)
{
    bool result = true;
    QSqlQuery slQuery(gDB);
    for (auto& it : list) {
        result = removeSignalPhase(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeSignalPhase(SignalPhase* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QList<int> serialNumbers;
    for (int i = 1; i <= it->mlSignalColor.size(); i++) {
        serialNumbers.push_back(i);
    }
    result = removeSignalColor(it, serialNumbers);
    if (!result) return false;
    QString deleteSql = QString(R"(delete from SignalPhase where signalPhaseID=%1;)").arg(it->signalPhaseID);
    result = slQuery.exec(deleteSql) && result;
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 信号灯组
bool TessngDBToolsRemoveBase::removeSignalGroup(const QList<SignalGroup*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeSignalGroup(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeSignalGroup(SignalGroup* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeSignalPhase(it->mlPhase);
    if (!result) return false;
    QString deleteSql = QString(R"(delete from SignalGroup where signalGroupID=%1;)").arg(it->signalGroupID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

//--------------------------------------公交------------------------------------
/// 乘客到站
bool TessngDBToolsRemoveBase::removeBusStationPassengerArriving(const QList<PassengerArriving*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusStationPassengerArriving(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeBusStationPassengerArriving(PassengerArriving* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from BusStationPassengerArriving where passengerArrivingID=%1;)").arg(it->passengerArrivingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交站点-线路
bool TessngDBToolsRemoveBase::removeBusStationLine(const QList<BusStationLine*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusStationLine(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeBusStationLine(BusStationLine* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeBusStationPassengerArriving(it->mlPassengerArriving);
    if (!result) return false;
    QString deleteSql = QString(R"(delete from BusStationLine where stationLineID=%1;)").arg(it->stationLineID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交线路的路段或连接段序列
bool TessngDBToolsRemoveBase::removeBusLineRoad(const QList<GBusLine*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusLineRoad(it->mpBusLine);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeBusLineRoad(BusLine* it)
{
    bool result = true;
    QSqlQuery slQuery(gDB);

    QString deleteSql = QString(R"(delete from BusLineRoad where busLineID=%1;)").arg(it->busLineID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交线路
bool TessngDBToolsRemoveBase::removeBusLine(const QList<GBusLine*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusLine(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeBusLine(GBusLine* it)
{
    bool result = true;
    QSqlQuery slQuery(gDB);

    result = removeBusLineRoad(it->mpBusLine);
    if (!result) return false;
    QString deleteSql = QString(R"(delete from BusLine where busLineID=%1;)").arg(it->mpBusLine->busLineID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交站
bool TessngDBToolsRemoveBase::removeBusStation(const QList<GBusStation*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusStation(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeBusStation(GBusStation* it)
{
    bool result = true;
    QSqlQuery slQuery(gDB);

    QString deleteSql = QString(R"(delete from BusStation where busStationID=%1;)").arg(it->mpBusStation->busStationID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

//---------------------------------车辆运行及检测-------------------------------
/// 车辆信息采集器
bool TessngDBToolsRemoveBase::removeDrivInfoCollector(const QList<GVehicleDrivInfoCollector*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeDrivInfoCollector(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeDrivInfoCollector(GVehicleDrivInfoCollector* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleDrivInfoCollecter where collecterID=%1;)").arg(it->mpVehicleDrivInfoCollector->collecterID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 排队计数器
bool TessngDBToolsRemoveBase::removeVehicleQueueCounter(const QList<GVehicleQueueCounter*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleQueueCounter(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeVehicleQueueCounter(GVehicleQueueCounter* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleQueueCounter where queueCounterID=%1;)").arg(it->mpVehicleQueueCounter->queueCounterID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 行程时间检测器
bool TessngDBToolsRemoveBase::removeVehicleTravelDetector(const QList<GVehicleTravelDetector*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleTravelDetector(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeVehicleTravelDetector(GVehicleTravelDetector* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleTravelDetector where detectorId=%1;)").arg(it->mpVehicleTravelDetector->detectorId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 导向箭头
bool TessngDBToolsRemoveBase::removeGuideArrow(const QList<GGuideArrow*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeGuideArrow(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeGuideArrow(GGuideArrow* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from GuideArrow where guideArrowId=%1;)").arg(it->mpGuideArrow->guideArrowID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 限速区时间间隔
bool TessngDBToolsRemoveBase::removeReduceSpeedInterval(const QList<ReduceSpeedInterval*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeReduceSpeedInterval(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeReduceSpeedInterval(ReduceSpeedInterval* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from ReduceSpeedInterval where intervalID=%1;)").arg(it->intervalID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 限速区车辆类型
bool TessngDBToolsRemoveBase::removeReduceSpeedVehiType(const QList<ReduceSpeedVehiType*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeReduceSpeedVehiType(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeReduceSpeedVehiType(ReduceSpeedVehiType* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from ReduceSpeedVehiType where reduceSpeedAreaID=%1 and vehicleTypeCode=%2;)").arg(it->reduceSpeedAreaID).arg(it->vehicleTypeCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 限速区
bool TessngDBToolsRemoveBase::removeReduceSpeedArea(const QList<GReduceSpeedArea*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeReduceSpeedArea(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeReduceSpeedArea(GReduceSpeedArea* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeReduceSpeedInterval(it->mpReduceSpeedArea->mlReduceSpeedInterval);
    if (!result) return false;
    result = removeReduceSpeedVehiType(it->mpReduceSpeedArea->mlReduceSpeedVehiType);
    if (!result) return false;
    QString deleteSql = QString(R"(delete from ReduceSpeedInterval where reduceSpeedAreaID=%1;)").arg(it->mpReduceSpeedArea->reduceSpeedAreaID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 发车间隔
bool TessngDBToolsRemoveBase::removeDepartureInterval(const QList<DepaInterval>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeDepartureInterval(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeDepartureInterval(DepaInterval it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from DepartureInterval where departureIntervalD=%1;)").arg(it.departureIntervalD);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsRemoveBase::removeDepartureInterval(GDeparturePoint* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from DepartureInterval where departurePointID=%1;)").arg(it->mpDeparturePoint->departurePointID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsRemoveBase::removeDepartureInterval(VehicleComposition* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from DepartureInterval where vehiCons=%1;)").arg(it->vehicleConsCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 发车点
bool TessngDBToolsRemoveBase::removeDeparturePoint(const QList<GDeparturePoint*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeDeparturePoint(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeDeparturePoint(GDeparturePoint* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;
    result = removeDepartureInterval(it->mpDeparturePoint->mlDepaInterval);
    if (!result) return false;
    QString deleteSql = QString(R"(delete from DeparturePoint where departurePointID=%1;)").arg(it->mpDeparturePoint->departurePointID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 车型组成和占比
bool TessngDBToolsRemoveBase::removeVehicleConsDetail(QList<VehicleConsDetail> list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleConsDetail(it);
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeVehicleConsDetail(VehicleConsDetail it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleConsDetail where vehicleConsCode=%1 and vehicleTypeCode=%2;)").arg(it.vehicleConsCode).arg(it.vehicleTypeCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 车型组成
bool TessngDBToolsRemoveBase::removeVehicleConstitutent(const QList<VehicleComposition>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleConstitutent(it);
        if (!result)return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeVehicleConstitutent(VehicleComposition it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeVehicleConsDetail(it.mlVehicleConsDetail);
    if (!result) return false;

    QString deleteSql = QString(R"(delete from VehicleConstitutent where vehicleConsCode=%1;)").arg(it.vehicleConsCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

//-----------------------------------道路及连接---------------------------------
/// 路径流量分配
bool TessngDBToolsRemoveBase::removeRoutingFLowRatio(const QList<RoutingFLowRatio*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRoutingFLowRatio(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingFLowRatio(const QList<GRouting*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRoutingFLowRatio(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingFLowRatio(GRouting* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingFLowRatio where routingID=%1;)").arg(it->routingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingFLowRatio(RoutingFLowRatio* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingFLowRatio where RoutingFLowRatioID=%1;)").arg(it->RoutingFLowRatioID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路径车道连接
bool TessngDBToolsRemoveBase::removeRoutingLaneConnector(GRouting* routing, const SafeHash<long, LCStruct*>& lcStructs)
{
    bool result = true;
    QList<long> connIDs = lcStructs.keys();
    for (auto& connID : connIDs) {
        QList<LCStruct*> lcStructs = routing->mhLCStruct.values(connID);

        for (auto& lcStruct : lcStructs) {
            result = removeRoutingLaneConnector(routing, connID, lcStruct);
            if (!result) return false;
        }
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingLaneConnector(long routingID, long connID, long fromLaneId, long toLaneId)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingLaneConnector where routingID=%1 and connID=%2 and laneID1=%3 and laneID2=%4;)")
        .arg(routingID).arg(connID).arg(fromLaneId).arg(toLaneId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingLaneConnector(GRouting* routing)
{
    bool result = true;
    QList<long> connIDs = routing->mhLCStruct.keys();
    for (auto& connID : connIDs) {
        QList<LCStruct*> lcStructs = routing->mhLCStruct.values(connID);

        for (auto& lcStruct : lcStructs) {
            result = removeRoutingLaneConnector(routing, connID, lcStruct);
            if (!result) return false;
        }
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingLaneConnector(GRouting* routing, long connID, LCStruct* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingLaneConnector where routingID=%1 and connID=%2 and laneID1=%3 and laneID2=%4;)")
        .arg(routing->routingID).arg(connID).arg(it->fromLaneId).arg(it->toLaneId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路径路段
bool TessngDBToolsRemoveBase::removeRoutingLink(const QList<GRouting*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRoutingLink(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingLink(GRouting* routing, const QList<ILink*> list)
{
    QSqlQuery slQuery(gDB);
    bool result = true;
    for (auto& link : list) {
        QString deleteSql = QString(R"(delete from RoutingLink where routingID=%1 and linkID=&2;)").arg(routing->routingID).arg(link->id());
        result = slQuery.exec(deleteSql);
        if(!result) {
            throw PH::Exception(slQuery.lastError().text().toStdString());
            break;
        }
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeRoutingLink(GRouting* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingLink where routingID=%1;)").arg(it->routingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路径
bool TessngDBToolsRemoveBase::removeRouting(const QList<GRouting*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRouting(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeRouting(GRouting* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeRoutingFLowRatio(it);
    if (!result) return false;
    result = removeRoutingLaneConnector(it);
    if (!result) return false;
    result = removeRoutingLink(it);
    if (!result) return false;
    QString deleteSql = QString(R"(delete from Routing where routingID=%1;)").arg(it->routingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 决策点
bool TessngDBToolsRemoveBase::removeDecisionPoint(const QList<GDecisionPoint*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeDecisionPoint(it) && result;
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeDecisionPoint(GDecisionPoint* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeRouting(it->mlGRouting);

    QString deleteSql = QString(R"(delete from DecisionPoint where deciPointID=%1;)").arg(it->mpDecisionPoint->deciPointID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 车道连接
bool TessngDBToolsRemoveBase::removeLaneConnector(const QList<GLaneConnector*>& list) {
    bool result = true;
    for (auto& it : list) {
        result = removeLaneConnector(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeLaneConnector(GLaneConnector* it) {
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from LaneConnector where connID=%1 and StartLaneID=%2 and EndLaneID=%3;)")
        .arg(it->mpGConnector->id()).arg(it->mpLaneConnector->mpFromLane->laneID).arg(it->mpLaneConnector->mpToLane->laneID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 连接段
bool TessngDBToolsRemoveBase::removeConnector(const QList<GConnector*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeConnector(it);
        if (!result) return false;
    }

    return result;
}

bool TessngDBToolsRemoveBase::removeConnector(GConnector* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeLaneConnector(it->mlGLaneConnector) && result;

    QString deleteSql = QString(R"(delete from Connector where connID=%1;)").arg(it->mpConnector->connID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 顶点
bool TessngDBToolsRemoveBase::removeVertex(const QList<GVertex*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVertex(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeVertex(GVertex* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from Vertex where vertexID=%1;)").arg(it->mpVertex->vertexID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路段顶点
bool TessngDBToolsRemoveBase::removeLinkVertex(const QList<GVertex*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeLinkVertex(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeLinkVertex(GVertex* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from LinkVertex where vertexID=%1;)").arg(it->mpVertex->vertexID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 车道
bool TessngDBToolsRemoveBase::removeLane(const QList<GLane*>& list)
{
    bool result = true;
    for(auto it: list) {
        result = removeLane(it->mpLane);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeLane(Lane* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from Lane where laneID=%1;)").arg(it->laneID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 节点
bool TessngDBToolsRemoveBase::removeNode(const QList<Node*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeNode(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeNode(Node* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from Node where nodeID=%1;)").arg(it->nodeID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路段
bool TessngDBToolsRemoveBase::removeLink(const QList<GLink*>& list) {
    bool result = true;
    for (auto& it : list) {
        result = removeLink(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsRemoveBase::removeLink(GLink* it) {
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeLane(it->mlGLane);
    if (!result) return false;
    result = removeVertex(it->mlGVertex);
    if (!result) return false;
    result = removeLinkVertex(it->mlGVertex);
    if (!result) return false;
    QList<Node*> nodes;
    nodes.push_back(it->mpLink->startNode);
    nodes.push_back(it->mpLink->endNode);
    result = removeNode(nodes);

    QString deleteSql = QString(R"(delete from Link where linkID=%1;)").arg(it->mpLink->roadId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}
