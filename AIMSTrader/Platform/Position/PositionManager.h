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

class Strategy;
class Position;
class Instrument;
struct ExecutionStatus;

typedef long PositionId;
typedef std::map<PositionId,Position*> PositionPtrMap;
typedef std::map<TickerId, PositionId> TickerIdToPositionIdMap;
typedef std::map<long, PositionId> ContractIdToPositionIdMap;
typedef std::map<OrderId, PositionId> OrderIdIdToPositionIdMap;

class PositionManager
{
	private:
		PositionId _currentPositionId;
		PositionPtrMap _currentPositions; 
		PositionPtrMap _historicalPositions;
        TickerIdToPositionIdMap _tickerIdToPositionId;
        ContractIdToPositionIdMap _contractIdToPositionId;
        OrderIdIdToPositionIdMap _orderIdToPositionId;

    private:
		Strategy* _strategyWPtr;
        PerformanceManager* _performanceManager;

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
        void addPosition(const OrderId, const Contract&);
        void updatePosition(const OrderId, const Execution&);
        void updatePosition(const TickerId, const double lastPrice);
        void closePosition(const PositionId);
		void closeAllPositions();	
        void setTickerId(const long contractId, const TickerId tickerId);
};
#endif
