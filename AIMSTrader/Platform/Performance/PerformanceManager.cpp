/*
 *  PerformaceManager.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/26/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Performance/PerformanceManager.h"
#include "Platform/Strategy/Strategy.h"
#include <algorithm>
#include <Platform/Strategy/StrategyOutput.h>
#include <Platform/Position/Position.h>

//ctor and dtor
PerformanceManager::PerformanceManager(Strategy* strategyWPtr):_strategyWPtr(strategyWPtr)
{
    initialize();
    //_outputInterface = OutputInterface::Instance();
}

PerformanceManager::~PerformanceManager()
{}

const double PerformanceManager::getGrossPnL() const
{
    return _performanceStats.unRealizedGrossPnL;
}

const double PerformanceManager::getProfitFactor()
{
	double profitFactor=0;
	//calculate profitfactor
	return profitFactor;
}

const long PerformanceManager::getNumberTrades() const
{
    return _performanceStats.trades;
}

const double PerformanceManager::getRatio() const
{
    return 1;
}

//Utility
const double PerformanceManager::getBias()
{
    if (_performanceStats.trades == 0) {
		return 0;
	}
    return 100 * (_performanceStats.longTrades - _performanceStats.shortTrades) / (double) _performanceStats.trades;
}

const double PerformanceManager::getPercentProfitableTrades()
{
    return (_performanceStats.profitableTrades/_performanceStats.trades)*100;
}

const double PerformanceManager::getAverageProfitPerTrade()
{
	return 0;
}

//this function will be called everytime strategy needs to know its performance
void PerformanceManager::initialize()
{
    _strategyId = _strategyWPtr->getStrategyId();
}

void PerformanceManager::updatePerformanceForPrice(const Position* position)
{
    double oldPnl = position->getOldPnL();
    double pnl = position->getPnL();

    if(oldPnl<=0 && pnl>0)
    {
        _performanceStats.profitableTrades++;
    }
    else if(oldPnl>0 && pnl<=0)
    {
        _performanceStats.profitableTrades--;
    }

     _performanceStats.unRealizedGrossPnL += position->getRunningPnl() - position->getOldRunningPnl();

     double oldNetPnl = _performanceStats.netPnL;
     _performanceStats.netPnL += (pnl - oldPnl);

     _performanceStats.peakNetProfit = (_performanceStats.netPnL > oldNetPnl) ? _performanceStats.netPnL : oldNetPnl;


      _performanceStats.drawDown = 100.0 * (_performanceStats.peakNetProfit - _performanceStats.netPnL)/ std::max(0.0001, _performanceStats.peakNetProfit);
     _performanceStats.maxDrawdown = (_performanceStats.drawDown > _performanceStats.maxDrawdown ) ? _performanceStats.drawDown : _performanceStats.maxDrawdown  ;

     strategyOutput().updatePerformance(_strategyId, _performanceStats);
     //_outputInterface->updateStrategy(_strategyId, _performanceStats);
}

void PerformanceManager::updatePerformanceForExecution(const Position* position)
{
    _performanceStats.totalBought += position->getTotalValueBought() - position->getOldTotalValueBought();
    _performanceStats.totalSold += position->getTotalValueSold() - position->getOldTotalValueSold();
    _performanceStats.totalCommission += position->getTotalCommission() - position->getOldTotalCommission();

    double oldPnl = position->getOldPnL();
    double pnl = position->getPnL();

    _performanceStats.netPnL += pnl - oldPnl - _performanceStats.totalCommission;

    _performanceStats.realizedGrossPnL += position->getRealizedPnl() - position->getOldRealizedPnl();
    _performanceStats.unRealizedGrossPnL += position->getRunningPnl() - position->getOldRunningPnl();

    double oldNetPnl = _performanceStats.netPnL;
    _performanceStats.peakNetProfit = (_performanceStats.netPnL > oldNetPnl) ? _performanceStats.netPnL : oldNetPnl;

   _performanceStats.drawDown = 100.0 * (_performanceStats.peakNetProfit - _performanceStats.netPnL)/std::max(0.0001, _performanceStats.peakNetProfit);
   _performanceStats.maxDrawdown = (_performanceStats.drawDown > _performanceStats.maxDrawdown ) ? _performanceStats.drawDown : _performanceStats.maxDrawdown  ;

   strategyOutput().updatePerformance(_strategyId, _performanceStats);
   //_outputInterface->updateStrategy(_strategyId, _performanceStats);
}






