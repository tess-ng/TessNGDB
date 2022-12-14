#include "DBPluginGui.h"
#include "TessngDBNet.h"
#include "tessinterface.h"
#include "guiinterface.h"
#include <QMainWindow>
#include "TessngDBCopy.h"

#include "TESS_API_EXAMPLE.h"
#include <QDockWidget>
DBPluginGui::DBPluginGui(QObject *parent)
    : QObject{parent}
{

}
void DBPluginGui::init()
{
    initGui();
    m_ptrDBNet=std::make_shared<TessngDBNet>();
}

void DBPluginGui::unload()
{

}

CustomerGui* DBPluginGui::customerGui()
{
    return nullptr;
}

CustomerNet* DBPluginGui::customerNet()
{
    return m_ptrDBNet.get();
}

CustomerSimulator* DBPluginGui::customerSimulator()
{
    return nullptr;
}

void DBPluginGui::initGui()
{
    //在TESS NG主界面上增加 QDockWidget对象
    mpExampleWindow = new TESS_API_EXAMPLE();
    QDockWidget* pDockWidget = new QDockWidget(QString("自定义与TESS NG交互界面"), (QWidget*)gpTessInterface->guiInterface()->mainWindow());
    pDockWidget->setObjectName(QStringLiteral("mainDockWidget"));
    pDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    pDockWidget->setWidget(mpExampleWindow->centralWidget());
    gpTessInterface->guiInterface()->addDockWidgetToMainWindow(static_cast<Qt::DockWidgetArea>(1), pDockWidget);

    QAction* pCloase = gpTessInterface->guiInterface()->actionClose();
    QMenu* pMenu = gpTessInterface->guiInterface()->fileMenu();
    QMenu* xodrMenu = new QMenu(tr("数据库测试"), gpTessInterface->guiInterface()->mainWindow());
    pMenu->insertMenu(pCloase, xodrMenu);

    QAction* action = xodrMenu->addAction(tr("显示表"));
    QObject::connect(action, &QAction::triggered, [=]() {
        TessngDBCopy::getInstance()->test();
    });
}
