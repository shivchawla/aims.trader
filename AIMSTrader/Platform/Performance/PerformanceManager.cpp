/*
 *  PerformaceManager.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Performance/PerformanceManager.h"
#include "Platform/Position/Position.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/View/MainWindow.h"
#include "Platform/View/StrategyView.h"

//ctor and dtor
PerformanceManager::PerformanceManager(Strategy* strategyWPtr):QObject(),_strategyWPtr(strategyWPtr)
{
    initialize();
}

PerformanceManager::~PerformanceManager()
{}

const double PerformanceManager::getGrossPnL() const
{
    return _unRealizedGrossPnL;
}

const double PerformanceManager::getProfitFactor()
{
	double profitFactor=0;
	//calculate profitfactor
	return profitFactor;
}

const long PerformanceManager::getNumberTrades() const
{
	return _trades;
}

const double PerformanceManager::getRatio() const
{
    return 1;
}

//Utility
const double PerformanceManager::getBias()
{
	if (_trades == 0) {
		return 0;
	}
	return 100 * (_longTrades - _shortTrades) / (double) _trades;
}

const double PerformanceManager::getPercentProfitableTrades()
{
	return (_profitableTrades/_trades)*100;
}

const double PerformanceManager::getAverageProfitPerTrade()
{
	return 0;
}


//What is performance manager
//it keeps a track of cumulative loss, cumulative gains, some ratios, profit factors etc.
//also, as it is cumulative in nature , it needs to understand and take into consideration the
//active positions...and accomodate the performance of active positions too

//There has to be a fedback mechanism for position manager. Everytime position manager 
//takes a position, it tells the performance manager. 
//This is important to know how the performance manager will work.
//It will have fields corresponding to historical trades
//and field related to current trades. Using TA, it will constantly inquire health 
//of current positions and will update the performance. 
//This is getting COMPLEX..

//TOTAL = CURRENT + HISTORICAL 

//Work functions

//this function will be called everytime strategy needs to know its performance
void PerformanceManager::initialize()
{
    _trades = _profitableTrades = _longTrades = _shortTrades = 0;
    _totalCommission = 0;
    _totalBought = _totalSold = 0;
    _unRealizedGrossPnL = _netPnL = _peakNetProfit = _maxDrawdown = 0;
    _sumTradeProfit = _sumTradeProfitSquared = 0;
    _strategyId = _strategyWPtr->getStrategyId();

    _unRealizedGrossPnL = 0;
    _realizedGrossPnL = 0;

    _unrealizedGains.reserve(500);

    for(int i = 0 ; i <500 ; ++i)
    {
        _unrealizedGains.push_back(0.0);
    }

    //connect(this, SIGNAL(performanceUpdatedOnExecution(StrategyId,int,double,double,double)),MainWindow::mainWindow()->getStrategyView(),SLOT(onExecutionUpdate(StrategyId,int,double,double,double)));
    //connect(this, SIGNAL(performanceUpdatedOnTrade(StrategyId,int,double)),MainWindow::mainWindow()->getStrategyView(),SLOT(onTradeUpdate(StrategyId,int,double)));
    //connect(this, SIGNAL(tradesUpdated(StrategyId, int, int, int)), MainWindow::mainWindow()->getStrategyView(), SLOT(updateTrades(StrategyId, int, int, int)));
}

void PerformanceManager::updatePerformance()
{}

void PerformanceManager::updateLongTrades()
{
    _longTrades++;
    _trades++;

    //emit tradesUpdated(_strategyId, _trades, _longTrades,_shortTrades);
}

void PerformanceManager::updateShortTrades()
{
    _shortTrades++;
    _trades++;
    //emit tradesUpdated(_strategyId, _trades, _longTrades,_shortTrades);
}

void PerformanceManager::updateOnOrderFill(const int shares, const double avgPrice, const double commission)
{
    if(shares>0)
    {
        _totalBought += shares*avgPrice;
    }
    else
    {
        _totalSold += shares*avgPrice;
    }
    _totalCommission += commission;

     //emit performanceUpdatedOnExecution(_strategyId, _profitableTrades, _totalBought, _totalSold, _totalCommission);
}

void PerformanceManager::updatePerformance(const PositionId positionId, const double pnl)
{
    double oldPnl = _unrealizedGains[positionId];
    _unRealizedGrossPnL += pnl-oldPnl;
    _unrealizedGains[positionId] = pnl;

    if(oldPnl<=0 && pnl>0)
    {
        _profitableTrades++;
    }
    else if(oldPnl>0 && pnl<=0)
    {
        _profitableTrades--;
    }

    //emit performanceUpdatedOnTrade(_strategyId, _profitableTrades, _unRealizedGrossPnL + _realizedGrossPnL);
}

void PerformanceManager::updatePerformance(const double oldTradeProfit, const double newTradeProfit, const double oldLastPrice, const double lastPrice, const double avgFillPrice)
{
    //update Profit
    if(oldLastPrice>avgFillPrice && lastPrice<avgFillPrice)
    {
        _profitableTrades--;
    }
    else if(oldLastPrice<avgFillPrice && lastPrice>avgFillPrice)
    {
       _profitableTrades++;
    }
    _unRealizedGrossPnL += newTradeProfit-oldTradeProfit;

    //emit performanceUpdatedOnTrade(_strategyId, _profitableTrades, _unRealizedGrossPnL + _realizedGrossPnL);
}

void PerformanceManager::updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability)
{
    if(currentProfitability && !lastProfitability)
    {
        _profitableTrades++;
    }
    else if(!currentProfitability && lastProfitability)
    {
        _profitableTrades--;
    }

    _unRealizedGrossPnL += pnl;
     //emit performanceUpdatedOnTrade(_strategyId, _profitableTrades, _unRealizedGrossPnL+_realizedGrossPnL);
}

void PerformanceManager::bookPnL(const double pnl)
{
    _realizedGrossPnL+=pnl;
    //emit performanceUpdatedOnTrade(_strategyId, _profitableTrades, _unRealizedGrossPnL + _realizedGrossPnL);
}






