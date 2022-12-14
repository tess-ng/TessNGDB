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

}

void TESS_API_EXAMPLE::on_btnVehicleQueueCounter_released()
{

}

void TESS_API_EXAMPLE::on_btnVehicleDrivInfoCollecter_released()
{

}

void TESS_API_EXAMPLE::on_btnGuideArrow_released()
{

}

void TESS_API_EXAMPLE::on_btnReduceSpeedArea_released()
{

}

void TESS_API_EXAMPLE::on_btnDeparturePoint_released()
{

}

void TESS_API_EXAMPLE::on_btnVehicleConstitutent_released()
{

}

void TESS_API_EXAMPLE::on_btnVehicleType_released()
{

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
