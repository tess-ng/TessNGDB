#pragma once
#include "singleholder.h"

#include "gsignallamp.h"
#include "signallamp.h"
#include "signalphase.h"
#include "signalGroup.h"
#include "ISignalPhase.h"

#include "PassengerArriving.h"
#include "BusStationLine.h"
#include "GBusLine.h"
#include "GBusStation.h"
#include "BusStation.h"

#include "GVehicleDrivInfoCollector.h"
#include "VehicleDrivInfoCollector.h"
#include "GVehicleQueueCounter.h"
#include "VehicleQueueCounter.h"
#include "GVehicleTravelDetector.h"
#include "VehicleTravelDetector.h"
#include "GGuideArrow.h"
#include "GuideArrow.h"
#include "ReduceSpeedInterval.h"
#include "ReduceSpeedVehiType.h"
#include "GReducespeedarea.h"
#include "ReduceSpeedArea.h"
#include "GDeparturepoint.h"
#include "DeparturePoint.h"

#include "RoutingFLowRatio.h"
#include "GRouting.h"
#include "Routing.h"
#include "GDecisionPoint.h"
#include "DecisionPoint.h"
#include "GLaneConnector.h"
#include "LaneConnector.h"
#include "GConnector.h"
#include "Connector.h"
#include "GVertex.h"
#include "Lane.h"
#include "Node.h"
#include "GLink.h"
#include "Link.h"
#include "ILink.h"
#include "Vehicletype.h"
#include "GVehicleDetector.h"
class TessngDBToolsCopy: public PH::SingleHolder<TessngDBToolsCopy>
{
public:
	TessngDBToolsCopy::TessngDBToolsCopy();
	//------------------------------------信号控制----------------------------------
	bool copySignalLamp(SignalLamp& dest, SignalLamp* src);
	bool copySignalPhase(SignalPhase* dest, SignalPhase* src);
	bool copySignalGroup(SignalGroup& dest, SignalGroup* src);
	bool initSignalLamp(SignalLamp& dest);
	bool initSignalPhase(SignalPhase* dest);
	bool initSignalGroup(SignalGroup& dest);
	//--------------------------------------公交------------------------------------
	bool copyPassengerArriving(PassengerArriving* dest, PassengerArriving* src);
	bool copyBusStationLine(BusStationLine* dest, BusStationLine* src);
	bool copyBusLine(BusLine& dest, BusLine* src);
	bool copyBusStation(BusStation& dest, BusStation* src);

	bool initPassengerArriving(PassengerArriving* dest);
	bool initBusStationLine(BusStationLine* dest);
	bool initBusLine(BusLine& dest);
	bool initBusStation(BusStation& dest);
	//---------------------------------车辆运行及检测-------------------------------
	bool copyReduceSpeedInterval(ReduceSpeedInterval* dest, ReduceSpeedInterval* src);
	bool copyReduceSpeedVehiType(ReduceSpeedVehiType* dest, ReduceSpeedVehiType* src);
	bool copyReduceSpeedArea(ReduceSpeedArea& dest, ReduceSpeedArea* src);
	bool copyVehicleTravelDetector(VehicleTravelDetector& dest, VehicleTravelDetector* src);
	bool copyVehicleQueueCounter(VehicleQueueCounter& dest, VehicleQueueCounter* src);
	bool copyVehicleDrivInfoCollector(VehicleDrivInfoCollector& dest, VehicleDrivInfoCollector* src);
	bool copyGuideArrow(GuideArrow& dest, GuideArrow* src);
	bool copyDeparturePoint(DeparturePoint& dest, DeparturePoint* src);

	bool initReduceSpeedInterval(ReduceSpeedInterval* dest);
	bool initReduceSpeedVehiType(ReduceSpeedVehiType* dest);
	bool initReduceSpeedArea(ReduceSpeedArea& dest);
	bool initVehicleTravelDetector(VehicleTravelDetector& dest);
	bool initVehicleQueueCounter(VehicleQueueCounter& dest);
	bool initVehicleDrivInfoCollector(VehicleDrivInfoCollector& dest);
	bool initGuideArrow(GuideArrow& dest);
	bool initDeparturePoint(DeparturePoint& dest);
	//-----------------------------------道路及连接---------------------------------

	bool copyRouting(Routing& dest, Routing* src);
	bool copyRoutingFLowRatio(RoutingFLowRatio* dest, RoutingFLowRatio* src);
	bool copyRoutingFlowByInterval(RoutingFlowByInterval* dest, RoutingFlowByInterval* src);
	bool copyDecisionPoint(DecisionPoint& dest, DecisionPoint* src);
	bool copyLaneConnector(LaneConnector* dest, LaneConnector* src);
	bool copyConnector(Connector& dest, Connector* src);
	bool copyLane(Lane& dest, Lane* src);
	bool copyLink(Link* dest, Link* src);
	bool copyLink(Link& dest, Link* src);

	bool initRouting(Routing& dest);
	bool initRoutingFLowRatio(RoutingFLowRatio* dest);
	bool initRoutingFlowByInterval(RoutingFlowByInterval* dest);
	bool initDecisionPoint(DecisionPoint& dest);
	bool initLaneConnector(LaneConnector* dest);
	bool initConnector(Connector& dest);
	bool initLane(Lane& dest);
	bool initLink(Link* dest);
	bool initLink(Link& dest);
protected:
	~TessngDBToolsCopy();
private:
	const __int64 __NaN = 0xFFF8000000000000;
};

