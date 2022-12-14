#include "TessngDBToolsBase.h"
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
TessngDBToolsBase::TessngDBToolsBase()
{

}

TessngDBToolsBase::~TessngDBToolsBase()
{

}

//------------------------------------信号控制----------------------------------
/// 信号灯
bool TessngDBToolsBase::removeSignalLamp(const QList<GSignalLamp*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeSignalLamp(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeSignalLamp(const QList<ISignalLamp*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeSignalLamp(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeSignalLamp(GSignalLamp* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from SignalLamp where signalLampID=%1;)").arg(it->mpSignalLamp->signalLampID);
    result = slQuery.exec(deleteSql);
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsBase::removeSignalLamp(ISignalLamp* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from SignalLamp where signalLampID=%1;)").arg(it->id());
    result = slQuery.exec(deleteSql);
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

///  相位信号颜色
bool TessngDBToolsBase::removeSignalColor(const SignalPhase* signalPhase, QList<int> serialNumbers)
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

bool TessngDBToolsBase::removeSignalColor(const SignalPhase* signalPhase, int serialNumber)
{
    QSqlQuery slQuery(gDB);
    QString deleteSql = QString(R"(delete from SignalColor where signalPhaseID=%1 and serialNumber=%2;)").arg(signalPhase->signalPhaseID).arg(serialNumber);
    bool result = slQuery.exec(deleteSql);
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 相位
bool TessngDBToolsBase::removeSignalPhase(const QList<SignalPhase*>& list)
{
    bool result = true;
    QSqlQuery slQuery(gDB);
    for (auto& it : list) {
        result = removeSignalPhase(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeSignalPhase(SignalPhase* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QList<int> serialNumbers;
    for (int i = 1; i <= it->mlSignalColor.size(); i++) {
        serialNumbers.push_back(i);
    }
    result = removeSignalColor(it, serialNumbers);

    QString deleteSql = QString(R"(delete from SignalPhase where signalPhaseID=%1;)").arg(it->signalPhaseID);
    result = slQuery.exec(deleteSql) && result;
    if (!result) throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 信号灯组
bool TessngDBToolsBase::removeSignalGroup(const QList<SignalGroup*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeSignalGroup(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeSignalGroup(SignalGroup* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeSignalPhase(it->mlPhase);

    QString deleteSql = QString(R"(delete from SignalGroup where signalGroupID=%1;)").arg(it->signalGroupID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

//--------------------------------------公交------------------------------------
/// 乘客到站
bool TessngDBToolsBase::removeBusStationPassengerArriving(const QList<PassengerArriving*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusStationPassengerArriving(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeBusStationPassengerArriving(PassengerArriving* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from BusStationPassengerArriving where passengerArrivingID=%1;)").arg(it->passengerArrivingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交站点-线路
bool TessngDBToolsBase::removeBusStationLine(const QList<BusStationLine*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusStationLine(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeBusStationLine(BusStationLine* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeBusStationPassengerArriving(it->mlPassengerArriving);

    QString deleteSql = QString(R"(delete from BusStationLine where stationLineID=%1;)").arg(it->stationLineID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交线路的路段或连接段序列
bool TessngDBToolsBase::removeBusLineRoad(const QList<GBusLine*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusLineRoad(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeBusLineRoad(GBusLine* it)
{
    bool result = true;
    QSqlQuery slQuery(gDB);

    QString deleteSql = QString(R"(delete from BusLineRoad where busLineID=%1;)").arg(it->mpBusLine->busLineID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交线路
bool TessngDBToolsBase::removeBusLine(const QList<GBusLine*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusLine(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeBusLine(GBusLine* it)
{
    bool result = true;
    QSqlQuery slQuery(gDB);

    result = removeBusLineRoad(it);

    QString deleteSql = QString(R"(delete from BusLine where busLineID=%1;)").arg(it->mpBusLine->busLineID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 公交站
bool TessngDBToolsBase::removeBusStation(const QList<GBusStation*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeBusStation(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeBusStation(GBusStation* it)
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
bool TessngDBToolsBase::removeDrivInfoCollector(const QList<GVehicleDrivInfoCollector*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeDrivInfoCollector(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeDrivInfoCollector(GVehicleDrivInfoCollector* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleDrivInfoCollecter where collecterID=%1;)").arg(it->mpVehicleDrivInfoCollector->collecterID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 排队计数器
bool TessngDBToolsBase::removeVehicleQueueCounter(const QList<GVehicleQueueCounter*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleQueueCounter(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeVehicleQueueCounter(GVehicleQueueCounter* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleQueueCounter where queueCounterID=%1;)").arg(it->mpVehicleQueueCounter->queueCounterID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 行程时间检测器
bool TessngDBToolsBase::removeVehicleTravelDetector(const QList<GVehicleTravelDetector*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleTravelDetector(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeVehicleTravelDetector(GVehicleTravelDetector* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleTravelDetector where detectorId=%1;)").arg(it->mpVehicleTravelDetector->detectorId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 导向箭头
bool TessngDBToolsBase::removeGuideArrow(const QList<GGuideArrow*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeGuideArrow(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeGuideArrow(GGuideArrow* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from GuideArrow where guideArrowId=%1;)").arg(it->mpGuideArrow->guideArrowID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 限速区时间间隔
bool TessngDBToolsBase::removeReduceSpeedInterval(const QList<ReduceSpeedInterval*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeReduceSpeedInterval(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeReduceSpeedInterval(ReduceSpeedInterval* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from ReduceSpeedInterval where intervalID=%1;)").arg(it->intervalID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 限速区车辆类型
bool TessngDBToolsBase::removeReduceSpeedVehiType(const QList<ReduceSpeedVehiType*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeReduceSpeedVehiType(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeReduceSpeedVehiType(ReduceSpeedVehiType* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from ReduceSpeedInterval where reduceSpeedAreaID=%1,vehicleTypeCode=%2;)").arg(it->reduceSpeedAreaID).arg(it->vehicleTypeCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 限速区
bool TessngDBToolsBase::removeReduceSpeedArea(const QList<GReduceSpeedArea*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeReduceSpeedArea(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeReduceSpeedArea(GReduceSpeedArea* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeReduceSpeedInterval(it->mpReduceSpeedArea->mlReduceSpeedInterval);

    result = removeReduceSpeedVehiType(it->mpReduceSpeedArea->mlReduceSpeedVehiType);

    QString deleteSql = QString(R"(delete from ReduceSpeedInterval where reduceSpeedAreaID=%1;)").arg(it->mpReduceSpeedArea->reduceSpeedAreaID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 发车间隔
bool TessngDBToolsBase::removeDepartureInterval(const QList<DepaInterval>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeDepartureInterval(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeDepartureInterval(DepaInterval it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from DepartureInterval where departureIntervalD=%1;)").arg(it.departureIntervalD);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsBase::removeDepartureInterval(GDeparturePoint* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from DepartureInterval where departurePointID=%1;)").arg(it->mpDeparturePoint->departurePointID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsBase::removeDepartureInterval(VehicleComposition* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from DepartureInterval where vehiCons=%1;)").arg(it->vehicleConsCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 发车点
bool TessngDBToolsBase::removeDeparturePoint(const QList<GDeparturePoint*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeDeparturePoint(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeDeparturePoint(GDeparturePoint* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeDepartureInterval(it->mpDeparturePoint->mlDepaInterval);

    QString deleteSql = QString(R"(delete from DeparturePoint where departurePointID=%1;)").arg(it->mpDeparturePoint->departurePointID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 车型组成和占比
bool TessngDBToolsBase::removeVehicleConsDetail(QList<VehicleConsDetail> list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleConsDetail(it);
    }
    return result;
}

bool TessngDBToolsBase::removeVehicleConsDetail(VehicleConsDetail it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from VehicleConsDetail where vehicleConsCode=%1,vehicleTypeCode=%2;)").arg(it.vehicleConsCode).arg(it.vehicleTypeCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 车型组成
bool TessngDBToolsBase::removeVehicleConstitutent(const QList<VehicleComposition*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVehicleConstitutent(it);
        if (!result)return false;
    }
    return result;
}

bool TessngDBToolsBase::removeVehicleConstitutent(VehicleComposition* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeVehicleConsDetail(it->mlVehicleConsDetail);

    //result = removeDepartureInterval(it);
    //if (!result)return false;

    QString deleteSql = QString(R"(delete from VehicleConstitutent where vehicleConsCode=%1;)").arg(it->vehicleConsCode);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

//-----------------------------------道路及连接---------------------------------
/// 路径流量分配
bool TessngDBToolsBase::removeRoutingFLowRatio(const QList<RoutingFLowRatio*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRoutingFLowRatio(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeRoutingFLowRatio(const QList<GRouting*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRoutingFLowRatio(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeRoutingFLowRatio(GRouting* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingFLowRatio where routingID=%1;)").arg(it->routingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsBase::removeRoutingFLowRatio(RoutingFLowRatio* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingFLowRatio where RoutingFLowRatioID=%1;)").arg(it->RoutingFLowRatioID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路径车道连接
bool TessngDBToolsBase::removeRoutingLaneConnector(GRouting* routing, const SafeHash<long, LCStruct*>& lcStructs)
{
    bool result = true;
    QList<long> connIDs = lcStructs.keys();
    for (auto& connID : connIDs) {
        QList<LCStruct*> lcStructs = routing->mhLCStruct.takeValues(connID);

        for (auto& lcStruct : lcStructs) {
            result = removeRoutingLaneConnector(routing, connID, lcStruct);
            if (!result) return false;
        }
    }
    return result;
}

bool TessngDBToolsBase::removeRoutingLaneConnector(long routingID, long connID, long fromLaneId, long toLaneId)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingFLowRatio where routingID=%1 and connID=%2 and laneID1=%3 and laneID2=%4;)")
        .arg(routingID).arg(connID).arg(fromLaneId).arg(toLaneId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

bool TessngDBToolsBase::removeRoutingLaneConnector(GRouting* routing)
{
    bool result = true;
    QList<long> connIDs = routing->mhLCStruct.keys();
    for (auto& connID : connIDs) {
        QList<LCStruct*> lcStructs = routing->mhLCStruct.takeValues(connID);

        for (auto& lcStruct : lcStructs) {
            result = removeRoutingLaneConnector(routing, connID, lcStruct);
            if (!result) return false;
        }
    }
    return result;
}

bool TessngDBToolsBase::removeRoutingLaneConnector(GRouting* routing, long connID, LCStruct* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingFLowRatio where routingID=%1,connID=%2,laneID1=%3,laneID2=%4;)")
        .arg(routing->routingID).arg(connID).arg(it->fromLaneId).arg(it->toLaneId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路径路段
bool TessngDBToolsBase::removeRoutingLink(const QList<GRouting*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRoutingLink(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeRoutingLink(GRouting* routing, const QList<ILink*> list)
{
    QSqlQuery slQuery(gDB);
    bool result = true;
    try
    {
        for (auto& link : list) {
            QString deleteSql = QString(R"(delete from RoutingLink where routingID=%1,linkID=&2;)").arg(routing->routingID).arg(link->id());
            result = slQuery.exec(deleteSql);
            if (!result) return false;
        }
    }
    catch (const QException& exc)
    {
        qWarning() << __FILE__ << __LINE__ << exc.what();
        result = false;
    }
    catch (...) {
        qWarning() << __FILE__ << __LINE__ << "Unknow Error.";
        result = false;
    }
    return result;
}

bool TessngDBToolsBase::removeRoutingLink(GRouting* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from RoutingLink where routingID=%1;)").arg(it->routingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路径
bool TessngDBToolsBase::removeRouting(const QList<GRouting*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeRouting(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeRouting(GRouting* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeRoutingFLowRatio(it);

    result = removeRoutingLaneConnector(it);

    result = removeRoutingLink(it);

    QString deleteSql = QString(R"(delete from Routing where routingID=%1;)").arg(it->routingID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 决策点
bool TessngDBToolsBase::removeDecisionPoint(const QList<GDecisionPoint*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeDecisionPoint(it) && result;
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeDecisionPoint(GDecisionPoint* it)
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
bool TessngDBToolsBase::removeLaneConnector(const QList<GLaneConnector*>& list) {
    bool result = true;
    for (auto& it : list) {
        result = removeLaneConnector(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeLaneConnector(GLaneConnector* it) {
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from LaneConnector where connID=%1,StartLaneID=%2,EndLaneID=%3;)")
        .arg(it->mpGConnector->id()).arg(it->mpLaneConnector->mpFromLane->laneID).arg(it->mpLaneConnector->mpToLane->laneID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 连接段
bool TessngDBToolsBase::removeConnector(const QList<GConnector*>& list)
{
    bool result = true;
    for(auto& it : list) {
        result = removeConnector(it);
        if (!result) return false;
    }

    return result;
}

bool TessngDBToolsBase::removeConnector(GConnector* it)
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
bool TessngDBToolsBase::removeVertex(const QList<GVertex*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeVertex(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeVertex(GVertex* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from Vertex where vertexID=%1;)").arg(it->mpVertex->vertexID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路段顶点
bool TessngDBToolsBase::removeLinkVertex(const QList<GVertex*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeLinkVertex(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeLinkVertex(GVertex* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from LinkVertex where vertexID=%1;)").arg(it->mpVertex->vertexID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 车道
bool TessngDBToolsBase::removeLane(const QList<GLane*>& list)
{
    bool result = true;
    for(auto it: list) {
        result = removeLane(it->mpLane);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeLane(Lane* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from Lane where laneID=%1;)").arg(it->laneID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 节点
bool TessngDBToolsBase::removeNode(const QList<Node*>& list)
{
    bool result = true;
    for (auto& it : list) {
        result = removeNode(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeNode(Node* it)
{
    QSqlQuery slQuery(gDB);
    bool result = true;

    QString deleteSql = QString(R"(delete from Node where nodeID=%1;)").arg(it->nodeID);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}

/// 路段
bool TessngDBToolsBase::removeLink(const QList<GLink*>& list) {
    bool result = true;
    for (auto& it : list) {
        result = removeLink(it);
        if (!result) return false;
    }
    return result;
}

bool TessngDBToolsBase::removeLink(GLink* it) {
    QSqlQuery slQuery(gDB);
    bool result = true;

    result = removeLane(it->mlGLane);

    result = removeVertex(it->mlGVertex);

    result = removeLinkVertex(it->mlGVertex);

    QList<Node*> nodes;
    nodes.push_back(it->mpLink->startNode);
    nodes.push_back(it->mpLink->endNode);
    result = removeNode(nodes);

    QString deleteSql = QString(R"(delete from Link where linkID=%1;)").arg(it->mpLink->roadId);
    result = slQuery.exec(deleteSql);
    if (!result)throw PH::Exception(gDB.lastError().text().toStdString());

    return result;
}
