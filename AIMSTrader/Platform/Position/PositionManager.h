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

//TickerId To positionID

//PositionDetail will have a pointer to parent position
//With TickerId, jump to PositionDetail


//GOOG - 500
//100 PositionID 1
//100 PositionID 2
//300 PositionID 3

//you need stats per TickerId
//per Postion-TickerId

class Strategy;
class Position;
class Instrument;
struct ExecutionStatus;
class OpenOrder;

typedef QHash<TickerId, Position*> Positions; // Aggregate position for a TickerId

class IOInterface;
class StrategyOutput;

class PositionManager
{
    friend class Strategy;
	private:
        Positions _positions;
        IOInterface* _outputInterface;
        StrategyOutput* _strategyOutput;
        int _numPositions;

    private:
		Strategy* _strategyWPtr;
        PerformanceManager* _performanceManager;
        StrategyId _strategyId;

    public:
        PositionManager(Strategy*);
        ~PositionManager();

    private:
        void initialize();
        const PositionId createNewPosition(const TickerId);
        void updatePerformanceForPrice(const Position*);
        void updatePerformanceForExecution(const Position*);
        void updateOutputsForExecution(const Position*, const Position*, const OutputType type = ALL);
        void updateOutputsForLastPrice(const Position* position, const OutputType type = ALL);

        void addPositionInOutputs(const StrategyId, const TickerId, const OutputType type = ALL);
        void bookPnLOnClosingTrade(const double pnl);
        void removeFromPositionView(const StrategyId, const PositionId);
        void subscribeToMktData(const TickerId);
        void unSubscribeToMktData(const TickerId);
        Position* getPosition(const TickerId);

    //Properties
    public:
		const Strategy& getStrategy();
        //const PositionPtrMap& getCurrentPositions();
        //const Position& getPosition(const TickerId);
        //const Position& getPosition(const TickerId);

    //Work functions
	public:
        void addPosition(const TickerId);
        void updatePosition(const TickerId, const TickType, const double lastPrice,const bool testExitConditions = true);
        void updatePosition(const OrderId, const TickerId, const int filledShares, const double fillPrice, const double commission);
        void updatePosition(const OrderId, const TickerId, const OrderDetail&, const bool unSubscribe = true);

        void closeAllPositions();
        void setInstrumentId(const long contractId, const TickerId);
        void closePositions(const TickerId);
        void closePosition(const PositionId);
        void closePosition(const Position*);
        void loadPosition(const TickerId, const PositionData&);
        void addSubPosition(const Position*);

};
#endif
