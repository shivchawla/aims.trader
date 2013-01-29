//
//  Dispatcher.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/28/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include <iostream>
#include "Platform/Startup/Service.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include <QObject>
#include "Platform/Trader/TraderAssistant.h"
#include <QMetaType>
#include "Platform/Utils/Timer.h"
#include "Platform/Utils/SnapshotGenerator.h"
#include "Platform/Utils/TestDataGenerator.h"
#include "Platform/View/IOInterface.h"
#include "Platform/Trader/RiskManager.h"

Service::Service()
{
    init();
}

void Service::setupConnections()
{
    qRegisterMetaType<TickerId>("TickerId");
    qRegisterMetaType<TickerId>("InstrumentId");
    qRegisterMetaType<StrategyId>("StrategyId");
    qRegisterMetaType<ContractDetails>("ContractDetails");
    qRegisterMetaType<OrderId>("OrderId");
    qRegisterMetaType<OrderStatus>("OrderStatus");
    qRegisterMetaType<ExecutionStatus>("ExecutionStatus");
    qRegisterMetaType<Execution>("Execution");
    qRegisterMetaType<Order>("Order");
    qRegisterMetaType<TickType>("TickType");
    qRegisterMetaType<String>("String");
    qRegisterMetaType<PositionId>("PositionId");
    qRegisterMetaType<PerformanceStats>("PerformanceStats");
    qRegisterMetaType<MessageType>("MessageType");
    qRegisterMetaType<OpenOrder>("OpenOrder");
    qRegisterMetaType<Position>("Position");
    qRegisterMetaType<InstrumentContract>("InstrumentContract");
    qRegisterMetaType<PositionDetail>("PositionDetail");
    qRegisterMetaType<OrderDetail>("OrderDetail");
}

Service::~Service()
{
    delete _traderSPtr;
    delete _orderManager;
    delete _instrumentManager;
    delete _activeTickSession;
    delete _riskManager;
}

void Service::init()
{
    reportEvent("Starting Services");

    reportEvent("Creating Trader");
    _traderSPtr = new Trader();

    reportEvent("Creating Order Manager");
    _orderManager = new OrderManager();

    reportEvent("Starting Instrument Manager");
    _instrumentManager = new InstrumentManager();

    reportEvent("Creating Risk Manager");
    _riskManager = new RiskManager();

    reportEvent("Starting ActiveTickAPI");
    _activeTickSession = new ActiveTickSession();

    reportEvent("Staring Snapshot generator");
    _snapshotGenerator = new SnapshotGenerator();

    reportEvent("Starting TestData Generator");
    _testDataGenerator = new DataGenerator();

    reportEvent("Setting up default mode as ForwardTest");
    _mode = ForwardTest;
    setupConnections();
   // setMode();
}

//EventReport* Service::getEventReport()
//{
//    return _eventReportSPtr;
//}

Trader* Service::getTrader()
{
    if(!_traderSPtr)
    {
        _traderSPtr = new Trader();
    }
    return _traderSPtr;
}

/*Mode Service::getMode()
{
    return _mode;
}*/

void Service::exit()
{
   if (_traderSPtr != NULL)
   {
       _traderSPtr->Disconnect();
   }
}

void Service::setMode(const Mode mode)
{
    if (_mode != mode)
    {
        String s =  "Running mode changed to: ";
        s.append(getModeName(mode));
        reportEvent(s);
    }

    _mode = mode;

     // Disable all reporting when JA runs in optimization mode. The optimizer
     // runs thousands of strategies, and the amount of data to report would be enormous
    //if (mode == Optimization)
//    {
//        _eventReportSPtr->disable();
//    }
//    else
//    {
        //_eventReportSPtr->enable();
    //}

    if(!_traderSPtr)
    {
        _traderSPtr = new Trader();
    }


    //if (mode == Trade || mode == ForwardTest)
    //{
          _traderSPtr->Connect();
           // MonitoringServer.start();
    //}
//    //else
//    /{
//        _traderSPtr->Disconnect();
//    }
}

InstrumentManager* Service::getInstrumentManager()
{
    return _instrumentManager;
}

OrderManager* Service::getOrderManager()
{
    return _orderManager;
}

ActiveTickSession* Service::getActiveTickSession()
{
    return _activeTickSession;
}

void Service::stopServices()
{
    //_traderSPtr->Disconnect();
    //_activeTickSession->disConnect();
    //_testDataGenerator->stop();
    //_snapshotGenerator->stop();
    ThreadManager::threadManager().stopThreads();

}

DataGenerator* Service::getTestDataGenerator()
{
    return _testDataGenerator;
}

const Mode Service::getMode()
{
    return _mode;
}

RiskManager* Service::getRiskManager()
{
    return _riskManager;
}

//void Service::reportEvent(const String &message)
//{
//    OutputInterface::Instance()->reportEvent("Service", message);
//}

void Service::reportEvent(const String &message, const MessageType mType)
{
    IOInterface::ioInterface().reportEvent("Service", message, mType);
}

Service* Service::_instance = NULL;


