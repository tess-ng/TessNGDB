#include "pch"
#include "TessngDBToolsCopy.h"
TessngDBToolsCopy::TessngDBToolsCopy()
{

}

TessngDBToolsCopy::~TessngDBToolsCopy()
{

}
//------------------------------------信号控制----------------------------------
bool TessngDBToolsCopy::copySignalLamp(SignalLamp& dest, SignalLamp* src) {
    dest.signalLampID = src->signalLampID;
    dest.name = src->name;
    dest.mpSignalPhase = src->mpSignalPhase;
    dest.laneID = src->laneID;
    dest.toLaneID = src->toLaneID;
    dest.x = src->x;
    dest.y = src->y;
    dest.z = src->z;
    return true;
}

bool TessngDBToolsCopy::copySignalPhase(SignalPhase* dest, SignalPhase* src) {
    dest->signalPhaseID = src->signalPhaseID;
    dest->serialNumber = src->serialNumber;
    dest->name = src->name;
    dest->mlSignalColor = src->mlSignalColor;
    dest->mpSignalGroup = src->mpSignalGroup;
    return true;
}

bool TessngDBToolsCopy::copySignalGroup(SignalGroup& dest, SignalGroup* src) {
    dest.signalGroupID = src->signalGroupID;
    dest.name = src->name;
    dest.timeLength = src->timeLength;
    dest.startTime = src->startTime;
    dest.endTime = src->endTime;
    dest.mlPhase = src->mlPhase;
    return true;
}

//--------------------------------------公交------------------------------------
bool TessngDBToolsCopy::copyPassengerArriving(PassengerArriving* dest, PassengerArriving* src) {
    dest->passengerArrivingID = src->passengerArrivingID;
    dest->startTime = src->startTime;
    dest->endTime = src->endTime;
    dest->passengerCount = src->passengerCount;
    return true;
}

bool TessngDBToolsCopy::copyBusStationLine(BusStationLine* dest, BusStationLine* src) {
    dest->stationLineID = src->stationLineID;
    dest->busStationID = src->busStationID;
    dest->linkID = src->linkID;
    dest->busLineID = src->busLineID;
    dest->parkingTime = src->parkingTime;
    dest->leavingPercent = src->leavingPercent;
    dest->getOnTimePerson = src->getOnTimePerson;
    dest->getOutTimePerson = src->getOutTimePerson;
    dest->mlPassengerArriving = src->mlPassengerArriving;
    dest->mlPassenger = src->mlPassenger;
    dest->mlPassengerLeaved = src->mlPassengerLeaved;
    return true;
}

bool TessngDBToolsCopy::copyBusLine(BusLine& dest, BusLine* src) {
    dest.busLineID = src->busLineID;
    dest.name = src->name;
    dest.length = src->length;
    dest.dischargeFreq = src->dischargeFreq;
    dest.dischargeStartTime = src->dischargeStartTime;
    dest.dischargeEndTime = src->dischargeEndTime;
    dest.startX = src->startX;
    dest.startY = src->startY;
    dest.endX = src->endX;
    dest.endY = src->endY;
    dest.desirSpeed = src->desirSpeed;
    dest.speedSD = src->speedSD;
    dest.passCountAtStartTime = src->passCountAtStartTime;
    dest.mlLink = src->mlLink;
    dest.mlLinkId = src->mlLinkId;
    dest.mlBusStationLine = src->mlBusStationLine;
    dest.mSpecialApp = src->mSpecialApp;
    return true;
}

bool TessngDBToolsCopy::copyBusStation(BusStation& dest, BusStation* src) {
    dest.busStationID = src->busStationID;
    dest.name = src->name;
    dest.laneNumber = src->laneNumber;
    dest.x = src->x;
    dest.y = src->y;
    dest.length = src->length;
    dest.type = src->type;
    dest.mlBusStationLine = src->mlBusStationLine;
    dest.mpLink = src->mpLink;
    return true;
}

//---------------------------------车辆运行及检测-------------------------------
bool TessngDBToolsCopy::copyReduceSpeedInterval(ReduceSpeedInterval* dest, ReduceSpeedInterval* src) {
    dest->intervalID = src->intervalID;
    dest->reduceSpeedAreaID = src->reduceSpeedAreaID;
    dest->startTime = src->startTime;
    dest->endTime = src->endTime;
    return true;
}

bool TessngDBToolsCopy::copyReduceSpeedVehiType(ReduceSpeedVehiType* dest, ReduceSpeedVehiType* src) {
    dest->vehicleTypeCode = src->vehicleTypeCode;
    dest->reduceSpeedAreaID = src->reduceSpeedAreaID;
    dest->avgSpeed = src->avgSpeed;
    dest->speedSD = src->speedSD;
    return true;
}

bool TessngDBToolsCopy::copyReduceSpeedArea(ReduceSpeedArea& dest, ReduceSpeedArea* src) {
    dest.reduceSpeedAreaID = src->reduceSpeedAreaID;
    dest.name = src->name;
    dest.location = src->location;
    dest.areaLength = src->areaLength;
    dest.roadID = src->roadID;
    dest.laneNumber = src->laneNumber;
    dest.toLaneNumber = src->toLaneNumber;
    dest.mlReduceSpeedInterval = src->mlReduceSpeedInterval;
    dest.mlReduceSpeedVehiType = src->mlReduceSpeedVehiType;
    return true;
}

