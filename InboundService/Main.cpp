#include "stdafx.h"
#include <QCoreApplication>

#include "InboundService.h"
#include "Utils/BootStrapper.h"
#include <QDebug>
#include <QStringList>
#include <QSqlDatabase>
#include <dlfcn.h>
#include "unitTests.h"

int main(int argc, char* argv[]) {
	QCoreApplication a(argc, argv);

//	//List of installed database drivers
//    cout << "Connecting to database..." << endl;
//    qDebug() << QSqlDatabase :: drivers();
    //Lets see if this makes it to the repo

    BootStrapper::InitDatabase();

    /* Enable this part to run the inbound service
    InboundService service;
    service.Init(); */

    //UtilBox::sleep(1);
	
	//service.StartInbound();
	//qDebug() << "Service started!" << endl;
	//service.Shutdown();
	
    // ************************************** //
    // Unit tests for db classes here
    unitTests();

    //BootStrapper::ShutdownDatabase();

	return a.exec();
}
