#ifndef TESSNGDBTOOLSUPDATE_H
#define TESSNGDBTOOLSUPDATE_H
#include "DBClassDefine.h"
#include "vehicletype.h"
class TessngDBToolsUpdate
{
public:
    ///update link
    bool updateLinks(const QList<GLink*> &links);

    ///update lane
    bool updateLanes(const QList<GLane*>& list);

    ///update guid arrow
    bool updateGuideArrows(const QList<GGuideArrow*>& list);

    ///update Connector
    bool updateConnectors(const QList<GConnector*>& list);

    ///update Routeing
    bool updateRouteings(const QList<GRouting*>& list);

    ///update DecisionPoint
    bool updateDecisionPoint(const QList<GDecisionPoint*>& list);

    ///update DeparturePoint
    bool updateDeparturePoint(const QList<GDeparturePoint*>& list);

    ///update signalGroup
    bool updateSignalGroups(const QList<SignalGroup*>& list);

    ///update SignalLamp
    bool updateSignalLamps(const QList<GSignalLamp*>& list);
protected:
    TessngDBToolsUpdate();

    ~TessngDBToolsUpdate();
private:
    TessngDBToolsUpdate(const TessngDBToolsUpdate&) = delete;
    bool updateVertex(const QList<Vertex*>& list);

    bool updateNode(const QList<Node*> &list);

    bool updateLinkVertex(long linkId,const QList<Vertex*>& list);

    bool updateLink(Link* link);

    bool updateLaneLimitVehicle(long laneId,QList<VehicleType> &list);

    bool updateLane(Lane* lane);

    bool updateGuideArrow(GuideArrow* arrow);

    bool updateLaneConnectors(long connId,const QList<LaneConnector*> &list);

    bool updateConnectorLimitVehicles(long connId,QList<VehicleType> &list);

    bool updateConnector(Connector* pConnector);

    bool updateRouteing(Routing* route);

    bool updateRoutingLinks(GRouting* routing);

    bool updateRoutingFlowByInterval(const QList<RoutingFlowByInterval*> pRPIs,const QList<Routing*> routes);

    bool updateDecisionPoint(DecisionPoint* pDecisionPoint);

    bool updateDepaInterval(long departurePointID,QList<DepaInterval> &lsit);

    bool updateDeparturePoint(DeparturePoint* dp);

    bool updateSignalLamp(SignalLamp* pSignalLamp);

    bool updateSignalPhase(const QList<SignalPhase*> &list);

    bool updateSignalColor(long phaseID,QList<SignalColor>& list);

    bool updateSignalGroup(SignalGroup* pSignalGroup);

    QString jsonObjToStr(const QJsonObject& obj);
};
#endif // TESSNGDBTOOLSUPDATE_H
