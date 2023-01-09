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

                result = TessngDBToolsUpdate::getInstance()->updateSignalLamp(test);
                break;
            }
        }
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
        TessngDBToolsCopy::getInstance()->initSignalGroup(test);
        test.signalGroupID = id;
        test.name = "test";

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
        TessngDBToolsCopy::getInstance()->initBusLine(test);
        test.busLineID = id;
        test.name = "test";
        result = TessngDBToolsUpdate::getInstance()->updateBusLine(test);

        //BusStationLine* tempLine = new BusStationLine();
        //PassengerArriving* tempPA = new PassengerArriving();
        //foreach(GBusLine * it, gpScene->mlGBusLine) {
        //    if (it->id() == id) {
        //        //填充表单数据
        //        TessngDBToolsCopy::getInstance()->copyBusLine(test, it->mpBusLine);
        //        test.mlLink.clear();
        //        test.mlLinkId.clear();
        // 
        //        //修改表单数据
        //        test.passCountAtStartTime = it->mpBusLine->passCountAtStartTime + 10;
        //        //只修改第一个BusStationLine
        // 
        //        if (!it->mpBusLine->mlBusStationLine.isEmpty()) {
        //            test.mlBusStationLine.clear();
        //            TessngDBToolsCopy::getInstance()->copyBusStationLine(tempLine, it->mpBusLine->mlBusStationLine[0]);
        //            tempLine->parkingTime = it->mpBusLine->mlBusStationLine[0]->parkingTime + 10;
        //            test.mlBusStationLine.push_back(tempLine);
        // 
        //            //只修改第一个BusStationLine的第一个PassengerArriving
        //            if (!it->mpBusLine->mlBusStationLine[0]->mlPassengerArriving.isEmpty()) {
        //                test.mlBusStationLine[0]->mlPassengerArriving.clear();
        //                TessngDBToolsCopy::getInstance()->copyPassengerArriving(tempPA, it->mpBusLine->mlBusStationLine[0]->mlPassengerArriving[0]);
        //                tempPA->passengerCount = it->mpBusLine->mlBusStationLine[0]->mlPassengerArriving[0]->passengerCount + 5;
        //                test.mlBusStationLine[0]->mlPassengerArriving.push_back(tempPA);
        //            }
        //        }
        // 
        //        result = TessngDBToolsUpdate::getInstance()->updateBusLine(test);
        //        break;
        //    }
        //}
        // 
        //delete tempPA;
        //tempPA = NULL;
        //if (!test.mlBusStationLine.isEmpty()) {
        //    test.mlBusStationLine[0]->mlPassengerArriving.clear();
        //}
        // 
        //delete tempLine;
        //tempLine = NULL;
        //test.mlBusStationLine.clear();
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
        TessngDBToolsCopy::getInstance()->initBusStation(test);
        test.busStationID = id;
        test.name = "test";
        result = TessngDBToolsUpdate::getInstance()->updateBustation(test);

        /*
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

                result = TessngDBToolsUpdate::getInstance()->updateBustation(test);
                break;
            }
        }

        delete tempPA;
        tempPA = NULL;
        if (!test.mlBusStationLine.isEmpty()) {
            test.mlBusStationLine[0]->mlPassengerArriving.clear();
        }

        delete tempLine;
        tempLine = NULL;
        test.mlBusStationLine.clear();

        test.mpLink = NULL;*/
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
        TessngDBToolsCopy::getInstance()->initVehicleTravelDetector(test);
        test.detectorId = id;
        test.name = "test";
        result = TessngDBToolsUpdate::getInstance()->updateVehicleTravelDetector(test);

        /*foreach(GVehicleTravelDetector * it, gpScene->mlGVehicleTravelDetector) {
            if (it->mpVehicleTravelDetector->detectorId == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyVehicleTravelDetector(test, it->mpVehicleTravelDetector);

                //修改表单数据
                test.name = it->mpVehicleTravelDetector->name + "1";

                result = TessngDBToolsUpdate::getInstance()->updateVehicleTravelDetector(test);
                break;
            }
        }*/
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
        TessngDBToolsCopy::getInstance()->initVehicleQueueCounter(test);
        test.queueCounterID = id;
        test.name = "test";
        result = TessngDBToolsUpdate::getInstance()->updateVehicleQueueCounter(test);
        /*
        foreach(GVehicleQueueCounter * it, gpScene->mlGVehicleQueueCounter) {
            if (it->mpVehicleQueueCounter->queueCounterID == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyVehicleQueueCounter(test, it->mpVehicleQueueCounter);

                //修改表单数据
                test.name = it->mpVehicleQueueCounter->name + "1";

                result = TessngDBToolsUpdate::getInstance()->updateVehicleQueueCounter(test);
                break;
            }
        }*/
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
        TessngDBToolsCopy::getInstance()->initVehicleDrivInfoCollector(test);
        test.collecterID = id;
        test.name = "test";
        result = TessngDBToolsUpdate::getInstance()->updateVehicleDrivInfoCollector(test);
        /*
        foreach(GVehicleDrivInfoCollector * it, gpScene->mlGVehicleDrivInfoCollector) {
            if (it->id() == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyVehicleDrivInfoCollector(test, it->mpVehicleDrivInfoCollector);

                //修改表单数据
                test.name = it->mpVehicleDrivInfoCollector->name + "1";

                result = TessngDBToolsUpdate::getInstance()->updateVehicleDrivInfoCollector(test);
                break;
            }
        }*/
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
        TessngDBToolsCopy::getInstance()->initGuideArrow(test);
        test.guideArrowID = id;
        test.arrowType = GuideArrow::Left;
        result = TessngDBToolsUpdate::getInstance()->updateGuideArrow(test);
        /*
        foreach(GGuideArrow * it, gpScene->mlGGuideArrow) {
            if (it->mpGuideArrow->guideArrowID == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyGuideArrow(test, it->mpGuideArrow);

                //修改表单数据
                test.length = it->mpGuideArrow->length * 1.2;
                test.arrowType = it->mpGuideArrow->Left;

                result = TessngDBToolsUpdate::getInstance()->updateGuideArrow(test);
                break;
            }
        }*/
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
        TessngDBToolsCopy::getInstance()->initReduceSpeedArea(test);
        test.reduceSpeedAreaID = id;
        test.name = "test";
        result = TessngDBToolsUpdate::getInstance()->updateReduceSpeedArea(test);

        /*
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
        test.mlReduceSpeedVehiType.clear();*/
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
        TessngDBToolsCopy::getInstance()->initDeparturePoint(test);
        test.departurePointID = id;
        test.name = "test";
        result = TessngDBToolsUpdate::getInstance()->updateDeparturePoint(test);

        /*
        foreach(GDeparturePoint * it, gpScene->mlGDeparturePoint) {
            if (it->id() == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyDeparturePoint(test, it->mpDeparturePoint);

                //修改表单数据
                test.name = it->mpDeparturePoint->name + "1";

                result = TessngDBToolsUpdate::getInstance()->updateDeparturePoint(test);
                break;
            }
        }*/
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
        TessngDBToolsCopy::getInstance()->initRouting(test);
        test.routingID = id;
        test.routingName = "test";
        result = TessngDBToolsUpdate::getInstance()->updateRouteing(test);
        /*
        foreach(GRouting * it, gpScene->mlGRouting) {
            if (it->id() == id) {
                Routing* itRouting = it->getRouting();
                
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyRouting(test, itRouting);

                //修改表单数据
                test.mllLink.clear();
                test.routingName = itRouting->routingName + "1";

                result = TessngDBToolsUpdate::getInstance()->updateRouteing(test);

                break;
            }
        }*/
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
        TessngDBToolsCopy::getInstance()->initDecisionPoint(test);
        test.deciPointID = id;
        test.deciPointName = "test";
        result = TessngDBToolsUpdate::getInstance()->updateDecisionPoint(test);
        /*
        RoutingFlowByInterval* tempInterval = new RoutingFlowByInterval();
        RoutingFLowRatio* tempRatio = new RoutingFLowRatio();
        foreach(GDecisionPoint * it, gpScene->mlGDecisionPoint) {
            if (it->id() == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyDecisionPoint(test, it->mpDecisionPoint);

                //修改表单数据
                test.deciPointName = it->mpDecisionPoint->deciPointName + "1";
                test.mlRoutingFlowByInterval.clear();

                if (!it->mpDecisionPoint->mlRoutingFlowByInterval.isEmpty()) {
                    TessngDBToolsCopy::getInstance()->copyRoutingFlowByInterval(tempInterval, it->mpDecisionPoint->mlRoutingFlowByInterval[0]);
                    test.mlRoutingFlowByInterval.push_back(tempInterval);

                    tempInterval->mlRoutingFlowRatio.clear();
                    if (!it->mpDecisionPoint->mlRoutingFlowByInterval[0]->mlRoutingFlowRatio.isEmpty()) {
                        TessngDBToolsCopy::getInstance()->copyRoutingFLowRatio(tempRatio, it->mpDecisionPoint->mlRoutingFlowByInterval[0]->mlRoutingFlowRatio[0]);
                        tempRatio->ratio = tempRatio->ratio * 1.2;
                        test.mlRoutingFlowByInterval[0]->mlRoutingFlowRatio.push_back(tempRatio);
                    }
                }

                result = TessngDBToolsUpdate::getInstance()->updateDecisionPoint(test);

                break;
            }
        }
        delete tempRatio;
        tempRatio = NULL;
        if (!test.mlRoutingFlowByInterval.isEmpty()) {
            test.mlRoutingFlowByInterval[0]->mlRoutingFlowRatio.clear();
        }

        delete tempInterval;
        tempInterval = NULL;
        test.mlRoutingFlowByInterval.clear();*/
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
            TessngDBToolsCopy::getInstance()->initConnector(test);
            test.connID = id;
            test.connName = "test";
            result = TessngDBToolsUpdate::getInstance()->updateConnector(test);
            /*
            LaneConnector* tempLC = new LaneConnector(
                testG->mpConnector, 
                testG->mlGLaneConnector[0]->mpLaneConnector->mpFromLane, 
                testG->mlGLaneConnector[0]->mpLaneConnector->mpToLane);
            //填充表单数据
            TessngDBToolsCopy::getInstance()->copyConnector(test, testG->mpConnector);

            //修改表单数据
            test.mlLaneConnector.clear();
            test.color = "F56C6C";

            TessngDBToolsCopy::getInstance()->copyLaneConnector(tempLC, testG->mlGLaneConnector[0]->mpLaneConnector);
            tempLC->weight = tempLC->weight * 2;
            test.mlLaneConnector.push_back(tempLC);

            result = TessngDBToolsUpdate::getInstance()->updateConnector(test);

            delete tempLC;
            tempLC = NULL;
            test.mlLaneConnector.clear();*/
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
        TessngDBToolsCopy::getInstance()->initLane(test);
        test.laneID = id;
        test.width = 10;
        result = TessngDBToolsUpdate::getInstance()->updateLane(test);
        /*
        foreach(GLane * it, gpScene->mlGLane) {
            if (it->id() == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyLane(test, it->mpLane);

                //修改表单数据
                test.mlGuideArrow.clear();
                test.width = test.width * 1.2;

                result = TessngDBToolsUpdate::getInstance()->updateLane(test);
                break;
            }
        }*/
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
        TessngDBToolsCopy::getInstance()->initLink(test);
        test.linkID = id;
        test.linkName = "test";
        result = TessngDBToolsUpdate::getInstance()->updateLink(test);
        /*
        foreach(GLink * it, gpScene->mlGLink) {
            if (it->id() == id) {
                //填充表单数据
                TessngDBToolsCopy::getInstance()->copyLink(test, it->mpLink);

                //修改表单数据
                test.mlVertex.clear();
                test.mlLane.clear();
                test.length = test.length * 2.0;

                result = TessngDBToolsUpdate::getInstance()->updateLink(test);

                test.startNode = NULL;
                test.endNode = NULL;
                break;
            }
        }*/
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
