#ifndef TESSNGDBCOPY_H
#define TESSNGDBCOPY_H
#include "singleholder.h"
#include <QSqlDatabase>
class Link;
class Connector;
class DecisionPoint;
class Routing;
class RoutingFlowByInterval;
class SignalGroup;
class GuideArrow;
class SignalLamp;
class VehicleType;
class VehicleComposition;
class VehicleConsDetail;
class DeparturePoint;
class VehicleDrivInfoCollector;
class VehicleQueueCounter;
class ReduceSpeedArea;
class BusStation;
class VehicleTravelDetector;
class BusLine;
class TessngDBCopy :public PH::SingleHolder<TessngDBCopy>
{
public:
    bool copyDb(const QString& dbFileName);
    QSqlDatabase* getSqlDatabase();
    bool saveConfiguration();
    bool insertNet();
    bool insertLink(Link* pLink);
    bool insertConnector(Connector *pConnector);
    bool insertDecisionPoint(DecisionPoint* pDecisionPoint);
    bool insertRoutingLaneConnector(long routingId, long connectorId, long laneId1, long laneId2);
    bool insertSignalGroup(SignalGroup* pSignalGroup);
    bool updateIds();
    bool insertGuideArrow(GuideArrow* pGuideArrow);
    bool insertSignalLamp(SignalLamp* pSignalLamp);
    bool insertVehicleType(const VehicleType& vt);
    bool insertVehicleConstitutent(const VehicleComposition& vc);
    bool insertVehicleConsDetail(const VehicleConsDetail& vcd);
    bool insertDeparturePoint(const DeparturePoint& dp);
    bool insertDrivInfoCollector(const VehicleDrivInfoCollector& drivInfoCollector);
    bool insertVehicleQueueCounter(VehicleQueueCounter* pQueue);
    bool insertVehicleTravelDetector(VehicleTravelDetector* pTravelDetector);
    bool insertReduceSpeedArea(ReduceSpeedArea* pReduceSpeedArea);
    bool insertBusStation(BusStation* pBusStation);
    bool insertBusLine(BusLine* pBusLine);
    void test();
private:
    TessngDBCopy();
    bool clearDBData();
    bool insertRouting(Routing* pRouting);
    bool insertRoutingFlowRatio(long startDateTime, RoutingFlowByInterval* pRoutingFlowByInterval);
    bool doSql(const QString& sql);
    friend class PH::SingleHolder<TessngDBCopy>;
    QSqlDatabase database;
};

#endif // TESSNGDBCOPY_H
