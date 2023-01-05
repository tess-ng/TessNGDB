#include "TESS_API_EXAMPLE.h"
#include "TessngDBToolsRemove.h"
#include "TessngDBToolsUpdate.h"
#include "TessngDBToolsCopy.h"

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
                TessngDBToolsCopy::getInstance()->copySignalLamp(test, it->mpSignalLamp);
                test.name = it->mpSignalLamp->name + "1";

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
        SignalPhase* tempSP = new SignalPhase();
        foreach(SignalGroup * it, gpScene->mlSignalGroup) {
            if (it->id() == id) {
                TessngDBToolsCopy::getInstance()->copySignalGroup(test, it);
                test.name = it->name + "1";

                test.mlPhase.clear();
                if (!it->mlPhase.isEmpty()) {
                    TessngDBToolsCopy::getInstance()->copySignalPhase(tempSP, it->mlPhase[0]);
                    tempSP->name = it->mlPhase[0]->name;
                    test.mlPhase.push_back(tempSP);
                }

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
        BusStationLine* tempLine = new BusStationLine();
        PassengerArriving* tempPA = new PassengerArriving();
        
        foreach(GBusLine * it, gpScene->mlGBusLine) {
            if (it->id() == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyBusLine(test, it->mpBusLine);
                test.mlLink.clear();
                test.mlLinkId.clear();

                //修改表单数据
                test.passCountAtStartTime = it->mpBusLine->passCountAtStartTime + 10;

                //只修改第一个BusStationLine
                if (!it->mpBusLine->mlBusStationLine.isEmpty()) {
                    test.mlBusStationLine.clear();
                    TessngDBToolsCopy::getInstance()->copyBusStationLine(tempLine, it->mpBusLine->mlBusStationLine[0]);
                    tempLine->parkingTime = it->mpBusLine->mlBusStationLine[0]->parkingTime + 10;
                    test.mlBusStationLine.push_back(tempLine);

                    //只修改第一个BusStationLine的第一个PassengerArriving
                    if (!it->mpBusLine->mlBusStationLine[0]->mlPassengerArriving.isEmpty()) {
                        test.mlBusStationLine[0]->mlPassengerArriving.clear();
                        TessngDBToolsCopy::getInstance()->copyPassengerArriving(tempPA, it->mpBusLine->mlBusStationLine[0]->mlPassengerArriving[0]);
                        tempPA->passengerCount = it->mpBusLine->mlBusStationLine[0]->mlPassengerArriving[0]->passengerCount + 5;
                        test.mlBusStationLine[0]->mlPassengerArriving.push_back(tempPA);
                    }
                }

                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateBusLine(test);

        delete tempPA;
        tempPA = NULL;
        if (!test.mlBusStationLine.isEmpty()) {
            test.mlBusStationLine[0]->mlPassengerArriving.clear();
        }
 
        delete tempLine;
        tempLine = NULL;
        test.mlBusStationLine.clear();
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
        BusStationLine* tempLine = new BusStationLine();
        PassengerArriving* tempPA = new PassengerArriving();
        
        foreach(GBusStation * it, gpScene->mlGBusStation) {
            if (it->id() == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyBusStation(test, it->mpBusStation);

                //修改表单数据
                test.length = it->mpBusStation->length * 2.0;

                //只修改第一个BusStationLine
                if (!it->mpBusStation->mlBusStationLine.isEmpty()) {
                    test.mlBusStationLine.clear();
                    TessngDBToolsCopy::getInstance()->copyBusStationLine(tempLine, it->mpBusStation->mlBusStationLine[0]);
                    tempLine->parkingTime = it->mpBusStation->mlBusStationLine[0]->parkingTime + 10;
                    test.mlBusStationLine.push_back(tempLine);

                    //只修改第一个BusStationLine的第一个PassengerArriving
                    if (!it->mpBusStation->mlBusStationLine[0]->mlPassengerArriving.isEmpty()) {
                        test.mlBusStationLine[0]->mlPassengerArriving.clear();
                        TessngDBToolsCopy::getInstance()->copyPassengerArriving(tempPA, it->mpBusStation->mlBusStationLine[0]->mlPassengerArriving[0]);
                        tempPA->passengerCount = it->mpBusStation->mlBusStationLine[0]->mlPassengerArriving[0]->passengerCount + 5;
                        test.mlBusStationLine[0]->mlPassengerArriving.push_back(tempPA);
                    }
                }

                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateBustation(test);

        delete tempPA;
        tempPA = NULL;
        if (!test.mlBusStationLine.isEmpty()) {
            test.mlBusStationLine[0]->mlPassengerArriving.clear();
        }

        delete tempLine;
        tempLine = NULL;
        test.mlBusStationLine.clear();

        test.mpLink = NULL;
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
        ReduceSpeedInterval* tempInterval = new ReduceSpeedInterval();
        ReduceSpeedVehiType* tempVehiType = new ReduceSpeedVehiType();
        foreach(GReduceSpeedArea * it, gpScene->mlGReduceSpeedArea) {
            if (it->mpReduceSpeedArea->reduceSpeedAreaID == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyReduceSpeedArea(test, it->mpReduceSpeedArea);
                test.mlReduceSpeedInterval.clear();
                test.mlReduceSpeedVehiType.clear();

                //修改表单数据
                test.areaLength = it->mpReduceSpeedArea->areaLength * 2.0;

                //copy并只修改第一个ReduceSpeedInterval
                if (!it->mpReduceSpeedArea->mlReduceSpeedInterval.isEmpty()) {
                    TessngDBToolsCopy::getInstance()->copyReduceSpeedInterval(tempInterval, it->mpReduceSpeedArea->mlReduceSpeedInterval[0]);
                    tempInterval->startTime = it->mpReduceSpeedArea->mlReduceSpeedInterval[0]->startTime + 10;
                    test.mlReduceSpeedInterval.push_back(tempInterval);
                }
                //copy并只修改第一个ReduceSpeedVehiType
                if (!it->mpReduceSpeedArea->mlReduceSpeedVehiType.isEmpty()) {
                    TessngDBToolsCopy::getInstance()->copyReduceSpeedVehiType(tempVehiType, it->mpReduceSpeedArea->mlReduceSpeedVehiType[0]);
                    tempVehiType->avgSpeed = it->mpReduceSpeedArea->mlReduceSpeedVehiType[0]->avgSpeed * 2.0;
                    test.mlReduceSpeedVehiType.push_back(tempVehiType);
                }
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateReduceSpeedArea(test);

        delete tempInterval;
        tempInterval = NULL;
        test.mlReduceSpeedInterval.clear();

        delete tempVehiType;
        tempVehiType = NULL;
        test.mlReduceSpeedVehiType.clear();
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
