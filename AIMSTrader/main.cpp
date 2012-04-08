#include "Platform/Utils/SingleApplication.h"
#include "Platform/Startup/Service.h"
#include <stdio.h>
#include <iostream>
#include "Platform/Strategy/StrategyManager.h"
#include <QGridLayout>
#include "Platform/View/MainWindow.h"
#include "Platform/Utils/Timer.h"

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv,"IBTrader");

    //MainWindow* mainWindow = new MainWindow();//setMainWindow();

    //printf( "Setting up EventReporter\n");
    Service::Instance()->setEventReporter();
    Service::Instance()->startService();
    Service::Instance()->setMode(ForwardTest);


    //wait here to IB to get ready

    if(Service::Instance()->getTrader()->IsConnected())
    {
        //StrategyManager* manager = new StrategyManager();
        StrategyManager::manager()->launchStrategies();
        printf( "Starting Threads\n");
        ThreadManager::Instance()->startThreads();
        ThreadManager::Instance()->waitOnThreads();
    }

    return app.exec();
}

