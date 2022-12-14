/***********************************************************************
 数据库层：仅包含数据库操作
 ***********************************************************************/
#ifndef TESSNGDBTOOLSBASE_H
#define TESSNGDBTOOLSBASE_H

#include "SafeHash.h"
#include "DBClassDefine.h"

class TessngDBToolsRemoveBase
{
protected:
    TessngDBToolsRemoveBase();
    virtual ~TessngDBToolsRemoveBase();
    //------------------------------------信号控制----------------------------------
    /// 信号灯
    bool removeSignalLamp(const QList<GSignalLamp*>& list);

    /// 相位信号颜色
    bool removeSignalColor(const SignalPhase* signalPhase, QList<int> serialNumbers);

    /// 相位：相位信号颜色，相位
    bool removeSignalPhase(const QList<SignalPhase*>& list);

    /// 信号灯组：相位信号颜色，相位，信号灯组
    bool removeSignalGroup(const QList<SignalGroup*>& list);

    //--------------------------------------公交------------------------------------
    /// 乘客到站
    bool removeBusStationPassengerArriving(const QList<PassengerArriving*>& list);

    /// 公交站点-线路：乘客到站，公交站点-线路
    bool removeBusStationLine(const QList<BusStationLine*>& list);

    /// 公交线路的路段或连接段序列
    bool removeBusLineRoad(const QList<GBusLine*>& list);
    bool removeBusLineLink(long busLineID, const QList<GLink*> busLineLinks);

    /// 公交线路：公交线路的路段或连接段序列，公交线路
    bool removeBusLine(const QList<GBusLine*>& list);

    /// 公交站
    bool removeBusStation(const QList<GBusStation*>& list);

    //---------------------------------车辆运行及检测-------------------------------
    /// 车辆信息采集器
    bool removeDrivInfoCollector(const QList<GVehicleDrivInfoCollector*>& list);

    /// 排队计数器
    bool removeVehicleQueueCounter(const QList<GVehicleQueueCounter*>& list);

    /// 行程时间检测器
    bool removeVehicleTravelDetector(const QList<GVehicleTravelDetector*>& list);

    /// 导向箭头
    bool removeGuideArrow(const QList<GGuideArrow*>& list);

    /// 限速区：限速区时间间隔，限速区车辆类型，限速区
    bool removeReduceSpeedArea(const QList<GReduceSpeedArea*>& list);

    /// 限速区时间间隔
    bool removeReduceSpeedInterval(const QList<ReduceSpeedInterval*>& list);

    /// 限速区车辆类型
    bool removeReduceSpeedVehiType(const QList<ReduceSpeedVehiType*>& list);

    /// 发车间隔
    bool removeDepartureInterval(const QList<DepaInterval>& list);

    /// 发车点：发车间隔，发车点
    bool removeDeparturePoint(const QList<GDeparturePoint*>& list);

    /// 车型组成和占比
    bool removeVehicleConsDetail(QList<VehicleConsDetail> list);

    /// 车型组成：车型组成和占比，车型组成
    bool removeVehicleConstitutent(const QList<VehicleComposition>& list);

    //-----------------------------------道路及连接---------------------------------
    /// 路径流量分配
    bool removeRoutingFLowRatio(const QList<RoutingFLowRatio*>& list);
    bool removeRoutingFLowRatio(const QList<GRouting*>& list);

    /// 路径车道连接
    bool insertRoutingLaneConnector(long routingID, long connID, long fromLaneId, long toLaneId);

    bool removeRoutingLaneConnector(GRouting* routing, const SafeHash<long, LCStruct*>& lcStructs);
    bool removeRoutingLaneConnector(GRouting* routing);
    bool removeRoutingLaneConnector(long routingID, long connID);
    bool removeRoutingLaneConnector(long routingID, long connID, long fromLaneId, long toLaneId);

    /// 路径路段
    bool removeRoutingLink(const QList<GRouting*>& list);
    bool removeRoutingLink(long routingID, const QList<ILink*> list);

    /// 路径：路径流量分配，路径车道连接，路径路段，路径
    bool removeRouting(const QList<GRouting*>& list);

    /// 决策点：路径流量分配，路径车道连接，路径路段，路径，决策点
    bool removeDecisionPoint(const QList<GDecisionPoint*>& list);

