/***********************************************************************
 业务层：通过调用数据库层接口，保障场景内存数据与数据库数据的一致性，控制删除顺序
 ***********************************************************************/
#ifndef TESSNGDBTOOLS_H
#define TESSNGDBTOOLS_H
#include "singleholder.h"
#include "TessngDBToolsBase.h"

class TessngDBTools :public TessngDBToolsBase, public PH::SingleHolder<TessngDBTools>
{
public:
	//------------------------------------信号控制----------------------------------
	/// 信号灯
	bool deleteSignalLamp(QList<long> ids);

	/**
	 * @brief 删除相位颜色
	 * @param id					[相位ID]
	 * @param serialNnumbers		[颜色序号]
	 *
	 * @return bool
	 */
	bool deleteSignalColor(long id, QList<int> serialNnumbers);

	/**
	 * @brief 删除相位
	 * @param ids					[相位ID]
	 *
	 * @return bool
	 */
	bool deleteSignalPhase(QList<long> ids);

	/**
	 * @brief 删除相位灯组
	 * @param ids					[相位灯组ID]
	 *
	 * @return bool
	 */
	bool deleteSignalGroup(QList<long> ids);

	//--------------------------------------公交------------------------------------
	/**
	 * @brief 删除乘客到站
	 * @param ids					[乘客到站ID]
	 *
	 * @return bool
	 */
	bool deleteBusStationPassengerArriving(QList<long> ids);

	/**
	 * @brief 删除站点线路
	 * @param ids					[站点线路ID]
	 *
	 * @return bool
	 */
	bool deleteBusStationLine(QList<long> ids);

	/**
	 * @brief 删除公交站
	 * @param ids					[公交站ID]
	 *
	 * @return bool
	 */
	bool deleteBusStation(QList<long> ids);

	/**
	 * @brief 删除公交线路
	 * @param ids					[线路ID]
	 *
	 * @return bool
	 */
	bool deleteBusLine(QList<long> ids);

	//---------------------------------车辆运行及检测-------------------------------
	/**
	 * @brief 删除车辆信息采集器
	 * @param ids					[车辆信息采集器ID]
	 *
	 * @return bool
	 */
	bool deleteDrivInfoCollector(QList<long> ids);

	/**
	 * @brief 删除排队计数器
	 * @param ids					[排队计数器ID]
	 *
	 * @return bool
	 */
	bool deleteVehicleQueueCounter(QList<long> ids);

	/**
	 * @brief 删除行程时间检测器
	 * @param ids					[行程时间检测器ID]
	 *
	 * @return bool
	 */
	bool deleteVehicleTravelDetector(QList<long> ids);

	/**
	 * @brief 删除导向箭头
	 * @param ids					[导向箭头ID]
	 *
	 * @return bool
	 */
	bool deleteGuideArrow(QList<long> ids);

	/**
	 * @brief 删除限速区间隔
	 * @param ids					[限速区间隔ID]
	 *
	 * @return bool
	 */
	bool deleteReduceSpeedInterval(QList<long> ids);

	/**
	 * @brief 删除限速区车辆类型
	 * @param id					[限速区ID]
	 * @param vehicleTypeCodes		[车型编码]
	 *
	 * @return bool
	 */
	bool deleteReduceSpeedVehiType(long id, QList<long> vehicleTypeCodes);

	/**
	 * @brief 删除限速区
	 * @param ids					[限速区ID]
	 *
	 * @return bool
	 */
	bool deleteReduceSpeedArea(QList<long> ids);

	/**
	 * @brief 删除发车间隔
	 * @param id					[发车点ID]
	 * @param departureIntervalDs	[发车间隔ID]
	 *
	 * @return bool
	 */
	bool deleteDepartureInterval(long id, QList<long> departureIntervalDs);

	/**
	 * @brief 删除发车点
	 * @param ids					[发车点ID]
	 *
	 * @return bool
	 */
	bool deleteDeparturePoint(QList<long> ids);

	/**
	 * @brief 删除车型组成和占比
	 * @param id					[车型组成编号]
	 * @param vehicleTypeCodes		[车型编码]
	 *
	 * @return bool
	 */
	bool deleteVehicleConsDetail(long id, QList<long> vehicleTypeCodes);

	/**
	 * @brief 删除车型组成
	 * @param ids					[车型组成ID]
	 *
	 * @return bool
	 */
	bool deleteVehicleConstitutent(QList<long> ids);

	//-----------------------------------道路及连接---------------------------------
	/**
	 * @brief 删除路径流量分配
	 * @param ids					[路径流量分配ID]
	 *
	 * @return bool
	 */
	bool deleteRoutingFLowRatio(QList<long> ids);

	/**
	 * @brief 删除路径车道连接
	 * @param id					[路径ID]
	 * @param connID				[连接段ID]
	 * @param fromLaneId			[From车道ID]
	 * @param toLaneId				[To车道ID]
	 *
	 * @return bool
	 */
	bool deleteRoutingLaneConnector(long routingID, long connID, long fromLaneId, long toLaneId);

	/**
	 * @brief 删除路径
	 * @param ids					[路径ID]
	 *
	 * @return bool
	 */
	bool deleteRouting(QList<long> ids);

	/**
	 * @brief 删除决策点
	 * @param ids					[决策点ID]
	 *
	 * @return bool
	 */
	bool deleteDecisionPoint(QList<long> ids);

	/**
	 * @brief 删除车道连接
	 * @param connID				[连接段ID]
	 * @param fromLaneId			[From车道ID]
	 * @param toLaneId				[To车道ID]
	 *
	 * @return bool
	 */
	bool deleteLaneConnector(long connID, long fromLaneId, long toLaneId);

	/**
	 * @brief 删除连接段
	 * @param ids					[连接段ID]
	 *
	 * @return bool
	 */
	bool deleteConnectors(QList<long> ids);

	/**
	 * @brief 删除车道
	 * @param ids					[车道ID]
	 *
	 * @return bool
	 */
	bool deleteLane(QList<long> ids);

	/**
	 * @brief 删除路段
	 * @param ids					[路段ID]
	 *
	 * @return bool
	 */
	bool deleteLink(QList<long> ids);

private:
	friend class PH::SingleHolder<TessngDBTools>;
	TessngDBTools();

	/**
	 * @brief 检查这些路段里是否包含该车道
	 * @param hash					[路段对象序列]
	 * @param laneId				[待搜索车道ID]
	 *
	 * @return bool
	 */
	bool checkContansLane(const QList<GLink*>& hash, long laneId);

	/**
	 * @brief 检查这些连接段里是否包含该车道连接
	 * @param hash					[连接段对象序列]
	 * @param fromlaneId			[From车道ID]
	 * @param tolaneId				[To车道ID]
	 *
	 * @return bool
	 */
	bool checkContansLane(const QList<GConnector*>& hash, long fromlaneId, long tolaneId);

	/**
	 * @brief 检查这些连接段里是否包含该连接段
	 * @param hash					[连接段对象序列]
	 * @param id					[连接段ID]
	 *
	 * @return bool
	 */
	bool checkContainsConnector(const QList<GConnector*>& list, long id);
};

#endif // TESSNGDBTOOLS_H
