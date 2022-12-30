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
        QList<GSignalLamp*> rmGSignalLamp;
        foreach(GSignalLamp * it, gpScene->mlGSignalLamp) {
            if (it->id() == id) {
                rmGSignalLamp.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateSignalLamps(rmGSignalLamp);
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
        QList<SignalGroup*> rmSignalGroup;
        foreach(SignalGroup * it, gpScene->mlSignalGroup) {
            if (it->id() == id) {
                rmSignalGroup.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateSignalGroups(rmSignalGroup);
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
        QList<GBusLine*> rmGBusLine;
        foreach(GBusLine * it, gpScene->mlGBusLine) {
            if (it->id() == id) {
                rmGBusLine.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateBusLine(rmGBusLine);
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
        QList<GBusStation*> rmGBusStation;
        foreach(GBusStation * it, gpScene->mlGBusStation) {
            if (it->id() == id) {
                rmGBusStation.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateBustation(rmGBusStation);
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
        QList<GVehicleTravelDetector*> rmGVehicleTravelDetector;
        foreach(GVehicleTravelDetector * it, gpScene->mlGVehicleTravelDetector) {
            if (it->mpVehicleTravelDetector->detectorId == id) {
                rmGVehicleTravelDetector.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateVehicleTravelDetectors(rmGVehicleTravelDetector);
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
        QList<GVehicleQueueCounter*> rmGVehicleQueueCounter;
        foreach(GVehicleQueueCounter * it, gpScene->mlGVehicleQueueCounter) {
            if (it->mpVehicleQueueCounter->queueCounterID == id) {
                rmGVehicleQueueCounter.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateVehicleQueueCounters(rmGVehicleQueueCounter);
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
        QList<GVehicleDrivInfoCollector*> rmGVehicleDrivInfoCollector;
        foreach(GVehicleDrivInfoCollector * it, gpScene->mlGVehicleDrivInfoCollector) {
            if (it->id() == id) {
                rmGVehicleDrivInfoCollector.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateVehicleDrivInfoCollectors(rmGVehicleDrivInfoCollector);
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
        QList<GGuideArrow*> rmGGuideArrow;
        foreach(GGuideArrow * it, gpScene->mlGGuideArrow) {
            if (it->mpGuideArrow->guideArrowID == id) {
                rmGGuideArrow.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateGuideArrows(rmGGuideArrow);
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
        QList<GReduceSpeedArea*> rmGReduceSpeedArea;
        foreach(GReduceSpeedArea * it, gpScene->mlGReduceSpeedArea) {
            if (it->mpReduceSpeedArea->reduceSpeedAreaID == id) {
                rmGReduceSpeedArea.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateReduceSpeedAreas(rmGReduceSpeedArea);
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
        QList<GDeparturePoint*> rmGDeparturePoint;
        foreach(GDeparturePoint * it, gpScene->mlGDeparturePoint) {
            if (it->id() == id) {
                rmGDeparturePoint.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateDeparturePoint(rmGDeparturePoint);
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
        QList<GRouting*> rmGRouting;
        foreach(GRouting * it, gpScene->mlGRouting) {
            if (it->id() == id) {
                rmGRouting.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateRouteings(rmGRouting);
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
        QList<GDecisionPoint*> rmGDecisionPoint;
        foreach(GDecisionPoint * it, gpScene->mlGDecisionPoint) {
            if (it->id() == id) {
                rmGDecisionPoint.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateDecisionPoint(rmGDecisionPoint);
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
        QList<GConnector*> rmGConnector;
        foreach(GConnector * it, gpScene->mlGConnector) {
            if (it->id() == id) {
                rmGConnector.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateConnectors(rmGConnector);
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
        QList<GLane*> rmGLane;
        foreach(GLane * it, gpScene->mlGLane) {
            if (it->id() == id) {
                rmGLane.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateLanes(rmGLane);
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
        QList<GLink*> udGLink;
        foreach(GLink * it, gpScene->mlGLink) {
            if (it->id() == id) {
                udGLink.push_back(it);
                break;
            }
        }
        result = TessngDBToolsUpdate::getInstance()->updateLinks(udGLink);
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
