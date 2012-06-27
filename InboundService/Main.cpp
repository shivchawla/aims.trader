#include "stdafx.h"
#include <QCoreApplication>

#include "InboundService.h"
#include "Utils/BootStrapper.h"
#include <QDebug>
#include <QStringList>
#include <QSqlDatabase>
//#include <dlfcn.h>


int main(int argc, char* argv[]) {
	QCoreApplication a(argc, argv);

//	//List of installed database drivers
//    cout << "Connecting to database..." << endl;
//    qDebug() << QSqlDatabase :: drivers();
    //Lets see if this makes it to the repo

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
