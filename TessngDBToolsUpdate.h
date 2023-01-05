#ifndef TESSNGDBTOOLSUPDATE_H
#define TESSNGDBTOOLSUPDATE_H
#include "TessngDBToolsUpdateBase.h"
#include "singleholder.h"
class TessngDBToolsUpdate : public TessngDBToolsUpdateBase,public PH::SingleHolder<TessngDBToolsUpdate>
{
public:
    ///update link
    Link* updateLink(const Link& link);

    ///update lane
    bool updateLane(const Lane& lane);

    ///updateGuide Arrow
    bool updateGuideArrow(const GuideArrow& arrow);

    ///update Connector
    bool updateConnector(const Connector&);
    
    ///update LaneConnector
    bool updateLaneConnector(const LaneConnector&);

    ///update Routeing
    Routing* updateRouteing(const Routing&);

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

    /*
        对BusStationLine仅能对已有元素进行更新
        对mpLink内部属性级别的更新需要调用其他特定接口
    */
    bool updateBustation(const BusStation&);

    /*
        对BusStationLine仅能对已有元素进行更新
        对mlLink，mlLinkId更新需要调用其他特定接口
    */
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
    bool findRoutingLink(int& m, int& n,int id,const QList<QList<Link*>>&);
    void gupdateRoutingFlowByInterval(QList<RoutingFLowRatio*>& dest, const QList<RoutingFLowRatio*>& src);
};
#endif // TESSNGDBTOOLSUPDATE_H