    /// 车道连接
    bool removeLaneConnector(const QList<GLaneConnector*>& list);
    bool removeLaneConnector(const QList<LaneConnector*>& list);

    /// 连接段：车道连接，连接段
    bool removeConnector(const QList<GConnector*>& list);

    /// 路段顶点：顶点，路段顶点
    bool removeLinkVertex(const QList<GVertex*>& list);

    /// 车道
    bool removeLane(const QList<GLane*>& list);

    /// 路段：结点，车道，顶点，路段顶点，路段
    bool removeLink(const QList<GLink*>& list);
private:
    friend class TessngDBToolsUpdateBase;
    TessngDBToolsRemoveBase(const TessngDBToolsRemoveBase&) = delete;

    //------------------------------------信号控制----------------------------------
    /// 信号灯
    bool removeSignalLamp(GSignalLamp* it);

    /// 相位信号颜色
    bool removeSignalColor(const SignalPhase* signalPhase, int serialNumber);

    /// 相位
    bool removeSignalPhase(SignalPhase* it);

    /// 信号灯组
    bool removeSignalGroup(SignalGroup* it);

    //--------------------------------------公交------------------------------------
    /// 乘客到站
    bool removeBusStationPassengerArriving(PassengerArriving* it);

    /// 公交站点-线路
    bool removeBusStationLine(BusStationLine* it);

    /// 公交线路的路段或连接段序列
    bool removeBusLineRoad(BusLine* it);

    /// 公交线路
    bool removeBusLine(GBusLine* it);

    /// 公交站
    bool removeBusStation(GBusStation* it);

    //---------------------------------车辆运行及检测-------------------------------
    /// 车辆信息采集器
    bool removeDrivInfoCollector(GVehicleDrivInfoCollector* it);

    /// 排队计数器
    bool removeVehicleQueueCounter(GVehicleQueueCounter* it);

    /// 行程时间检测器
    bool removeVehicleTravelDetector(GVehicleTravelDetector* it);

    /// 导向箭头
    bool removeGuideArrow(GGuideArrow* it);

    /// 限速区间隔
    bool removeReduceSpeedInterval(ReduceSpeedInterval* it);

    /// 限速区车辆类型
    bool removeReduceSpeedVehiType(ReduceSpeedVehiType* it);

    /// 限速区
    bool removeReduceSpeedArea(GReduceSpeedArea* it);

    /// 发车间隔
    bool removeDepartureInterval(DepaInterval it);
    bool removeDepartureInterval(GDeparturePoint* it);
    bool removeDepartureInterval(VehicleComposition* it);

    /// 发车点
    bool removeDeparturePoint(GDeparturePoint* it);

    /// 车型组成和占比
    bool removeVehicleConsDetail(VehicleConsDetail it);

    /// 车型组成
    bool removeVehicleConstitutent(VehicleComposition it);

    //-----------------------------------道路及连接---------------------------------
    /// 路径流量分配
    bool removeRoutingFLowRatio(RoutingFLowRatio* it);
    bool removeRoutingFLowRatio(GRouting* it);

    /// 路径车道连接
    bool removeRoutingLaneConnector(GRouting* routing, long connID, LCStruct* it);

    /// 路径路段
    bool removeRoutingLink(GRouting* it);
    bool removeRoutingLink(Routing* it);

    /// 路径
    bool removeRouting(GRouting* it);

    /// 决策点
    bool removeDecisionPoint(GDecisionPoint* it);

    /// 车道连接
    bool removeLaneConnector(GLaneConnector* it);
    bool removeLaneConnector(LaneConnector* it);

    /// 连接段
    bool removeConnector(GConnector* it);

    /// 顶点-应仅给路段顶点使用
    bool removeVertex(const QList<GVertex*>& list);
    bool removeVertex(GVertex* it);

    /// 路段顶点
    bool removeLinkVertex(GVertex* it);

    /// 车道
    bool removeLane(Lane* it);

    /// 节点--不对外开放，仅发生在路段的增删
    bool removeNode(const QList<Node*>& list);
    bool removeNode(Node* it);

    /// 路段
    bool removeLink(GLink* it);

    //-----------------------------------参数及模型---------------------------------

};

#endif // TESSNGDBTOOLSBASE_H
