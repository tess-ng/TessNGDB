#ifndef TESSNGDBTOOLSUPDATEBASE_H
#define TESSNGDBTOOLSUPDATEBASE_H
#include "DBClassDefine.h"
#include "vehicletype.h"

class TessngDBToolsUpdateBase
{
protected:
    ///update link
    bool updateLinkPtr(Link* link);

    ///update lane
    bool updateLanePtr(Lane*);
    bool updateLanesPtr(const QList<Lane*>&,bool trans=true);
    bool updateLanesPtr(const QList<GLane*>&,bool trans=true);

    ///update guid arrow
    bool updateGuideArrowByTransPtr(GuideArrow*);
    bool updateGuideArrowPtr(GuideArrow* arrow);

    ///update Connector
    bool updateConnectorsPtr(Connector*);

    bool updateLaneConnectorPtr(LaneConnector*);

    ///update Routeing
    bool updateRouteingPtr(Routing*,bool trans=true);

    ///update DecisionPoint
    bool updateDecisionPointPtr(DecisionPoint*);

    ///update DeparturePoint
    bool updateDeparturePointPtr(DeparturePoint*);

    ///update signalGroup
    bool updateSignalGroupPtr(SignalGroup*);

    ///update SignalLamp
    bool updateSignalLampByTransPtr(SignalLamp*);
    bool updateSignalLampPtr(SignalLamp* pSignalLamp);

    ///update VehicleDrivInfoCollector
    bool updateVehicleDrivInfoCollectorPtr(VehicleDrivInfoCollector*);

    ///update VehicleQueueCounter
    bool updateVehicleQueueCounterPtr(VehicleQueueCounter*);

    ///update VehicleTravelDetector
    bool updateVehicleTravelDetectorPtr(VehicleTravelDetector*);

    ///update VehicleDetector
    bool updateVehicleDetectorPtr(VehicleDetector*);

    ///update ReduceSpeedArea
    bool updateReduceSpeedAreaPtr(ReduceSpeedArea*);

    ///update BusStation
    bool updateBustationPtr(BusStation*);

    ///update BusLine
    bool updateBusLinePtr(BusLine*);
    TessngDBToolsUpdateBase();
private:
    friend class TessngDBToolsRemove;
    bool updateVertex(const QList<Vertex*>& list);

    bool updateNode(const QList<Node*> &list);

    bool updateLinkVertex(long linkId,const QList<Vertex*>& list);

    bool updateLinkBase(Link* link);

    bool updateLaneLimitVehicle(long laneId,QList<VehicleType> &list);

    bool updateLaneBase(Lane* lane);

    bool updateLaneConnectors(long connId,const QList<LaneConnector*> &list);

    bool updateConnectorLimitVehicles(long connId,QList<VehicleType> &list);

    bool updateConnector(Connector* pConnector);

    bool updateRouteingBase(Routing* route);

    bool updateRoutingLinks(Routing* routing);

    bool updateRoutingFlowByInterval(const QList<RoutingFlowByInterval*> &pRPIs,const QList<Routing*> &routes);

    bool updateDecisionPointBase(DecisionPoint* pDecisionPoint);

    bool updateDepaInterval(long departurePointID,QList<DepaInterval> &lsit);

    bool updateDeparturePointBase(DeparturePoint* dp);

    bool updateSignalPhase(const QList<SignalPhase*> &list);

    bool updateSignalColor(long phaseID,QList<SignalColor>& list);

    bool updateSignalGroupBase(SignalGroup* pSignalGroup);

    bool updateReduceSpeedVehiTypes(const QList<ReduceSpeedVehiType*>& list);

    bool updateReduceSpeedIntervals(const QList<ReduceSpeedInterval*>& list);

    bool updateReduceSpeedAreaBase(ReduceSpeedArea* pReduceSpeedArea);

    bool updateBusLineRoads(BusLine* bline);

    bool updateBusStationPassengerArriving(long stationLineID,const QList<PassengerArriving*>& list);

    bool updateBusStationLine(const QList<BusStationLine*>& list);

    bool updateBusLineBase(BusLine* pBusLine);

    QString jsonObjToStr(const QJsonObject& obj);

    bool checkSqlString(QString& str);
};

#endif // TESSNGDBTOOLSUPDATEBASE_H
