/***********************************************************************
 业务层：通过调用数据库层接口，保障场景内存数据与数据库数据的一致性，控制删除顺序
 ***********************************************************************/
#ifndef TESSNGDBTOOLS_H
#define TESSNGDBTOOLS_H
#include "singleholder.h"
#include "TessngDBToolsBase.h"

class TessngDBTools:public TessngDBToolsBase, public PH::SingleHolder<TessngDBTools>
{
public:
    //------------------------------------信号控制----------------------------------
    /// 信号灯
    bool deleteSignalLamp(QList<long> ids);

    /// 相位信号颜色
    bool deleteSignalColor(long id, QList<int> colors);

    /// 相位：相位信号颜色，相位
    bool deleteSignalPhase(QList<long> ids);

	/// 删除信号灯组
	bool deleteSignalGroup(QList<long> ids);

	//--------------------------------------公交------------------------------------
	bool deleteBusStationPassengerArriving(QList<long> ids);

	bool deleteBusStationLine(QList<long> ids);

	bool deleteBusStation(QList<long> ids);

	bool deleteBusLine(QList<long> ids);

	//---------------------------------车辆运行及检测-------------------------------
	/**删除车辆信息采集器**/
	bool deleteDrivInfoCollector(QList<long> ids);

	/**删除计数器**/
	bool deleteVehicleQueueCounter(QList<long> ids);

	/**删除行程时间检测器**/
	bool deleteVehicleTravelDetector(QList<long> ids);

	/**删除检测器**/
	bool deleteVehicleDetector(QList<long> ids);

	/**删除限速区**/
	bool deleteReduceSpeedArea(QList<long> ids);
	//-----------------------------------道路及连接---------------------------------
	/// 删除路径
	bool deleteRouting(QList<long> ids);

    /// 删除连接段
    bool deleteConnectors(QList<long> ids);

    /// 删除车道
    bool deleteLane(QList<long> ids);

    /// 删除路段
    bool deleteLink(QList<long> ids);
    
private:
    friend class PH::SingleHolder<TessngDBTools>;
    TessngDBTools();

    /// 检查这些路段里是否包含该车道
    bool checkContansLane(const QList<GLink*>& hash, long laneId);

    /// 检查这些连接段里是否包含该车道连接
    bool checkContansLane(const QList<GConnector*>& hash, long fromlaneId, long tolaneId);

    /// 检查这些连接段里是否包含该连接段
    bool checkContainsConnector(const QList<GConnector*>& list, long id);
};

#endif // TESSNGDBTOOLS_H
