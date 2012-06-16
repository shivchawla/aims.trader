/*
 *  Position.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/28/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Position/Position.h"
#include <math.h>

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

/*Position::Position(const Position* pos)
{
    _strategyId = pos->getStrategyId();
    _tickerId = pos->getTickerId();
    _sharesBought = pos->;
    _sharesSold;
   _netShares;
    _avgBought;
    _avgSold;
    _totalValueBought;
    _totalValueSold;
    _totalCommision;
    _realizedPnl;
    _runningPnl;
}*/

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
    _oldSharesBought = 0;
    _oldSharesSold = 0;
    _oldNetShares=0;
    _oldAvgBought = 0;
    _oldAvgSold = 0;
    _oldTotalValueBought = 0;
    _oldTotalValueSold = 0;
    _oldTotalCommision = 0;
    _oldRealizedPnl = 0;
    _oldRunningPnl = 0;

    _sharesBought = 0;
    _sharesSold = 0;
    _netShares=0;
    _avgBought = 0;
    _avgSold = 0;
    _totalValueBought = 0;
    _totalValueSold = 0;
    _totalCommision = 0;
    _realizedPnl = 0;
    _runningPnl = 0;

}


///*
// * returns the quantity of the position
// */
//const int Position::getQuantity()
//{
//    mutex.lock();
//    int quantity = _quantity;
//    mutex.unlock();
//    return quantity;
//}


///*
// * returns the average fill price of trade
// */
//const double Position::getAvgFillPrice()
//{
//    mutex.lock();
//    double avgPrice = _avgFillPrice;
//    mutex.unlock();
//    return avgPrice;
//}

//const String Position::getTime()
//{
//    mutex.lock();
//    String time = _time;
//    mutex.unlock();
//    return time;
//}

///*
// * returns the current status of position
// */
//const bool Position::IsPositionClosed()
//{
//    mutex.lock();
//    bool closed = _isPositionClosed;
//    mutex.unlock();
//    return closed;
//}

///*
// * returns the last price of associated instrument
// */
//const double Position::getLastPrice()
//{
//    mutex.lock();
//    double lastPrice = _lastPrice;
//    mutex.unlock();
//    return lastPrice;
//}

///*
// * returns the position value
// */
//const double Position::getPositionValue()
//{
//    mutex.lock();
//    double positionValue = _quantity*_lastPrice;
//    mutex.unlock();
//    return positionValue;
//}


///*
// * return the tickerId of the instrument
// */
//const TickerId Position::getTickerId()
//{

//    mutex.lock();
//    TickerId tickerId = _tickerId;
//    mutex.unlock();

//    return tickerId;
//}

///*
// *returns the id of associated strategy
// */
//const StrategyId Position::getStrategyId()
//{
//    return _strategyId;
//}

///*
// *returns the commission pad for this position
// */
//const double Position::getCommission()
//{
//    return _tradeCommission;
//}

//const PositionStatus Position::getPositionStatus()
//{
//    return _status;
//}

///*
// * returns the trade profit associated with position
// */
//const double Position::getTradeProfit()
//{
//    return _tradeProfit;
//}

///*
// *Updates a position with execution information
// */
//const double Position::updatePosition(const ExecutionStatus& executionStatus, const bool isClosingPosition)
//{
//    double pnl;
//    mutex.lock();
//    int oldFillPrice = _avgFillPrice;
//    int oldQuantity = _quantity;
//    //update quantity;
//    if(executionStatus.execution.side=="BOT")
//    {
//        _quantity += executionStatus.execution.shares;
//    }
//    else
//    {
//        _quantity -= executionStatus.execution.shares;
//    }

//    //update fill price
//    if(!isClosingPosition)
//    {
//        _avgFillPrice = (executionStatus.execution.price * executionStatus.execution.shares + oldFillPrice*abs(oldQuantity))/abs(_quantity);
//    }

//    //close the position when the trade is meant to close the psotion
//    //and updated quantity = 0
//    if(isClosingPosition && _quantity==0)
//    {
//        //_isPositionClosed = true;
//        _status = Closed;
//    }

//    _time = QString::fromStdString(executionStatus.execution.time);
//    pnl = isClosingPosition? (executionStatus.execution.price - _avgFillPrice) * executionStatus.execution.shares : 0;
////    mutex.unlock();

////    return pnl;
//// //   _positionValue = _quantity * _lastPrice;
////}
/*
 * Updates a positon with new trade price
 */
void Position::update(const double currentPrice)
{
    _oldRunningPnl = _runningPnl;
    if(_netShares>0)
    {
        _runningPnl = _netShares * (currentPrice - _avgBought);
    }
    else
    {
        _runningPnl = _netShares * (_avgSold-currentPrice);
    }
}

void Position::update(const Execution& execution)
{

    int quantity = execution.shares;
     double fillPrice = execution.price;
    //_time = executionStatus.execution.time;
    _oldAvgBought = _avgBought;
    _oldSharesBought = _sharesBought;
    _oldAvgSold = _avgSold;
    _oldSharesSold = _sharesSold;
    _oldNetShares = _netShares;

    _oldTotalValueBought = _totalValueBought;

    _oldTotalValueSold = _totalValueSold;

    _oldRealizedPnl = _realizedPnl;
    _oldRunningPnl = _runningPnl;

    if(execution.side == "BOT")
    {
        if(_netShares<0 )
        {
            if(abs(_netShares) > quantity)
            {
                _realizedPnl += quantity * (_avgSold - fillPrice);
            }
            else
            {
                _realizedPnl += abs(_netShares) * (_avgSold - fillPrice);
            }
        }
        _avgBought = (quantity*fillPrice +_sharesBought*_avgBought)/(_sharesBought += quantity);
    }
    else
    {
        if(_netShares>0)
        {
            if(_netShares > quantity)
            {
                _realizedPnl += quantity * (fillPrice - _avgBought);
            }
            else
            {
                _realizedPnl += _netShares * (fillPrice - _avgBought);
            }
        }
        _avgSold = (quantity*fillPrice + _sharesSold*_avgSold)/(_sharesSold += quantity);
    }

    _netShares = _sharesBought - _sharesSold;

    if(_netShares == 0)
    {
        _runningPnl = 0;

    }
    _totalValueBought = _sharesBought * _avgBought ;
    _totalValueSold = _sharesSold * _avgSold ;

}


