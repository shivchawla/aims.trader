/*
 *  PositionManager.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/28/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#pragma once
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

typedef std::map<PositionId,Position*> PositionPtrMap;
typedef std::map<TickerId, PositionId> TickerIdToPositionIdMap;
typedef std::map<long, PositionId> ContractIdToPositionIdMap;
typedef std::map<OrderId, TickerId> OrderIdToTickerIdMap;

class OutputInterface;
class PositionManager
{
	private:
		PositionPtrMap _currentPositions; 
        OrderIdToTickerIdMap _orderIdToTickerId;
        OutputInterface* _outputInterface;

    private:
		Strategy* _strategyWPtr;
        PerformanceManager* _performanceManager;

    private:
        QReadWriteLock* _lockForPositionMap;

    public:
        PositionManager(Strategy*);
        ~PositionManager();

    private:
        void initialize();
        const PositionId createNewPosition(const TickerId);
        void updatePerformanceForPrice(const Position*);
        void updatePerformanceForExecution(const Position*);
        void updateOutputsForExecution(const Position* position);
        void updateOutputsForLastPrice(const Position* position);

        void addPositionInOutputs(const StrategyId, const TickerId);
        void bookPnLOnClosingTrade(const double pnl);
        void removeFromPositionView(const StrategyId, const PositionId);
        void subscribeToMktData(const TickerId);
        void unSubscribeToMktData(const TickerId);

    public:    
        //void linkPerformanceManager(PerformanceManager*);

	//Properties
	public:
		const Strategy& getStrategy();
		const PositionPtrMap& getCurrentPositions();

    //Work functions
	public:
        void addPosition(const TickerId);
        void updatePosition(const TickerId, const Execution&);
        void updatePosition(const TickerId, const double lastPrice);
        void updatePosition(const TickerId, const int filledShares, const double fillPrice);
        void closeAllPositions();
        void setTickerId(const long contractId, const TickerId tickerId);
        void closePosition(const TickerId);
        void closePosition(const Position*);
};
#endif
