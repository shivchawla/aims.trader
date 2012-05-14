#include "stdafx.h"
#include <QCoreApplication>

#include "InboundService.h"
#include "BootStrapper.h"
#include <QDebug>
#include <QStringList>
#include <QSqlDatabase>
#include <dlfcn.h>
#include <QPluginLoader>

int main(int argc, char* argv[]) {
	QCoreApplication a(argc, argv);


    /*void *handle1 = dlopen("/Users/shivkumarchawla/QtSDK/Desktop/Qt/4.8.1/gcc/plugins/sqldrivers/libqsqlmysql_debug.dylib", RTLD_NOW);

    if(!handle1)
    {
        qDebug()<<dlerror();
    }

    void *handle2 = dlopen("/usr/local/mysql/lib/libmysqlclient.16.dylib", RTLD_NOW);

    if(!handle2)
    {
        qDebug()<<dlerror();
    }

    QPluginLoader* loader =  new QPluginLoader("libqsqlmysql_debug");
    //loader->
    //List of installed database drivers
    cout << "Connecting to database..." << endl;
    qDebug() << QSqlDatabase :: drivers();

    foreach (const QString &path, a.libraryPaths())
         qDebug()<<path;

    */

    BootStrapper::InitDatabase();

    InboundService service;
	service.Init();

    //UtilBox::sleep(1);
	
	//service.StartInbound();
	//qDebug() << "Service started!" << endl;
	//service.Shutdown();
	
	
	//BootStrapper::ShutdownDatabase();

	return a.exec();
}
