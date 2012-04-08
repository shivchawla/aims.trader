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
#include <QObject>
#include <vector>

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

class PerformanceManager:public QObject
{
    Q_OBJECT
    //Performance Diagnostics
    private:
        int _trades, _profitableTrades;
        double _totalBought, _totalSold, _unRealizedGrossPnL, _netPnL;
        double _peakNetProfit, _maxDrawdown;
        double _sumTradeProfit, _sumTradeProfitSquared;
        long _longTrades, _shortTrades;
        double _totalCommission;
        double _realizedGrossPnL;
        //corresponding strategy

	private:
        Strategy* _strategyWPtr;
        StrategyId _strategyId;

        std::vector<double> _unrealizedGains;
	
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
        void updatePerformance(const Position&, const Position&);
        void updatePerformance();
        void updatePerformance(const double oldPositionValue, const double newPositionValue, const double oldTradeProfit, const double newTradeProfit);
        void updatePerformance(const double oldPositionValue, const double newPositionValue, const double oldLastPrice, const double lastPrice, const double avgFillPrice);
        void updateShortTrades();
        void updateLongTrades();
        void updateOnOrderFill(const int shares, const double avgPrice, const double commission);
        void bookPnL(const double pnl);
        void updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability);
        void updatePerformance(const PositionId positionId, const double pnl);

    signals:
        void performanceUpdatedOnTrade(const StrategyId, const int profitableTrades, const double netProfit);
        void performanceUpdatedOnExecution(const StrategyId strategyId, const int profitableTrades, const double totalBought, const double totalSold, const double commission);
        void tradesUpdated(const StrategyId, const int, const int, const int);
        //void strategyCreated(StrategyId,String);
};

#endif
