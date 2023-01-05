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

bool copySignalPhase(SignalPhase* dest, SignalPhase* src) {
    dest->signalPhaseID = src->signalPhaseID;
    dest->serialNumber = src->serialNumber;
    dest->name = src->name;
    dest->mlSignalColor = src->mlSignalColor;
    dest->mpSignalGroup = src->mpSignalGroup;
    return true;
}

bool copySignalGroup(SignalGroup& dest, SignalGroup* src) {
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

//-----------------------------------道路及连接---------------------------------
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