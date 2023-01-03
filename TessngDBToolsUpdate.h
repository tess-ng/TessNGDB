#ifndef TESSNGDBTOOLSUPDATE_H
#define TESSNGDBTOOLSUPDATE_H
#include "TessngDBToolsUpdateBase.h"
#include "singleholder.h"
class TessngDBToolsUpdate : public TessngDBToolsUpdateBase,public PH::SingleHolder<TessngDBToolsUpdate>
{
public:
    ///update link
    bool updateLink(const Link& link);

    ///update lane
    bool updateLane(const Lane& lane);

    ///updateGuide Arrow
    bool updateGuideArrow(const GuideArrow& arrow);

    ///update Connector
    bool updateConnector(const Connector&);
    
    ///update LaneConnector
    bool updateLaneConnector(const LaneConnector&);

    ///update Routeing
    bool updateRouteing(const Routing&);

    ///update DecisionPoint
    bool updateDecisionPoint(const DecisionPoint&);

    ///update DeparturePoint
    bool updateDeparturePoint(const DeparturePoint&);

    ///update signalGroup
    bool updateSignalGroup(const SignalGroup& );

    ///update SignalLamp
    bool updateSignalLamp(const SignalLamp& pSignalLamp);

    ///update VehicleDrivInfoCollector
    bool updateVehicleDrivInfoCollector(const VehicleDrivInfoCollector&);

    ///update VehicleQueueCounter
    bool updateVehicleQueueCounter(const VehicleQueueCounter&);

    ///update VehicleTravelDetector
    bool updateVehicleTravelDetector(const VehicleTravelDetector&);

    ///update VehicleDetector
    bool updateVehicleDetector(const VehicleDetector&);

    ///update ReduceSpeedArea
    bool updateReduceSpeedArea(const ReduceSpeedArea&);

    ///update BusStation
    bool updateBustation(const BusStation&);

    ///update BusLine
    bool updateBusLine(const BusLine&);
protected:
    ~TessngDBToolsUpdate();
private:
    friend class  PH::SingleHolder<TessngDBToolsUpdate>;
    TessngDBToolsUpdate(const TessngDBToolsUpdate&) = delete;
    TessngDBToolsUpdate();

    void gupdateNode(Node* dest,Node* src);
    void gupdateVertex(Vertex* dest,const QList<Vertex*>& src);
    void gupdateLane(Lane* dest,const QList<Lane*>& src);
    void gupdateGuideArrow(GuideArrow* dest,const QList<GuideArrow*>& src);
};
#endif // TESSNGDBTOOLSUPDATE_H
