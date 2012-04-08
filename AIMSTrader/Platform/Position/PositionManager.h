/*
 *  PositionManager.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/28/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef PositionManager_h
#define PositionManager_h

/**
 * Position Manager keeps track of current and historical positions.
 * Every strategy will have its own position Manager which will keep track of its positions
 */
#include "Platform/Position/Position.h"
#include <list>
#include <map>
#include <memory>
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/Performance/PerformanceManager.h"
#include <QReadWriteLock>
#include <QObject>

class Strategy;
class Position;
class Instrument;
struct ExecutionStatus;

typedef long PositionId;
typedef std::map<PositionId,Position*> PositionPtrMap;
typedef std::map<TickerId, PositionId> TickerIdToPositionIdMap;
typedef std::map<long, PositionId> ContractIdToPositionIdMap;
typedef std::map<OrderId, PositionId> OrderIdIdToPositionIdMap;

class PositionManager: public QObject
{
    Q_OBJECT
	private:
		PositionId _currentPositionId;
		PositionPtrMap _currentPositions; 
		PositionPtrMap _historicalPositions;
        TickerIdToPositionIdMap _tickerIdToPositionId;
        ContractIdToPositionIdMap _contractIdToPositionId;
        OrderIdIdToPositionIdMap _orderIdToPositionId;

        //this map tracks whether, it's a first execution of order
        std::map<OrderId, bool> _firstExecutionHasArrived;

    private:
		Strategy* _strategyWPtr;
        PerformanceManager* _performanceManager;

    private:
        QReadWriteLock* lockForPositionMap;
        //QMutex* lockForPositionMap;

    private:
        const PositionId createNewPosition(const OrderId, const TickerId);
        void addPositionInView(const StrategyId, const PositionId, const TickerId);
        void updatePositionViewForLastPrice(const StrategyId, const PositionId, const double positionValue, const double tradeProfit);
        void updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability);
        void updatePerformance(const PositionId, const double);
        void updatePositionViewForExecution(const PositionId, Position* currentPosition);
        void bookPnLOnClosingTrade(const double pnl);
        void removeFromPositionView(const StrategyId, const PositionId);
        void subscribeToMktData(const TickerId);

	public:
		PositionManager();
		PositionManager(Strategy*);
		~PositionManager();
    
    public:    
        void linkPerformanceManager(PerformanceManager*);

	//Properties
	public:
		const Strategy& getStrategy();
		const PositionPtrMap& getCurrentPositions();
		const PositionPtrMap& getHistoricalPositions();
	
	//work functions
	public:
        //void addPosition(const OrderId, const Contract&);
        void addPosition(const OrderId, const TickerId, const bool isClosingPosition);
        void updatePosition(const OrderId, const ExecutionStatus&, const bool);
        void updatePosition(const TickerId, const double lastPrice);
        //void closePosition(const PositionId);
        void closePosition(const TickerId, const int);
        void closeAllPositions();
        void setTickerId(const long contractId, const TickerId tickerId);

    signals:
        //void positionUpdated(const Position&);
        void executionUpdated(const StrategyId, const PositionId, const int quantity, const double avgFillPrice, const double positionValue, const double commission);
        void lastPriceUpdated(const StrategyId, const PositionId, const double positionValue, const double netProfit);
        void positionCreated(const StrategyId, const PositionId, const TickerId);
        void positionRemoved(const StrategyId, const PositionId);

};
#endif
