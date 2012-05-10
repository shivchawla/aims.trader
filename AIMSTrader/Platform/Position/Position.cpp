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
#include <math.h>

<<<<<<< HEAD
/*
 * default constructor
 */
Position::~Position()
{}

/*
 *
 */
Position::Position(const TickerId tickerId):_tickerId(tickerId)
{
    initialize();
}

/*
 *
 */
Position::Position(const TickerId tickerId, const StrategyId strategyId):_tickerId(tickerId),_strategyId(strategyId)
{
    initialize();
}

/*
 * initialize the positon intral variabes with defaults
 */
void Position::initialize()
{
    _isPositionClosed = false;
    _positionValue=0;
    _quantity=0;
    _avgFillPrice=0;
    _lastPrice=0;
    _tradeCommission=0;
    _tradeProfit=0;
    _status = InActive;
}
=======
/*Position::Position(const Contract& contract, PerformanceManager* performanceManager):_contract(contract), _performanceManager(performanceManager)
{ 
    _isPositionClosed = false;
}*/
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

/*
 * returns the quantity of the position
 */
const int Position::getQuantity() const
{
    mutex.lock();
    int quantity = _quantity;
    mutex.unlock();
    return quantity;
}

<<<<<<< HEAD
/*
 * returns the average fill price of trade
 */
const double Position::getAvgFillPrice() const
{
    mutex.lock();
    double avgPrice = _avgFillPrice;
    mutex.unlock();
    return avgPrice;
}

/*
 *
 */
const String Position::getTime() const
=======
Position::Position(const TickerId tickerId):_tickerId(tickerId)
{}

/*const Contract& Position::getContract()
{
	return _contract;
}*/

const double Position::getQuantity() const
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    mutex.lock();
    String time = _time;
    mutex.unlock();
    return time;
}

<<<<<<< HEAD
/*
 * returns the current status of position
 */
const bool Position::IsPositionClosed() const
=======
const double Position::getAvgFillPrice() const
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    mutex.lock();
    bool closed = _isPositionClosed;
    mutex.unlock();
    return closed;
}

<<<<<<< HEAD
/*
 * returns the last price of associated instrument
 */
const double Position::getLastPrice() const
=======
const String& Position::getTime() const
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    mutex.lock();
    double lastPrice = _lastPrice;
    mutex.unlock();
    return lastPrice;
}

<<<<<<< HEAD
/*
 * returns the position value
 */
const double Position::getPositionValue() const
=======
const bool Position::IsPositionClosed() const
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    mutex.lock();
    double positionValue = _quantity*_lastPrice;
    mutex.unlock();
    return positionValue;
}

<<<<<<< HEAD
/*
 * return the tickerId of the instrument
 */
const TickerId Position::getTickerId() const
{

    mutex.lock();
    TickerId tickerId = _tickerId;
    mutex.unlock();

    return tickerId;
}

/*
 *returns the id of associated strategy
 */
const StrategyId Position::getStrategyId() const
{
    return _strategyId;
}

/*
 *returns the commission pad for this position
 */
const double Position::getCommission() const
{
    return _tradeCommission;
}

const PositionStatus Position::getPositionStatus() const
{
    return _status;
}

/*
 * returns the trade profit associated with position
 */
const double Position::getTradeProfit() const
=======
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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    return (_lastPrice!=0)?_quantity*(_lastPrice-_avgFillPrice):0;
}

/*
 *Updates a position with execution information
 */
const double Position::updatePosition(const ExecutionStatus& executionStatus, const bool isClosingPosition)
{
    double pnl;
    mutex.lock();
    int oldFillPrice = _avgFillPrice;
    int oldQuantity = _quantity;
    //update quantity;
    if(executionStatus.execution.side=="BOT")
    {
        _quantity += executionStatus.execution.shares;
    }
    else
    {
        _quantity -= executionStatus.execution.shares;
    }

    //update fill price
    if(!isClosingPosition)
    {
        _avgFillPrice = (executionStatus.execution.price * executionStatus.execution.shares + oldFillPrice*abs(oldQuantity))/abs(_quantity);
    }

    //close the position when the trade is meant to close the psotion
    //and updated quantity = 0
    if(isClosingPosition && _quantity==0)
    {
        //_isPositionClosed = true;
        _status = Closed;
    }

    _time = QString::fromStdString(executionStatus.execution.time);
    pnl = isClosingPosition? (executionStatus.execution.price - _avgFillPrice) * executionStatus.execution.shares : 0;
    mutex.unlock();

    return pnl;
 //   _positionValue = _quantity * _lastPrice;
}

<<<<<<< HEAD
/*
 * Updates a positon with new trade price
 */
const double Position::updatePosition(const double currentPrice)
{
    double tradeProfit=0;
    mutex.lock();
    _lastPrice = currentPrice;
    tradeProfit = _tradeProfit = (_lastPrice-_avgFillPrice)*_quantity;
    //_positionValue = _lastPrice*_quantity;
    mutex.unlock();
    return tradeProfit;
=======
    _performanceManager->updatePerformance(oldPositionValue, _positionValue, oldTradeProfit, _tradeProfit);
}*/

void Position::updatePosition(const ExecutionStatus& executionStatus)
{
    double incomingAvgPrice = executionStatus.execution.avgPrice ;
    double incomingQuantity = executionStatus.execution.shares;
    _quantity = executionStatus.execution.cumQty;
    _avgFillPrice = (_avgFillPrice*_quantity + incomingQuantity*incomingAvgPrice)/_quantity;
    _time = executionStatus.execution.time;
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

void Position::updateStatus(const PositionStatus status)
{
<<<<<<< HEAD
   mutex.lock();
   _status =  status;
   mutex.unlock();
=======
    _lastPrice = currentPrice;
    _tradeProfit = (_lastPrice-_avgFillPrice)*_quantity;
    _positionValue = _lastPrice*_quantity;
    //_timeInMarket
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}