bool TessngDBToolsCopy::copyVehicleTravelDetector(VehicleTravelDetector& dest, VehicleTravelDetector* src) {
    dest.detectorId = src->detectorId;
    dest.name = src->name;
    dest.startRoadId = src->startRoadId;
    dest.start_laneNumber = src->start_laneNumber;
    dest.start_toLaneNumber = src->start_toLaneNumber;
    dest.startDist = src->startDist;
    dest.startX = src->startX;
    dest.startY = src->startY;
    dest.teminalRoadId = src->teminalRoadId;
    dest.teminal_laneNumber = src->teminal_laneNumber;
    dest.teminal_toLaneNumber = src->teminal_toLaneNumber;
    dest.teminalDist = src->teminalDist;
    dest.teminalX = src->teminalX;
    dest.teminalY = src->teminalY;
    dest.startTime = src->startTime;
    dest.endTime = src->endTime;
    dest.dataInterval = src->dataInterval;
    dest.shortestDistance = src->shortestDistance;
    return true;
}

bool TessngDBToolsCopy::copyVehicleQueueCounter(VehicleQueueCounter& dest, VehicleQueueCounter* src) {
    dest.queueCounterID = src->queueCounterID;
    dest.name = src->name;
    dest.roadID = src->roadID;
    dest.laneNumber = src->laneNumber;
    dest.toLaneNumber = src->toLaneNumber;
    dest.x = src->x;
    dest.y = src->y;
    dest.z = src->z;
    dest.speedLowLimit = src->speedLowLimit;
    dest.speedUpLimit = src->speedUpLimit;
    dest.maxDistInterval = src->maxDistInterval;
    dest.maxQueueLength = src->maxQueueLength;
    dest.distance = src->distance;
    dest.startTime = src->startTime;
    dest.endTime = src->endTime;
    dest.dataInterval = src->dataInterval;
    dest.countInterval = src->countInterval;
    return true;
}

bool TessngDBToolsCopy::copyVehicleDrivInfoCollector(VehicleDrivInfoCollector& dest, VehicleDrivInfoCollector* src) {
    dest.collecterID = src->collecterID;
    dest.name = src->name;
    dest.roadID = src->roadID;
    dest.laneNumber = src->laneNumber;
    dest.toLaneNumber = src->toLaneNumber;
    dest.distance = src->distance;
    dest.x = src->x;
    dest.y = src->y;
    dest.z = src->z;
    dest.startTime = src->startTime;
    dest.endTime = src->endTime;
    dest.dataInterval = src->dataInterval;
    return true;
}

bool TessngDBToolsCopy::copyGuideArrow(GuideArrow& dest, GuideArrow* src) {
    dest.guideArrowID = src->guideArrowID;
    dest.laneID = src->laneID;
    dest.length = src->length;
    dest.distToTerminal = src->distToTerminal;
    dest.arrowType = src->arrowType;
    return true;
}

bool TessngDBToolsCopy::copyDeparturePoint(DeparturePoint& dest, DeparturePoint* src) {
    dest.departurePointID = src->departurePointID;
    dest.name = src->name;
    dest.mpLink = src->mpLink;
    dest.mlDepaInterval = src->mlDepaInterval;
    dest.mlDepaPossion = src->mlDepaPossion;
    return true;
}
//-----------------------------------道路及连接---------------------------------
bool TessngDBToolsCopy::copyRouting(Routing& dest, Routing* src) {
    dest.routingID = src->routingID;
    dest.routingName = src->routingName;
    dest.proportion = src->proportion;
    dest.mpDecisionPoint = src->mpDecisionPoint;
    dest.mllLink = src->mllLink;
    return true;
}

bool TessngDBToolsCopy::copyRoutingFLowRatio(RoutingFLowRatio* dest, RoutingFLowRatio* src) {
    dest->RoutingFLowRatioID = src->RoutingFLowRatioID;
    dest->routingID = src->routingID;
    dest->startDateTime = src->startDateTime;
    dest->endDateTime = src->endDateTime;
    dest->ratio = src->ratio;
    dest->vehiCount = src->vehiCount;
    return true;
}

bool TessngDBToolsCopy::copyRoutingFlowByInterval(RoutingFlowByInterval* dest, RoutingFlowByInterval* src) {
    dest->endDateTime = src->endDateTime;
    dest->mlRoutingFlowRatio = src->mlRoutingFlowRatio;
    return true;
}

bool TessngDBToolsCopy::copyDecisionPoint(DecisionPoint& dest, DecisionPoint* src) {
    dest.deciPointID = src->deciPointID;
    dest.deciPointName = src->deciPointName;
    dest.X = src->X;
    dest.Y = src->Y;
    dest.Z = src->Z;
    dest.mFromPoint = src->mFromPoint;
    dest.mToPoint = src->mToPoint;
    dest.mlRouting = src->mlRouting;
    dest.mpLink = src->mpLink;
    dest.mlRoutingFlowByInterval = src->mlRoutingFlowByInterval;
    return true;
}

