//
//  Header.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/28/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#ifndef Service_h
#define Service_h

#include "Platform/Reports/EventReport.h"
#include "Platform/Trader/Trader.h"
#include "Platform/Model/Mode.h"
#include <QThread>

#include "Platform/Utils/ThreadManager.h"
#include "ActiveTickFeed/Utils/ActiveTickApi.h"

class StrategyManager;
class InstrumentManager;
class OrderManager;
class SnapshotGenerator;
class DataGenerator;

#include <QObject>
class Service
{
    private:
        static Service* _instance;
        EventReport* _eventReportSPtr;
        Trader* _traderSPtr;
        OrderManager* _orderManager;
        InstrumentManager* _instrumentManager;
        ActiveTickSession* _activeTickSession;
        SnapshotGenerator* _snapshotGenerator;
        DataGenerator* _testDataGenerator;
        //Timer* timer;
        Mode _mode;

    private:
        Service();
    public:
        ~Service();

    private:
        void setupConnections();
        void reportEvent(const String& message);

    public:
        static Service* Instance();
        void startService();
        EventReport* getEventReport();
        Trader* getTrader();
        StrategyManager* getStrategyManager();
        InstrumentManager* getInstrumentManager();
        OrderManager* getOrderManager();
        ActiveTickSession* getActiveTickSession();
        DataGenerator* getTestDataGenerator();
        const Mode getMode();

    public:
        //void setReport();
        void exit();
        void createStrategyManager();
        void setMode(const Mode mode);
        void setEventReporter();
        void stopServices();
};

#endif
