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
#include "Platform/Utils/TradingSchedule.h"

class Instrument;
class PerformanceManager;
class IndicatorManager;
class Indicator;
class PositionManager;
class StrategyReport;
class EventReport;
class OpenOrder;
class Position; 
class StrategyOutput;

typedef QHash<OrderId, TickerId> OrderIdToTickerIdMap;

class Strategy: public DataSubscriber
{
    friend class PositionManager;
    friend class Indicator;
    Q_OBJECT
    private:
        long _time;
        Mode _mode;
        EventReport* _eventReportWPtr;

    protected:
        static long id;
        StrategyId _strategyId;
        String _strategyName;
        bool _canOpenNewPositions;
        String _defaultTradeDirection;
        QList<InstrumentContract> _buyList;
        QHash<String,String> _strategyParams;
        OrderIdToTickerIdMap _orderIdToTickerId;
        StrategyType _strategyType;
        QHash<TickerId, int> _pendingOrderQuantity;

    protected:
        int _timeout;
        QDate _nextValidDate;
        bool _isIndicatorRunning;
        bool _isStrategyActive;
        double _targetReturn;
        double _stopLossReturn;
        int _maxHoldingPeriod;
        bool _isExtensionAllowed;
        int _timeScale;
        double _totalInvested;
        double _maxInvestment;
        QDateTime _nextStartDateTime;
        QDateTime _nextEndDateTime;
        bool _isSpreadTradingStrategy;

    protected:
        TradingSchedule* _tradingSchedule;
        Indicator* _indicatorPtr;
        StrategyOutput* _strategyOutput;

    protected:
        //to evaluate the performance of strategy
        PerformanceManager* _performanceManagerSPtr;
        PositionManager* _positionManager;
        StrategyReport* _strategyReportSPtr;

    private:
        void createWorkers();
        void linkWorkers();
        void setupConnection();
        void timerEvent(QTimerEvent *);
        const QDate& getNextValidDate();
        void addPosition(const OrderId, const TickerId);
        void loadOpenPositions();
        void loadBuyList();
        void closeAllPositions();

    protected:
        void loadStrategyParameters();
        virtual void populateStrategySpecificPreferences(){}
        virtual void loadStrategyDataFromDb();
        void populateGeneralStrategyPreferences();
        void registerBuyList();
        bool IsValid(const QDateTime&, const TradeType exclusiontype = OPENEXTEND);
        bool canPlaceOrder(const TickerId, const Order&);
        void updatePendingQuantity(const TickerId, int);
        void setTimeout();

    public:
        Strategy(const String&);
        Strategy();
        virtual ~Strategy();

    public:
        void initialize();
        void requestCloseAllPositions();
        void requestClosePosition(const TickerId);
        void requestAdjustPosition(const TickerId, const Order&);
        void addPosition(const OrderId, const TickerId, const bool);
        void requestStrategyUpdateForExecution(const OrderId,  const OrderDetail&);
        void placeOrder(const TickerId, const Order&);
        void placeClosingOrder(const TickerId, const Order&);
        void reportEvent(const String& message, const MessageType mType = INFO);
        void connectIndicatorSignals();

        void setBuyList(const QList<InstrumentData>& buyList);
        void setStrategyId(const StrategyId strategyId)
        {
            _strategyId = strategyId;
        }
        void onTickPriceUpdate(const TickerId, const TickType, const double);
        void setName(const QString&);
        void setupStrategy(const StrategyId, const StrategyData&);
        virtual void setupIndicator(){}

    public:
        const String getStrategyName();
        const StrategyId getStrategyId();
        StrategyType getStrategyType();
        const double getTargetReturn();
        const int getMaxHoldingPeriod();
        const double getStopLossReturn();
        PerformanceManager* getPerformanceManager();
        PositionManager* getPositionManager();
        StrategyReport* getStrategyReport();

    private slots:
        void startIndicator();

    protected slots:
         virtual void updatePositionOnExecution(const OrderId, const OrderDetail&);
         void stopIndicator();

    public slots:
            virtual void stopStrategy();
            virtual void startStrategy();
            virtual void closeAll();
            void onOrderRequestFromIndicator(const TickerId, const Order&);
            void closePosition(const TickerId);
            void adjustPosition(const TickerId, const Order&);

//    signals:
//        void requestStartIndicator();
//        void requestStopIndicator();
//        void closeAllPositionsRequested();
//        void closePositionRequested(const TickerId);
//        void adjustPositionRequested(const TickerId, const Order&);
//        void positionUpdateOnExecutionRequested(const OrderId, const OrderDetail&);
};

#endif
