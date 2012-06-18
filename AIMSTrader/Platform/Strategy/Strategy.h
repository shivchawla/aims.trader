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
class OpenOrder;

typedef long StrategyId; 

class Position; 
class Strategy: public DataSubscriber
{
    Q_OBJECT
    private:
        static int _id;
        StrategyId _strategyId;
        long _time;
        Mode _mode;
        String _strategyName;

    protected:
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
        void onExecutionUpdate(const TickerId, const Execution&);//, const bool);
        void onTickPriceUpdate(const TickerId, const TickType, const double);

    public:
        void requestCloseAllPositions();
        void requestClosePosition(const TickerId);
        void requestAdjustPosition(const TickerId, const Order&);

    public:
        void addPosition(const OrderId, const TickerId, const bool);

    public:
        void updatePosition(const OrderId, const Execution&);
        void requestStrategyUpdateForExecution(const OpenOrder*);


    private:
        void createWorkers();
        void linkWorkers();
        void setName();
        void setupConnection();

    private slots:
        void closeAllPositions();
        void closePosition(const TickerId);
        void adjustPosition(const TickerId, const Order&);
        void updatePositionForExecution(const TickerId, const int filledShares, const double lastFillPrice);

    public:
        void initialize();

    public:
        void updatePositionScreen(const Position&);
        void placeOrder(const Contract&, const Order&);
        void placeOrder(const TickerId, const Order&);
        void placeClosingOrder(const Contract&, const Order&);
        void placeClosingOrder(const TickerId, const Order&);


    //terminate all existng positions
	public:
        void reportEvent(const String& message);

    public:
        const String& getStrategyName();
        const StrategyId getStrategyId();

    public slots:
        virtual void startStrategy();
        void stopStrategy();

    private:
        void timerEvent(QTimerEvent *);
        void setTimeout();
        const QDate& getNextValidDate();
        void addPosition(const OrderId, const TickerId);

    signals:
        void startIndicator();
        void stopIndicator();
        void closeAllPositionsRequested();
        void closePositionRequested(const TickerId);
        void adjustPositionRequested(const TickerId, const Order&);
        void positionUpdateForExecutionRequested(const TickerId, const int filledShares, const double lastFillPrice);
};

#endif
