/***********************************************************************
 业务层：通过调用数据库层接口，保障场景内存数据与数据库数据的一致性，控制删除顺序
 ***********************************************************************/
#ifndef TESSNGDBTOOLSREMOVE_H
#define TESSNGDBTOOLSREMOVE_H
#include "singleholder.h"
#include "TessngDBToolsRemoveBase.h"
class TessngDBToolsRemove :public TessngDBToolsRemoveBase ,public PH::SingleHolder<TessngDBToolsRemove>
{
public:
    //------------------------------------信号控制----------------------------------
    //删除信号灯
    bool deleteSignalLamp(QList<long> ids);

    //删除相位颜色
    bool deleteSignalColor(long id, QList<int> serialNnumbers);

    //删除相位
    bool deleteSignalPhase(QList<long> ids);

    //删除相位灯组
    bool deleteSignalGroup(QList<long> ids);

    //--------------------------------------公交------------------------------------
    //删除乘客到站
    bool deleteBusStationPassengerArriving(QList<long> ids);

    //删除站点线路
    bool deleteBusStationLine(QList<long> ids);

    //删除公交站
    bool deleteBusStation(QList<long> ids);

    //删除公交线路
    bool deleteBusLine(QList<long> ids);

    //---------------------------------车辆运行及检测-------------------------------
    //删除车辆信息采集器
    bool deleteDrivInfoCollector(QList<long> ids);

    //删除排队计数器
    bool deleteVehicleQueueCounter(QList<long> ids);

    //删除行程时间检测器
    bool deleteVehicleTravelDetector(QList<long> ids);

    //删除导向箭头
    bool deleteGuideArrow(QList<long> ids);

    //删除限速区间隔
    bool deleteReduceSpeedInterval(QList<long> ids);

    //删除限速区车辆类型
    bool deleteReduceSpeedVehiType(long id, QList<long> vehicleTypeCodes);

    //删除限速区
    bool deleteReduceSpeedArea(QList<long> ids);

    //删除发车间隔
    bool deleteDepartureInterval(long id, QList<long> departureIntervalDs);

    //删除发车点
    bool deleteDeparturePoint(QList<long> ids);

    //删除车型组成和占比
    bool deleteVehicleConsDetail(long id, QList<long> vehicleTypeCodes);

    //删除车型组成
    bool deleteVehicleConstitutent(QList<long> ids);

    //-----------------------------------道路及连接---------------------------------
    //删除路径流量分配
    bool deleteRoutingFLowRatio(QList<long> ids);

    //删除路径车道连接: @param isFix[调用方是否需要在删除路径车道连接是最后一条的情况下，对路径进行保护处理]
    bool deleteRoutingLaneConnector(long routingID, long connID, long fromLaneId, long toLaneId, bool isFix = false);

    //删除路径
    bool deleteRouting(QList<long> ids);

    //删除决策点
    bool deleteDecisionPoint(QList<long> ids);

    //删除车道连接: @param trans[开启数据回滚，默认为true]
    bool deleteLaneConnector(long connID, long fromLaneId, long toLaneId, bool trans=true);

    //删除连接段
    bool deleteConnectors(QList<long> ids);

    //删除车道
    bool deleteLane(QList<long> ids);

    //删除路段
    bool deleteLinkVertex(long id, QList<int> nums);

    //删除路段: @param clearCache[清理内存，默认为true] @param trans[开启数据回滚，默认为true]
    bool deleteLink(QList<long> ids, bool clearCache=true, bool trans=true);

private:
    friend class PH::SingleHolder<TessngDBToolsRemove>;
    TessngDBToolsRemove();

    //检查这些路段里是否包含该车道
    bool checkContansLane(const QList<GLink*>& list, long laneId);

    //检查这些连接段里是否包含该车道连接
    bool checkContansLane(const QList<GConnector*>& list, long fromlaneId, long tolaneId);

    //检查这些连接段里是否包含该连接段
    bool checkContainsConnector(const QList<GConnector*>& list, long id);

    bool protectRoutingLaneConnector(GConnector* connector, long fromLaneId, long toLaneId);

    bool protectRoutingLinks(GConnector* connector, long fromLaneId, long toLaneId);

    bool protectBusLineLinks(GConnector* connector, long fromLaneId, long toLaneId);
};

#endif // TESSNGDBTOOLSREMOVE_H
