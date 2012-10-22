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
#include <QTimer>
#include <QHash>

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
    friend class Indicator;
    Q_OBJECT
    private:
        static int _id;
        StrategyId _strategyId;
        long _time;
        Mode _mode;
        String _strategyName;
        //QTimer* _timer;

    protected:
        bool _canOpenNewPositions;
        String _defaultTradeDirection;
        QList<InstrumentContract*> _buyList;
        QHash<String,String> _strategyParams;

    protected:
        int _timeout;
        QDate _nextValidDate;
        bool _isIndicatorRunning;
        double _targetReturn;
        double _stopLossReturn;
        int _maxHoldingPeriod;
        bool _isExtensionAllowed;
        int _timeScale;

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
        void populateGeneralStrategyPreferences();
        void loadPositions(const DbStrategyId);
        void loadBuyList(const DbStrategyId);

    private slots:
        void closeAllPositions();
        void closePosition(const TickerId);
        void adjustPosition(const TickerId, const Order&);
        void updatePositionOnExecution(const OrderId, const TickerId, const int filledShares, const double lastFillPrice, const double commission);
        void updatePositionOnExecution(const OpenOrder&);
        void updatePositionOnExecution(const OrderId, const OrderDetail&);

   protected:
        virtual void populateStrategySpecificPreferences(){}

    public:
        Strategy(const String&);
        Strategy();
        virtual ~Strategy();
        void setName(const QString&);
        void setupStrategy(const StrategyData*);
        virtual void setupIndicator(){}

        //void initialize();

    public:
        //void timerEvent(QTimerEvent *);
        void initialize();
        void requestCloseAllPositions();
        void requestClosePosition(const TickerId);
        void requestAdjustPosition(const TickerId, const Order&);
        void addPosition(const OrderId, const TickerId, const bool);
        void updatePosition(const OrderId, const Execution&);
        //void requestStrategyUpdateForExecution(const OpenOrder*);
        void requestStrategyUpdateForExecution(const OrderId,  const OrderDetail&);
        void updatePositionScreen(const Position&);
        //void placeOrder(const ATContract&, const Order&);
        void placeOrder(const TickerId, const Order&);
        //void placeClosingOrder(const ATContract&, const Order&);
        void placeClosingOrder(const TickerId, const Order&);
        void reportEvent(const String& message, const MessageType mType = INFO);
        void connectIndicatorSignals();
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
        void onInstrumentSelectionByIndicator(const TickerId, const Order&);

   private slots:
        void startIndicator();
        void stopIndicator();

    signals:
        void requestStartIndicator();
        void requestStopIndicator();
        void closeAllPositionsRequested();
        void closePositionRequested(const TickerId);
        void adjustPositionRequested(const TickerId, const Order&);
        //void positionUpdateOnExecutionRequested(const TickerId, const int filledShares, const double lastFillPrice, const double commission);
        //void positionUpdateOnExecutionRequested(const OpenOrder&);
        void positionUpdateOnExecutionRequested(const OrderId, const OrderDetail&);
};




#endif
