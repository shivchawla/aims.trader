//
//  Header.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/28/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//
#pragma once
#ifndef Service_h
#define Service_h

#include "Platform/Reports/EventReport.h"
#include "Platform/Trader/Trader.h"
#include "Platform/Model/Mode.h"
#include <QThread>

#include "Platform/Utils/ThreadManager.h"
#include "ActiveTickFeed/Utils/ActiveTickApi.h"
#include "Platform/Utils/Singleton.h"

class StrategyManager;
class InstrumentManager;
class OrderManager;
class SnapshotGenerator;
class DataGenerator;

#include <QObject>
class Service //: public Singleton<Service>
{
    //friend class Singleton<Service>;
    private:
        static Service* _instance;
        //EventReport* _eventReportSPtr;
        Trader* _traderSPtr;
        OrderManager* _orderManager;
        InstrumentManager* _instrumentManager;
        ActiveTickSession* _activeTickSession;
        SnapshotGenerator* _snapshotGenerator;
        DataGenerator* _testDataGenerator;
        //Timer* timer;
        Mode _mode;

    public:
        Service();
        ~Service();
        static Service& service()
        {
            if(_instance ==NULL)
            {
                _instance = new Service();

            }
            return *_instance;
//            static Service service;// = new Service();
//            return service;
        }


    private:
        void init();
        void setupConnections();
        //void reportEvent(const String& message);
        void reportEvent(const String& message, const MessageType=INFO);

    public:
        //static Service& Instance() { static Service s; return s;}
        void startService();
        //EventReport* getEventReport();
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

//static Service service()
//{
//    static Service service;// = new Service();
//    return service;
//}


#endif
