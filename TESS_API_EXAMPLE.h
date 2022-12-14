#ifndef __TESS_API_EXAMPLE__
#define __TESS_API_EXAMPLE__

#include <QtWidgets/QMainWindow>
#include "ui_TESS_API_EXAMPLE.h"
class TESS_API_EXAMPLE : public QMainWindow
{
    Q_OBJECT

public:
    TESS_API_EXAMPLE(QWidget *parent = Q_NULLPTR);
private slots:
    //------------------------------------信号控制----------------------------------
    void on_btnSignalLamp_released();

    void on_btnPhaseGreen_released();

    void on_btnPhase_released();

    void on_btnSignalGroup_released();
    //--------------------------------------公交------------------------------------
    void on_btnBusLine_released();

    void on_btnBusStation_released();

    void on_btnBusStationLine_released();

    void on_btnPassengerArriving_released();
    //---------------------------------车辆运行及检测-------------------------------
    void on_btnVehicleTravelDetector_released();

    void on_btnVehicleQueueCounter_released();

    void on_btnVehicleDrivInfoCollecter_released();

    void on_btnGuideArrow_released();

    void on_btnReduceSpeedArea_released();

    void on_btnDeparturePoint_released();

    void on_btnVehicleConstitutent_released();

    void on_btnVehicleType_released();
    //-----------------------------------道路及连接---------------------------------
    void on_btnRouting_released();

    void on_btnRoutingLaneConnector_released();

    void on_btnRoutingFLowRatio_released();

    void on_btnDecisionPoint_released();

    void on_btnLink_released();

    void on_btnLinkVertex_released();

    void on_btnLane_released();

    void on_btnConnector_released();

    void on_btnLaneConnector_released();
public:
    Ui::TESS_API_EXAMPLEClass ui;
};



#endif
