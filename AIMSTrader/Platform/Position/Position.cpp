/*
 *  Position.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/28/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Position/Position.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Performance/PerformanceManager.h"

Position::Position(const Contract& contract, PerformanceManager* performanceManager):_contract(contract), _performanceManager(performanceManager)
{ 
    _isPositionClosed = false;
}

Position::~Position()
{}

const Contract& Position::getContract()
{
	return _contract;
}

const double Position::getQuantity()
{
	return _quantity;
}

const double Position::getAvgFillPrice()
{
	return _avgFillPrice;
}

const double Position::getTime()
{
	return _time;
}

const bool Position::IsPositionClosed()
{
    return _isPositionClosed;
}

void Position::updatePosition(const Execution& execution)
{
    _avgFillPrice = execution.avgPrice;
    int oldQuantity=_quantity;
    _quantity = execution.shares;

    if(oldQuantity==0 && _quantity>0)
    {
        _performanceManager->updateLongTrades();
    }
    else if(oldQuantity==0 && _quantity<0)
    {
        _performanceManager->updateShortTrades();
    }


    double oldPositionValue = _positionValue;
    _positionValue =_currentPrice*_quantity;

    double oldTradeProfit = _tradeProfit;
    _tradeProfit = (_currentPrice-_avgFillPrice)*_quantity;
    _timeInMarketStart=execution.time;
    //_tradeCommission;

    _performanceManager->updatePerformance(oldPositionValue, _positionValue, oldTradeProfit, _tradeProfit);
}

void Position::updatePosition(const double currentPrice)
{
    _currentPrice = currentPrice;
    _tradeProfit = (_currentPrice-_avgFillPrice)*_quantity;
    _positionValue = _currentPrice*_quantity;
    //_timeInMarket
}

