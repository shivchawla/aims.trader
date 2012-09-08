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
    friend class PositionManager;
    Q_OBJECT
    private:
        static int _id;
        StrategyId _strategyId;
        long _time;
        Mode _mode;
        String _strategyName;

    protected:
        bool _canOpenNewPositions;
        QList<InstrumentContract*> _buyList;

    protected:
        QBasicTimer _basicTimer;
        int _timeout;
        bool _running;
        double _targetReturn;
        double _stopLossReturn;
        int _maxHoldingPeriod;
        bool _isExtensionAllowed;

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
        void addPosition(const OrderId, const TickerId);
        void loadStrategyPreferences(const DbStrategyId);
        void loadPositions(const DbStrategyId);
        void loadBuyList(const DbStrategyId);

    private slots:
        void closeAllPositions();
        void closePosition(const TickerId);
        void adjustPosition(const TickerId, const Order&);
        void updatePositionForExecution(const TickerId, const int filledShares, const double lastFillPrice, const double commission);

   public:
        Strategy(const String&);
        Strategy();
        virtual ~Strategy();
        void setName(const QString&);
        void setStrategy(const StrategyData*);

        //void initialize();

    public:
        void initialize();
        void requestCloseAllPositions();
        void requestClosePosition(const TickerId);
        void requestAdjustPosition(const TickerId, const Order&);
        void addPosition(const OrderId, const TickerId, const bool);
        void updatePosition(const OrderId, const Execution&);
        void requestStrategyUpdateForExecution(const OpenOrder*);
        void updatePositionScreen(const Position&);
        //void placeOrder(const ATContract&, const Order&);
        void placeOrder(const TickerId, const Order&);
        //void placeClosingOrder(const ATContract&, const Order&);
        void placeClosingOrder(const TickerId, const Order&);
        void reportEvent(const String& message, const MessageType mType = INFO);
        void setupIndicator();
        //void loadBuyListFromIndex(const String index);

        void setBuyList(const QList<InstrumentData*>& buyList);
        void setStrategyId(const StrategyId id)
        {
            _strategyId = id;
        }

        void loadStrategyDataFromDb(const StrategyData*);



    public:
        const String& getStrategyName();
        const StrategyId getStrategyId();
        const double getTargetReturn();
        const int getMaxHoldingPeriod();
        const double getStopLossReturn();

        PerformanceManager* getPerformanceManager();
        PositionManager* getPositionManager();
        StrategyReport* getStrategyReport();



    public slots:
//        void onTradeUpdate(const TickerId tickerId, const TradeUpdate&);
//        void onQuoteUpdate(const TickerId tickerId, const QuoteUpdate&);
        void onExecutionUpdate(const TickerId, const Execution&);//, const bool);
        void onTickPriceUpdate(const TickerId, const TickType, const double);
        void stopStrategy();
        virtual void startStrategy();
        void onInstrumentSelection(const TickerId);


    signals:
        void startIndicator();
        void stopIndicator();
        void closeAllPositionsRequested();
        void closePositionRequested(const TickerId);
        void adjustPositionRequested(const TickerId, const Order&);
        void positionUpdateForExecutionRequested(const TickerId, const int filledShares, const double lastFillPrice, const double commission);
};




#endif
