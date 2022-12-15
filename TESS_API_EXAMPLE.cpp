#include "TESS_API_EXAMPLE.h"
#include "TessngDBTools.h"

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
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteSignalLamp(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除信号灯", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除信号灯"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnPhaseGreen_released()
{
    long id = ui.inputID->text().toInt();
    QList<int> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteSignalColor(1, list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除相位颜色", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除相位颜色"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnPhase_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteSignalPhase(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除相位", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除相位"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnSignalGroup_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteSignalGroup(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除信号灯组", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除信号灯组"), "删除失败");
    }
}
//--------------------------------------公交------------------------------------
void TESS_API_EXAMPLE::on_btnPassengerArriving_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteBusStationPassengerArriving(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除乘客到站", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除乘客到站"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnBusStationLine_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteBusStationLine(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除公交站点-线路", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除公交站点-线路"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnBusLine_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteBusLine(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除公交线路", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除公交线路"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnBusStation_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteBusStation(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除公交站", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除公交站"), "删除失败");
    }
}

//---------------------------------车辆运行及检测-------------------------------
void TESS_API_EXAMPLE::on_btnVehicleTravelDetector_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteVehicleTravelDetector(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除行程时间检测器", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除行程时间检测器"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleQueueCounter_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteVehicleQueueCounter(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除排队计数器", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除排队计数器"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleDrivInfoCollecter_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteDrivInfoCollector(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除车辆信息采集器", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除车辆信息采集器"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnGuideArrow_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteGuideArrow(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除导向箭头", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除导向箭头"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnReduceSpeedInterval_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteReduceSpeedInterval(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除减速区时间间隔", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除减速区时间间隔"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnReduceSpeedVehiType_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteReduceSpeedVehiType(1, list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除减速区限速车型", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除减速区限速车型"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnReduceSpeedArea_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteReduceSpeedArea(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除减速区", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除减速区"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnDepartureInterval_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteDepartureInterval(1, list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除发车间隔", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除发车间隔"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnDeparturePoint_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteDeparturePoint(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除发车点", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除发车点"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleConsDetail_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteVehicleConsDetail(1, list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除车型组成与占比", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除车型组成与占比"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleConstitutent_released()
{
    long id = ui.inputID->text().toInt();
    QList<long> list;
    list.push_back(id);
    bool result = TessngDBTools::getInstance()->deleteVehicleConstitutent(list);
    if (result) {
        QMessageBox::information(gpTessInterface->guiInterface()->mainWindow(), "删除车型组成", "删除成功");
    }
    else {
        QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除车型组成"), "删除失败");
    }
}

void TESS_API_EXAMPLE::on_btnVehicleType_released()
{
    QMessageBox::warning(gpTessInterface->guiInterface()->mainWindow(), QString("删除车型"), "未开发");
}

//-----------------------------------道路及连接---------------------------------
void TESS_API_EXAMPLE::on_btnRouting_released()
{

}

void TESS_API_EXAMPLE::on_btnRoutingLaneConnector_released()
{

}

void TESS_API_EXAMPLE::on_btnRoutingFLowRatio_released()
{

}

void TESS_API_EXAMPLE::on_btnDecisionPoint_released()
{

}

void TESS_API_EXAMPLE::on_btnLink_released()
{

}

void TESS_API_EXAMPLE::on_btnLinkVertex_released()
{

}

void TESS_API_EXAMPLE::on_btnLane_released()
{

}

void TESS_API_EXAMPLE::on_btnConnector_released()
{

}

void TESS_API_EXAMPLE::on_btnLaneConnector_released()
{

}
