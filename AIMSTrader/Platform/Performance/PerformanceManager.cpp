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
     double oldNetPnl = _performanceStats.netPnL;

     double realizedPnl =  position->getRealizedPnl();
     double runningPnl =  position->getRunningPnl();
     double oldRunningPnl =  position->getOldRunningPnl();

    _performanceStats.unRealizedGrossPnL += runningPnl - oldRunningPnl;

    _performanceStats.netPnL = _performanceStats.realizedGrossPnL + _performanceStats.unRealizedGrossPnL - _performanceStats.totalCommission;

    if(oldRunningPnl + realizedPnl <=0 && runningPnl+realizedPnl > 0)
    {
        _performanceStats.profitableTrades++;
    }
    else if(oldRunningPnl + realizedPnl >=0 && runningPnl+realizedPnl < 0)
    {
        int profitableTrades = _performanceStats.profitableTrades;
        _performanceStats.profitableTrades = profitableTrades>0 ? profitableTrades-- : 0;
    }

    _performanceStats.peakNetProfit = (_performanceStats.netPnL > _performanceStats.peakNetProfit) ? _performanceStats.netPnL : _performanceStats.peakNetProfit;

     _performanceStats.drawDown =_performanceStats.peakNetProfit > 0 ? 100.0 * (_performanceStats.peakNetProfit - _performanceStats.netPnL)/ _performanceStats.peakNetProfit  : 0;
     _performanceStats.maxDrawdown = (_performanceStats.drawDown > _performanceStats.maxDrawdown ) ? _performanceStats.drawDown : _performanceStats.maxDrawdown  ;

     StrategyOutput::strategyOutput().updatePerformance(_strategyId, _performanceStats);
     //_outputInterface->updateStrategy(_strategyId, _performanceStats);
}


void PerformanceManager::updatePerformanceForExecution(const Position* position)
{
    _performanceStats.totalBought += position->getTotalValueBought() - position->getOldTotalValueBought();
    _performanceStats.totalSold += position->getTotalValueSold() - position->getOldTotalValueSold();
    _performanceStats.totalCommission += position->getTotalCommission() - position->getOldTotalCommission();

     double oldNetPnl = _performanceStats.netPnL;

     double realizedPnl =  position->getRealizedPnl();
     double oldRealizedPnl =  position->getOldRealizedPnl();
     double runningPnl =  position->getRunningPnl();
     double oldRunningPnl =  position->getOldRunningPnl();

    _performanceStats.realizedGrossPnL += realizedPnl - oldRealizedPnl;
    _performanceStats.unRealizedGrossPnL += runningPnl - oldRunningPnl;

    _performanceStats.netPnL = _performanceStats.realizedGrossPnL + _performanceStats.unRealizedGrossPnL - _performanceStats.totalCommission;

    if(oldRunningPnl + oldRealizedPnl <=0 && runningPnl+realizedPnl > 0)
    {
        _performanceStats.profitableTrades++;
    }
    else if(oldRunningPnl + oldRealizedPnl >=0 && runningPnl+realizedPnl < 0)
    {
        int profitableTrades = _performanceStats.profitableTrades;
        _performanceStats.profitableTrades = profitableTrades>0 ? --profitableTrades : 0;
    }

    if((position->getNetShares() && position->getOldNetShares() == 0) || (position->getOldNetShares()*position->getNetShares() < 0))
    {
      _performanceStats.trades++;
    }

   _performanceStats.peakNetProfit = (_performanceStats.netPnL > _performanceStats.peakNetProfit) ? _performanceStats.netPnL : _performanceStats.peakNetProfit;

    _performanceStats.drawDown =_performanceStats.peakNetProfit > 0 ? 100.0 * (_performanceStats.peakNetProfit - _performanceStats.netPnL)/ _performanceStats.peakNetProfit  : 0;
    _performanceStats.maxDrawdown = (_performanceStats.drawDown > _performanceStats.maxDrawdown ) ? _performanceStats.drawDown : _performanceStats.maxDrawdown  ;


   StrategyOutput::strategyOutput().updatePerformance(_strategyId, _performanceStats);
   //_outputInterface->updateStrategy(_strategyId, _performanceStats);
}

void PerformanceManager::loadPosition(const Position* position)
{
    _strategyId = _strategyWPtr->getStrategyId();

    _performanceStats.totalBought += position->getTotalValueBought() - position->getOldTotalValueBought();
    _performanceStats.totalSold += position->getTotalValueSold() - position->getOldTotalValueSold();
    _performanceStats.totalCommission += position->getTotalCommission() - position->getOldTotalCommission();

    double oldNetPnl = _performanceStats.netPnL;

    double realizedPnl =  position->getRealizedPnl();
    double oldRealizedPnl =  position->getOldRealizedPnl();
    double runningPnl =  position->getRunningPnl();
    double oldRunningPnl =  position->getOldRunningPnl();

   _performanceStats.realizedGrossPnL += realizedPnl - oldRealizedPnl;
   _performanceStats.unRealizedGrossPnL += runningPnl - oldRunningPnl;

   _performanceStats.netPnL = _performanceStats.realizedGrossPnL + _performanceStats.unRealizedGrossPnL - _performanceStats.totalCommission;

   if(oldRunningPnl + oldRealizedPnl <=0 && runningPnl+realizedPnl > 0)
   {
       _performanceStats.profitableTrades++;
   }
   else if(oldRunningPnl + oldRealizedPnl >=0 && runningPnl+realizedPnl < 0)
   {
       int profitableTrades = _performanceStats.profitableTrades;
       _performanceStats.profitableTrades = profitableTrades>0 ? profitableTrades-- : 0;
   }

   _performanceStats.peakNetProfit = (_performanceStats.netPnL > _performanceStats.peakNetProfit) ? _performanceStats.netPnL : _performanceStats.peakNetProfit;

   _performanceStats.drawDown = _performanceStats.peakNetProfit > 0 ? 100.0 * (_performanceStats.peakNetProfit - _performanceStats.netPnL)/ _performanceStats.peakNetProfit  : 0;
   _performanceStats.maxDrawdown = (_performanceStats.drawDown > _performanceStats.maxDrawdown ) ? _performanceStats.drawDown : _performanceStats.maxDrawdown  ;

    StrategyOutput::strategyOutput().updatePerformance(_strategyId, _performanceStats, GUI);
}






