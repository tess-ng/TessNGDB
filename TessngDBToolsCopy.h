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
	//--------------------------------------公交------------------------------------

	bool copyPassengerArriving(PassengerArriving* dest, PassengerArriving* src);
	bool copyBusStationLine(BusStationLine* dest, BusStationLine* src);
	bool copyBusLine(BusLine& dest, BusLine* src);
	bool copyBusStation(BusStation& dest, BusStation* src);
	//---------------------------------车辆运行及检测-------------------------------

	bool copyReduceSpeedInterval(ReduceSpeedInterval* dest, ReduceSpeedInterval* src);
	bool copyReduceSpeedVehiType(ReduceSpeedVehiType* dest, ReduceSpeedVehiType* src);
	bool copyReduceSpeedArea(ReduceSpeedArea& dest, ReduceSpeedArea* src);

	//-----------------------------------道路及连接---------------------------------
	bool copyLink(Link* dest, Link* src);
protected:
	~TessngDBToolsCopy();
};

