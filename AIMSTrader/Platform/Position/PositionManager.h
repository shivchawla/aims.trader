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
typedef std::map<InstrumentId, PositionId> InstrumentIdToPositionIdMap;
//typedef std::map<long, PositionId> ContractIdToPositionIdMap;
typedef std::map<OrderId, InstrumentId> OrderIdToInstrumentIdMap;

class IOInterface;
class StrategyOutput;

class PositionManager
{
	private:
		PositionPtrMap _currentPositions; 
        OrderIdToInstrumentIdMap _orderIdToInstrumentId;
        IOInterface* _outputInterface;
        StrategyOutput* _strategyOutput;

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
        const PositionId createNewPosition(const InstrumentId);
        void updatePerformanceForPrice(const Position*);
        void updatePerformanceForExecution(const Position*);
        void updateOutputsForExecution(const Position* position, const OutputType type = ALL);
        void updateOutputsForLastPrice(const Position* position, const OutputType type = ALL);

        void addPositionInOutputs(const StrategyId, const InstrumentId, const OutputType type = ALL);
        void bookPnLOnClosingTrade(const double pnl);
        void removeFromPositionView(const StrategyId, const PositionId);
        void subscribeToMktData(const InstrumentId);
        void unSubscribeToMktData(const InstrumentId);

    public:    
        //void linkPerformanceManager(PerformanceManager*);

	//Properties
	public:
		const Strategy& getStrategy();
		const PositionPtrMap& getCurrentPositions();

    //Work functions
	public:
        void addPosition(const InstrumentId);
        void updatePosition(const InstrumentId, const Execution&);
        void updatePosition(const InstrumentId, const double lastPrice);
        void updatePosition(const InstrumentId, const int filledShares, const double fillPrice);
        void closeAllPositions();
        void setInstrumentId(const long contractId, const InstrumentId);
        void closePosition(const InstrumentId);
        void closePosition(const Position*);
        void loadPositions(const QList<StrategyLinkedPositionData*>& positions);

};
#endif
