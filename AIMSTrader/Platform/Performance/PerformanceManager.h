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

/*
This class manages the performance of a strategy 
*/

class Strategy;
class PositionManager;
class Position;

typedef long PositionId;

struct PerformanceStats
{

};

class PerformanceManager 
{
    //Performance Diagnostics
    private:
        int _trades, _profitableTrades;
        double _totalBought, _totalSold, _grossPnL, _netPnL;
        double _peakNetProfit, _maxDrawdown;
        double _sumTradeProfit, _sumTradeProfitSquared;
        long _longTrades, _shortTrades;
        double _totalCommission;
        //corresponding strategy
	private:
        Strategy* _strategyWPtr;
	
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
        void updatePerformance(const Position&);
        void updatePerformance();
        void updatePerformance(const double oldPositionValue, const double newPositionValue, const double oldTradeProfit, const double newTradeProfit);
        void updateShortTrades();
        void updateLongTrades();
};

#endif
