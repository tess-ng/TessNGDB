#include "TessngDBCopy.h"
#include "SpecialWords.h"
#include "exception.h"
#include <QSqlError>
#include "DbGlobal.h"
#include "configuration.h"
#include "net.h"
#include <QFile>
#include <QFileInfo>
#include <QSqlQuery>
#include "Vertex.h"
#include "Node.h"
#include "Lane.h"
#include "connector.h"
#include "laneconnector.h"
#include "DecisionPoint.h"
#include "Routing.h"
#include "RoutingFLowRatio.h"
#include "signalgroup.h"
#include "signallamp.h"
#include "VehicleComposition.h"
#include "VehicleConsDetail.h"
#include "DeparturePoint.h"
#include "VehicleDrivInfoCollector.h"
#include "VehicleTravelDetector.h"
#include "VehicleQueueCounter.h"
#include "ReduceSpeedArea.h"
#include "ReduceSpeedInterval.h"
#include "ReduceSpeedVehiType.h"
#include "BusStation.h"
#include "BusLine.h"
#include "BusStationLine.h"
#include "PassengerArriving.h"
#include <QDebug>
#include <QUuid>
bool TessngDBCopy::copyDb(const QString& dbFileName)
{

    if (database.isOpen()) {
        database.close();
    }
    QString file = dbFileName;
    QFileInfo fileInfo(dbFileName);
    if (fileInfo.isDir()) {
        file = dbFileName + "default.tess";
    }
    if ("tess" != fileInfo.completeSuffix()) {
        file = dbFileName + ".tess";
    }
    QFile::remove(dbFileName);
    QFile::copy("template/template.tess", file);

    try {
        database = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
        database.setDatabaseName(file);
        database.open();
        return clearDBData();
    }
    catch (...) {
        throw PH::Exception(database.lastError().text().toStdString());
    }
    return false;
}
QSqlDatabase* TessngDBCopy::getSqlDatabase()
{
    return &database;
}
TessngDBCopy::TessngDBCopy()
{

}
bool TessngDBCopy::doSql(const QString& sql) {
    QSqlQuery query(database);
    query.prepare(sql);
    return query.exec();
}
bool TessngDBCopy::clearDBData() {
    bool result = true;
    QStringList listStrs;
    listStrs << "Net";
    listStrs << "VehicleType";
    listStrs << "VehicleConstitutent";
    listStrs << "VehicleConsDetail";
    listStrs << "Configuration";
    foreach(const QString & tablename, listStrs) {
        if ("IDGenerater" == tablename) continue;
        result = doSql(QString("delete from %1").arg(tablename)) && result;
    }
    return result;
}
bool TessngDBCopy::saveConfiguration() {
    QString sql = QString("insert into Configuration(sceneWidth, sceneHeight, scale, showGrid, showBackgroundMap, "
        "backgroundMap, saveDate) values(:sceneWidth, :sceneHeight, :scale, :showGrid, :showBackgroundMap, :backgroundMap, :saveDate)");
    QSqlQuery query(database);
    query.prepare(sql);
    Configuration* pConfig = gNet.config();
    query.bindValue(":sceneWidth", pConfig->sceneWidth);
    query.bindValue(":sceneHeight", pConfig->sceneHeight);
    query.bindValue(":scale", pConfig->scale);
    query.bindValue(":showGrid", pConfig->showGrid ? "T" : "F");
    query.bindValue(":showBackgroundMap", pConfig->showBackgroundMap ? "T" : "F");
    query.bindValue(":backgroundMap", pConfig->backgroundMap);
    query.bindValue(":saveDate", QVariant(pConfig->saveDate));
    return query.exec();
}
bool TessngDBCopy::insertLink(Link* pLink) {
    bool result = true;
    QSqlQuery query(database);
    QString sql = "insert into Vertex(VertexID, X, Y, Z) values(:vertexID, :X, :Y, :Z)";
    query.prepare(sql);
    for (int i = 0, size = pLink->mlVertex.size(); i < size; ++i)
    {
        Vertex* pVertex = pLink->mlVertex.at(i);
        qreal z = pVertex->z;
        query.bindValue(":vertexID", QVariant::fromValue(pVertex->vertexID));
        query.bindValue(":X", pVertex->x);
        query.bindValue(":Y", pVertex->y);
        query.bindValue(":Z", z);
        result = query.exec() && result;
    }

    //插入node
    if (pLink->startNode->nodeName.isEmpty())
    {
        pLink->startNode->nodeName = QString::number(pLink->startNode->nodeID);
    }
    sql = "insert into Node(nodeID, nodeName, vertexID) values(:nodeID, :nodeName, :vertexID)";
    query.prepare(sql);
    query.bindValue(":nodeID", QVariant::fromValue(pLink->startNode->nodeID));
    query.bindValue(":nodeName", QVariant::fromValue(pLink->startNode->nodeName));
    query.bindValue(":vertexID", QVariant::fromValue(pLink->startNode->mpVertex->vertexID));
    result = query.exec() && result;

    if (pLink->endNode->nodeName.isEmpty())
    {
        pLink->endNode->nodeName = QString::number(pLink->endNode->nodeID);
    }
    query.bindValue(":nodeID", QVariant::fromValue(pLink->endNode->nodeID));
    query.bindValue(":nodeName", pLink->endNode->nodeName);
    query.bindValue(":vertexID", QVariant::fromValue(pLink->endNode->mpVertex->vertexID));
    result = query.exec() && result;

    //插入link

    sql = "insert into Link(linkID,netId,StartNodeID, EndNodeID,LinkName,laneNumber,laneWidth,laneColor,linkType,length,curvature,"
        "nonLinearCoefficient, linkSaturationFlow, linkTrafficFlow, desiredSpeed, limitSpeed, minSpeed, addValue) values(:linkID, :netId, :StartNodeID,"
        ":EndNodeID,:linkName,:laneNumber,:laneWidth,:laneColor,:linkType,"
        ":length,:curvature,:nonLinearCoefficient,:linkSaturationFlow,:linkTrafficFlow,:desiredSpeed,:limitSpeed,:minSpeed,:addValue)";
    query.prepare(sql);
    query.bindValue(":linkID", QVariant::fromValue(pLink->linkID));
    query.bindValue(":netId", QVariant::fromValue(gNet.NetId));
    query.bindValue(":StartNodeID", QVariant::fromValue(pLink->startNode->nodeID));
    query.bindValue(":EndNodeID", QVariant::fromValue(pLink->endNode->nodeID));
    query.bindValue(":linkName", pLink->linkName);
    query.bindValue(":laneNumber", pLink->laneNumber);
    query.bindValue(":laneWidth", pLink->laneWidth);
    query.bindValue(":laneColor", pLink->laneColor);
    query.bindValue(":linkType", pLink->linkType);
    query.bindValue(":length", pLink->length * SCALE);
    query.bindValue(":curvature", pLink->curvature);
    query.bindValue(":nonLinearCoefficient", pLink->nonLinearCoefficient);
    query.bindValue(":linkSaturationFlow", pLink->linkSaturationFlow);
    query.bindValue(":linkTrafficFlow", pLink->linkTrafficFlow);
    query.bindValue(":desiredSpeed", pLink->desiredSpeed);
    query.bindValue(":limitSpeed", pLink->limitSpeed);
    query.bindValue(":minSpeed", pLink->minSpeed);
    query.bindValue(":addValue", pLink->addValue);
    result = query.exec() && result;
    if (result)
    {
        if (pLink->linkID > 0)
        {
            QJsonDocument jsonDoc = QJsonDocument(pLink->centerLinePointsJson);
            QByteArray centerLinePointsJson = jsonDoc.toJson(QJsonDocument::Compact);
            jsonDoc = QJsonDocument(pLink->mlLane.last()->leftBreakPointsJson);
            QByteArray leftBreakPointsJson = jsonDoc.toJson(QJsonDocument::Compact);
            jsonDoc = QJsonDocument(pLink->mlLane.first()->rightBreakPointsJson);
            QByteArray rightBreakPointsJson = jsonDoc.toJson(QJsonDocument::Compact);
            sql = QString::fromUtf8("update Link set centerLinePointsJson=:centerLinePointsJson,leftBreakPointsJson=:leftBreakPointsJson,rightBreakPointsJson=:rightBreakPointsJson where linkID=%1").arg(pLink->linkID);
            query.prepare(sql);
            query.bindValue(":centerLinePointsJson", QVariant::fromValue(centerLinePointsJson));
            query.bindValue(":leftBreakPointsJson", QVariant::fromValue(leftBreakPointsJson));
            query.bindValue(":rightBreakPointsJson", QVariant::fromValue(rightBreakPointsJson));
            result = query.exec() && result;
            if (!pLink->otherAttrsJson.isEmpty())
            {
                QJsonDocument jsonDoc = QJsonDocument(pLink->otherAttrsJson);
                QByteArray otherAttrsJson = jsonDoc.toJson(QJsonDocument::Compact);
                sql = QString::fromUtf8("update Link set otherAttrsJson=:otherAttrsJson where linkID=%1").arg(pLink->linkID);
                query.prepare(sql);
                query.bindValue(":otherAttrsJson", otherAttrsJson);
                result = query.exec() && result;
            }
        }
    }

    //插入车道
    if (pLink->netId == 0)
    {
        sql = "insert into Lane(LaneID, LinkID, SerialNumber, Width, ActionType, ExpectTravelDirection) values(:LaneID, :LinkID, :SerialNumber, :Width, :ActionType, :ExpectTravelDirection)";

        query.prepare(sql);
        foreach(Lane * pLane, pLink->mlLane)
        {
            query.bindValue(":LaneID", QVariant::fromValue(pLane->laneID));
            query.bindValue(":LinkID", QVariant::fromValue(pLane->linkID));
            query.bindValue(":SerialNumber", pLane->serialNumber);
            query.bindValue(":Width", pLane->width);
            //将车道类型翻译成英文（只有路网是英文版，软件是中文版时才翻译）
            QString atype = gSpecialWords.trZhToEn(pLane->actionType);
            query.bindValue(":ActionType", atype);
            //将期望行驶方向翻译成英文（只有路网是英文版，软件是中文版时才翻译）
            QString expectDir = gSpecialWords.trZhToEn(pLane->expectTravelDirection);
            query.bindValue(":ExpectTravelDirection", expectDir);
            result = query.exec() && result;
        }
    }
    else
    {
        sql = "insert into Lane(LaneID, LinkID, SerialNumber, Width, ActionType, ExpectTravelDirection, centerLinePointsJson, leftBreakPointsJson, rightBreakPointsJson) values(:LaneID, :LinkID, :SerialNumber, :Width, :ActionType, :ExpectTravelDirection, :centerLinePointsJson, :leftBreakPointsJson, :rightBreakPointsJson)";

        query.prepare(sql);
        foreach(Lane * pLane, pLink->mlLane)
        {
            QJsonDocument jsonDoc = QJsonDocument(pLane->centerLinePointsJson);
            QByteArray centerLinePointsJson = jsonDoc.toJson(QJsonDocument::Compact);
            jsonDoc = QJsonDocument(pLane->leftBreakPointsJson);
            QByteArray leftBreakPointsJson = jsonDoc.toJson(QJsonDocument::Compact);
            jsonDoc = QJsonDocument(pLane->rightBreakPointsJson);
            QByteArray rightBreakPointsJson = jsonDoc.toJson(QJsonDocument::Compact);

            query.bindValue(":LaneID", QVariant::fromValue(pLane->laneID));
            query.bindValue(":LinkID", QVariant::fromValue(pLane->linkID));
            query.bindValue(":SerialNumber", pLane->serialNumber);
            query.bindValue(":Width", pLane->width);
            query.bindValue(":ActionType", pLane->actionType);
            query.bindValue(":ExpectTravelDirection", pLane->expectTravelDirection);
            query.bindValue(":centerLinePointsJson", QVariant::fromValue(centerLinePointsJson));
            query.bindValue(":leftBreakPointsJson", QVariant::fromValue(leftBreakPointsJson));
            query.bindValue(":rightBreakPointsJson", QVariant::fromValue(rightBreakPointsJson));
            result = query.exec() && result;
            if (result)
            {
                if (!pLane->otherAttrsJson.isEmpty())
                {
                    QJsonDocument jsonDoc = QJsonDocument(pLink->otherAttrsJson);
                    QByteArray otherAttrsJson = jsonDoc.toJson(QJsonDocument::Compact);
                    sql = QString::fromUtf8("update Lane set otherAttrsJson=:otherAttrsJson where linkID=%0 and laneID=%1").arg(pLink->linkID).arg(pLane->laneID);
                    query.prepare(sql);
                    query.bindValue(":otherAttrsJson", otherAttrsJson);
                    result = query.exec() && result;
                }
            }
        }
    }

    //插入车道限制车型
    sql = "insert into LaneLimitVehicle(LaneID, VehicleTypeCode) values(:LaneID, :VehicleTypeCode)";
    query.prepare(sql);
    foreach(Lane * pLane, pLink->mlLane)
    {
        foreach(VehicleType vt, pLane->mlVehicleType)
        {
            query.bindValue(":LaneID", QVariant::fromValue(pLane->laneID));
            query.bindValue(":VehicleTypeCode", QVariant::fromValue(vt.vehicleTypeCode));
            result = query.exec() && result;
        }
    }

    //插入linkvertex
    sql = "insert into LinkVertex(LinkID, VertexID, Num) values(:linkID, :vertexID, :Num)";
    query.prepare(sql);
    int num = 0;
    foreach(Vertex * pVertex, pLink->mlVertex)
    {
        num++;
        query.bindValue(":linkID", QVariant::fromValue(pLink->linkID));
        query.bindValue(":vertexID", QVariant::fromValue(pVertex->vertexID));
        query.bindValue(":Num", num);
        result = query.exec() && result;
    }
    return result;
}
bool TessngDBCopy::insertConnector(Connector* pConnector) {
    bool result = true;
    QSqlQuery query(database);
    QString sql;
    //插入连接段
    sql = QString("insert into Connector(ConnID, StartLinkID, EndLinkID, ConnName, Length, Curvature, NonLinearCoefficient, Color, DesiredSpeed, LimitSpeed) values(:ConnID, :StartLinkID, :EndLinkID, :ConnName, :Length, :Curvature, :NonLinearCoefficient, :Color, :DesiredSpeed, :LimitSpeed)");

    query.prepare(sql);
    query.bindValue(":ConnID", QVariant::fromValue(pConnector->connID));
    query.bindValue(":StartLinkID", QVariant::fromValue(pConnector->mpFromLink->linkID));
    query.bindValue(":EndLinkID", QVariant::fromValue(pConnector->mpToLink->linkID));
    query.bindValue(":ConnName", pConnector->connName);
    query.bindValue(":Length", pConnector->length);
    query.bindValue(":Curvature", pConnector->curvature);
    query.bindValue(":NonLinearCoefficient", pConnector->nonLinearCoefficient);
    query.bindValue(":Color", pConnector->color);
    query.bindValue(":DesiredSpeed", pConnector->desiredSpeed);
    query.bindValue(":LimitSpeed", pConnector->limitSpeed);
    result = query.exec() && result;
    if (pConnector->mpFromLink->netId > 0)
    {
        QJsonDocument jsonDoc = QJsonDocument(pConnector->leftBreakPointsJson);
        QByteArray leftBreakPointsJson = jsonDoc.toJson(QJsonDocument::Compact);
        jsonDoc = QJsonDocument(pConnector->rightBreakPointsJson);
        QByteArray rightBreakPointsJson = jsonDoc.toJson(QJsonDocument::Compact);
        sql = QString::fromUtf8("update Connector set leftBreakPointsJson=:leftBreakPointsJson,rightBreakPointsJson=:rightBreakPointsJson where connID=%1").arg(pConnector->connID);
        query.prepare(sql);
        query.bindValue(":leftBreakPointsJson", QVariant::fromValue(leftBreakPointsJson));
        query.bindValue(":rightBreakPointsJson", QVariant::fromValue(rightBreakPointsJson));
        result = query.exec() && result;
        if (!pConnector->otherAttrsJson.isEmpty())
        {
            QJsonDocument jsonDoc = QJsonDocument(pConnector->otherAttrsJson);
            QByteArray otherAttrsJson = jsonDoc.toJson(QJsonDocument::Compact);
            sql = QString::fromUtf8("update Connector set otherAttrsJson=:otherAttrsJson where connID=%1").arg(pConnector->connID);
            query.prepare(sql);
            query.bindValue(":otherAttrsJson", otherAttrsJson);
            result = query.exec() && result;
        }
    }

    //插入连接段限制车型
    sql = QString("insert into ConnectorLimitVehicle(ConnID, VehicleTypeCode) values(:ConnID, :VehicleTypeCode)");
    query.prepare(sql);
    foreach(VehicleType vt, pConnector->mlVehicleType)
    {
        query.bindValue(":", QVariant::fromValue(pConnector->connID));
        query.bindValue(":", QVariant::fromValue(vt.vehicleTypeCode));
        result = query.exec() && result;
    }

    //插入车道连接
    if (pConnector->mpFromLink->netId == 0)
    {
        sql = QString("insert into LaneConnector(connID, StartLaneID, EndLaneID, weight) values(:connID, :StartLaneID, :EndLaneID, :weight)");
        query.prepare(sql);
        foreach(LaneConnector * pLaneConnector, pConnector->mlLaneConnector)
        {
            query.bindValue(":connID", QVariant::fromValue(pConnector->connID));
            query.bindValue(":StartLaneID", QVariant::fromValue(pLaneConnector->mpFromLane->laneID));
            query.bindValue(":EndLaneID", QVariant::fromValue(pLaneConnector->mpToLane->laneID));
            query.bindValue(":weight", pLaneConnector->weight);
            result = query.exec() && result;
        }
    }
    else
    {
        sql = QString("insert into LaneConnector(connID, StartLaneID, EndLaneID, weight, centerLinePointsJson, leftBreakPointsJson, rightBreakPointsJson) "
            "values(:connID, :StartLaneID, :EndLaneID, :weight, :centerLinePointsJson, :leftBreakPointsJson, :rightBreakPointsJson)");
        query.prepare(sql);
        foreach(LaneConnector * pLaneConnector, pConnector->mlLaneConnector)
        {
            QJsonDocument jsonDoc = QJsonDocument(pLaneConnector->centerLinePointsJson);
            QByteArray centerLinePointsJson = jsonDoc.toJson(QJsonDocument::Compact);
            jsonDoc = QJsonDocument(pLaneConnector->leftBreakPointsJson);
            QByteArray leftPointsJson = jsonDoc.toJson(QJsonDocument::Compact);
            jsonDoc = QJsonDocument(pLaneConnector->rightBreakPointsJson);
            QByteArray rightPointsJson = jsonDoc.toJson(QJsonDocument::Compact);

            query.bindValue(":connID", QVariant::fromValue(pConnector->connID));
            query.bindValue(":StartLaneID", QVariant::fromValue(pLaneConnector->mpFromLane->laneID));
            query.bindValue(":EndLaneID", QVariant::fromValue(pLaneConnector->mpToLane->laneID));
            query.bindValue(":weight", pLaneConnector->weight);
            query.bindValue(":centerLinePointsJson", QVariant::fromValue(centerLinePointsJson));
            query.bindValue(":leftBreakPointsJson", QVariant::fromValue(leftPointsJson));
            query.bindValue(":rightBreakPointsJson", QVariant::fromValue(rightPointsJson));
            result = query.exec() && result;
        }
    }


    return result;
}
bool TessngDBCopy::insertNet() {
    long netId = gNet.id();
    QString name = gNet.netName();
    QPointF centerPoint = gNet.centerPoint();
    QString sourceType = gNet.type();
    QString backgroundUrl = gNet.bkgUrl();
    QJsonObject otherAttrsJson = gNet.otherAttrs();


    QString sql = "insert into Net(NetId, name, centerX, centerY, sourceType, backgroundUrl, otherAttrsJson)"
        " values(:NetId, :name, :centerX, :centerY, :sourceType, :backgroundUrl, :otherAttrsJson)";
    QSqlQuery query(database);
    query.prepare(sql);
    if (name.isEmpty())
    {
        name = "TESSNG";
    }
    query.bindValue(":NetId", QVariant::fromValue(netId));
    query.bindValue(":name", name);
    query.bindValue(":centerX", centerPoint.x());
    query.bindValue(":centerY", centerPoint.y());
    query.bindValue(":sourceType", sourceType);
    query.bindValue(":backgroundUrl", backgroundUrl);
    QJsonDocument jsonDoc = QJsonDocument(otherAttrsJson);
    QByteArray otherAttrs = jsonDoc.toJson(QJsonDocument::Compact);
    query.bindValue(":otherAttrsJson", QVariant::fromValue(QString::fromUtf8(otherAttrs)));
    bool flag = query.exec();

#ifdef _DEBUG
    if (!flag) { qDebug() << query.lastError(); }
#endif

    return flag;
}
bool TessngDBCopy::insertRouting(Routing* pRouting) {
    bool result = true;
    if (!pRouting)
    {
        return result;
    }

    QSqlQuery query(database);
    QString sql = "insert into Routing(routingID, deciPointID, routingName, proportion) values(:routingID, :deciPointID, :routingName, :proportion)";
    query.prepare(sql);
    query.bindValue(":routingID", QVariant::fromValue(pRouting->routingID));
    query.bindValue(":deciPointID", QVariant::fromValue(pRouting->mpDecisionPoint->deciPointID));
    query.bindValue(":routingName", pRouting->routingName);
    query.bindValue(":proportion", pRouting->proportion);
    result = query.exec() && result;

    sql = "insert into RoutingLink(routingID, linkID, num1, num2) values(:routingID, :linkID, :num1, :num2)";
    query.prepare(sql);
    int num1 = 1;
    foreach(QList<Link*> lLink, pRouting->mllLink)
    {
        int num2 = 1;
        foreach(Link * pLink, lLink)
        {
            query.bindValue(":routingID", QVariant::fromValue(pRouting->routingID));
            query.bindValue(":linkID", QVariant::fromValue(pLink->linkID));
            query.bindValue(":num1", num1);
            query.bindValue(":num2", num2);
            result = query.exec() && result;
            num2++;
        }
        num1++;
    }

    return result;
}
bool TessngDBCopy::insertRoutingFlowRatio(long startDateTime, RoutingFlowByInterval* pRoutingFlowByInterval) {
    bool result = true;

    QSqlQuery query(database);
    QString sql = "insert into RoutingFLowRatio(RoutingFLowRatioID, routingID, startDateTime, endDateTime, ratio) values(:RoutingFLowRatioID, :routingID, :startDateTime, :endDateTime, :ratio)";
    query.prepare(sql);
    foreach(RoutingFLowRatio * pRoutingFLowRatio, pRoutingFlowByInterval->mlRoutingFlowRatio)
    {
        long endDateTime = pRoutingFlowByInterval->endDateTime;
        query.bindValue(":RoutingFLowRatioID", QVariant::fromValue(pRoutingFLowRatio->RoutingFLowRatioID));
        query.bindValue(":routingID", QVariant::fromValue(pRoutingFLowRatio->routingID));
        query.bindValue(":startDateTime", QVariant::fromValue(startDateTime));
        query.bindValue(":endDateTime", QVariant::fromValue(endDateTime));
        query.bindValue(":ratio", pRoutingFLowRatio->ratio);
        result = query.exec() && result;
    }

    return result;
}
bool TessngDBCopy::insertDecisionPoint(DecisionPoint* pDecisionPoint) {
    bool result = true;
    QSqlQuery query(database);
    QString sql = "insert into DecisionPoint(deciPointID, deciPointName, linkID, x, y, z) values(:deciPointID, :deciPointName, :linkID, :x, :y, :z)";
    query.prepare(sql);
    query.bindValue(":deciPointID", QVariant::fromValue(pDecisionPoint->deciPointID));
    query.bindValue(":deciPointName", pDecisionPoint->deciPointName);
    query.bindValue(":linkID", QVariant::fromValue(pDecisionPoint->mpLink->linkID));
    query.bindValue(":x", pDecisionPoint->X);
    query.bindValue(":y", pDecisionPoint->Y);
    query.bindValue(":z", pDecisionPoint->Z);
    ;
    result = query.exec() && result;

    foreach(Routing * pRouting, pDecisionPoint->mlRouting)
    {
        result = insertRouting(pRouting) && result;
    }

    long startDateTime = 1;
    foreach(RoutingFlowByInterval * pRPI, pDecisionPoint->mlRoutingFlowByInterval)
    {
        foreach(RoutingFLowRatio * pRFR, pRPI->mlRoutingFlowRatio)
        {
            bool found = false;
            foreach(Routing * pRouting, pDecisionPoint->mlRouting)
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
        result = insertRoutingFlowRatio(startDateTime, pRPI) && result;
        startDateTime = pRPI->endDateTime + 1;
    }

    return result;
}
bool TessngDBCopy::insertRoutingLaneConnector(long routingId, long connectorId, long laneId1, long laneId2) {
    QSqlQuery query(database);
    QString sql = "insert into RoutingLaneConnector(routingID, connID, laneID1, laneID2) values(:routingID, :connID, :laneID1, :laneID2)";
    query.prepare(sql);
    query.bindValue(":routingID", QVariant::fromValue(routingId));
    query.bindValue(":connID", QVariant::fromValue(connectorId));
    query.bindValue(":laneID1", QVariant::fromValue(laneId1));
    query.bindValue(":laneID2", QVariant::fromValue(laneId2));
    return query.exec();
}
bool TessngDBCopy::insertSignalGroup(SignalGroup* pSignalGroup) {
    bool result = true;
    if (!pSignalGroup)
    {
        return result;
    }

    QSqlQuery sgQuery(database);
    QSqlQuery spQuery(database);
    QSqlQuery scQuery(database);

    //插入信号灯组
    QString sgSql = "insert into SignalGroup(signalGroupID, name, timeLength, startTime, endTime) values(:signalGroupID, :name, :timeLength, :startTime, :endTime)";
    QString spSql = "insert into SignalPhase(signalPhaseID, signalGroupID, serialNumber, name) values(:signalPhaseID, :signalGroupID, :serialNumber, :name)";
    QString scSql = "insert into SignalColor(signalPhaseID, serialNumber, color, timeLength) values(:signalPhaseID, :serialNumber, :color, :timeLength)";

    sgQuery.prepare(sgSql);
    sgQuery.bindValue(":signalGroupID", QVariant::fromValue(pSignalGroup->signalGroupID));
    sgQuery.bindValue(":name", pSignalGroup->name);
    sgQuery.bindValue(":timeLength", pSignalGroup->timeLength);
    sgQuery.bindValue(":startTime", QVariant::fromValue(pSignalGroup->startTime));
    sgQuery.bindValue(":endTime", QVariant::fromValue(pSignalGroup->endTime));
    result = sgQuery.exec() && result;

    //插入相位
    spQuery.prepare(spSql);
    foreach(SignalPhase * pSp, pSignalGroup->mlPhase)
    {
        spQuery.bindValue(":signalPhaseID", QVariant::fromValue(pSp->signalPhaseID));
        spQuery.bindValue(":signalGroupID", QVariant::fromValue(pSp->mpSignalGroup->signalGroupID));
        spQuery.bindValue(":serialNumber", pSp->serialNumber);
        spQuery.bindValue(":name", pSp->name);
        result = spQuery.exec() && result;
        // 插入相位的信号颜色
        scQuery.prepare(scSql);
        int serialNumber = 0;
        foreach(SignalColor sc, pSp->mlSignalColor)
        {
            scQuery.bindValue(":signalPhaseID", QVariant::fromValue(pSp->signalPhaseID));
            scQuery.bindValue(":serialNumber", ++serialNumber);
            //将灯色翻译成英文（只有当路网文件为英文，软件为中文时才会翻译）
            QString color = gSpecialWords.trZhToEn(sc.color);
            scQuery.bindValue(":color", color);
            scQuery.bindValue(":timeLength", sc.timeLength);
            result = scQuery.exec() && result;
        }
    }

    return result;
}
bool TessngDBCopy::updateIds()
{
    bool result = true;
    QSqlQuery query3(database);
    QSqlQuery query2(database);
    QSqlQuery query(database);
    query.exec("select TableName from IDGenerater");
    while (query.next())
    {
        if (!query2.prepare(QString("select last_insert_rowid() from %1").arg(query.value(0).toString())))
            continue;

        if (!query2.exec())
            continue;

        if (!query2.next())
            continue;
        result= result&&query3.exec(QString("update IDGenerater set ID = %1 where TableName = '%2'").arg(query2.value(0).toInt() + 1).arg(query.value(0).toString()));
    }
    return result;
}

bool TessngDBCopy::insertGuideArrow(GuideArrow* pGuideArrow)
{
    QSqlQuery query(database);
    QString sql = "insert into GuideArrow(guideArrowID,laneID,length,distToTerminal,arrowType) values(:guideArrowID,:laneID,:length,:distToTerminal,:arrowType)";
    query.prepare(sql);
    query.bindValue(":guideArrowID", QVariant::fromValue(pGuideArrow->guideArrowID));
    query.bindValue(":laneID", QVariant::fromValue(pGuideArrow->laneID));
    query.bindValue(":length", pGuideArrow->length);
    query.bindValue(":distToTerminal", pGuideArrow->distToTerminal);
    query.bindValue(":arrowType", (int)pGuideArrow->arrowType);
    return query.exec();
}

bool TessngDBCopy::insertSignalLamp(SignalLamp* pSignalLamp)
{
    QSqlQuery slQuery(database);
    QString slSql = "insert into SignalLamp(signalLampID, signalPhaseID, laneID, toLaneID, name, x, y, z) values(:signalLampID, :signalPhaseID, :laneID, :toLaneID, :name, :x, :y, :z)";
    //插入信号灯
    slQuery.prepare(slSql);
    slQuery.bindValue(":signalLampID", QVariant::fromValue(pSignalLamp->signalLampID));
    slQuery.bindValue(":signalPhaseID", QVariant::fromValue(pSignalLamp->mpSignalPhase->signalPhaseID));
    slQuery.bindValue(":laneID", QVariant::fromValue(pSignalLamp->laneID));
    slQuery.bindValue(":toLaneID", QVariant::fromValue(pSignalLamp->toLaneID));
    slQuery.bindValue(":name", pSignalLamp->name);
    slQuery.bindValue(":x", pSignalLamp->x);
    slQuery.bindValue(":y", pSignalLamp->y);
    slQuery.bindValue(":z", pSignalLamp->z);
    return  slQuery.exec();
}

bool TessngDBCopy::insertVehicleConstitutent(const VehicleComposition& vc)
{
    QSqlQuery query(database);
    QString sql = "insert into VehicleConstitutent(VehicleConsCode, name, IsDefault) values(:VehicleConsCode, :name, :IsDefault)";
    query.prepare(sql);
    query.bindValue(":VehicleConsCode", QVariant::fromValue(vc.vehicleConsCode));
    query.bindValue(":name", vc.name);
    if (vc.isDefault)
    {
        query.bindValue(":IsDefault", "T");
    }
    else
    {
        query.bindValue(":IsDefault", "F");
    }
    bool isOk = true;
    if (query.exec())
    {
        foreach(const VehicleConsDetail & vcd, vc.mlVehicleConsDetail)
        {
            if (!insertVehicleConsDetail(vcd))
            {
                isOk = false;
                break;
            }
        }
    }
    else
    {
#ifdef _DEBUG
    { qDebug() << query.lastError(); }
#endif
        isOk = false;
    }

    return isOk;
}

bool TessngDBCopy::insertVehicleConsDetail(const VehicleConsDetail& vcd)
{
    QSqlQuery query(database);
    QString sql = "insert into VehicleConsDetail(vehicleConsCode, vehicleTypeCode, proportion) values(:vehicleConsCode, :vehicleTypeCode, :proportion)";
    query.prepare(sql);
    query.bindValue(":vehicleConsCode", QVariant::fromValue(vcd.vehicleConsCode));
    query.bindValue(":vehicleTypeCode", QVariant::fromValue(vcd.vehicleTypeCode));
    query.bindValue(":proportion", vcd.proportion);
    bool flag = query.exec();
#ifdef _DEBUG
    if (!flag) { qDebug() << query.lastError(); }
#endif

    return flag;
}

bool TessngDBCopy::insertVehicleType(const VehicleType& vt)
{
    QSqlQuery query(database);
    QString sql = "insert into VehicleType(vehicleTypeCode, vehicleTypeName, Length, lengthSD, width, widthSD, avgSpeed, speedSD, maxSpeed, avgAcce, acceSD, maxAcce, avgDece, deceSD, maxDece) values(:vehicleTypeCode, :vehicleTypeName, :Length, :lengthSD, :width, :widthSD, :avgSpeed, :speedSD, :maxSpeed, :avgAcce, :acceSD, :maxAcce, :avgDece, :deceSD, :maxDece)";
    query.prepare(sql);
    query.bindValue(":vehicleTypeCode", QVariant::fromValue(vt.vehicleTypeCode));
    //将车辆类型翻译成中文（只有路网文件是英文，软件是中文时才翻译）
    QString vtype = gSpecialWords.trZhToEn(vt.vehicleTypeName);
    query.bindValue(":vehicleTypeName", vtype);
    query.bindValue(":Length", vt.Length);
    query.bindValue(":lengthSD", vt.lengthSD);
    query.bindValue(":width", vt.width);
    query.bindValue(":widthSD", vt.widthSD);
    query.bindValue(":avgSpeed", vt.avgSpeed);
    query.bindValue(":speedSD", vt.speedSD);
    query.bindValue(":maxSpeed", vt.maxSpeed);
    query.bindValue(":avgAcce", vt.avgAcce);
    query.bindValue(":acceSD", vt.acceSD);
    query.bindValue(":maxAcce", vt.maxAcce);
    query.bindValue(":avgDece", vt.avgDece);
    query.bindValue(":deceSD", vt.deceSD);
    query.bindValue(":maxDece", vt.maxDece);
    bool flag = query.exec();

#ifdef _DEBUG
    if (!flag) { qDebug() << query.lastError(); }
#endif

    return flag;
}

bool TessngDBCopy::insertDeparturePoint(const DeparturePoint& dp) {
    bool result = true;
    QSqlQuery query(database);
    QString sql = "insert into DeparturePoint(departurePointID,linkID,name) values(:departurePointID,:linkID,:name)";
    query.prepare(sql);
    query.bindValue(":departurePointID", QVariant::fromValue(dp.departurePointID));
    query.bindValue(":linkID", QVariant::fromValue(dp.mpLink->linkID));
    query.bindValue(":name", dp.name);
    result = query.exec() && result;

    sql = "insert into DepartureInterval(departureIntervalD,departurePointID,interval,vehiCount,vehiCons) values(:departureIntervalD,:departurePointID,:interval,:vehiCount,:vehiCons)";
    query.prepare(sql);
    foreach(DepaInterval di, dp.mlDepaInterval)
    {
        query.bindValue(":departureIntervalD", QVariant::fromValue(di.departureIntervalD));
        query.bindValue(":departurePointID", QVariant::fromValue(dp.departurePointID));
        query.bindValue(":interval", QVariant::fromValue(di.interval));
        query.bindValue(":vehiCount", QVariant::fromValue(di.vehiCount));
        query.bindValue(":vehiCons", QVariant::fromValue(di.vehiCons.vehicleConsCode));
        result = query.exec() && result;
    }
    return result;
}

bool TessngDBCopy::insertDrivInfoCollector(const VehicleDrivInfoCollector& drivInfoCollector)
{
    QSqlQuery query(database);
    QString sql = "insert into VehicleDrivInfoCollecter(collecterID,name,roadID,laneNumber,toLaneNumber,x,y,z,distance,dataInterval,startTime,endTime) values(:collecterID,:name,:roadID,:laneNumber,:toLaneNumber,:x,:y,:z,:distance,:dataInterval,:startTime,:endTime)";
    query.prepare(sql);
    query.bindValue(":collecterID", QVariant::fromValue(drivInfoCollector.collecterID));
    query.bindValue(":name", drivInfoCollector.name);
    query.bindValue(":roadID", QVariant::fromValue(drivInfoCollector.roadID));
    query.bindValue(":laneNumber", drivInfoCollector.laneNumber);
    query.bindValue(":toLaneNumber", drivInfoCollector.toLaneNumber);
    query.bindValue(":x", drivInfoCollector.x);
    query.bindValue(":y", drivInfoCollector.y);
    query.bindValue(":z", drivInfoCollector.z);
    query.bindValue(":distance", drivInfoCollector.distance);
    query.bindValue(":dataInterval", QVariant::fromValue(drivInfoCollector.dataInterval));
    query.bindValue(":startTime", QVariant::fromValue(drivInfoCollector.startTime));
    query.bindValue(":endTime", QVariant::fromValue(drivInfoCollector.endTime));
    return  query.exec();
}

bool TessngDBCopy::insertVehicleQueueCounter(VehicleQueueCounter* pQueue)
{
    QSqlQuery query(database);
    QString sql = "insert into VehicleQueueCounter(queueCounterID, name, roadID, laneNumber, toLaneNumber, x, y, z, distance, speedLowLimit, speedUpLimit, maxDistInterval, maxQueueLength, startTime, endTime, dataInterval,countInterval) values(:queueCounterID, :name, :roadID, :laneNumber, :toLaneNumber, :x, :y, :z, :distance, :speedLowLimit, :speedUpLimit, :maxDistInterval, :maxQueueLength, :startTime, :endTime, :dataInterval,:countInterval)";
    query.prepare(sql);
    query.bindValue(":queueCounterID", QVariant::fromValue(pQueue->queueCounterID));
    query.bindValue(":name", pQueue->name);
    query.bindValue(":roadID", QVariant::fromValue(pQueue->roadID));
    query.bindValue(":laneNumber", pQueue->laneNumber);
    query.bindValue(":toLaneNumber", pQueue->toLaneNumber);
    query.bindValue(":x", pQueue->x);
    query.bindValue(":y", pQueue->y);
    query.bindValue(":z", pQueue->z);
    query.bindValue(":distance", pQueue->distance);
    query.bindValue(":speedLowLimit", pQueue->speedLowLimit);
    query.bindValue(":speedUpLimit", pQueue->speedUpLimit);
    query.bindValue(":maxDistInterval", pQueue->maxDistInterval);
    query.bindValue(":maxQueueLength", pQueue->maxQueueLength);
    query.bindValue(":startTime", QVariant::fromValue(pQueue->startTime));
    query.bindValue(":endTime", QVariant::fromValue(pQueue->endTime));
    query.bindValue(":dataInterval", QVariant::fromValue(pQueue->dataInterval));
    query.bindValue(":countInterval", QVariant::fromValue(pQueue->countInterval));
    return  query.exec();
}

bool TessngDBCopy::insertVehicleTravelDetector(VehicleTravelDetector* pTravelDetector)
{
    QSqlQuery query(database);

    QString sql = "insert into VehicleTravelDetector(detectorId,name,startRoadId,start_laneNumber,start_toLaneNumber,startDist,startX,startY,teminalRoadId,teminal_laneNumber,teminal_toLaneNumber,teminalDist,teminalX,teminalY,startTime,endTime,dataInterval,shortestDistance) values(:detectorId,:name,:startRoadId,:start_laneNumber,:start_toLaneNumber,:startDist,:startX,:startY,:teminalRoadId,:teminal_laneNumber,:teminal_toLaneNumber,:teminalDist,:teminalX,:teminalY,:startTime,:endTime,:dataInterval,:shortestDistance)";
    query.prepare(sql);
    query.bindValue(":detectorId", QVariant::fromValue(pTravelDetector->detectorId));
    query.bindValue(":name", pTravelDetector->name);
    query.bindValue(":startRoadId", QVariant::fromValue(pTravelDetector->startRoadId));
    query.bindValue(":start_laneNumber", pTravelDetector->start_laneNumber);
    query.bindValue(":start_toLaneNumber", pTravelDetector->start_toLaneNumber);
    query.bindValue(":startDist", pTravelDetector->startDist);
    query.bindValue(":startX", pTravelDetector->startX);
    query.bindValue(":startY", pTravelDetector->startY);
    query.bindValue(":teminalRoadId", QVariant::fromValue(pTravelDetector->teminalRoadId));
    query.bindValue(":teminal_laneNumber", pTravelDetector->teminal_laneNumber);
    query.bindValue(":teminal_toLaneNumber", pTravelDetector->teminal_toLaneNumber);
    query.bindValue(":teminalDist", pTravelDetector->teminalDist);
    query.bindValue(":teminalX", pTravelDetector->teminalX);
    query.bindValue(":teminalY", pTravelDetector->teminalY);
    query.bindValue(":startTime", QVariant::fromValue(pTravelDetector->startTime));
    query.bindValue(":endTime", QVariant::fromValue(pTravelDetector->endTime));
    query.bindValue(":dataInterval", QVariant::fromValue(pTravelDetector->dataInterval));
    query.bindValue(":shortestDistance", pTravelDetector->shortestDistance);

    return  query.exec();
}

bool TessngDBCopy::insertReduceSpeedArea(ReduceSpeedArea* pReduceSpeedArea)
{
    bool bResult = true;

    QSqlQuery query(database);
    QString sql = "insert into ReduceSpeedArea(reduceSpeedAreaID, name, location, areaLength, roadID, laneNumber, toLaneNumber) values(:reduceSpeedAreaID, :name, :location, :areaLength, :roadID, :laneNumber, :toLaneNumber)";
    query.prepare(sql);
    query.bindValue(":reduceSpeedAreaID", QVariant::fromValue(pReduceSpeedArea->reduceSpeedAreaID));
    query.bindValue(":name", pReduceSpeedArea->name);
    query.bindValue(":location", pReduceSpeedArea->location);
    query.bindValue(":areaLength", pReduceSpeedArea->areaLength);
    query.bindValue(":roadID", QVariant::fromValue(pReduceSpeedArea->roadID));
    query.bindValue(":laneNumber", pReduceSpeedArea->laneNumber);
    query.bindValue(":toLaneNumber", pReduceSpeedArea->toLaneNumber);
    bResult = query.exec();

    QSqlQuery query2(database);
    QString sql2 = "insert into ReduceSpeedInterval(intervalID, reduceSpeedAreaID, startTime, endTime) values(:intervalID, :reduceSpeedAreaID, :startTime, :endTime)";
    query2.prepare(sql2);
    foreach(ReduceSpeedInterval * pOne, pReduceSpeedArea->mlReduceSpeedInterval)
    {
        query2.bindValue(":intervalID", QVariant::fromValue(pOne->intervalID));
        query2.bindValue(":reduceSpeedAreaID", QVariant::fromValue(pOne->reduceSpeedAreaID));
        query2.bindValue(":startTime", QVariant::fromValue(pOne->startTime));
        query2.bindValue(":endTime", QVariant::fromValue(pOne->endTime));
        bResult = query2.exec() && bResult;
    }

    QSqlQuery query3(database);
    QString sql3 = "insert into ReduceSpeedVehiType(reduceSpeedAreaID, vehicleTypeCode, avgSpeed, speedSD) values(:reduceSpeedAreaID, :vehicleTypeCode, :avgSpeed, :speedSD)";
    query3.prepare(sql3);
    foreach(ReduceSpeedVehiType * pOne, pReduceSpeedArea->mlReduceSpeedVehiType)
    {
        query3.bindValue(":reduceSpeedAreaID", QVariant::fromValue(pOne->reduceSpeedAreaID));
        query3.bindValue(":vehicleTypeCode", QVariant::fromValue(pOne->vehicleTypeCode));
        query3.bindValue(":avgSpeed", pOne->avgSpeed);
        query3.bindValue(":speedSD", pOne->speedSD);
        bResult = query3.exec() && bResult;
    }
    return bResult;
}

bool TessngDBCopy::insertBusStation(BusStation* pBusStation)
{
    QSqlQuery query(database);
    QString sql = "insert into BusStation(busStationID, linkID, name, laneNumber, length, x, y, type) values(:busStationID, :linkID, :name, :laneNumber, :length, :x, :y, :type)";
    query.prepare(sql);
    query.bindValue(":busStationID", QVariant::fromValue(pBusStation->busStationID));
    query.bindValue(":linkID", QVariant::fromValue(pBusStation->mpLink->linkID));
    query.bindValue(":name", pBusStation->name);
    query.bindValue(":laneNumber", pBusStation->laneNumber);
    query.bindValue(":length", pBusStation->length);
    query.bindValue(":x", pBusStation->x);
    query.bindValue(":y", pBusStation->y);
    //将公交站点类型翻译成英文（只有当路网是英文，软件是中文时才翻译）
    QString type = gSpecialWords.trZhToEn(pBusStation->type);
    query.bindValue(":type", type);
    return  query.exec();
}
bool TessngDBCopy::insertBusLine(BusLine* pBusLine) {
    bool bResult = true;
    QSqlQuery query(database);

    //插入线路主表数据
    QString sql = "insert into BusLine(busLineID, name, length, dischargeFreq, dischargeStartTime, dischargeEndTime, startX, startY, endX, endY, desirSpeed, speedSD, passCountAtStartTime) values(:busLineID, :name, :length, :dischargeFreq, :dischargeStartTime, :dischargeEndTime, :startX, :startY, :endX, :endY, :desirSpeed, :speedSD, :passCountAtStartTime)";
    query.prepare(sql);
    query.bindValue(":busLineID", QVariant::fromValue(pBusLine->busLineID));
    query.bindValue(":name", pBusLine->name);
    query.bindValue(":length", pBusLine->length);
    query.bindValue(":dischargeFreq", pBusLine->dischargeFreq);
    query.bindValue(":dischargeStartTime", pBusLine->dischargeStartTime);
    query.bindValue(":dischargeEndTime", pBusLine->dischargeEndTime);
    query.bindValue(":startX", pBusLine->startX);
    query.bindValue(":startY", pBusLine->startY);
    query.bindValue(":endX", pBusLine->endX);
    query.bindValue(":endY", pBusLine->endY);
    query.bindValue(":desirSpeed", pBusLine->desirSpeed);
    query.bindValue(":speedSD", pBusLine->speedSD);
    query.bindValue(":passCountAtStartTime", pBusLine->passCountAtStartTime);
    bResult=query.exec();

    //插入相关站点线路，以及乘客到站
    if (bResult)
    {
        QString sqlInsBSL = "insert into BusStationLine(stationLineID, busStationID, busLineID, parkingTime, leavingPercent, getOnTimePerson, getOutTimePerson) values(:stationLineID, :busStationID, :busLineID, :parkingTime, :leavingPercent, :getOnTimePerson, :getOutTimePerson)";
        QString sqlInsPassArriving = "insert into BusStationPassengerArriving(passengerArrivingID, stationLineID, startTime, endTime, passengerCount) values(:passengerArrivingID, :stationLineID, :startTime, :endTime, :passengerCount)";
        QSqlQuery queryInsBSL;
        QSqlQuery queryInsPA;
        queryInsBSL.prepare(sqlInsBSL);
        queryInsPA.prepare(sqlInsPassArriving);
        foreach(BusStationLine * pBusStationLine, pBusLine->mlBusStationLine)
        {
            queryInsBSL.bindValue(":stationLineID", QVariant::fromValue(pBusStationLine->stationLineID));
            queryInsBSL.bindValue(":busStationID", QVariant::fromValue(pBusStationLine->busStationID));
            queryInsBSL.bindValue(":busLineID", QVariant::fromValue(pBusStationLine->busLineID));
            queryInsBSL.bindValue(":parkingTime", pBusStationLine->parkingTime);
            queryInsBSL.bindValue(":leavingPercent", pBusStationLine->leavingPercent);
            queryInsBSL.bindValue(":getOnTimePerson", pBusStationLine->getOnTimePerson);
            queryInsBSL.bindValue(":getOutTimePerson", pBusStationLine->getOutTimePerson);
            bResult = queryInsBSL.exec();

            if (!bResult)
            {
                break;
            }
            else
            {
                foreach(PassengerArriving * pPA, pBusStationLine->mlPassengerArriving)
                {
                    queryInsPA.bindValue(":passengerArrivingID", QVariant::fromValue(pPA->passengerArrivingID));
                    queryInsPA.bindValue(":stationLineID", QVariant::fromValue(pBusStationLine->stationLineID));
                    queryInsPA.bindValue(":startTime", QVariant::fromValue(pPA->startTime));
                    queryInsPA.bindValue(":endTime", QVariant::fromValue(pPA->endTime));
                    queryInsPA.bindValue(":passengerCount", pPA->passengerCount);
                    bResult = queryInsPA.exec();
                    if (!bResult)
                    {
                        break;
                    }
                }

                if (!bResult)
                {
                    break;
                }
            }
        }
    }

    //插入线路相关路段
    if (bResult)
    {
        QString sqlInsLinks = "insert into BusLineRoad(busLineID, serialNumber, roadID) values(:busLineID, :serialNumber, :roadID)";
        QSqlQuery queryInsLinks;
        queryInsLinks.prepare(sqlInsLinks);

        for (int i = 0, size = pBusLine->mlLink.size(); i < size; ++i)
        {
            Link* pLink = pBusLine->mlLink.at(i);
            queryInsLinks.bindValue(":busLineID", QVariant::fromValue(pBusLine->busLineID));
            queryInsLinks.bindValue(":serialNumber", i + 1);
            queryInsLinks.bindValue(":roadID", QVariant::fromValue(pLink->linkID));
            bResult = queryInsLinks.exec();

            if (!bResult)
            {
                break;
            }
        }
    }

    return bResult;
}
void TessngDBCopy::test(){

}

