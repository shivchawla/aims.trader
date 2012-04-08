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

/*Position::Position(const Contract& contract, PerformanceManager* performanceManager):_contract(contract), _performanceManager(performanceManager)
{ 
    _isPositionClosed = false;
}*/

Position::~Position()
{}

Position::Position(const TickerId tickerId):_tickerId(tickerId)
{}

/*const Contract& Position::getContract()
{
	return _contract;
}*/

const double Position::getQuantity() const
{
	return _quantity;
}

const double Position::getAvgFillPrice() const
{
	return _avgFillPrice;
}

const String& Position::getTime() const
{
	return _time;
}

const bool Position::IsPositionClosed() const
{
    return _isPositionClosed;
}

const double Position::getLastPrice() const
{
    return _lastPrice;
}

const double Position::getPositionValue() const
{
    return _quantity*_lastPrice;
}

const TickerId Position::getTickerId() const
{
    return _tickerId;
}

/*void Position::updatePosition(const Execution& execution)
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
}*/

void Position::updatePosition(const ExecutionStatus& executionStatus)
{
    double incomingAvgPrice = executionStatus.execution.avgPrice ;
    double incomingQuantity = executionStatus.execution.shares;
    _quantity = executionStatus.execution.cumQty;
    _avgFillPrice = (_avgFillPrice*_quantity + incomingQuantity*incomingAvgPrice)/_quantity;
    _time = executionStatus.execution.time;
}

void Position::updatePosition(const double currentPrice)
{
    _lastPrice = currentPrice;
    _tradeProfit = (_lastPrice-_avgFillPrice)*_quantity;
    _positionValue = _lastPrice*_quantity;
    //_timeInMarket
}

