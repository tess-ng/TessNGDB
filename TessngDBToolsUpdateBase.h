#ifndef TESSNGDBTOOLSUPDATEBASE_H
#define TESSNGDBTOOLSUPDATEBASE_H
#include "DBClassDefine.h"
#include "vehicletype.h"

class TessngDBToolsUpdateBase
{
public:
    ///update link
    bool updateLink(Link* link);

    ///update lane
    bool updateLane(Lane*);
    bool updateLanes(const QList<Lane*>&,bool trans=true);
    bool updateLanes(const QList<GLane*>&,bool trans=true);

    ///update guid arrow
    bool updateGuideArrowByTrans(GuideArrow*);
    bool updateGuideArrow(GuideArrow* arrow);

    ///update Connector
    bool updateConnectors(Connector*);

    ///update Routeing
    bool updateRouteing(Routing*);

    ///update DecisionPoint
    bool updateDecisionPoint(DecisionPoint*);

    ///update DeparturePoint
    bool updateDeparturePoint(DeparturePoint*);

    ///update signalGroup
    bool updateSignalGroup(SignalGroup*);

    ///update SignalLamp
    bool updateSignalLampByTrans(SignalLamp*);
    bool updateSignalLamp(SignalLamp* pSignalLamp);

    ///update VehicleDrivInfoCollector
    bool updateVehicleDrivInfoCollector(VehicleDrivInfoCollector*);

    ///update VehicleQueueCounter
    bool updateVehicleQueueCounter(VehicleQueueCounter*);

    ///update VehicleTravelDetector
    bool updateVehicleTravelDetector(VehicleTravelDetector*);

    ///update VehicleDetector
    bool updateVehicleDetector(VehicleDetector*);

    ///update ReduceSpeedArea
    bool updateReduceSpeedArea(ReduceSpeedArea*);

    ///update BusStation
    bool updateBustation(BusStation*);

    ///update BusLine
    bool updateBusLine(BusLine*);
    TessngDBToolsUpdateBase();
private:
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

    bool updatePassengerArriving(long stationLineID,const QList<PassengerArriving*>& list);

    bool updateBusStationLine(const QList<BusStationLine*>& list);

    bool updateBusLineBase(BusLine* pBusLine);

    QString jsonObjToStr(const QJsonObject& obj);
};

#endif // TESSNGDBTOOLSUPDATEBASE_H
