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

#include <QObject>
class Service
{
    private:
        static Service* _instance;
        EventReport* _eventReportSPtr;
        Trader* _traderSPtr;
        OrderManager* _orderManager;
        InstrumentManager* _instrumentManager;
        ActiveTickAPI* _activeTickAPI;
        Mode _mode;

    private:
        Service();
    public:
        ~Service();

    private:
        void setupConnections();

    public:
        static Service* Instance();
        void startService();
        EventReport* getEventReport();
        Trader* getTrader();
        StrategyManager* getStrategyManager();
        InstrumentManager* getInstrumentManager();
        OrderManager* getOrderManager();
        ActiveTickAPI* getActiveTickAPI();

    public:
        void setReport(); 
        void exit();
        void createStrategyManager();
        void setMode(const Mode mode);
};

#endif
