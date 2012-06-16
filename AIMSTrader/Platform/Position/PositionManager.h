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
typedef std::map<OrderId, TickerId> OrderIdToTickerIdMap;

class OutputInterface;
class PositionManager: public QObject
{
    Q_OBJECT
	private:
        //PositionId _currentPositionId;
		PositionPtrMap _currentPositions; 
        //PositionPtrMap _historicalPositions;
        //TickerIdToPositionIdMap _tickerIdToPositionId;
        //ContractIdToPositionIdMap _contractIdToPositionId;
        OrderIdToTickerIdMap _orderIdToTickerId;
        OutputInterface* _outputInterface;

        //this map tracks whether, it's a first execution of order
        //std::map<OrderId, bool> _firstExecutionHasArrived;

    private:
		Strategy* _strategyWPtr;
        PerformanceManager* _performanceManager;

    private:
        QReadWriteLock* _lockForPositionMap;

    private:
        const PositionId createNewPosition(const OrderId, const TickerId);
        void updatePerformanceForPrice(const Position*);
        void updatePerformanceForExecution(const Position*);

        //void updateOutputs(const StrategyId, const TickerId, const double lastPrice);
        //void updateOutputs(const StrategyId, const TickerId, const ExecutionStatus);
        void updateOutputsForExecution(const Position* position);
        void updateOutputsForLastPrice(const Position* position);

        void addPositionInOutputs(const StrategyId, const TickerId);
        void bookPnLOnClosingTrade(const double pnl);
        void removeFromPositionView(const StrategyId, const PositionId);
        void subscribeToMktData(const TickerId);
        void unSubscribeToMktData(const TickerId);

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
        //const PositionPtrMap& getHistoricalPositions();
	
    //Work functions
	public:
        void addPosition(const OrderId, const TickerId);//, const bool isClosingPosition = false);
        void updatePosition(const OrderId, const TickerId, const Execution&);//, const bool);
        void updatePosition(const TickerId, const double lastPrice);
        //void closePosition(const PositionId);
        void closeAllPositions();
        void setTickerId(const long contractId, const TickerId tickerId);
        void closePosition(const TickerId);
        void closePosition(const Position*);

    /*signals:
        void positionCreated(const StrategyId, const TickerId);
        //void executionUpdated(const StrategyId, const TickerId, const ExecutionStatus);
        //void lastPriceUpdated(const StrategyId strategyId, const TickerId tickerId, const double lastPrice);
        //void executionUpdated(const StrategyId, const TickerId, const long sharesBought, const long sharesSold, const long netShares, const double avgBought, const double avgSold, const double totalValueBought, const double totalValueSold, const double netTotal, const double realizedPnl, const double runningPnl, const double PnL, const double totalCommision, const double netTotalIncCommission);
        void executionUpdated(const Position&);
        void lastPriceUpdated(const StrategyId, const TickerId, const double runningPnl, const double pnl);
     */

};
#endif
