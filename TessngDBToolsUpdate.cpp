#include "TessngDBToolsUpdate.h"
#include <QDebug>
#include <QException>
#include <QSqlQuery>
#include "exception.h"
#include <QSqlError>
#include "DbGlobal.h"
#include "Vertex.h"
#include "glink.h"
#include <QJsonDocument>
#include "GuideArrow.h"
#include "GGuideArrow.h"
#include "Node.h"
#include "connector.h"
#include "laneconnector.h"
#include "TessngDBToolsRemove.h"
#include "Routing.h"
#include "grouting.h"
#include "DecisionPoint.h"
#include "RoutingFLowRatio.h"
#include "gdecisionpoint.h"
#include "DeparturePoint.h"
#include "gdeparturepoint.h"
#include "signallamp.h"
#include "signalphase.h"
#include "signalgroup.h"
#include "gsignallamp.h"
#include "gvehicledrivinfocollector.h"
#include "VehicleDrivInfoCollector.h"
#include "gvehiclequeuecounter.h"
#include "VehicleQueueCounter.h"
#include "gvehicletraveldetector.h"
#include "VehicleTravelDetector.h"
#include "GVehicleDetector.h"
#include "VehicleDetector.h"
#include "ReduceSpeedArea.h"
#include "greducespeedarea.h"
#include "ReduceSpeedInterval.h"
#include "ReduceSpeedVehiType.h"
#include "gbusstation.h"
#include "BusStation.h"
#include "BusLine.h"
#include "PassengerArriving.h"
#include "gbusline.h"
TessngDBToolsUpdate::TessngDBToolsUpdate()
{

}


