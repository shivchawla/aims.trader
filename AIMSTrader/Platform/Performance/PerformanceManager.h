/*
 *  PerformanceManager.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef PerformanceManager_h
#define PerformanceManager_h
#include "Platform/typedefs.h"

/*
This class manages the performance of a strategy
*/

class OutputInterface;
class Strategy;
class PositionManager;
class Position;

typedef long PositionId;

class PerformanceManager
{
    //Performance Diagnostics
    private:
       PerformanceStats _performanceStats;
    //corresponding strategy
	private:
        Strategy* _strategyWPtr;
        StrategyId _strategyId;
        OutputInterface* _outputInterface;
	
    //ctor and dtor
	public:
		PerformanceManager();
        PerformanceManager(Strategy*);
		~PerformanceManager();

    private:
        void initialize();
		
    public:
        void linkWorkers(PositionManager*);
    
	//Properties
	public:
        const double getGrossPnL() const;
		const long getNumberTrades() const;
		const double getRatio() const;
	
	//Utility
	public:
		const double getBias();
		const double getPercentProfitableTrades();
		const double getAverageProfitPerTrade(); 
		const double getProfitFactor();
	
	//Work functions
	public:
        void updatePerformanceForPrice(const Position*);
        void updatePerformanceForExecution(const Position*);

//    signals:
//        void performanceUpdatedOnTrade(const StrategyId, const int profitableTrades, const double netProfit);
//        void performanceUpdatedOnExecution(const StrategyId strategyId, const int profitableTrades, const double totalBought, const double totalSold, const double commission);
//        void tradesUpdated(const StrategyId, const int, const int, const int);
//        //void strategyCreated(StrategyId,String);
};

#endif
