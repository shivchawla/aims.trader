/*
 *  Strategy.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#pragma once
#ifndef Strategy_h
#define Strategy_h

#include <string>
#include "InteractiveBroker/Shared/Contract.h"
#include "InteractiveBroker/Shared/Order.h"

#include "AimsTraderDefs/typedefs.h"
//#include "Platform/Enumerations/TickType.h"
#include "Platform/Model/Mode.h"
#include <QObject>
#include "Platform/Utils/DataSubscriber.h"
#include <QBasicTimer>
#include <QDate>
#include <QList>
#include <Data/DataObjects.h>

class Instrument;
class PerformanceManager;
class IndicatorManager;
class Indicator;
class PositionManager;
class StrategyReport;
class EventReport;
class TradingSchedule;
class OpenOrder;
class Position; 
class StrategyOutput;

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
        QList<InstrumentData*> _buyList;

    protected:
        QBasicTimer _basicTimer;
        int _timeout;
        bool _running;

    protected:
        TradingSchedule* _tradingSchedule;
        Indicator* _indicatorSPtr;
        StrategyOutput* _strategyOutput;

    private:
		PerformanceManager* _performanceManagerSPtr; //to evaluate the performance of strategy
		PositionManager* _positionManagerSPtr;
        StrategyReport* _strategyReportSPtr;

    private: 
        EventReport* _eventReportWPtr;

    private:
        void createWorkers();
        void linkWorkers();
        void setupConnection();
        void timerEvent(QTimerEvent *);
        void setTimeout();
        const QDate& getNextValidDate();
        void addPosition(const OrderId, const InstrumentId);

    private slots:
        void closeAllPositions();
        void closePosition(const InstrumentId);
        void adjustPosition(const InstrumentId, const Order&);
        void updatePositionForExecution(const InstrumentId, const int filledShares, const double lastFillPrice);

   public:
        Strategy(const String&);
        Strategy();
        virtual ~Strategy();
        void setName(const QString&);
        //void initialize();

    public:
        void initialize();
        void requestCloseAllPositions();
        void requestClosePosition(const InstrumentId);
        void requestAdjustPosition(const InstrumentId, const Order&);
        void addPosition(const OrderId, const InstrumentId, const bool);
        void updatePosition(const OrderId, const Execution&);
        void requestStrategyUpdateForExecution(const OpenOrder*);
        void updatePositionScreen(const Position&);
        //void placeOrder(const ATContract&, const Order&);
        void placeOrder(const InstrumentId, const Order&);
        //void placeClosingOrder(const ATContract&, const Order&);
        void placeClosingOrder(const InstrumentId, const Order&);
        void reportEvent(const String& message, const MessageType mType = INFO);
        void setupIndicatorConnections();
        //void loadBuyListFromIndex(const String index);

        void setBuyList(const QList<InstrumentData*>& buyList);

    public:
        const String& getStrategyName();
        const StrategyId getStrategyId();
        PerformanceManager* getPerformanceManager();
        PositionManager* getPositionManager();
        StrategyReport* getStrategyReport();

    public slots:
//        void onTradeUpdate(const TickerId tickerId, const TradeUpdate&);
//        void onQuoteUpdate(const TickerId tickerId, const QuoteUpdate&);
        void onExecutionUpdate(const InstrumentId, const Execution&);//, const bool);
        void onTickPriceUpdate(const InstrumentId, const TickType, const double);
        void stopStrategy();
        virtual void startStrategy();

    signals:
        void startIndicator();
        void stopIndicator();
        void closeAllPositionsRequested();
        void closePositionRequested(const InstrumentId);
        void adjustPositionRequested(const InstrumentId, const Order&);
        void positionUpdateForExecutionRequested(const InstrumentId, const int filledShares, const double lastFillPrice);
};




#endif