TessngDBToolsUpdate::~TessngDBToolsUpdate()
{

}
bool TessngDBToolsUpdate::updateVertex(const QList<Vertex *> &list)
{
    bool result=true;
    QSqlQuery  query(gDB);
    foreach(auto pVertex,list){
        query.prepare(QString("UPDATE Vertex set X=%1,Y=%2,Z=%3 WHERE VertexID=%4")
                      .arg(pVertex->x)
                      .arg(pVertex->y)
                      .arg(pVertex->z)
                      .arg(pVertex->vertexID));

        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateNode(const QList<Node*> &list){
    bool result=true;
    QSqlQuery  query(gDB);
    foreach(auto it,list){
        query.prepare(QString("UPDATE Node set nodeName='%1',vertexID=%2 WHERE nodeID=%3")
                      .arg(it->nodeName)
                      .arg(it->mpVertex->vertexID)
                      .arg(it->nodeID));

        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateLinkVertex(long linkId,const QList<Vertex*>& list){
    bool result=true;
    QSqlQuery  query(gDB);
    foreach(auto it,list){
        query.prepare(QString("UPDATE LinkVertex set Num=%1 WHERE LinkID=%2 and VertexID=%3")
                      .arg(list.size())
                      .arg(linkId)
                      .arg(it->vertexID));

        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
QString TessngDBToolsUpdate::jsonObjToStr(const QJsonObject& obj){
    QByteArray bt=QJsonDocument(obj).toJson(QJsonDocument::Compact);
    return bt.data();
}
bool TessngDBToolsUpdate::updateLink(Link* link){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE Link set ";
    sql+=QString("StartNodeID=%1").arg(link->startNode->nodeID);
    sql+=QString(",EndNodeID=%1").arg(link->endNode->nodeID);
    sql+=QString(",LinkName='%1'").arg(link->linkName);
    sql+=QString(",laneNumber=%1").arg(link->laneNumber);
    sql+=QString(",laneWidth=%1").arg(link->laneWidth);
    sql+=QString(",laneColor='%1'").arg(link->laneColor);
    sql+=QString(",linkType='%1'").arg(link->linkType);
    sql+=QString(",length=%1").arg(link->length);
    sql+=QString(",curvature=%1").arg(link->curvature);
    sql+=QString(",nonLinearCoefficient=%1").arg(link->nonLinearCoefficient);
    sql+=QString(",linkSaturationFlo=%1").arg(link->linkSaturationFlow);
    sql+=QString(",linkTrafficFlow=%1").arg(link->linkTrafficFlow);
    sql+=QString(",desiredSpeed=%1").arg(link->desiredSpeed);
    sql+=QString(",limitSpeed=%1").arg(link->limitSpeed);
    sql+=QString(",minSpeed=%1").arg(link->minSpeed);
    sql+=QString(",addValue=%1").arg(link->addValue);
    sql+=QString(",centerLinePointsJson='%1'").arg(jsonObjToStr(link->centerLinePointsJson));
    sql+=QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(link->leftBreakPointsJson));
    sql+=QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(link->rightBreakPointsJson));
    sql+=QString(",otherAttrsJson='%1'").arg(jsonObjToStr(link->otherAttrsJson));
    sql+=QString(" WHERE linkID=%1").arg(link->linkID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}
bool TessngDBToolsUpdate::updateLinks(const QList<GLink*>& links) {
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, links) {
            result=updateVertex(it->mpLink->mlVertex);
            if(!result) goto failed;

            result=updateNode(QList<Node*>()<<it->mpLink->startNode<<it->mpLink->endNode);
            if(!result) goto failed;

            result=updateLinkVertex(it->mpLink->linkID,it->mpLink->mlVertex);
            if(!result) goto failed;

            result=updateLink(it->mpLink);
            if(!result) goto failed;
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
        qWarning() << "update Links failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateLaneLimitVehicle(long laneId,QList<VehicleType> &list){
    bool result=true;
    QSqlQuery  query(gDB);
    foreach (auto &it, list) {
        query.prepare(QString("UPDATE LaneLimitVehicle set VehicleTypeCode=%1 WHERE LaneID=%2").arg(it.vehicleTypeCode).arg(laneId));
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateLane(Lane* lane){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE Lane set ";
    sql+=QString("LaneID=%1").arg(lane->laneID);
    sql+=QString(",SerialNumber=%1").arg(lane->serialNumber);
    sql+=QString(",Width=%1").arg(lane->width);
    sql+=QString(",ActionType='%1'").arg(lane->actionType);
    sql+=QString(",ExpectTravelDirection='%1'").arg(lane->expectTravelDirection);
    sql+=QString(",centerLinePointsJson='%1'").arg(jsonObjToStr(lane->centerLinePointsJson));
    sql+=QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(lane->leftBreakPointsJson));
    sql+=QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(lane->rightBreakPointsJson));
    sql+=QString(",otherAttrsJson='%1'").arg(jsonObjToStr(lane->otherAttrsJson));
    sql+=QString(" WHERE LinkID=%1").arg(lane->laneID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}


bool TessngDBToolsUpdate::updateLanes(const QList<GLane*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result=updateLaneLimitVehicle(it->mpLane->laneID,it->mpLane->mlVehicleType);
            if(!result) goto failed;

            result=updateLane(it->mpLane);
            if(!result) goto failed;
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
        qWarning() << "update Lanes failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateGuideArrow(GuideArrow *arrow)
{
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE GuideArrow set ";
    sql+=QString("laneID=%1").arg(arrow->laneID);
    sql+=QString(",length=%1").arg(arrow->length);
    sql+=QString(",distToTerminal=%1").arg(arrow->distToTerminal);
    sql+=QString(",arrowType='%1'").arg(arrow->arrowType);
    sql+=QString(" WHERE guideArrowID=%1").arg(arrow->laneID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}


bool TessngDBToolsUpdate::updateGuideArrows(const QList<GGuideArrow *> &list)
{
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result=updateGuideArrow(it->mpGuideArrow);
            if(!result) goto failed;
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
        qWarning() << "update GuideArrow failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateLaneConnectors(long connId,const QList<LaneConnector *> &list)
{
    bool result=true;
    QSqlQuery  query(gDB);
    foreach(auto it,list){
        QString sql="UPDATE LaneConnector set ";
        sql+=QString("StartLaneID=%1").arg(it->mpFromLane->laneID);
        sql+=QString(",EndLaneID=%1").arg(it->mpToLane->laneID);
        sql+=QString(",weight=%1").arg(it->weight);
        sql+=QString(",centerLinePointsJson='%1'").arg(jsonObjToStr(it->centerLinePointsJson));
        sql+=QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(it->leftBreakPointsJson));
        sql+=QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(it->rightBreakPointsJson));
        sql+=QString(",otherAttrsJson='%1'").arg(jsonObjToStr(it->otherAttrsJson));
        sql+=QString(" WHERE connID=%1").arg(connId);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}

bool TessngDBToolsUpdate::updateConnectorLimitVehicles(long connId, QList<VehicleType> &list)
{
    bool result=true;
    QSqlQuery  query(gDB);
    foreach(auto it,list){
        QString sql="UPDATE ConnectorLimitVehicle set ";
        sql+=QString("VehicleTypeCode=%1").arg(it.vehicleTypeCode);
        sql+=QString(" WHERE connID=%1").arg(connId);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateConnector(Connector* it){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE Connector set ";
    sql+=QString("StartLinkID=%1").arg(it->mpFromLink->linkID);
    sql+=QString(",EndLinkID=%1").arg(it->mpToLink->linkID);
    sql+=QString(",Length=%1").arg(it->length);
    sql+=QString(",ConnName='%1'").arg(it->connName);
    sql+=QString(",Color='%1'").arg(it->color);
    sql+=QString(",NonLinearCoefficient=%1").arg(it->nonLinearCoefficient);
    sql+=QString(",DesiredSpeed=%1").arg(it->desiredSpeed);
    sql+=QString(",LimitSpeed=%1").arg(it->limitSpeed);
    sql+=QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(it->leftBreakPointsJson));
    sql+=QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(it->rightBreakPointsJson));
    sql+=QString(",otherAttrsJson='%1'").arg(jsonObjToStr(it->otherAttrsJson));
    sql+=QString(" WHERE ConnID=%1").arg(it->connID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}
bool TessngDBToolsUpdate::updateConnectors(const QList<GConnector *> &list)
{
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result=updateLaneConnectors(it->mpConnector->connID,it->mpConnector->mlLaneConnector);
            if(!result) goto failed;

            result=updateConnectorLimitVehicles(it->mpConnector->connID,it->mpConnector->mlVehicleType);
            if(!result) goto failed;

            result=updateConnector(it->mpConnector);
            if(!result) goto failed;
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
        qWarning() << "update Connectors failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateRoutingLinks(GRouting* routing){
    bool result=TessngDBToolsRemove::getInstance()->removeRoutingLink(routing);
    if(!result) return false;
    QSqlQuery  query(gDB);
    QString sql = "insert into RoutingLink(routingID, linkID, num1, num2) values(:routingID, :linkID, :num1, :num2)";
    query.prepare(sql);
    int num1 = 1;
    foreach(QList<Link*> lLink, routing->getRouting()->mllLink)
    {
        int num2 = 1;
        foreach(Link * pLink, lLink)
        {
            query.bindValue(":routingID", QVariant::fromValue(routing->getRouting()->routingID));
            query.bindValue(":linkID", QVariant::fromValue(pLink->linkID));
            query.bindValue(":num1", num1);
            query.bindValue(":num2", num2);
            result =query.exec();
            if(!result) {
                throw PH::Exception(query.lastError().text().toStdString());
                return result;
            }
            num2++;
        }
        num1++;
    }
    return result;
}
bool TessngDBToolsUpdate::updateRouteing(Routing* route){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE Routing set ";
    sql+=QString("deciPointID=%1").arg(route->mpDecisionPoint->deciPointID);
    sql+=QString(",routingName=%1").arg(route->routingName);
    sql+=QString(",proportion=%1").arg(route->proportion);
    sql+=QString(" WHERE routingID=%1").arg(route->routingID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}
bool TessngDBToolsUpdate::updateRouteings(const QList<GRouting*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result =updateRoutingLinks(it);
            if(!result) goto failed;

            result=updateRouteing(it->getRouting());
            if(!result) goto failed;
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
        qWarning() << "update Routeing failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateRoutingFlowByInterval(const QList<RoutingFlowByInterval*>& pRPIs,const QList<Routing*>& routes){
    long startDateTime = 1;
    bool result=true;
    QSqlQuery  query(gDB);
    foreach(RoutingFlowByInterval * pRPI, pRPIs)
    {
        foreach(RoutingFLowRatio * pRFR, pRPI->mlRoutingFlowRatio)
        {
            bool found = false;
            foreach(Routing * pRouting, routes)
            {
                if (pRouting->routingID == pRFR->routingID)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                pRPI->mlRoutingFlowRatio.removeAll(pRFR);
                delete pRFR;
            }
        }
        foreach(RoutingFLowRatio * pRoutingFLowRatio, pRPI->mlRoutingFlowRatio)
        {
            QString sql="UPDATE RoutingFLowRatio set ";
            sql+=QString("routingID=%1").arg(pRoutingFLowRatio->routingID);
            sql+=QString(",startDateTime=%1").arg(startDateTime);
            sql+=QString(",endDateTime=%1").arg(pRPI->endDateTime);
            sql+=QString(",ratio=%1").arg(pRoutingFLowRatio->ratio);
            sql+=QString(" WHERE RoutingFLowRatioID=%1").arg(pRoutingFLowRatio->RoutingFLowRatioID);
            query.prepare(sql);
            result=query.exec();
            if(!result) {
                throw PH::Exception(query.lastError().text().toStdString());
                break;
            }
        }
        startDateTime = pRPI->endDateTime + 1;
    }
    return result;
}
bool TessngDBToolsUpdate::updateDecisionPoint(DecisionPoint* pDecisionPoint){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE DecisionPoint set ";
    sql+=QString("deciPointName='%1'").arg(pDecisionPoint->deciPointName);
    sql+=QString(",linkID=%1").arg(pDecisionPoint->mpLink->linkID);
    sql+=QString(",X=%1").arg(pDecisionPoint->X);
    sql+=QString(",Y=%1").arg(pDecisionPoint->Y);
    sql+=QString(",Z=%1").arg(pDecisionPoint->Z);
    sql+=QString(" WHERE deciPointID=%1").arg(pDecisionPoint->deciPointID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}
bool TessngDBToolsUpdate::updateDecisionPoint(const QList<GDecisionPoint *> &list)
{
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result =updateRoutingFlowByInterval(it->mpDecisionPoint->mlRoutingFlowByInterval,it->mpDecisionPoint->mlRouting);
            if(!result) goto failed;

            result =updateRouteings(it->mlGRouting);
            if(!result) goto failed;

            result =updateDecisionPoint(it->mpDecisionPoint);
            if(!result) goto failed;
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
        qWarning() << "update DecisionPoint failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateDepaInterval(long departurePointID,QList<DepaInterval> &lsit){
    bool result=true;
    QSqlQuery  query(gDB);
    foreach(DepaInterval di, lsit){
        QString sql="UPDATE DepartureInterval set ";
        sql+=QString("departurePointID=%1").arg(di.departureIntervalD);
        sql+=QString(",interval=%1").arg(di.interval);
        sql+=QString(",vehiCount=%1").arg(di.vehiCount);
        sql+=QString(",vehiCons=%1").arg(di.vehiCons.vehicleConsCode);
        sql+=QString(" WHERE departureIntervalD=%1").arg(departurePointID);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateDeparturePoint(DeparturePoint* dp){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE DeparturePoint set ";
    sql+=QString("name='%1'").arg(dp->name);
    sql+=QString(",linkID=%1").arg(dp->mpLink->linkID);
    sql+=QString(" WHERE departurePointID=%1").arg(dp->departurePointID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}
bool TessngDBToolsUpdate::updateDeparturePoint(const QList<GDeparturePoint*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result =updateDepaInterval(it->mpDeparturePoint->departurePointID,it->mpDeparturePoint->mlDepaInterval);
            if(!result) goto failed;

            result =updateDeparturePoint(it->mpDeparturePoint);
            if(!result) goto failed;
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
        qWarning() << "update DeparturePoint failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateSignalLamp(SignalLamp* pSignalLamp){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE SignalLamp set ";
    sql+=QString("name='%1'").arg(pSignalLamp->name);
    sql+=QString(",signalPhaseID=%1").arg(pSignalLamp->mpSignalPhase->id());
    sql+=QString(",laneID=%1").arg(pSignalLamp->laneID);
    sql+=QString(",toLaneID=%1").arg(pSignalLamp->toLaneID);
    sql+=QString(",X=%1").arg(pSignalLamp->x);
    sql+=QString(",Y=%1").arg(pSignalLamp->y);
    sql+=QString(",Z=%1").arg(pSignalLamp->z);
    sql+=QString(" WHERE signalLampID=%1").arg(pSignalLamp->signalLampID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}
bool TessngDBToolsUpdate::updateSignalColor(long phaseID,QList<SignalColor>& list){
    bool result=true;
    QSqlQuery  query(gDB);
    int serialNumber = 0;
    foreach (auto &it, list) {
        QString sql="UPDATE SignalColor set ";
        sql+=QString("color='%1'").arg(it.color);
        sql+=QString(",serialNumber=%1").arg(++serialNumber);
        sql+=QString(",timeLength=%1").arg(it.timeLength);
        sql+=QString(" WHERE signalPhaseID=%1").arg(phaseID);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateSignalPhase(const QList<SignalPhase *> &list)
{
    QSqlQuery  query(gDB);
    bool result=true;
    foreach (auto it, list) {
        result =updateSignalColor(it->signalPhaseID,it->mlSignalColor);
        if(!result) return result;

        QString sql="UPDATE SignalPhase set ";
        sql+=QString("name='%1'").arg(it->name);
        sql+=QString(",signalGroupID=%1").arg(it->signalPhaseID);
        sql+=QString(",serialNumber=%1").arg(it->serialNumber);
        sql+=QString(" WHERE signalPhaseID=%1").arg(it->signalPhaseID);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }

    return result;
}
bool TessngDBToolsUpdate::updateSignalGroup(SignalGroup* pSignalGroup){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE SignalGroup set ";
    sql+=QString("name='%1'").arg(pSignalGroup->groupName());
    sql+=QString(",timeLength=%1").arg(pSignalGroup->timeLength);
    sql+=QString(",startTime=%1").arg(pSignalGroup->startTime);
    sql+=QString(",endTime=%1").arg(pSignalGroup->endTime);
    sql+=QString(" WHERE signalGroupID=%1").arg(pSignalGroup->signalGroupID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}


bool TessngDBToolsUpdate::updateSignalGroups(const QList<SignalGroup *> &list)
{
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result =updateSignalPhase(it->mlPhase);
            if(!result) goto failed;

            result =updateSignalGroup(it);
            if(!result) goto failed;
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
        qWarning() << "update SignalGroups failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateSignalLamps(const QList<GSignalLamp*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result =updateSignalLamp(it->mpSignalLamp);
            if(!result) goto failed;
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
        qWarning() << "update SignalLamps failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsUpdate::updateVehicleDrivInfoCollectors(const QList<GVehicleDrivInfoCollector*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        QSqlQuery  query(gDB);
        foreach (auto it, list) {
            QString sql="UPDATE VehicleDrivInfoCollecter set ";
            sql+=QString("name='%1'").arg(it->mpVehicleDrivInfoCollector->name);
            sql+=QString(",roadID=%1").arg(it->mpVehicleDrivInfoCollector->roadID);
            sql+=QString(",laneNumber=%1").arg(it->mpVehicleDrivInfoCollector->laneNumber);
            sql+=QString(",toLaneNumber=%1").arg(it->mpVehicleDrivInfoCollector->toLaneNumber);
            sql+=QString(",x=%1").arg(it->mpVehicleDrivInfoCollector->x);
            sql+=QString(",y=%1").arg(it->mpVehicleDrivInfoCollector->y);
            sql+=QString(",z=%1").arg(it->mpVehicleDrivInfoCollector->x);
            sql+=QString(",distance=%1").arg(it->mpVehicleDrivInfoCollector->distance);
            sql+=QString(",dataInterval=%1").arg(it->mpVehicleDrivInfoCollector->dataInterval);
            sql+=QString(",startTime=%1").arg(it->mpVehicleDrivInfoCollector->startTime);
            sql+=QString(",endTime=%1").arg(it->mpVehicleDrivInfoCollector->endTime);
            sql+=QString(" WHERE collecterID=%1").arg(it->mpVehicleDrivInfoCollector->collecterID);
            query.prepare(sql);
            result=query.exec();
            if(!result) {
                throw PH::Exception(query.lastError().text().toStdString());
                break;
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
        qWarning() << "update VehicleDrivInfoCollectors failed! Unknow Error.";
        result = false;
    }
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsUpdate::updateVehicleQueueCounters(const QList<GVehicleQueueCounter*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        QSqlQuery  query(gDB);
        foreach (auto it, list) {
            QString sql="UPDATE VehicleQueueCounter set ";
            sql+=QString("name='%1'").arg(it->mpVehicleQueueCounter->name);
            sql+=QString(",roadID=%1").arg(it->mpVehicleQueueCounter->roadID);
            sql+=QString(",laneNumber=%1").arg(it->mpVehicleQueueCounter->laneNumber);
            sql+=QString(",toLaneNumber=%1").arg(it->mpVehicleQueueCounter->toLaneNumber);
            sql+=QString(",x=%1").arg(it->mpVehicleQueueCounter->x);
            sql+=QString(",y=%1").arg(it->mpVehicleQueueCounter->y);
            sql+=QString(",z=%1").arg(it->mpVehicleQueueCounter->x);
            sql+=QString(",speedLowLimit=%1").arg(it->mpVehicleQueueCounter->speedLowLimit);
            sql+=QString(",speedUpLimit=%1").arg(it->mpVehicleQueueCounter->speedUpLimit);
            sql+=QString(",maxDistInterval=%1").arg(it->mpVehicleQueueCounter->maxDistInterval);
            sql+=QString(",maxQueueLength=%1").arg(it->mpVehicleQueueCounter->maxQueueLength);
            sql+=QString(",distance=%1").arg(it->mpVehicleQueueCounter->distance);
            sql+=QString(",dataInterval=%1").arg(it->mpVehicleQueueCounter->dataInterval);
            sql+=QString(",startTime=%1").arg(it->mpVehicleQueueCounter->startTime);
            sql+=QString(",endTime=%1").arg(it->mpVehicleQueueCounter->endTime);
            sql+=QString(",countInterval=%1").arg(it->mpVehicleQueueCounter->countInterval);
            sql+=QString(" WHERE QueueCounterID=%1").arg(it->mpVehicleQueueCounter->queueCounterID);
            query.prepare(sql);
            result=query.exec();
            if(!result) {
                throw PH::Exception(query.lastError().text().toStdString());
                break;
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
        qWarning() << "update VehicleQueueCounters failed! Unknow Error.";
        result = false;
    }
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsUpdate::updateVehicleTravelDetectors(const QList<GVehicleTravelDetector*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        QSqlQuery  query(gDB);
        foreach (auto it, list) {
            QString sql="UPDATE VehicleTravelDetector set ";
            sql+=QString("name='%1'").arg(it->mpVehicleTravelDetector->name);
            sql+=QString(",startRoadId=%1").arg(it->mpVehicleTravelDetector->startRoadId);
            sql+=QString(",start_laneNumber=%1").arg(it->mpVehicleTravelDetector->start_laneNumber);
            sql+=QString(",start_toLaneNumber=%1").arg(it->mpVehicleTravelDetector->start_toLaneNumber);
            sql+=QString(",startDist=%1").arg(it->mpVehicleTravelDetector->startDist);
            sql+=QString(",startX=%1").arg(it->mpVehicleTravelDetector->startX);
            sql+=QString(",startY=%1").arg(it->mpVehicleTravelDetector->startY);
            sql+=QString(",teminalRoadId=%1").arg(it->mpVehicleTravelDetector->teminalRoadId);
            sql+=QString(",teminal_laneNumber=%1").arg(it->mpVehicleTravelDetector->teminal_laneNumber);
            sql+=QString(",teminal_toLaneNumber=%1").arg(it->mpVehicleTravelDetector->teminal_toLaneNumber);
            sql+=QString(",teminalDist=%1").arg(it->mpVehicleTravelDetector->teminalDist);
            sql+=QString(",teminalX=%1").arg(it->mpVehicleTravelDetector->teminalX);
            sql+=QString(",teminalY=%1").arg(it->mpVehicleTravelDetector->teminalY);
            sql+=QString(",startTime=%1").arg(it->mpVehicleTravelDetector->startTime);
            sql+=QString(",endTime=%1").arg(it->mpVehicleTravelDetector->endTime);
            sql+=QString(",dataInterval=%1").arg(it->mpVehicleTravelDetector->dataInterval);
            sql+=QString(",shortestDistance=%1").arg(it->mpVehicleTravelDetector->shortestDistance);
            sql+=QString(" WHERE detectorId=%1").arg(it->mpVehicleTravelDetector->detectorId);
            query.prepare(sql);
            result=query.exec();
            if(!result) {
                throw PH::Exception(query.lastError().text().toStdString());
                break;
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
        qWarning() << "update VehicleTravelDetectors failed! Unknow Error.";
        result = false;
    }

    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}

bool TessngDBToolsUpdate::updateVehicleDetectors(const QList<GVehicleDetector*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        QSqlQuery  query(gDB);
        foreach (auto it, list) {
            QString sql="UPDATE VehicleDetector set ";
            sql+=QString("name='%1'").arg(it->mpVehicleDetector->name);
            sql+=QString(",roadType='%1'").arg(it->mpVehicleDetector->roadType);
            sql+=QString(",type='%1'").arg(it->mpVehicleDetector->type);
            sql+=QString(",phaseNumber=%1").arg(it->mpVehicleDetector->phaseNumber);
            sql+=QString(",roadId=%1").arg(it->mpVehicleDetector->roadId);
            sql+=QString(",laneNumber=%1").arg(it->mpVehicleDetector->laneNumber);
            sql+=QString(",toLaneNumber=%1").arg(it->mpVehicleDetector->toLaneNumber);
            sql+=QString(",length=%1").arg(it->mpVehicleDetector->length);
            sql+=QString(",distToTerminal=%1").arg(it->mpVehicleDetector->distToTerminal);
            sql+=QString(",maxGreen=%1").arg(it->mpVehicleDetector->maxGreen);
            sql+=QString(" WHERE vehicleDetectorId=%1").arg(it->mpVehicleDetector->vehicleDetectorId);
            query.prepare(sql);
            result=query.exec();
            if(!result) {
                throw PH::Exception(query.lastError().text().toStdString());
                break;
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
        qWarning() << "update VehicleDetectors failed! Unknow Error.";
        result = false;
    }

    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateReduceSpeedVehiTypes(const QList<ReduceSpeedVehiType *> &list)
{
    bool result=true;
    QSqlQuery  query(gDB);
    foreach (auto it, list) {
        QString sql="UPDATE ReduceSpeedVehiType set ";
        sql+=QString(",avgSpeed=%1").arg(it->avgSpeed);
        sql+=QString(",speedSD=%1").arg(it->speedSD);
        sql+=QString(" WHERE vehicleTypeCode=%1 and reduceSpeedAreaID=%2").arg(it->vehicleTypeCode).arg(it->reduceSpeedAreaID);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateReduceSpeedIntervals(const QList<ReduceSpeedInterval*>& list){
    bool result=true;
    QSqlQuery  query(gDB);
    foreach (auto it, list) {
        QString sql="UPDATE ReduceSpeedInterval set ";
        sql+=QString(",reduceSpeedAreaID=%1").arg(it->reduceSpeedAreaID);
        sql+=QString(",startTime=%1").arg(it->startTime);
        sql+=QString(",endTime=%1").arg(it->endTime);
        sql+=QString(" WHERE intervalID=%1").arg(it->intervalID);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }
    return result;
}
bool TessngDBToolsUpdate::updateReduceSpeedArea(ReduceSpeedArea* pReduceSpeedArea){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE ReduceSpeedArea set ";
    sql+=QString("name='%1'").arg(pReduceSpeedArea->name);
    sql+=QString(",location=%1").arg(pReduceSpeedArea->location);
    sql+=QString(",areaLength=%1").arg(pReduceSpeedArea->areaLength);
    sql+=QString(",roadID=%1").arg(pReduceSpeedArea->roadID);
    sql+=QString(",laneNumber=%1").arg(pReduceSpeedArea->laneNumber);
    sql+=QString(",toLaneNumber=%1").arg(pReduceSpeedArea->toLaneNumber);
    sql+=QString(" WHERE reduceSpeedAreaID=%1").arg(pReduceSpeedArea->reduceSpeedAreaID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}
bool TessngDBToolsUpdate::updateReduceSpeedAreas(const QList<GReduceSpeedArea *> &list)
{
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result=updateReduceSpeedVehiTypes(it->mpReduceSpeedArea->mlReduceSpeedVehiType);
            if(!result) goto failed;

            result=updateReduceSpeedIntervals(it->mpReduceSpeedArea->mlReduceSpeedInterval);
            if(!result) goto failed;

            result=updateReduceSpeedArea(it->mpReduceSpeedArea);
            if(!result) goto failed;
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
        qWarning() << "update ReduceSpeedAreas failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateBustation(const QList<GBusStation*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        QSqlQuery  query(gDB);
        foreach (auto it, list) {
            QString sql="UPDATE BusStation set ";
            sql+=QString("name='%1'").arg(it->mpBusStation->name);
            sql+=QString("type='%1'").arg(gSpecialWords.trZhToEn(it->mpBusStation->type));
            sql+=QString(",linkID=%1").arg(it->mpBusStation->mpLink->linkID);
            sql+=QString(",laneNumber=%1").arg(it->mpBusStation->laneNumber);
            sql+=QString(",x=%1").arg(it->mpBusStation->x);
            sql+=QString(",y=%1").arg(it->mpBusStation->y);
            sql+=QString(",length=%1").arg(it->mpBusStation->length);
            sql+=QString(" WHERE busStationID=%1").arg(it->mpBusStation->busStationID);
            query.prepare(sql);
            result=query.exec();
            if(!result) {
                throw PH::Exception(query.lastError().text().toStdString());
                break;
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
        qWarning() << "update Bustation failed! Unknow Error.";
        result = false;
    }
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
bool TessngDBToolsUpdate::updateBusLineRoads(BusLine* bline){
    bool result=TessngDBToolsRemove::getInstance()->removeBusLineRoad(bline);
    if(!result) return false;
    QSqlQuery  query(gDB);
    QString sqlInsLinks = "insert into BusLineRoad(busLineID, serialNumber, roadID) values(:busLineID, :serialNumber, :roadID)";
    query.prepare(sqlInsLinks);
    for (int i = 0, size = bline->mlLink.size(); i < size; ++i)
    {
        Link* pLink = bline->mlLink.at(i);
        query.bindValue(":busLineID", QVariant::fromValue(bline->busLineID));
        query.bindValue(":serialNumber", i + 1);
        query.bindValue(":roadID", QVariant::fromValue(pLink->linkID));
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }

    return result;
}

bool TessngDBToolsUpdate::updatePassengerArriving(long stationLineID,const QList<PassengerArriving*>& list){
    bool result=true;
    QSqlQuery  query(gDB);
    foreach (auto it, list) {
        QString sql="UPDATE PassengerArriving set ";
        sql+=QString(",stationLineID=%1").arg(stationLineID);
        sql+=QString(",startTime=%1").arg(it->startTime);
        sql+=QString(",endTime=%1").arg(it->endTime);
        sql+=QString(",passengerCount=%1").arg(it->passengerCount);
        sql+=QString(" WHERE passengerID=%1").arg(it->passengerArrivingID);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }

    return result;
}

bool TessngDBToolsUpdate::updateBusStationLine(const QList<BusStationLine*>& list){
    bool result=true;
    QSqlQuery  query(gDB);
    foreach (auto it, list) {
        result=updatePassengerArriving(it->stationLineID,it->mlPassengerArriving);
        if(!result) break;
        QString sql="UPDATE BusStationLine set ";
        sql+=QString(",busStationID=%1").arg(it->busStationID);
        sql+=QString(",busLineID=%1").arg(it->busLineID);
        sql+=QString(",parkingTime=%1").arg(it->parkingTime);
        sql+=QString(",getOnTimePerson=%1").arg(it->getOnTimePerson);
        sql+=QString(",getOutTimePerson=%1").arg(it->getOutTimePerson);
        sql+=QString(",leavingPercent=%1").arg(it->leavingPercent);
        sql+=QString(" WHERE stationLineID=%1").arg(it->stationLineID);
        query.prepare(sql);
        result=query.exec();
        if(!result) {
            throw PH::Exception(query.lastError().text().toStdString());
            break;
        }
    }

    return result;
}

bool TessngDBToolsUpdate::updateBusLine(BusLine* pBusLine){
    bool result=true;
    QSqlQuery  query(gDB);
    QString sql="UPDATE BusLine set ";
    sql+=QString("name='%1'").arg(pBusLine->name);
    sql+=QString(",length=%1").arg(pBusLine->length);
    sql+=QString(",dischargeFreq=%1").arg(pBusLine->dischargeFreq);
    sql+=QString(",dischargeStartTime=%1").arg(pBusLine->dischargeStartTime);
    sql+=QString(",dischargeEndTime=%1").arg(pBusLine->dischargeEndTime);
    sql+=QString(",startX=%1").arg(pBusLine->startX);
    sql+=QString(",startY=%1").arg(pBusLine->startY);
    sql+=QString(",endX=%1").arg(pBusLine->endX);
    sql+=QString(",endY=%1").arg(pBusLine->endY);
    sql+=QString(",desirSpeed=%1").arg(pBusLine->desirSpeed);
    sql+=QString(",speedSD=%1").arg(pBusLine->speedSD);
    sql+=QString(",passCountAtStartTime=%1").arg(pBusLine->passCountAtStartTime);
    sql+=QString(" WHERE busLineID=%1").arg(pBusLine->busLineID);
    query.prepare(sql);
    result=query.exec();
    if(!result) throw PH::Exception(query.lastError().text().toStdString());
    return result;
}

bool TessngDBToolsUpdate::updateBusLine(const QList<GBusLine*>& list){
    bool result = true;
    try {
        //开启事务
        gDB.transaction();
        foreach (auto it, list) {
            result =updateBusLineRoads(it->mpBusLine);
            if(!result) goto failed;

            result =updateBusStationLine(it->mpBusLine->mlBusStationLine);
            if(!result) goto failed;

            result =updateBusLine(it->mpBusLine);
            if(!result) goto failed;
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
        qWarning() << "update GBusLine failed! Unknow Error.";
        result = false;
    }
failed:
    result = gDB.commit() && result;
    if (!result) {
        gDB.rollback();
    }

    return result;
}
