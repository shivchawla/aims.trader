#include "stdafx.h"
#include <QCoreApplication>

#include "InboundService.h"
#include "Utils/BootStrapper.h"
#include <QDebug>
#include <QStringList>
#include <QSqlDatabase>
//#include <dlfcn.h>
#include <QSqlDriver>
#include <QPluginLoader>
#include <myglobal.h>
#include "Utils/ThreadManager.h"
#include "utils/Log.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);
    Logger::logFile()->open(QIODevice::Append | QIODevice::Text);

    InboundService service;
    service.StartInbound();
    return a.exec();
}
