#include "Platform/Utils/SingleApplication.h"
#include "MainWindow.h"
#include "Platform/Startup/Service.h"
#include <stdio.h>
#include <iostream>
#include "Platform/Strategy/StrategyManager.h"

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv,"IBTrader");

    printf( "Starting Services\n");
    Service::Instance()->startService();
    printf("\nMain Thread \t");
    std::cout<<QThread::currentThreadId();
    Service::Instance()->setMode(ForwardTest);
    StrategyManager* manager = new StrategyManager();


    printf( "Starting Threads\n");
    ThreadManager::Instance()->startThreads();
    ThreadManager::Instance()->waitOnThreads();

    manager->launchStrategies();

    //MainWindow w;
    //w.show();
    return app.exec();
}

