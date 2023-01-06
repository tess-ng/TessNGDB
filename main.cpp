#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QLibrary>
#include <QTextCodec>
#include <QMainWindow>
#include "DBPluginGui.h"
#include <QException>


int main(int argc, char *argv[])
{
    const char* pAppFilePath = "C:\\TESSNG_2.1.0\\TESSNG.exe";
    QTextCodec* pLocalCode = QTextCodec::codecForLocale();
    QTextCodec* pUtf8 = QTextCodec::codecForName("UTF-8");
    QString tmpString = pLocalCode->toUnicode(pAppFilePath);
    QString appFilePath = pUtf8->fromUnicode(tmpString);
    appFilePath = QString(pAppFilePath);
    appFilePath.replace('\\', '/');
    QFileInfo appFileInfo = QFileInfo(appFilePath);
    QDir appDir = appFileInfo.dir();
    QString pluginDir = appDir.path() + "/plugins";
    QCoreApplication::addLibraryPath(pluginDir);

    QApplication a(argc, argv);
    a.setFont(QFont("新宋体 ", 12, 2));
    int result = -1;

    QDir::setCurrent(a.applicationDirPath());
    QMainWindow* pWindow = tessng();
    if (pWindow) {
        DBPluginGui* ptrGui = new DBPluginGui;
        ptrGui->init();
        gpTessInterface->loadPluginFromMem(ptrGui);
        pWindow->showMaximized();
        result = a.exec();
        pWindow->deleteLater();
    }
    else {
        result = a.exec();
    }
    return result;
}
