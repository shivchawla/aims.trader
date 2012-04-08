#include "Platform/Utils/SingleApplication.h"
#include "Platform/Startup/Service.h"
#include <stdio.h>
#include <iostream>
#include "Platform/Strategy/StrategyManager.h"
#include "Platform/View/InstrumentView.h"
#include "Platform/View/StrategyView.h"
#include <QGridLayout>
#include "Platform/View/MainWindow.h"

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv,"IBTrader");

    MainWindow::mainWindow();

    printf( "Starting Services\n");
    Service::Instance()->startService();
    printf("\nMain Thread \t");
    std::cout<<QThread::currentThreadId();
    Service::Instance()->setMode(ForwardTest);


    if(Service::Instance()->getTrader()->IsConnected())
    {
        StrategyManager* manager = new StrategyManager();
        manager->launchStrategies();
        printf( "Starting Threads\n");
        ThreadManager::Instance()->startThreads();
        ThreadManager::Instance()->waitOnThreads();
    }

    //QGridLayout gridLayout;
    //InstrumentView* iv = new InstrumentView();
    //StrategyView* sv = new StrategyView();

    //iv->show();
    //sv->show();
    /*gridLayout.addWidget(iv,0,0);
    gridLayout.addWidget(sv,1,0);

    QWidget mainWindow;

    */
    return app.exec();
}

