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
typedef std::map<TickerId, PositionId> InstrumentIdToPositionIdMap;
//typedef std::map<long, PositionId> ContractIdToPositionIdMap;
typedef std::map<OrderId, TickerId> OrderIdToInstrumentIdMap;

class IOInterface;
class StrategyOutput;

class PositionManager
{
	private:
		PositionPtrMap _currentPositions; 
        PositionPtrMap _cumulativePositions;
        OrderIdToInstrumentIdMap _orderIdToInstrumentId;
        IOInterface* _outputInterface;
        StrategyOutput* _strategyOutput;

    private:
		Strategy* _strategyWPtr;
        PerformanceManager* _performanceManager;
        StrategyId _strategyId;

    private:
        QReadWriteLock* _lockForPositionMap;

    public:
        PositionManager(Strategy*);
        ~PositionManager();

    private:
        void initialize();
        const PositionId createNewPosition(const TickerId);
        void removeCurrentPosition(const TickerId);
        void updatePerformanceForPrice(const Position*);
        void updatePerformanceForExecution(const Position*);
        void updateOutputsForExecution(const Position*, const Position*, const OutputType type = ALL);
        void updateOutputsForLastPrice(const Position* position, const OutputType type = ALL);

        void addPositionInOutputs(const StrategyId, const TickerId, const OutputType type = ALL);
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
        const Position& getPosition(const TickerId);

    //Work functions
	public:
        void addPosition(const TickerId);
        void updatePosition(const TickerId, const Execution&);
        void updatePosition(const TickerId, const TickType, const double lastPrice);
        void updatePosition(const TickerId, const int filledShares, const double fillPrice, const double commission);
        void closeAllPositions();
        void setInstrumentId(const long contractId, const TickerId);
        void closePosition(const TickerId);
        void closePosition(const Position*);
        void loadPosition(const TickerId, const StrategyLinkedPositionData*);

};
#endif
