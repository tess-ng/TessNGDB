/***********************************************************************
 更新模块 接口仅对基本属性进行更新，对于更新元素内部相关联的QList类型成员：
    不对其进行新增删除的检查和更新，QList内部元素所指向的元素的属性也不在这里进行更新
 如需要新增删除或属性成员关联元素的更新，请使用对应的特定接口！
 ***********************************************************************/
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
    bool findRoutingLink(int& m, int& n,int id,const QList<QList<Link*>>&);
};
#endif // TESSNGDBTOOLSUPDATE_H
