/*
 *  PerformaceManager.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Performance/PerformanceManager.h"

//ctor and dtor
PerformanceManager::PerformanceManager(Strategy* strategyWPtr):_strategyWPtr(strategyWPtr)
{
    initialize();
}

PerformanceManager::~PerformanceManager()
{}

const double PerformanceManager::getGrossPnL() const
{
    return _grossPnL;
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
    _grossPnL = _netPnL = _peakNetProfit = _maxDrawdown = 0;
    _sumTradeProfit = _sumTradeProfitSquared = 0;
}

void PerformanceManager::updatePerformance()
{}

void PerformanceManager::updatePerformance(const double oldPositionValue, const double newPositionValue, const double oldTradeProfit, const double newTradeProfit)
{
    if(oldTradeProfit<=0 && newTradeProfit>0)
    {
        _profitableTrades++;
    }
    else if(oldTradeProfit>0 && newTradeProfit<0)
    {
        _profitableTrades--;
    }

    _grossPnL += newTradeProfit-oldTradeProfit;
    _netPnL = _grossPnL - _totalCommission;

    //PEAK NET PROFIT
    if(_grossPnL>_peakNetProfit)
    {
       _peakNetProfit = _grossPnL;
    }
    //DRAWDOWN
}

void PerformanceManager::updateLongTrades()
{
    _longTrades++;
    _trades++;
}

void PerformanceManager::updateShortTrades()
{
    _shortTrades++;
    _trades++;
}


