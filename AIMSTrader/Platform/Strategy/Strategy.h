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
//#include "Platform/Shared/Execution.h"
#include "Platform/Shared/Order.h"
#include "Platform/typedefs.h"
//#include "Platform/Utils/Bootstrap.h"
#include "Platform/Enumerations/TickType.h"
#include "Platform/Model/Mode.h"
#include <QObject>
#include "Platform/Utils/DataSubscriber.h"
#include "Platform/Enumerations/DataSource.h"
#include <QBasicTimer>
#include <QDate>

class Instrument;
class PerformanceManager;
class IndicatorManager;
class Indicator;
class PositionManager;
class StrategyReport;
class EventReport;
class TradingSchedule;

typedef long StrategyId; 

/*enum StrategyStatus
{
    Active,
    InActive,
    Closed,
    Closing
};*/

class Position; 

//typedef std::list<Strategy*> StrategyRegister;

class Strategy: public DataSubscriber
{
    Q_OBJECT
    private:
       static std::list<Strategy*> _strategyRegister;
       static int _instances;

    public:
       static std::list<Strategy*> getStrategies();
       static void registerStrategy(Strategy* ts);

    private:
        StrategyId _id;
        //StrategyStatus _status;
        long _time;
        Mode _mode;
        String _strategyName;

    private:
        bool _canOpenNewPositions;

    protected:
        QBasicTimer _basicTimer;
        int _timeout;
        bool _running;

    protected:
        TradingSchedule* _tradingSchedule;
        Indicator* _indicatorSPtr;

    private:
		PerformanceManager* _performanceManagerSPtr; //to evaluate the performance of strategy
		PositionManager* _positionManagerSPtr;
        StrategyReport* _strategyReportSPtr;

    private: 
        EventReport* _eventReportWPtr;

	public:
        Strategy(const String&);
        Strategy();
        virtual ~Strategy();
        //void initialize();

    public slots:
        void onTradeUpdate(const TickerId tickerId, const TradeUpdate&);
        void onQuoteUpdate(const TickerId tickerId, const QuoteUpdate&);
        void onExecutionUpdate(const OrderId, const ExecutionStatus&, const bool);
        void onTickPriceUpdate(const TickerId, const TickType, const double);

    public:
        void addPosition(const OrderId, const TickerId, const bool);
        void updatePosition(const OrderId, const Execution&);

    private:
        void createWorkers();
        void linkWorkers();
        void setName();

    public:
        virtual void initialize();

    public:
        void requestMarketData(const Contract&, const DataSource source = ActiveTick);
        void requestMarketData(const TickerId, const DataSource source = ActiveTick);
        void cancelMarketData(const TickerId);

    public:
        void updatePositionScreen(const Position&);
        void placeOrder(const Contract&, const Order&);
        void placeOrder(const TickerId, const Order&);
        void placeClosingOrder(const Contract&, const Order&);
        void placeClosingOrder(const TickerId, const Order&);

    //terminate all existng positions
	public:
		void closeAllPositions();
        void reportEvent(const String& message);


	
    public:
        const String& getStrategyName();
        const StrategyId getStrategyId();

    public:
        const static int numStrategies()
        {
            return _instances;
        }

    public slots:
        virtual void startStrategy();
        void stopStrategy();

    private:
        void timerEvent(QTimerEvent *);
        void setTimeout();
        const QDate& getNextValidDate();

    signals:
        void startIndicator();
        void stopIndicator();


};


#endif
