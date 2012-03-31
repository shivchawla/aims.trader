/*
 *  Strategy.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef Strategy_h
#define Strategy_h

#include <string>
#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Shared/Order.h"
#include "Platform/Shared/CommonDefs.h"
//#include "Platform/Utils/Bootstrap.h"
#include "Platform/Enumerations/TickType.h"
#include "Platform/Model/Mode.h"
#include <QObject>
#include "Platform/Utils/MarketDataSubscriber.h"
class Instrument;
class PerformanceManager;
class IndicatorManager;
class PositionManager;
class StrategyReport;
class EventReport;
class TradingSchedule;

typedef long StrategyId; 
typedef std::string String;


enum StrategyStatus
{
    Active,
    InActive,
    Closed,
    Closing
};

class Position; 

//typedef std::list<Strategy*> StrategyRegister;

class Strategy: public MarketDataSubscriber
{
    Q_OBJECT
    private:
       static std::list<Strategy*> _strategyRegister;

    public:
       static std::list<Strategy*> getStrategies();
       static void registerStrategy(Strategy* ts);

    private:
        StrategyId id;
        StrategyStatus _status;
        long _time;
        Mode _mode;
        String _strategyName;

    protected:
        TradingSchedule* _tradingSchedule;

    private:
		PerformanceManager* _performanceManagerSPtr; //to evaluate the performance of strategy
		IndicatorManager* _indicatorManagerSPtr;	
		PositionManager* _positionManagerSPtr;
        StrategyReport* _strategyReportSPtr;

    private: 
        EventReport* _eventReportWPtr;

	public:
        Strategy(const String&);
        Strategy();
        virtual ~Strategy();
        void initialize();

    public slots:
        virtual void onTradeUpdate(const TickerId tickerId, TradeUpdate pTradeUpdate);
        virtual void onQuoteUpdate(const TickerId tickerId, QuoteUpdate pQuoteUpdate);

        //void updatePosition(const TickerId, const double lastPrice);
        void updatePosition(const OrderId, const Execution&);
        //void setTickerId(const long contractId, const TickerId);
        void addPosition(const OrderId, const Contract&);

    private:
        void createWorkers();
        void linkWorkers();
        void setName();
        //void createModelnView();

    public:
        void requestMarketData(const Contract&);

    public:
        //void updatePerformanceScreen(const PerformanceStats&);
        void updatePositionScreen(const Position&);
        void placeOrder(const Contract&, const Order&);

    //terminate all existng positions
	public:
		void closeAllPositions();
	
    public:
        const String& getStrategyName();

    public slots:
        virtual void startStrategy();

};

#endif
