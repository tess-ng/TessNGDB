#include "TESS_API_EXAMPLE.h"
#include "TessngDBToolsRemove.h"
#include "TessngDBToolsUpdate.h"

#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

#include "tessinterface.h"
#include "netinterface.h"
#include "simuinterface.h"
#include "guiinterface.h"

#include "gsignallamp.h"
#include "signallamp.h"
#include "signalphase.h"
#include "signalGroup.h"
#include "ISignalPhase.h"

#include "PassengerArriving.h"
#include "BusStationLine.h"
#include "GBusLine.h"
#include "GBusStation.h"

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

TESS_API_EXAMPLE::TESS_API_EXAMPLE(QWidget *parent): QMainWindow(parent){
    ui.setupUi(this);
    /*QObject::connect(ui.btnOpenNet, &QPushButton::clicked, [this]() {
        openNet();
    });*/
}
//------------------------------------信号控制----------------------------------
void TESS_API_EXAMPLE::on_btnSignalLamp_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;
    
    if (bInsert) {

    }
    else if (bUpdate) {
        SignalLamp test;
        foreach(GSignalLamp* it, gpScene->mlGSignalLamp) {
            if (it->id() == id) {
                test.signalLampID = it->mpSignalLamp->signalLampID;
                test.name = it->mpSignalLamp->name;
                test.mpSignalPhase = it->mpSignalLamp->mpSignalPhase;
                test.laneID = it->mpSignalLamp->laneID;
                test.toLaneID = it->mpSignalLamp->toLaneID;
                test.x = it->mpSignalLamp->x;
                test.y = it->mpSignalLamp->y;
                test.z = it->mpSignalLamp->z;
                break;
            }
        }

        result = TessngDBToolsUpdate::getInstance()->updateSignalLamp(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteSignalLamp(list);
    }
    
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "信号灯", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("信号灯"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnPhaseGreen_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<int> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteSignalColor(1, list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "相位颜色", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("相位颜色"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnPhase_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteSignalPhase(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "相位", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("相位"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnSignalGroup_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        SignalGroup test;
        foreach(SignalGroup * it, gpScene->mlSignalGroup) {
            if (it->id() == id) {
                test.signalGroupID = it->signalGroupID;
                test.name = it->name;
                test.timeLength = it->timeLength;
                test.startTime = it->startTime;
                test.endTime = it->endTime;
                test.mlPhase = it->mlPhase;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateSignalGroup(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteSignalGroup(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "信号灯组", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("信号灯组"), "失败");
    }
}
//--------------------------------------公交------------------------------------
void TESS_API_EXAMPLE::on_btnPassengerArriving_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteBusStationPassengerArriving(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "乘客到站", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("乘客到站"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnBusStationLine_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteBusStationLine(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "公交站点-线路", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("公交站点-线路"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnBusLine_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        BusLine test;
        foreach(GBusLine * it, gpScene->mlGBusLine) {
            if (it->id() == id) {
                test.busLineID = it->mpBusLine->busLineID;
                test.name = it->mpBusLine->name;
                test.length = it->mpBusLine->length;
                test.dischargeFreq = it->mpBusLine->dischargeFreq;
                test.dischargeStartTime = it->mpBusLine->dischargeStartTime;
                test.dischargeEndTime = it->mpBusLine->dischargeEndTime;
                test.startX = it->mpBusLine->startX;
                test.startY = it->mpBusLine->startY;
                test.endX = it->mpBusLine->endX;
                test.endY = it->mpBusLine->endY;
                test.desirSpeed = it->mpBusLine->desirSpeed;
                test.speedSD = it->mpBusLine->speedSD;
                test.passCountAtStartTime = it->mpBusLine->passCountAtStartTime;
                test.mlLink = it->mpBusLine->mlLink;
                test.mlLinkId = it->mpBusLine->mlLinkId;
                test.mlBusStationLine = it->mpBusLine->mlBusStationLine;
                test.mSpecialApp = it->mpBusLine->mSpecialApp;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateBusLine(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteBusLine(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "公交线路", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("公交线路"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnBusStation_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        BusStation test;
        foreach(GBusStation * it, gpScene->mlGBusStation) {
            if (it->id() == id) {
                test.busStationID = it->mpBusStation->busStationID;
                test.name = it->mpBusStation->name;
                test.laneNumber = it->mpBusStation->laneNumber;
                test.x = it->mpBusStation->x;
                test.y = it->mpBusStation->y;
                test.length = it->mpBusStation->length;
                test.type = it->mpBusStation->type;
                test.mlBusStationLine = it->mpBusStation->mlBusStationLine;
                test.mpLink = it->mpBusStation->mpLink;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateBustation(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteBusStation(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "公交站", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("公交站"), "失败");
    }
}

//---------------------------------车辆运行及检测-------------------------------
void TESS_API_EXAMPLE::on_btnVehicleTravelDetector_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        VehicleTravelDetector test;
        foreach(GVehicleTravelDetector * it, gpScene->mlGVehicleTravelDetector) {
            if (it->mpVehicleTravelDetector->detectorId == id) {
                test.detectorId = it->mpVehicleTravelDetector->detectorId;
                test.name = it->mpVehicleTravelDetector->name;
                test.startRoadId = it->mpVehicleTravelDetector->startRoadId;
                test.start_laneNumber = it->mpVehicleTravelDetector->start_laneNumber;
                test.start_toLaneNumber = it->mpVehicleTravelDetector->start_toLaneNumber;
                test.startDist = it->mpVehicleTravelDetector->startDist;
                test.startX = it->mpVehicleTravelDetector->startX;
                test.startY = it->mpVehicleTravelDetector->startY;
                test.teminalRoadId = it->mpVehicleTravelDetector->teminalRoadId;
                test.teminal_laneNumber = it->mpVehicleTravelDetector->teminal_laneNumber;
                test.teminal_toLaneNumber = it->mpVehicleTravelDetector->teminal_toLaneNumber;
                test.teminalDist = it->mpVehicleTravelDetector->teminalDist;
                test.teminalX = it->mpVehicleTravelDetector->teminalX;
                test.teminalY = it->mpVehicleTravelDetector->teminalY;
                test.startTime = it->mpVehicleTravelDetector->startTime;
                test.endTime = it->mpVehicleTravelDetector->endTime;
                test.dataInterval = it->mpVehicleTravelDetector->dataInterval;
                test.shortestDistance = it->mpVehicleTravelDetector->shortestDistance;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateVehicleTravelDetector(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteVehicleTravelDetector(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "行程时间检测器", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("行程时间检测器"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleQueueCounter_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        VehicleQueueCounter test;
        foreach(GVehicleQueueCounter * it, gpScene->mlGVehicleQueueCounter) {
            if (it->mpVehicleQueueCounter->queueCounterID == id) {
                test.queueCounterID = it->mpVehicleQueueCounter->queueCounterID;
                test.name = it->mpVehicleQueueCounter->name;
                test.roadID = it->mpVehicleQueueCounter->roadID;
                test.laneNumber = it->mpVehicleQueueCounter->laneNumber;
                test.toLaneNumber = it->mpVehicleQueueCounter->toLaneNumber;
                test.x = it->mpVehicleQueueCounter->x;
                test.y = it->mpVehicleQueueCounter->y;
                test.z = it->mpVehicleQueueCounter->z;
                test.speedLowLimit = it->mpVehicleQueueCounter->speedLowLimit;
                test.speedUpLimit = it->mpVehicleQueueCounter->speedUpLimit;
                test.maxDistInterval = it->mpVehicleQueueCounter->maxDistInterval;
                test.maxQueueLength = it->mpVehicleQueueCounter->maxQueueLength;
                test.distance = it->mpVehicleQueueCounter->distance;
                test.startTime = it->mpVehicleQueueCounter->startTime;
                test.endTime = it->mpVehicleQueueCounter->endTime;
                test.dataInterval = it->mpVehicleQueueCounter->dataInterval;
                test.countInterval = it->mpVehicleQueueCounter->countInterval;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateVehicleQueueCounter(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteVehicleQueueCounter(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "排队计数器", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("排队计数器"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleDrivInfoCollecter_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        VehicleDrivInfoCollector test;
        foreach(GVehicleDrivInfoCollector * it, gpScene->mlGVehicleDrivInfoCollector) {
            if (it->id() == id) {
                test.collecterID = it->mpVehicleDrivInfoCollector->collecterID;
                test.name = it->mpVehicleDrivInfoCollector->name;
                test.roadID = it->mpVehicleDrivInfoCollector->roadID;
                test.laneNumber = it->mpVehicleDrivInfoCollector->laneNumber;
                test.toLaneNumber = it->mpVehicleDrivInfoCollector->toLaneNumber;
                test.distance = it->mpVehicleDrivInfoCollector->distance;
                test.x = it->mpVehicleDrivInfoCollector->x;
                test.y = it->mpVehicleDrivInfoCollector->y;
                test.z = it->mpVehicleDrivInfoCollector->z;
                test.startTime = it->mpVehicleDrivInfoCollector->startTime;
                test.endTime = it->mpVehicleDrivInfoCollector->endTime;
                test.dataInterval = it->mpVehicleDrivInfoCollector->dataInterval;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateVehicleDrivInfoCollector(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteDrivInfoCollector(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "车辆信息采集器", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("车辆信息采集器"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnGuideArrow_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        GuideArrow test;
        foreach(GGuideArrow * it, gpScene->mlGGuideArrow) {
            if (it->mpGuideArrow->guideArrowID == id) {
                test.guideArrowID = it->mpGuideArrow->guideArrowID;
                test.laneID = it->mpGuideArrow->laneID;
                test.length = it->mpGuideArrow->length;
                test.distToTerminal = it->mpGuideArrow->distToTerminal;
                test.arrowType = it->mpGuideArrow->arrowType;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateGuideArrow(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteGuideArrow(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "导向箭头", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("导向箭头"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnReduceSpeedInterval_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteReduceSpeedInterval(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "减速区时间间隔", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("减速区时间间隔"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnReduceSpeedVehiType_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteReduceSpeedVehiType(1, list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "减速区限速车型", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("减速区限速车型"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnReduceSpeedArea_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        ReduceSpeedArea test;
        foreach(GReduceSpeedArea * it, gpScene->mlGReduceSpeedArea) {
            if (it->mpReduceSpeedArea->reduceSpeedAreaID == id) {
                test.reduceSpeedAreaID = it->mpReduceSpeedArea->reduceSpeedAreaID;
                test.name = it->mpReduceSpeedArea->name;
                test.location = it->mpReduceSpeedArea->location;
                test.areaLength = it->mpReduceSpeedArea->areaLength;
                test.roadID = it->mpReduceSpeedArea->roadID;
                test.laneNumber = it->mpReduceSpeedArea->laneNumber;
                test.toLaneNumber = it->mpReduceSpeedArea->toLaneNumber;
                test.mlReduceSpeedInterval = it->mpReduceSpeedArea->mlReduceSpeedInterval;
                test.mlReduceSpeedVehiType = it->mpReduceSpeedArea->mlReduceSpeedVehiType;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateReduceSpeedArea(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteReduceSpeedArea(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "减速区", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("减速区"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnDepartureInterval_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteDepartureInterval(1, list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "发车间隔", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("发车间隔"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnDeparturePoint_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        DeparturePoint test;
        foreach(GDeparturePoint * it, gpScene->mlGDeparturePoint) {
            if (it->id() == id) {
                test.departurePointID = it->mpDeparturePoint->departurePointID;
                test.name = it->mpDeparturePoint->name;
                test.mpLink = it->mpDeparturePoint->mpLink;
                test.mlDepaInterval = it->mpDeparturePoint->mlDepaInterval;
                test.mlDepaPossion = it->mpDeparturePoint->mlDepaPossion;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateDeparturePoint(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteDeparturePoint(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "发车点", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("发车点"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleConsDetail_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteVehicleConsDetail(1, list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "车型组成与占比", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("车型组成与占比"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleConstitutent_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteVehicleConstitutent(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "车型组成", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("车型组成"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleType_released()
{
    QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("车型"), "未开发");
}

//-----------------------------------道路及连接---------------------------------
void TESS_API_EXAMPLE::on_btnRoutingFLowRatio_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteRoutingFLowRatio(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "路径流量分配", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("路径流量分配"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnRoutingLaneConnector_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteRoutingLaneConnector(1, 5, 13, 7);//1,5,13,7
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "路径车道连接", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("路径车道连接"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnRouting_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        Routing test;
        foreach(GRouting * it, gpScene->mlGRouting) {
            if (it->id() == id) {
                Routing* itRouting = it->getRouting();
                test.routingID = itRouting->routingID;
                test.routingName = itRouting->routingName;
                test.proportion = itRouting->proportion;
                test.mpDecisionPoint = itRouting->mpDecisionPoint;
                test.mllLink = itRouting->mllLink;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateRouteing(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteRouting(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "路径", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("路径"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnDecisionPoint_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
        DecisionPoint test;
        foreach(GDecisionPoint * it, gpScene->mlGDecisionPoint) {
            if (it->id() == id) {
                test.deciPointID = it->mpDecisionPoint->deciPointID;
                test.deciPointName = it->mpDecisionPoint->deciPointName;
                test.X = it->mpDecisionPoint->X;
                test.Y = it->mpDecisionPoint->Y;
                test.Z = it->mpDecisionPoint->Z;
                test.mFromPoint = it->mpDecisionPoint->mFromPoint;
                test.mToPoint = it->mpDecisionPoint->mToPoint;
                test.mlRouting = it->mpDecisionPoint->mlRouting;
                test.mpLink = it->mpDecisionPoint->mpLink;
                test.mlRoutingFlowByInterval = it->mpDecisionPoint->mlRoutingFlowByInterval;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateDecisionPoint(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteDecisionPoint(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "发车点", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("发车点"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnLaneConnector_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        result = TessngDBToolsRemove::getInstance()->deleteLaneConnector(2, 4, 7);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "车道连接", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("车道连接"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnConnector_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        GConnector* testG = NULL;
        foreach(GConnector * it, gpScene->mlGConnector) {
            if (it->id() == id) {
                testG = it;
                break;
            }
        }
        if (testG != NULL) {
            Connector test(testG->id(), testG->mpConnector->mpFromLink, testG->mpConnector->mpToLink);
            test.roadId = testG->mpConnector->roadId;
            test.connName = testG->mpConnector->connName;
            test.length = testG->mpConnector->length;
            test.curvature = testG->mpConnector->curvature;
            test.nonLinearCoefficient = testG->mpConnector->nonLinearCoefficient;
            test.color = testG->mpConnector->color;
            test.desiredSpeed = testG->mpConnector->desiredSpeed;
            test.limitSpeed = testG->mpConnector->limitSpeed;
            test.leftBreakPointsJson = testG->mpConnector->leftBreakPointsJson;
            test.rightBreakPointsJson = testG->mpConnector->rightBreakPointsJson;
            test.otherAttrsJson = testG->mpConnector->otherAttrsJson;
            test.mpFromLink = testG->mpConnector->mpFromLink;
            test.mpToLink = testG->mpConnector->mpToLink;
            test.mlLaneConnector = testG->mpConnector->mlLaneConnector;
            test.mlVehicleType = testG->mpConnector->mlVehicleType;
            result = TessngDBToolsUpdate::getInstance()->updateConnector(test);
        }
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteConnectors(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "连接段", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("连接段"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnLane_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {

    }
    else if (bUpdate) {
        Lane test;
        foreach(GLane * it, gpScene->mlGLane) {
            if (it->id() == id) {
                test.laneID = it->mpLane->laneID;
                test.linkID = it->mpLane->linkID;
                test.serialNumber = it->mpLane->serialNumber;
                test.width = it->mpLane->width;
                test.expectTravelDirection = it->mpLane->expectTravelDirection;
                test.actionType = it->mpLane->actionType;
                test.centerLinePointsJson = it->mpLane->centerLinePointsJson;
                test.leftBreakPointsJson = it->mpLane->leftBreakPointsJson;
                test.rightBreakPointsJson = it->mpLane->rightBreakPointsJson;
                test.otherAttrsJson = it->mpLane->otherAttrsJson;
                test.mlGuideArrow = it->mpLane->mlGuideArrow;
                test.mlVehicleType = it->mpLane->mlVehicleType;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateLane(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteLane(list);
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "车道", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("车道"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnLinkVertex_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
    }
    else if (bDelete) {
        result = TessngDBToolsRemove::getInstance()->deleteLinkVertex(id, { 1 });
    }

    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "顶点", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("顶点"), "失败");
    }
}

void TESS_API_EXAMPLE::on_btnLink_released()
{
    long id = ui.inputID->text().toInt();
    bool bInsert = ui.radioButtonInsert->isChecked();
    bool bUpdate = ui.radioButtonUpdate->isChecked();
    bool bDelete = ui.radioButtonDelete->isChecked();

    bool result = false;

    if (bInsert) {
    }
    else if (bUpdate) {
        Link test;
        foreach(GLink * it, gpScene->mlGLink) {
            if (it->id() == id) {
                test.linkID = it->mpLink->linkID;
                test.linkName = it->mpLink->linkName;
                test.netId = it->mpLink->netId;
                test.roadId = it->mpLink->roadId;
                test.laneNumber = it->mpLink->laneNumber;
                test.laneWidth = it->mpLink->laneWidth;
                test.laneColor = it->mpLink->laneColor;
                test.linkType = it->mpLink->linkType;
                test.length = it->mpLink->length;
                test.curvature = it->mpLink->curvature;
                test.nonLinearCoefficient = it->mpLink->nonLinearCoefficient;
                test.linkSaturationFlow = it->mpLink->linkSaturationFlow;
                test.linkTrafficFlow = it->mpLink->linkTrafficFlow;
                test.desiredSpeed = it->mpLink->desiredSpeed;
                test.limitSpeed = it->mpLink->limitSpeed;
                test.minSpeed = it->mpLink->minSpeed;
                test.addValue = it->mpLink->addValue;
                test.centerLinePointsJson = it->mpLink->centerLinePointsJson;
                test.leftBreakPointsJson = it->mpLink->leftBreakPointsJson;
                test.rightBreakPointsJson = it->mpLink->rightBreakPointsJson;
                test.otherAttrsJson = it->mpLink->otherAttrsJson;
                test.startNode = it->mpLink->startNode;
                test.endNode = it->mpLink->endNode;
                test.mlVertex = it->mpLink->mlVertex;
                test.mlLane = it->mpLink->mlLane;
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateLink(test);
    }
    else if (bDelete) {
        QList<long> list;
        list.push_back(id);
        result = TessngDBToolsRemove::getInstance()->deleteLink(list);
    }
    
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "路段", "成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("路段"), "失败");
    }
}
