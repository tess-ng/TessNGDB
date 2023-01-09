#include "TessngDBToolsUpdateBase.h"
#include "TessngDBToolsRemove.h"
#include "exception.h"
#include <QSqlError>
#include <QException>
#include <QSqlQuery>
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
TessngDBToolsUpdateBase::TessngDBToolsUpdateBase()
{

}
bool TessngDBToolsUpdateBase::checkSqlString(QString& str) {
	int index = str.indexOf("set") + 3;
	bool result = true;
	int dot = -1;
	for (int i = index; i < str.size(); i++) {
		result = str[i].isSpace() && result;
		if (result) continue;
		if (',' != str[i]) break;
		dot = i;
		break;
	}

	if (dot > 0) {
		str.remove(dot, 1);
	}
	int indexEnd = str.indexOf("WHERE");

	for (int i = index; i < indexEnd; i++) {
		if (!str[i].isSpace()) return true;
	}

	return false;
}
bool TessngDBToolsUpdateBase::updateVertex(const QList<Vertex*>& list)
{
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto pVertex, list) {
		if (!std::_Is_nan(pVertex->x)) {
			query.prepare(QString("UPDATE Vertex set X=%1,Y=%2,Z=%3 WHERE VertexID=%4")
				.arg(pVertex->x)
				.arg(pVertex->y)
				.arg(pVertex->z)
				.arg(pVertex->vertexID));
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}

		}
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateNode(const QList<Node*>& list) {
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto it, list) {
		QString sql = "UPDATE Node set ";
		if(!it->nodeName.isEmpty()) sql += QString("nodeName='%1'").arg(it->nodeName);

		if(it->mpVertex!=NULL) sql += QString(",vertexID=%1").arg(it->mpVertex->vertexID);

		sql += QString(" WHERE nodeID=%1").arg(it->nodeID);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec() && result;

			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}
		
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateLinkVertex(long linkId, const QList<Vertex*>& list) {
	bool result = true;
	QSqlQuery  query(gDB);
	int num = 1;
	foreach(auto it, list) {
		query.prepare(QString("UPDATE LinkVertex set Num=%1 WHERE LinkID=%2 and VertexID=%3")
			.arg(num)
			.arg(linkId)
			.arg(it->vertexID));
		num++;
		result = query.exec();
		if (!result) {
			throw PH::Exception(query.lastError().text().toStdString());
			break;
		}
	}
	return result;
}
QString TessngDBToolsUpdateBase::jsonObjToStr(const QJsonObject& obj) {
	QByteArray bt = QJsonDocument(obj).toJson(QJsonDocument::Compact);
	return bt.data();
}
bool TessngDBToolsUpdateBase::updateLinkBase(Link* link) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE Link set ";
	if (link->startNode->nodeID > 0)sql += QString("StartNodeID=%1").arg(link->startNode->nodeID);
	if (link->endNode->nodeID > 0)sql += QString(",EndNodeID=%1").arg(link->endNode->nodeID);
	if (!link->linkName.isEmpty())sql += QString(",LinkName='%1'").arg(link->linkName);
	if (!std::_Is_nan(link->laneNumber))sql += QString(",laneNumber=%1").arg(link->laneNumber);
	if (!std::_Is_nan(link->laneWidth))sql += QString(",laneWidth=%1").arg(link->laneWidth);
	if (!link->laneColor.isEmpty())sql += QString(",laneColor='%1'").arg(link->laneColor);
	if (!link->linkType.isEmpty())sql += QString(",linkType='%1'").arg(link->linkType);
	if (!std::_Is_nan(link->length))sql += QString(",length=%1").arg(link->length);
	if (!std::_Is_nan(link->curvature))sql += QString(",curvature=%1").arg(link->curvature);
	if (!std::_Is_nan(link->nonLinearCoefficient))sql += QString(",nonLinearCoefficient=%1").arg(link->nonLinearCoefficient);
	if (!std::_Is_nan(link->linkSaturationFlow)) sql += QString(",LinkSaturationFlow=%1").arg(link->linkSaturationFlow);
	if (!std::_Is_nan(link->linkTrafficFlow))sql += QString(",linkTrafficFlow=%1").arg(link->linkTrafficFlow);
	if (!std::_Is_nan(link->desiredSpeed))sql += QString(",desiredSpeed=%1").arg(link->desiredSpeed);
	if (!std::_Is_nan(link->limitSpeed))sql += QString(",limitSpeed=%1").arg(link->limitSpeed);
	if (!std::_Is_nan(link->minSpeed))sql += QString(",minSpeed=%1").arg(link->minSpeed);
	if (!std::_Is_nan(link->addValue))sql += QString(",addValue=%1").arg(link->addValue);
	if (!link->centerLinePointsJson.isEmpty())sql += QString(",centerLinePointsJson='%1'").arg(jsonObjToStr(link->centerLinePointsJson));
	if (!link->leftBreakPointsJson.isEmpty())sql += QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(link->leftBreakPointsJson));
	if (!link->rightBreakPointsJson.isEmpty())sql += QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(link->rightBreakPointsJson));
	if (!link->otherAttrsJson.isEmpty())sql += QString(",otherAttrsJson='%1'").arg(jsonObjToStr(link->otherAttrsJson));
	sql += QString(" WHERE linkID=%1").arg(link->linkID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateLinkPtr(Link* mpLink) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();

		result = updateVertex(mpLink->mlVertex);
		if (!result) goto failed;

		result = updateNode(QList<Node*>() << mpLink->startNode << mpLink->endNode);
		if (!result) goto failed;

		result = updateLinkVertex(mpLink->linkID, mpLink->mlVertex);
		if (!result) goto failed;

		result = updateLinkBase(mpLink);
		if (!result) goto failed;

	}
	catch (QException& exc) {
		qWarning() << exc.what()<<__FILE__<<__LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update Links failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateLaneLimitVehicle(long laneId, QList<VehicleType>& list) {
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto & it, list) {
		if (it.vehicleTypeCode < 0) continue;
		query.prepare(QString("UPDATE LaneLimitVehicle set VehicleTypeCode=%1 WHERE LaneID=%2").arg(it.vehicleTypeCode).arg(laneId));
		result = query.exec();
		if (!result) {
			throw PH::Exception(query.lastError().text().toStdString());
			break;
		}
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateLaneBase(Lane* lane) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE Lane set ";
	if (lane->linkID > 0)sql += QString("LinkID=%1").arg(lane->linkID);
	if (lane->serialNumber > 0)sql += QString(",SerialNumber=%1").arg(lane->serialNumber);
	if (!std::_Is_nan(lane->width))sql += QString(",Width=%1").arg(lane->width);
	if (!lane->actionType.isEmpty())sql += QString(",ActionType='%1'").arg(lane->actionType);
	if (!lane->expectTravelDirection.isEmpty())sql += QString(",ExpectTravelDirection='%1'").arg(lane->expectTravelDirection);
	if (!lane->centerLinePointsJson.isEmpty())sql += QString(",centerLinePointsJson='%1'").arg(jsonObjToStr(lane->centerLinePointsJson));
	if (!lane->leftBreakPointsJson.isEmpty())sql += QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(lane->leftBreakPointsJson));
	if (!lane->rightBreakPointsJson.isEmpty())sql += QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(lane->rightBreakPointsJson));
	if (!lane->otherAttrsJson.isEmpty())sql += QString(",otherAttrsJson='%1'").arg(jsonObjToStr(lane->otherAttrsJson));
	sql += QString(" WHERE LaneID=%1").arg(lane->laneID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateLanesPtr(const QList<GLane*>& list, bool trans) {
	bool result = true;
	try {
		//开启事务
		if (trans)gDB.transaction();
		foreach(auto it, list) {
			result = updateLanePtr(it->mpLane);
			if (!result) goto failed;
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update Lanes failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	if (trans) {
		result = gDB.commit() && result;
		if (!result) {
			gDB.rollback();
		}
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateLanesPtr(const QList<Lane*>& list, bool trans) {
	bool result = true;
	try {
		//开启事务
		if (trans)gDB.transaction();
		foreach(auto it, list) {
			result = updateLanePtr(it);
			if (!result) goto failed;
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update Lanes failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	if (trans) {
		result = gDB.commit() && result;
		if (!result) {
			gDB.rollback();
		}
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateLanePtr(Lane* mpLane) {
	bool result = true;
	result = updateLaneLimitVehicle(mpLane->laneID, mpLane->mlVehicleType);
	if (!result) return result;

	result = updateLaneBase(mpLane);
	if (!result) return result;

	return result;
}
bool TessngDBToolsUpdateBase::updateGuideArrowPtr(GuideArrow* arrow)
{
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE GuideArrow set ";
	if (arrow->laneID > 0)sql += QString("laneID=%1").arg(arrow->laneID);
	if (!std::_Is_nan(arrow->length))sql += QString(",length=%1").arg(arrow->length);
	if (!std::_Is_nan(arrow->distToTerminal))sql += QString(",distToTerminal=%1").arg(arrow->distToTerminal);
	sql += QString(",arrowType=%1").arg(arrow->arrowType);
	sql += QString(" WHERE guideArrowID=%1").arg(arrow->guideArrowID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}


bool TessngDBToolsUpdateBase::updateGuideArrowByTransPtr(GuideArrow* mpGuideArrow)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		result = updateGuideArrowPtr(mpGuideArrow);
		if (!result) goto failed;
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update GuideArrow failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateLaneConnectorPtr(LaneConnector* it) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE LaneConnector set ";
	if (it->weight > 0)sql += QString("weight=%1").arg(it->weight);
	if (!it->centerLinePointsJson.isEmpty())sql += QString(",centerLinePointsJson='%1'").arg(jsonObjToStr(it->centerLinePointsJson));
	if (!it->leftBreakPointsJson.isEmpty())sql += QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(it->leftBreakPointsJson));
	if (!it->rightBreakPointsJson.isEmpty())sql += QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(it->rightBreakPointsJson));
	if (!it->otherAttrsJson.isEmpty())sql += QString(",otherAttrsJson='%1'").arg(jsonObjToStr(it->otherAttrsJson));
	sql += QString(" WHERE connID=%1 and StartLaneID=%2 and EndLaneID=%3").arg(it->mpConnector->connID).arg(it->mpFromLane->laneID).arg(it->mpToLane->laneID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) {
			throw PH::Exception(query.lastError().text().toStdString());
		}
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateLaneConnectors(long connId, const QList<LaneConnector*>& list)
{
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto it, list) {
		QString sql = "UPDATE LaneConnector set ";
		if (it->weight > 0)sql += QString("weight=%1").arg(it->weight);
		if (!it->centerLinePointsJson.isEmpty())sql += QString(",centerLinePointsJson='%1'").arg(jsonObjToStr(it->centerLinePointsJson));
		if (!it->leftBreakPointsJson.isEmpty())sql += QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(it->leftBreakPointsJson));
		if (!it->rightBreakPointsJson.isEmpty())sql += QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(it->rightBreakPointsJson));
		if (!it->otherAttrsJson.isEmpty())sql += QString(",otherAttrsJson='%1'").arg(jsonObjToStr(it->otherAttrsJson));
		sql += QString(" WHERE connID=%1 and StartLaneID=%2 and EndLaneID=%3").arg(connId).arg(it->mpFromLane->laneID).arg(it->mpToLane->laneID);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}

	}
	return result;
}

bool TessngDBToolsUpdateBase::updateConnectorLimitVehicles(long connId, QList<VehicleType>& list)
{
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto it, list) {
		QString sql = "UPDATE ConnectorLimitVehicle set ";
		if (it.vehicleTypeCode > 0)sql += QString("VehicleTypeCode=%1").arg(it.vehicleTypeCode);
		sql += QString(" WHERE connID=%1").arg(connId);
		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}

	}
	return result;
}
bool TessngDBToolsUpdateBase::updateConnector(Connector* it) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE Connector set ";
	if (it->mpFromLink->linkID > 0)sql += QString("StartLinkID=%1").arg(it->mpFromLink->linkID);
	if (it->mpToLink->linkID > 0)sql += QString(",EndLinkID=%1").arg(it->mpToLink->linkID);
	if (!std::_Is_nan(it->length))sql += QString(",Length=%1").arg(it->length);
	if (!it->connName.isEmpty())sql += QString(",ConnName='%1'").arg(it->connName);
	if (!it->color.isEmpty())sql += QString(",Color='%1'").arg(it->color);
	if (!std::_Is_nan(it->nonLinearCoefficient))sql += QString(",NonLinearCoefficient=%1").arg(it->nonLinearCoefficient);
	if (!std::_Is_nan(it->desiredSpeed))sql += QString(",DesiredSpeed=%1").arg(it->desiredSpeed);
	if (!std::_Is_nan(it->limitSpeed))sql += QString(",LimitSpeed=%1").arg(it->limitSpeed);
	if (!it->leftBreakPointsJson.isEmpty())sql += QString(",leftBreakPointsJson='%1'").arg(jsonObjToStr(it->leftBreakPointsJson));
	if (!it->rightBreakPointsJson.isEmpty())sql += QString(",rightBreakPointsJson='%1'").arg(jsonObjToStr(it->rightBreakPointsJson));
	if (!it->otherAttrsJson.isEmpty())sql += QString(",otherAttrsJson='%1'").arg(jsonObjToStr(it->otherAttrsJson));
	sql += QString(" WHERE ConnID=%1").arg(it->connID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateConnectorsPtr(Connector* mpConnector)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();

		result = updateLaneConnectors(mpConnector->connID, mpConnector->mlLaneConnector);
		if (!result) goto failed;

		result = updateConnectorLimitVehicles(mpConnector->connID, mpConnector->mlVehicleType);
		if (!result) goto failed;

		result = updateConnector(mpConnector);
		if (!result) goto failed;
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update Connectors failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateRoutingLinks(Routing* routing) {
	if (routing->mllLink.isEmpty()) return true;
	bool result = TessngDBToolsRemove::getInstance()->removeRoutingLink(routing);
	if (!result) return false;
	QSqlQuery  query(gDB);
	QString sql = "insert into RoutingLink(routingID, linkID, num1, num2) values(:routingID, :linkID, :num1, :num2)";
	query.prepare(sql);
	int num1 = 1;
	foreach(QList<Link*> lLink, routing->mllLink)
	{
		int num2 = 1;
		foreach(Link * pLink, lLink)
		{
			query.bindValue(":routingID", QVariant::fromValue(routing->routingID));
			query.bindValue(":linkID", QVariant::fromValue(pLink->linkID));
			query.bindValue(":num1", num1);
			query.bindValue(":num2", num2);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				return result;
			}
			num2++;
		}
		num1++;
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateRouteingBase(Routing* route) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE Routing set ";
	if (route->mpDecisionPoint->deciPointID > 0)sql += QString("deciPointID=%1").arg(route->mpDecisionPoint->deciPointID);
	if (!route->routingName.isEmpty())sql += QString(",routingName=%1").arg(route->routingName);
	if (!std::_Is_nan(route->proportion))sql += QString(",proportion=%1").arg(route->proportion);
	sql += QString(" WHERE routingID=%1").arg(route->routingID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateRouteingPtr(Routing* it, bool trans) {
	bool result = true;
	try {
		//开启事务
		if(trans)gDB.transaction();
		result = updateRoutingLinks(it);
		if (!result) goto failed;

		result = updateRouteingBase(it);
		if (!result) goto failed;

	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update Routeing failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	if (trans) {
		result = gDB.commit() && result;
		if (!result) {
			gDB.rollback();
		}
	}


	return result;
}
bool TessngDBToolsUpdateBase::updateRoutingFlowByInterval(const QList<RoutingFlowByInterval*>& pRPIs, const QList<Routing*>& routes) {
	long startDateTime = 1;
	bool result = true;
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
			QString sql = "UPDATE RoutingFLowRatio set ";
			if (pRoutingFLowRatio->routingID > 0)sql += QString("routingID=%1").arg(pRoutingFLowRatio->routingID);
			if (startDateTime > 0)sql += QString(",startDateTime=%1").arg(startDateTime);
			if (pRPI->endDateTime > 0)sql += QString(",endDateTime=%1").arg(pRPI->endDateTime);
			if (!std::_Is_nan(pRoutingFLowRatio->ratio))sql += QString(",ratio=%1").arg(pRoutingFLowRatio->ratio);
			sql += QString(" WHERE RoutingFLowRatioID=%1").arg(pRoutingFLowRatio->RoutingFLowRatioID);

			if (checkSqlString(sql)) {
				query.prepare(sql);
				result = query.exec();
				if (!result) {
					throw PH::Exception(query.lastError().text().toStdString());
					break;
				}
			}

		}
		startDateTime = pRPI->endDateTime + 1;
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateDecisionPointBase(DecisionPoint* pDecisionPoint) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE DecisionPoint set ";
	if (!pDecisionPoint->deciPointName.isEmpty())sql += QString("deciPointName='%1'").arg(pDecisionPoint->deciPointName);
	if (pDecisionPoint->mpLink->linkID > 0)sql += QString(",linkID=%1").arg(pDecisionPoint->mpLink->linkID);
	if (!std::_Is_nan(pDecisionPoint->X))sql += QString(",X=%1").arg(pDecisionPoint->X);
	if (!std::_Is_nan(pDecisionPoint->Y))sql += QString(",Y=%1").arg(pDecisionPoint->Y);
	if (!std::_Is_nan(pDecisionPoint->Z))sql += QString(",Z=%1").arg(pDecisionPoint->Z);
	sql += QString(" WHERE deciPointID=%1").arg(pDecisionPoint->deciPointID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateDecisionPointPtr(DecisionPoint* mpDecisionPoint)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		result = updateRoutingFlowByInterval(mpDecisionPoint->mlRoutingFlowByInterval, mpDecisionPoint->mlRouting);
		if (!result) goto failed;

		foreach(auto it, mpDecisionPoint->mlRouting) {
			result = updateRouteingPtr(it,false);
			if (!result) goto failed;
		}

        result = updateDecisionPointBase(mpDecisionPoint);
        if (!result) goto failed;
    }
    catch (QException& exc) {
        qWarning() << exc.what() << __FILE__ << __LINE__;
        result = false;
    }
    catch (const PH::Exception& exc)
    {
        qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
        result = false;
    }
    catch (...) {
        qWarning() << "update DecisionPoint failed! Unknow Error." << __FILE__ << __LINE__;
        result = false;
    }
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateDepaInterval(long departurePointID, QList<DepaInterval>& lsit) {
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(DepaInterval di, lsit) {
		QString sql = "UPDATE DepartureInterval set ";
		if (di.departureIntervalD > 0)sql += QString("departurePointID=%1").arg(di.departureIntervalD);
		if (di.interval > 0)sql += QString(",interval=%1").arg(di.interval);
		if (di.vehiCount > 0)sql += QString(",vehiCount=%1").arg(di.vehiCount);
		if (di.vehiCons.vehicleConsCode > 0)sql += QString(",vehiCons=%1").arg(di.vehiCons.vehicleConsCode);
		sql += QString(" WHERE departureIntervalD=%1").arg(departurePointID);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}

	}
	return result;
}
bool TessngDBToolsUpdateBase::updateDeparturePointBase(DeparturePoint* dp) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE DeparturePoint set ";
	if (!dp->name.isEmpty())sql += QString("name='%1'").arg(dp->name);
	if (dp->mpLink->linkID > 0)sql += QString(",linkID=%1").arg(dp->mpLink->linkID);
	sql += QString(" WHERE departurePointID=%1").arg(dp->departurePointID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateDeparturePointPtr(DeparturePoint* mpDeparturePoint) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		result = updateDepaInterval(mpDeparturePoint->departurePointID, mpDeparturePoint->mlDepaInterval);
		if (!result) goto failed;

		result = updateDeparturePointBase(mpDeparturePoint);
		if (!result) goto failed;
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update DeparturePoint failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateSignalLampPtr(SignalLamp* pSignalLamp) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE SignalLamp set ";
	if (!pSignalLamp->name.isEmpty())sql += QString("name='%1'").arg(pSignalLamp->name);
	sql += QString(",signalPhaseID=%1").arg(pSignalLamp->mpSignalPhase->id());
	if (pSignalLamp->laneID > 0)sql += QString(",laneID=%1").arg(pSignalLamp->laneID);
	if (pSignalLamp->toLaneID > 0)sql += QString(",toLaneID=%1").arg(pSignalLamp->toLaneID);
	if (0 == _isnan(pSignalLamp->x)) sql += QString(",X=%1").arg(pSignalLamp->x);
	if (0 == _isnan(pSignalLamp->y)) sql += QString(",Y=%1").arg(pSignalLamp->y);
	if (0 == _isnan(pSignalLamp->z))sql += QString(",Z=%1").arg(pSignalLamp->z);
	sql += QString(" WHERE signalLampID=%1").arg(pSignalLamp->signalLampID);

	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateSignalColor(long phaseID, QList<SignalColor>& list) {
	bool result = true;
	QSqlQuery  query(gDB);
	int serialNumber = 0;
	foreach(auto & it, list) {
		QString sql = "UPDATE SignalColor set ";
		if (!it.color.isEmpty())sql += QString("color='%1'").arg(it.color);
		sql += QString(",serialNumber=%1").arg(++serialNumber);
		if (it.timeLength > 0)sql += QString(",timeLength=%1").arg(it.timeLength);
		sql += QString(" WHERE signalPhaseID=%1").arg(phaseID);
		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateSignalPhase(const QList<SignalPhase*>& list)
{
	QSqlQuery  query(gDB);
	bool result = true;
	foreach(auto it, list) {
		result = updateSignalColor(it->signalPhaseID, it->mlSignalColor);
		if (!result) return result;

		QString sql = "UPDATE SignalPhase set ";
		if (!it->name.isEmpty())sql += QString("name='%1'").arg(it->name);
		if (it->signalPhaseID > 0)sql += QString(",signalGroupID=%1").arg(it->signalPhaseID);
		if (it->serialNumber > 0)sql += QString(",serialNumber=%1").arg(it->serialNumber);
		sql += QString(" WHERE signalPhaseID=%1").arg(it->signalPhaseID);
		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateSignalGroupBase(SignalGroup* pSignalGroup) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE SignalGroup set ";
	if (!pSignalGroup->groupName().isEmpty())sql += QString("name='%1'").arg(pSignalGroup->groupName());
	if (pSignalGroup->timeLength > 0)sql += QString(",timeLength=%1").arg(pSignalGroup->timeLength);
	if (pSignalGroup->startTime > 0)sql += QString(",startTime=%1").arg(pSignalGroup->startTime);
	if (pSignalGroup->endTime > 0)sql += QString(",endTime=%1").arg(pSignalGroup->endTime);
	sql += QString(" WHERE signalGroupID=%1").arg(pSignalGroup->signalGroupID);
	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}
	return result;
}


bool TessngDBToolsUpdateBase::updateSignalGroupPtr(SignalGroup* it)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		result = updateSignalPhase(it->mlPhase);
		if (!result) goto failed;

		result = updateSignalGroupBase(it);
		if (!result) goto failed;
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update SignalGroups failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateSignalLampByTransPtr(SignalLamp* sl) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		result = updateSignalLampPtr(sl);
		if (!result) goto failed;
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update SignalLamps failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateVehicleDrivInfoCollectorPtr(VehicleDrivInfoCollector* it) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		QSqlQuery  query(gDB);
		QString sql = "UPDATE VehicleDrivInfoCollecter set ";
		if (!it->name.isEmpty())sql += QString("name='%1'").arg(it->name);
		if (it->roadID > 0)sql += QString(",roadID=%1").arg(it->roadID);
		if (it->laneNumber > 0)sql += QString(",laneNumber=%1").arg(it->laneNumber);
		if (it->toLaneNumber > 0)sql += QString(",toLaneNumber=%1").arg(it->toLaneNumber);
		if (!std::_Is_nan(it->x))sql += QString(",x=%1").arg(it->x);
		if (!std::_Is_nan(it->y))sql += QString(",y=%1").arg(it->y);
		if (!std::_Is_nan(it->z))sql += QString(",z=%1").arg(it->x);
		if (!std::_Is_nan(it->distance))sql += QString(",distance=%1").arg(it->distance);
		if (it->dataInterval > 0)sql += QString(",dataInterval=%1").arg(it->dataInterval);
		if (it->startTime > 0)sql += QString(",startTime=%1").arg(it->startTime);
		if (it->endTime > 0)sql += QString(",endTime=%1").arg(it->endTime);
		sql += QString(" WHERE collecterID=%1").arg(it->collecterID);
		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
			}
		}

	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update VehicleDrivInfoCollectors failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateVehicleQueueCounterPtr(VehicleQueueCounter* it) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		QSqlQuery  query(gDB);
		QString sql = "UPDATE VehicleQueueCounter set ";
		if (!it->name.isEmpty())sql += QString("name='%1'").arg(it->name);
		if (it->roadID > 0)sql += QString(",roadID=%1").arg(it->roadID);
		if (it->laneNumber > 0)sql += QString(",laneNumber=%1").arg(it->laneNumber);
		if (it->toLaneNumber > 0)sql += QString(",toLaneNumber=%1").arg(it->toLaneNumber);
		if (!std::_Is_nan(it->x))sql += QString(",x=%1").arg(it->x);
		if (!std::_Is_nan(it->y))sql += QString(",y=%1").arg(it->y);
		if (!std::_Is_nan(it->z))sql += QString(",z=%1").arg(it->x);
		if (!std::_Is_nan(it->speedLowLimit))sql += QString(",speedLowLimit=%1").arg(it->speedLowLimit);
		if (!std::_Is_nan(it->speedUpLimit))sql += QString(",speedUpLimit=%1").arg(it->speedUpLimit);
		if (!std::_Is_nan(it->maxDistInterval))sql += QString(",maxDistInterval=%1").arg(it->maxDistInterval);
		if (!std::_Is_nan(it->maxQueueLength))sql += QString(",maxQueueLength=%1").arg(it->maxQueueLength);
		if (!std::_Is_nan(it->distance))sql += QString(",distance=%1").arg(it->distance);
		if (it->dataInterval > 0)sql += QString(",dataInterval=%1").arg(it->dataInterval);
		if (it->startTime > 0)sql += QString(",startTime=%1").arg(it->startTime);
		if (it->endTime > 0)sql += QString(",endTime=%1").arg(it->endTime);
		if (it->countInterval > 0)sql += QString(",countInterval=%1").arg(it->countInterval);
		sql += QString(" WHERE QueueCounterID=%1").arg(it->queueCounterID);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
			}
		}

	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update VehicleQueueCounters failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateVehicleTravelDetectorPtr(VehicleTravelDetector* it) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		QSqlQuery  query(gDB);
		QString sql = "UPDATE VehicleTravelDetector set ";
		if (!it->name.isEmpty())sql += QString("name='%1'").arg(it->name);
		if (it->startRoadId > 0)sql += QString(",startRoadId=%1").arg(it->startRoadId);
		if (it->start_laneNumber > 0)sql += QString(",start_laneNumber=%1").arg(it->start_laneNumber);
		if (it->start_toLaneNumber > 0)sql += QString(",start_toLaneNumber=%1").arg(it->start_toLaneNumber);
		if (!std::_Is_nan(it->startDist))sql += QString(",startDist=%1").arg(it->startDist);
		if (!std::_Is_nan(it->startX))sql += QString(",startX=%1").arg(it->startX);
		if (!std::_Is_nan(it->startY))sql += QString(",startY=%1").arg(it->startY);
		if (it->teminalRoadId > 0)sql += QString(",teminalRoadId=%1").arg(it->teminalRoadId);
		if (it->teminal_laneNumber > 0)sql += QString(",teminal_laneNumber=%1").arg(it->teminal_laneNumber);
		if (it->teminal_toLaneNumber > 0)sql += QString(",teminal_toLaneNumber=%1").arg(it->teminal_toLaneNumber);
		if (!std::_Is_nan(it->teminalDist))sql += QString(",teminalDist=%1").arg(it->teminalDist);
		if (!std::_Is_nan(it->teminalX))sql += QString(",teminalX=%1").arg(it->teminalX);
		if (!std::_Is_nan(it->teminalY))sql += QString(",teminalY=%1").arg(it->teminalY);
		if (it->startTime > 0)sql += QString(",startTime=%1").arg(it->startTime);
		if (it->endTime > 0)sql += QString(",endTime=%1").arg(it->endTime);
		if (it->dataInterval > 0)sql += QString(",dataInterval=%1").arg(it->dataInterval);
		if (!std::_Is_nan(it->shortestDistance))sql += QString(",shortestDistance=%1").arg(it->shortestDistance);
		sql += QString(" WHERE detectorId=%1").arg(it->detectorId);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
			}
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update VehicleTravelDetectors failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}

	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateVehicleDetectorPtr(VehicleDetector* it) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		QSqlQuery  query(gDB);
		QString sql = "UPDATE VehicleDetector set ";
		if (!it->name.isEmpty())sql += QString("name='%1'").arg(it->name);
		if (!it->roadType.isEmpty())sql += QString(",roadType='%1'").arg(it->roadType);
		if (!it->type.isEmpty())sql += QString(",type='%1'").arg(it->type);
		if (it->phaseNumber > 0)sql += QString(",phaseNumber=%1").arg(it->phaseNumber);
		if (it->roadId > 0)sql += QString(",roadId=%1").arg(it->roadId);
		if (it->laneNumber > 0)sql += QString(",laneNumber=%1").arg(it->laneNumber);
		if (it->toLaneNumber > 0)sql += QString(",toLaneNumber=%1").arg(it->toLaneNumber);
		if (!std::_Is_nan(it->length))sql += QString(",length=%1").arg(it->length);
		if (!std::_Is_nan(it->distToTerminal))sql += QString(",distToTerminal=%1").arg(it->distToTerminal);
		if (it->maxGreen > 0)sql += QString(",maxGreen=%1").arg(it->maxGreen);
		sql += QString(" WHERE vehicleDetectorId=%1").arg(it->vehicleDetectorId);
		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
			}
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update VehicleDetectors failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}

	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateReduceSpeedVehiTypes(const QList<ReduceSpeedVehiType*>& list)
{
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto it, list) {
		QString sql = "UPDATE ReduceSpeedVehiType set ";
		if (!std::_Is_nan(it->avgSpeed))sql += QString("avgSpeed=%1").arg(it->avgSpeed);
		if (!std::_Is_nan(it->speedSD))sql += QString(",speedSD=%1").arg(it->speedSD);
		sql += QString(" WHERE vehicleTypeCode=%1 and reduceSpeedAreaID=%2").arg(it->vehicleTypeCode).arg(it->reduceSpeedAreaID);
		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateReduceSpeedIntervals(const QList<ReduceSpeedInterval*>& list) {
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto it, list) {
		QString sql = "UPDATE ReduceSpeedInterval set ";
		if (it->reduceSpeedAreaID > 0)sql += QString("reduceSpeedAreaID=%1").arg(it->reduceSpeedAreaID);
		if (it->startTime > 0)sql += QString(",startTime=%1").arg(it->startTime);
		if (it->endTime > 0)sql += QString(",endTime=%1").arg(it->endTime);
		sql += QString(" WHERE intervalID=%1").arg(it->intervalID);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateReduceSpeedAreaBase(ReduceSpeedArea* pReduceSpeedArea) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE ReduceSpeedArea set ";
	if (!pReduceSpeedArea->name.isEmpty())sql += QString("name='%1'").arg(pReduceSpeedArea->name);
	if (!std::_Is_nan(pReduceSpeedArea->location))sql += QString(",location=%1").arg(pReduceSpeedArea->location);
	if (!std::_Is_nan(pReduceSpeedArea->areaLength))sql += QString(",areaLength=%1").arg(pReduceSpeedArea->areaLength);
	if (pReduceSpeedArea->roadID > 0)sql += QString(",roadID=%1").arg(pReduceSpeedArea->roadID);
	if (pReduceSpeedArea->laneNumber > 0)sql += QString(",laneNumber=%1").arg(pReduceSpeedArea->laneNumber);
	if (pReduceSpeedArea->toLaneNumber > 0)sql += QString(",toLaneNumber=%1").arg(pReduceSpeedArea->toLaneNumber);
	sql += QString(" WHERE reduceSpeedAreaID=%1").arg(pReduceSpeedArea->reduceSpeedAreaID);
	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}
	return result;
}
bool TessngDBToolsUpdateBase::updateReduceSpeedAreaPtr(ReduceSpeedArea* mpReduceSpeedArea)
{
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		result = updateReduceSpeedVehiTypes(mpReduceSpeedArea->mlReduceSpeedVehiType);
		if (!result) goto failed;

		result = updateReduceSpeedIntervals(mpReduceSpeedArea->mlReduceSpeedInterval);
		if (!result) goto failed;

		result = updateReduceSpeedAreaBase(mpReduceSpeedArea);
		if (!result) goto failed;
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update ReduceSpeedAreas failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateBustationPtr(BusStation* mpBusStation) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		QSqlQuery  query(gDB);
		QString sql = "UPDATE BusStation set ";
		if (!mpBusStation->name.isEmpty())sql += QString("name='%1'").arg(mpBusStation->name);
		if (!mpBusStation->type.isEmpty())sql += QString(",type='%1'").arg(gSpecialWords.trZhToEn(mpBusStation->type));
		if (mpBusStation->mpLink->linkID > 0)sql += QString(",linkID=%1").arg(mpBusStation->mpLink->linkID);
		if (mpBusStation->mpLink->laneNumber > 0)sql += QString(",laneNumber=%1").arg(mpBusStation->laneNumber);
		if (!std::_Is_nan(mpBusStation->x))sql += QString(",x=%1").arg(mpBusStation->x);
		if (!std::_Is_nan(mpBusStation->y))sql += QString(",y=%1").arg(mpBusStation->y);
		if (!std::_Is_nan(mpBusStation->length))sql += QString(",length=%1").arg(mpBusStation->length);
		sql += QString(" WHERE busStationID=%1").arg(mpBusStation->busStationID);
		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
			}
		}
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update Bustation failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
bool TessngDBToolsUpdateBase::updateBusLineRoads(BusLine* bline) {
	bool result = TessngDBToolsRemove::getInstance()->removeBusLineRoad(bline);
	if (!result) return false;
	QSqlQuery  query(gDB);
	QString sqlInsLinks = "insert into BusLineRoad(busLineID, serialNumber, roadID) values(:busLineID, :serialNumber, :roadID)";
	query.prepare(sqlInsLinks);
	for (int i = 0, size = bline->mlLink.size(); i < size; ++i)
	{
		Link* pLink = bline->mlLink.at(i);
		query.bindValue(":busLineID", QVariant::fromValue(bline->busLineID));
		query.bindValue(":serialNumber", i + 1);
		query.bindValue(":roadID", QVariant::fromValue(pLink->linkID));
		result = query.exec();
		if (!result) {
			throw PH::Exception(query.lastError().text().toStdString());
			break;
		}
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateBusStationPassengerArriving(long stationLineID, const QList<PassengerArriving*>& list) {
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto it, list) {
		QString sql = "UPDATE BusStationPassengerArriving set ";
		if (stationLineID > 0) sql += QString("stationLineID=%1").arg(stationLineID);
		if (it->startTime > 0)sql += QString(",startTime=%1").arg(it->startTime);
		if (it->endTime > 0)sql += QString(",endTime=%1").arg(it->endTime);
		if (it->passengerCount > 0)sql += QString(",passengerCount=%1").arg(it->passengerCount);
		sql += QString(" WHERE PassengerArrivingID=%1").arg(it->passengerArrivingID);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateBusStationLine(const QList<BusStationLine*>& list) {
	bool result = true;
	QSqlQuery  query(gDB);
	foreach(auto it, list) {
		result = updateBusStationPassengerArriving(it->stationLineID, it->mlPassengerArriving);
		if (!result) break;
		QString sql = "UPDATE BusStationLine set ";
		if (it->busStationID > 0)sql += QString("busStationID=%1").arg(it->busStationID);
		if (it->busLineID > 0)sql += QString(",busLineID=%1").arg(it->busLineID);
		if (it->parkingTime > 0)sql += QString(",parkingTime=%1").arg(it->parkingTime);
		if (!std::_Is_nan(it->getOnTimePerson))sql += QString(",getOnTimePerson=%1").arg(it->getOnTimePerson);
		if (!std::_Is_nan(it->getOutTimePerson))sql += QString(",getOutTimePerson=%1").arg(it->getOutTimePerson);
		if (!std::_Is_nan(it->leavingPercent))sql += QString(",leavingPercent=%1").arg(it->leavingPercent);
		sql += QString(" WHERE stationLineID=%1").arg(it->stationLineID);

		if (checkSqlString(sql)) {
			query.prepare(sql);
			result = query.exec();
			if (!result) {
				throw PH::Exception(query.lastError().text().toStdString());
				break;
			}
		}
	}

	return result;
}

bool TessngDBToolsUpdateBase::updateBusLineBase(BusLine* pBusLine) {
	bool result = true;
	QSqlQuery  query(gDB);
	QString sql = "UPDATE BusLine set ";
	if (!pBusLine->name.isEmpty())sql += QString("name='%1'").arg(pBusLine->name);
	if (!std::_Is_nan(pBusLine->length))sql += QString(",length=%1").arg(pBusLine->length);
	if (pBusLine->dischargeFreq > 0)sql += QString(",dischargeFreq=%1").arg(pBusLine->dischargeFreq);
	if (pBusLine->dischargeStartTime > 0)sql += QString(",dischargeStartTime=%1").arg(pBusLine->dischargeStartTime);
	if (pBusLine->dischargeEndTime > 0)sql += QString(",dischargeEndTime=%1").arg(pBusLine->dischargeEndTime);
	if (!std::_Is_nan(pBusLine->startX))sql += QString(",startX=%1").arg(pBusLine->startX);
	if (!std::_Is_nan(pBusLine->startY))sql += QString(",startY=%1").arg(pBusLine->startY);
	if (!std::_Is_nan(pBusLine->endX))sql += QString(",endX=%1").arg(pBusLine->endX);
	if (!std::_Is_nan(pBusLine->endY))sql += QString(",endY=%1").arg(pBusLine->endY);
	if (!std::_Is_nan(pBusLine->desirSpeed))sql += QString(",desirSpeed=%1").arg(pBusLine->desirSpeed);
	if (!std::_Is_nan(pBusLine->speedSD))sql += QString(",speedSD=%1").arg(pBusLine->speedSD);
	if (pBusLine->passCountAtStartTime > 0)sql += QString(",passCountAtStartTime=%1").arg(pBusLine->passCountAtStartTime);
	sql += QString(" WHERE busLineID=%1").arg(pBusLine->busLineID);
	if (checkSqlString(sql)) {
		query.prepare(sql);
		result = query.exec();
		if (!result) throw PH::Exception(query.lastError().text().toStdString());
	}
	return result;
}

bool TessngDBToolsUpdateBase::updateBusLinePtr(BusLine* mpBusLine) {
	bool result = true;
	try {
		//开启事务
		gDB.transaction();
		result = updateBusLineRoads(mpBusLine);
		if (!result) goto failed;

		result = updateBusStationLine(mpBusLine->mlBusStationLine);
		if (!result) goto failed;

		result = updateBusLineBase(mpBusLine);
		if (!result) goto failed;
	}
	catch (QException& exc) {
		qWarning() << exc.what() << __FILE__ << __LINE__;
		result = false;
	}
	catch (const PH::Exception& exc)
	{
		qWarning() << exc.message().c_str() << __FILE__ << __LINE__;
		result = false;
	}
	catch (...) {
		qWarning() << "update GBusLine failed! Unknow Error." << __FILE__ << __LINE__;
		result = false;
	}
failed:
	result = gDB.commit() && result;
	if (!result) {
		gDB.rollback();
	}

	return result;
}
