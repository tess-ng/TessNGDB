#ifndef DBPLUGINGUI_H
#define DBPLUGINGUI_H
#include "tessplugin.h"
#include <QObject>
class TessngDBNet;
class TESS_API_EXAMPLE;
class DBPluginGui : public QObject,public TessPlugin
{
    Q_OBJECT
public:
    explicit DBPluginGui(QObject *parent = nullptr);

    void init() override;

    void unload() override;

    CustomerGui *customerGui() override;

    CustomerNet *customerNet() override;

    CustomerSimulator *customerSimulator() override;
private:
    void initGui();
    TESS_API_EXAMPLE* mpExampleWindow;
    std::shared_ptr<TessngDBNet> m_ptrDBNet;
};

#endif // DBPLUGINGUI_H
