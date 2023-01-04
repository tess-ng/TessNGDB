#include "pch"
#include "TessngDBToolsCopy.h"
TessngDBToolsCopy::TessngDBToolsCopy()
{

}

TessngDBToolsCopy::~TessngDBToolsCopy()
{

}
//------------------------------------信号控制----------------------------------


//--------------------------------------公交------------------------------------
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

bool TessngDBToolsCopy::copyPassengerArriving(PassengerArriving* dest, PassengerArriving* src) {
    dest->passengerArrivingID = src->passengerArrivingID;
    dest->startTime = src->startTime;
    dest->endTime = src->endTime;
    dest->passengerCount = src->passengerCount;
    return true;
}

//---------------------------------车辆运行及检测-------------------------------


//-----------------------------------道路及连接---------------------------------