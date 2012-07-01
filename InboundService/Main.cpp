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

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

    InboundService service;
    service.StartInbound();

    return a.exec();
}
