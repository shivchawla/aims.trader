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
    return _strategyPerformanceStats.runningGrossPnL;
}

const double PerformanceManager::getProfitFactor()
{
	double profitFactor=0;
	//calculate profitfactor
	return profitFactor;
}

const long PerformanceManager::getNumberTrades() const
{
    return _strategyPerformanceStats.trades;
}

const double PerformanceManager::getRatio() const
{
    return 1;
}

//Utility
const double PerformanceManager::getBias()
{
    if (_strategyPerformanceStats.trades == 0) {
		return 0;
	}
    return 100 * (_strategyPerformanceStats.longTrades - _strategyPerformanceStats.shortTrades) / (double) _strategyPerformanceStats.trades;
}

const double PerformanceManager::getPercentProfitableTrades()
{
    return (_strategyPerformanceStats.profitableTrades/_strategyPerformanceStats.trades)*100;
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
     _strategyPerformanceStats.runningGrossPnL += position->getChgRunningPnl();
     _strategyPerformanceStats.netPnL = _strategyPerformanceStats.realizedGrossPnL + _strategyPerformanceStats.runningGrossPnL - _strategyPerformanceStats.totalCommission;

     int indicator = position->getCurrentPositionDetail().getIndicator();

     if(_strategyPerformanceStats.profitableTrades>0)
     {
         _strategyPerformanceStats.profitableTrades += indicator;
     }
     else if(_strategyPerformanceStats.profitableTrades == 0 && indicator>0)
     {
         _strategyPerformanceStats.profitableTrades++;
     }

     _strategyPerformanceStats.peakNetProfit = (_strategyPerformanceStats.netPnL > _strategyPerformanceStats.peakNetProfit) ? _strategyPerformanceStats.netPnL : _strategyPerformanceStats.peakNetProfit;
     _strategyPerformanceStats.drawDown =_strategyPerformanceStats.peakNetProfit > 0 ? 100.0 * (_strategyPerformanceStats.peakNetProfit - _strategyPerformanceStats.netPnL)/ _strategyPerformanceStats.peakNetProfit  : 0;
     _strategyPerformanceStats.maxDrawdown = (_strategyPerformanceStats.drawDown > _strategyPerformanceStats.maxDrawdown ) ? _strategyPerformanceStats.drawDown : _strategyPerformanceStats.maxDrawdown;

     StrategyOutput::strategyOutput().updatePerformance(_strategyId, _strategyPerformanceStats);
     //_outputInterface->updateStrategy(_strategyId, _performanceStats);
}

void PerformanceManager::updatePerformanceForExecution(const Position* position)
{
    _strategyPerformanceStats.totalBought += position->getChgValueBought();
    _strategyPerformanceStats.totalSold += position->getChgValueSold();
    _strategyPerformanceStats.totalCommission += position->getChgTotalCommission();

    double oldNetPnl = _strategyPerformanceStats.netPnL;

    _strategyPerformanceStats.realizedGrossPnL += position->getChgRealizedPnl();
    _strategyPerformanceStats.runningGrossPnL += position->getChgRunningPnl();
    _strategyPerformanceStats.netPnL = _strategyPerformanceStats.realizedGrossPnL + _strategyPerformanceStats.runningGrossPnL - _strategyPerformanceStats.totalCommission;

    int indicator = position->getCurrentPositionDetail().getIndicator();

    int newTrade = position->getNewTrade();
    if(newTrade>0)
    {
        _strategyPerformanceStats.profitableTrades++;
        _strategyPerformanceStats.trades++;
        _strategyPerformanceStats.longTrades++;
    }
    else if(newTrade < 0)
    {
        _strategyPerformanceStats.profitableTrades++;
        _strategyPerformanceStats.trades++ ;
        _strategyPerformanceStats.shortTrades++;
    }

    if(_strategyPerformanceStats.profitableTrades > 0)
    {
        _strategyPerformanceStats.profitableTrades += indicator;
    }
    else if(_strategyPerformanceStats.profitableTrades == 0 && indicator > 0)
    {
        _strategyPerformanceStats.profitableTrades++;
    }

    _strategyPerformanceStats.peakNetProfit = (_strategyPerformanceStats.netPnL > _strategyPerformanceStats.peakNetProfit) ? _strategyPerformanceStats.netPnL : _strategyPerformanceStats.peakNetProfit;
    _strategyPerformanceStats.drawDown =_strategyPerformanceStats.peakNetProfit > 0 ? 100.0 * (_strategyPerformanceStats.peakNetProfit - _strategyPerformanceStats.netPnL)/ _strategyPerformanceStats.peakNetProfit  : 0;
    _strategyPerformanceStats.maxDrawdown = (_strategyPerformanceStats.drawDown > _strategyPerformanceStats.maxDrawdown ) ? _strategyPerformanceStats.drawDown : _strategyPerformanceStats.maxDrawdown  ;

   StrategyOutput::strategyOutput().updatePerformance(_strategyId, _strategyPerformanceStats);
 }

void PerformanceManager::loadPosition(const Position* position)
{
    _strategyId = _strategyWPtr->getStrategyId();

    _strategyPerformanceStats.totalBought += position->getChgValueBought();
    _strategyPerformanceStats.totalSold += position->getChgValueSold();
    _strategyPerformanceStats.totalCommission += position->getChgTotalCommission();

    double oldNetPnl = _strategyPerformanceStats.netPnL;

   _strategyPerformanceStats.realizedGrossPnL += position->getChgRealizedPnl();
   _strategyPerformanceStats.runningGrossPnL += position->getChgRunningPnl();

   _strategyPerformanceStats.netPnL = _strategyPerformanceStats.realizedGrossPnL + _strategyPerformanceStats.runningGrossPnL - _strategyPerformanceStats.totalCommission;
   //_strategyPerformanceStats.profitableTrades += position->getCurrentPositionDetail().getIndicator();
   _strategyPerformanceStats.peakNetProfit = (_strategyPerformanceStats.netPnL > _strategyPerformanceStats.peakNetProfit) ? _strategyPerformanceStats.netPnL : _strategyPerformanceStats.peakNetProfit;

   _strategyPerformanceStats.drawDown = _strategyPerformanceStats.peakNetProfit > 0 ? 100.0 * (_strategyPerformanceStats.peakNetProfit - _strategyPerformanceStats.netPnL)/ _strategyPerformanceStats.peakNetProfit  : 0;
   _strategyPerformanceStats.maxDrawdown = (_strategyPerformanceStats.drawDown > _strategyPerformanceStats.maxDrawdown ) ? _strategyPerformanceStats.drawDown : _strategyPerformanceStats.maxDrawdown  ;

    StrategyOutput::strategyOutput().updatePerformance(_strategyId, _strategyPerformanceStats, GUI);
}