bool TessngDBToolsCopy::copyLaneConnector(LaneConnector* dest, LaneConnector* src) {
    dest->mpConnector = src->mpConnector;
    dest->mpFromLane = src->mpFromLane;
    dest->mpToLane = src->mpToLane;
    dest->laneConnectorID = src->laneConnectorID;
    dest->mrLength = src->mrLength;
    dest->weight = src->weight;
    dest->centerLinePointsJson = src->centerLinePointsJson;
    dest->leftBreakPointsJson = src->leftBreakPointsJson;
    dest->rightBreakPointsJson = src->rightBreakPointsJson;
    dest->otherAttrsJson = src->otherAttrsJson;
    return true;
}

bool TessngDBToolsCopy::copyConnector(Connector& dest, Connector* src) {
    dest.connID = src->connID;
    dest.roadId = src->roadId;
    dest.connName = src->connName;
    dest.length = src->length;
    dest.curvature = src->curvature;
    dest.nonLinearCoefficient = src->nonLinearCoefficient;
    dest.color = src->color;
    dest.desiredSpeed = src->desiredSpeed;
    dest.limitSpeed = src->limitSpeed;
    dest.leftBreakPointsJson = src->leftBreakPointsJson;
    dest.rightBreakPointsJson = src->rightBreakPointsJson;
    dest.otherAttrsJson = src->otherAttrsJson;
    dest.mpFromLink = src->mpFromLink;
    dest.mpToLink = src->mpToLink;
    dest.mlLaneConnector = src->mlLaneConnector;
    dest.mlVehicleType = src->mlVehicleType;
    return true;
}

bool TessngDBToolsCopy::copyLane(Lane& dest, Lane* src) {
    dest.laneID = src->laneID;
    dest.linkID = src->linkID;
    dest.serialNumber = src->serialNumber;
    dest.width = src->width;
    dest.expectTravelDirection = src->expectTravelDirection;
    dest.actionType = src->actionType;
    dest.centerLinePointsJson = src->centerLinePointsJson;
    dest.leftBreakPointsJson = src->leftBreakPointsJson;
    dest.rightBreakPointsJson = src->rightBreakPointsJson;
    dest.otherAttrsJson = src->otherAttrsJson;
    dest.mlGuideArrow = src->mlGuideArrow;
    dest.mlVehicleType = src->mlVehicleType;
    return true;
}

bool TessngDBToolsCopy::copyLink(Link* dest, Link* src) {
    dest->linkID = src->linkID;
    dest->linkName = src->linkName;
    dest->netId = src->netId;
    dest->roadId = src->roadId;
    dest->laneNumber = src->laneNumber;
    dest->laneWidth = src->laneWidth;
    dest->laneColor = src->laneColor;
    dest->linkType = src->linkType;
    dest->length = src->length;
    dest->curvature = src->curvature;
    dest->nonLinearCoefficient = src->nonLinearCoefficient;
    dest->linkSaturationFlow = src->linkSaturationFlow;
    dest->linkTrafficFlow = src->linkTrafficFlow;
    dest->desiredSpeed = src->desiredSpeed;
    dest->limitSpeed = src->limitSpeed;
    dest->minSpeed = src->minSpeed;
    dest->addValue = src->addValue;
    dest->centerLinePointsJson = src->centerLinePointsJson;
    dest->leftBreakPointsJson = src->leftBreakPointsJson;
    dest->rightBreakPointsJson = src->rightBreakPointsJson;
    dest->otherAttrsJson = src->otherAttrsJson;

    dest->startNode = src->startNode;
    dest->endNode = src->endNode;
    dest->mlVertex = src->mlVertex;
    dest->mlLane = src->mlLane;
    return true;
}

bool TessngDBToolsCopy::copyLink(Link& dest, Link* src) {
    dest.linkID = src->linkID;
    dest.linkName = src->linkName;
    dest.netId = src->netId;
    dest.roadId = src->roadId;
    dest.laneNumber = src->laneNumber;
    dest.laneWidth = src->laneWidth;
    dest.laneColor = src->laneColor;
    dest.linkType = src->linkType;
    dest.length = src->length;
    dest.curvature = src->curvature;
    dest.nonLinearCoefficient = src->nonLinearCoefficient;
    dest.linkSaturationFlow = src->linkSaturationFlow;
    dest.linkTrafficFlow = src->linkTrafficFlow;
    dest.desiredSpeed = src->desiredSpeed;
    dest.limitSpeed = src->limitSpeed;
    dest.minSpeed = src->minSpeed;
    dest.addValue = src->addValue;
    dest.centerLinePointsJson = src->centerLinePointsJson;
    dest.leftBreakPointsJson = src->leftBreakPointsJson;
    dest.rightBreakPointsJson = src->rightBreakPointsJson;
    dest.otherAttrsJson = src->otherAttrsJson;

    dest.startNode = src->startNode;
    dest.endNode = src->endNode;
    dest.mlVertex = src->mlVertex;
    dest.mlLane = src->mlLane;
    return true;
}
