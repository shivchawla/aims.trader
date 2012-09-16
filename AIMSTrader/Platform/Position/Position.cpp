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
#include "Data/strategylinkedpositiondata.h"
#include "Platform/Commission/CommissionFactory.h"

/*
 * default constructor
 */
Position::~Position()
{}

Position::Position()
{
   _tickerId = 0;
   _strategyId = 0;
   initialize();
}

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

Position::Position(const Position& pos)
{
    _chgTotalCommission = pos._chgTotalCommission;
    _chgValueBought = pos._chgValueBought;
    _chgValueSold = pos._chgValueSold;
    _chgRealizedPnl = pos._chgRealizedPnl;
    _chgRunningPnl = pos._chgRunningPnl;
    _chgProfit = pos._chgProfit;
    _chgLoss = pos._chgLoss;

    _tickerId = pos._tickerId;
    _strategyId = pos._strategyId;

    _netPositionDetail = pos._netPositionDetail;
    _currentPositionDetail = pos._currentPositionDetail;
    _status = pos._status;
    _lastUpdated = pos._lastUpdated;
}

/*
 * initialize the positon intral variabes with defaults
 */
void Position::initialize()
{
    _chgTotalCommission = 0;
    _chgValueBought = 0;
    _chgValueSold = 0;
    _chgRealizedPnl = 0;
    _chgRunningPnl = 0;

    _chgProfit = 0;
    _chgLoss = 0;
    _status = Open;
    _newTrade = 0;

    _netPositionDetail.reset();
    _currentPositionDetail.reset();
    _lastUpdated = QDateTime::currentMSecsSinceEpoch();
}

Position::Position(const TickerId tickerId, const StrategyId strategyId, const StrategyLinkedPositionData* data)
{
    _tickerId = tickerId;
    _strategyId = strategyId;


    _lastUpdated = data->updatedDate.toMSecsSinceEpoch();
    //_markedPrice = 0;
}

/*
 * Updates a position with new trade price
 */
void Position::update(const TickType tickType, const double currentPrice)
{
    _lastUpdated = QDateTime::currentMSecsSinceEpoch();

    double runningPnl = _netPositionDetail._runningPnl;

    double profit = _currentPositionDetail._profit;
    double loss = _currentPositionDetail._loss;

    _currentPositionDetail.update(tickType, currentPrice);
    _netPositionDetail.update(tickType, currentPrice);

    _chgRunningPnl = _netPositionDetail._runningPnl - runningPnl;
    _chgProfit = _currentPositionDetail._profit - profit;
    _chgLoss = _currentPositionDetail._loss - loss;
}

void Position::update(const OrderId orderId, const Execution& execution, const double commission)
{
     _lastUpdated = QDateTime::currentMSecsSinceEpoch();
     int quantity = execution.shares;
     double fillPrice = execution.price;
    //_time = executionStatus.execution.time;

     double totalCommission = _netPositionDetail._totalCommission;
     double totalBought = _netPositionDetail._totalValueBought;
     double totalSold = _netPositionDetail._totalValueSold;
     double runningPnl = _netPositionDetail._runningPnl;
     double realizedPnl = _netPositionDetail._realizedPnl;

    _netPositionDetail.update(orderId, execution, commission);

    _chgTotalCommission = _netPositionDetail._totalCommission - totalCommission;
    _chgValueBought = _netPositionDetail._totalValueBought - totalBought;
    _chgValueSold = _netPositionDetail._totalValueSold - totalSold;
    _chgRunningPnl = _netPositionDetail._runningPnl - runningPnl;
    _chgRealizedPnl= _netPositionDetail._realizedPnl - realizedPnl;

    _currentPositionDetail.update(orderId, execution, commission);

}

void Position::update(const OrderId orderId, const int quantity, const double fillPrice, const double commission)
{
    double totalCommission = _netPositionDetail._totalCommission;
    double totalBought = _netPositionDetail._totalValueBought;
    double totalSold = _netPositionDetail._totalValueSold;
    double runningPnl = _netPositionDetail._runningPnl;
    double realizedPnl = _netPositionDetail._realizedPnl;

    _netPositionDetail.update(orderId, quantity, fillPrice, commission);

    _chgTotalCommission = _netPositionDetail._totalCommission - totalCommission;
    _chgValueBought = _netPositionDetail._totalValueBought - totalBought;
    _chgValueSold = _netPositionDetail._totalValueSold - totalSold;
    _chgRunningPnl = _netPositionDetail._runningPnl - runningPnl;
    _chgRealizedPnl= _netPositionDetail._realizedPnl - realizedPnl;

   if(_netPositionDetail.getNetShares()!=0)
   {
        _status = Open;
   }

   _newTrade = 0;
   if(_currentPositionDetail.getNetShares() == 0)
   {
       _currentPositionDetail.reset();
       _newTrade = (quantity > 0) ? 1 : 0;
   }

   double profit = _currentPositionDetail._profit;
   double loss = _currentPositionDetail._loss;

   _currentPositionDetail.update(orderId, quantity, fillPrice, commission);

   _chgProfit = _currentPositionDetail._profit - profit;
   _chgLoss = _currentPositionDetail._loss - loss;

}

PositionDetail::PositionDetail()
{
    reset();
}

PositionDetail::PositionDetail(const StrategyLinkedPositionData* data)
{
     reset();
    _sharesBought = data->numberBought;
    _sharesSold = data->numberSold;
    _avgBought = data->avgAmountBought;
    _avgSold = data->avgAmountSold;
    _totalValueBought = data->avgAmountBought * data->numberBought;
    _totalValueSold = data->avgAmountSold * data->numberSold;
    _totalCommission = data->totalAmountCommission;
    _realizedPnl = (_sharesBought >_sharesSold) ? _sharesSold * (_avgSold - _avgBought) : _sharesBought * (_avgBought - _avgSold);
    _runningPnl = 0;
    _netPnl = _realizedPnl + _runningPnl - _totalCommission;
}

PositionDetail::PositionDetail(const PositionDetail& positionDetail)
{
    _sharesBought = positionDetail._sharesBought;
    _sharesSold = positionDetail._sharesSold;
    _avgBought = positionDetail._avgBought;
    _avgSold = positionDetail._avgSold;
    _totalValueBought = positionDetail._totalValueBought;
    _totalValueSold = positionDetail._totalValueSold;
    _totalCommission = positionDetail._totalCommission;
    _realizedPnl = positionDetail._realizedPnl;
    _runningPnl = positionDetail._runningPnl;
    _markedPrice = positionDetail._markedPrice;
    _indicator = positionDetail._indicator;
    _netPnl = positionDetail._netPnl;
    _profit = positionDetail._profit;
    _loss = positionDetail._loss;
}

void PositionDetail::reset()
{
    _sharesBought = 0;
    _sharesSold = 0;
    _avgBought = 0;
    _avgSold = 0;
    _totalValueBought = 0;
    _totalValueSold = 0;
    _totalCommission = 0;
    _realizedPnl = 0;
    _runningPnl = 0;
    _netPnl = 0;
    _markedPrice = 0;
    _indicator = 0;
    _lastOrderId = 0;
    _profit = 0;
    _loss = 0;
}

void PositionDetail::update(const TickType tickType, const double currentPrice)
{
    double runningPnl = _runningPnl;
    double netShares = _sharesBought - _sharesSold;

    if(netShares>0 && tickType == ASK)
    {
        double avgPrice = (netShares*currentPrice + _sharesSold*_avgSold)/(_sharesBought);
        _runningPnl = netShares * (avgPrice - _avgBought);
    }
    else if(netShares<0 && tickType == BID)
    {
        double avgPrice = (-netShares*currentPrice + _sharesBought*_avgBought)/(_sharesSold);
        _runningPnl = netShares * (_avgSold-currentPrice);
    }

    double oldNetPnl = _netPnl;
    _netPnl = _runningPnl + _realizedPnl - _totalCommission;

    if(_netPnl > 0)
    {
        _profit = _netPnl;
        _loss = 0;
    }
    else if (_netPnl < 0)
    {
        _loss = -_netPnl;
        _profit = 0;
    }

    _indicator = 0;


    if(_netPnl >0 && oldNetPnl<=0)
    {
        _indicator = 1;
    }
    else if(_netPnl<0 && oldNetPnl>=0)
    {
        _indicator = -1;
    }
}

void PositionDetail::update(const OrderId orderId, const Execution& execution, const double commission)
{
    int quantity = execution.shares;
    double fillPrice = execution.price;
    double oldNetPnl = _netPnl;
    double runningPnl = _runningPnl;
    long netShares = _sharesBought - _sharesSold;

    if(execution.side == "BOT")
    {
        if(netShares<0 )
        {
            if(-netShares > quantity)
            {
                _realizedPnl += quantity * (_avgSold - fillPrice);
            }
            else
            {
                _realizedPnl += -netShares * (_avgSold - fillPrice);
            }
        }
        _avgBought = (quantity*fillPrice +_sharesBought*_avgBought)/(_sharesBought += quantity);
    }
    else
    {
        if(netShares>0)
        {
            if(netShares > quantity)
            {
                _realizedPnl += quantity * (fillPrice - _avgBought);
            }
            else
            {
                _realizedPnl += netShares * (fillPrice - _avgBought);
            }
        }
        _avgSold = (quantity*fillPrice + _sharesSold*_avgSold)/(_sharesSold += quantity);
    }

    netShares = _sharesBought - _sharesSold;

    _runningPnl = netShares* _markedPrice;

    //_chgRunningPnl = _runningPnl - runningPnl;
    _totalValueBought = _sharesBought * _avgBought ;
    _totalValueSold = _sharesSold * _avgSold ;

    _netPnl = _runningPnl + _realizedPnl - _totalCommission;

    _indicator = 0;
    if(_netPnl > 0 && oldNetPnl <= 0)
    {
        _indicator = 1;
    }
    else if(_netPnl < 0 && oldNetPnl >= 0)
    {
        _indicator = -1;
    }
}

void PositionDetail::update(const OrderId orderId, const int quantity, const double fillPrice, const double commission)
{
    double oldNetPnl = _netPnl;
    double runningPnl = _runningPnl;
    long netShares = _sharesBought - _sharesSold;
    double realizedPnl = _realizedPnl;

    //int absQuantity;
    if(quantity>0)
    {
        int absQuantity = quantity;
        _avgBought = (absQuantity*fillPrice +_sharesBought*_avgBought)/(_sharesBought += absQuantity);

        if(netShares < 0)
        {
            if(-netShares > absQuantity)
            {
                _realizedPnl = _sharesBought * (_avgSold - _avgBought);
            }
            else
            {
                _realizedPnl = _sharesSold * (_avgSold - _avgBought);
            }
        }
    }
    else
    {
        int absQuantity = -quantity;
        _avgSold = (absQuantity*fillPrice + _sharesSold*_avgSold)/(_sharesSold += absQuantity);

        if(netShares>0)
        {
            if(netShares > absQuantity)
            {
                _realizedPnl = _sharesSold * (_avgSold - _avgBought);
            }
            else
            {
                _realizedPnl = _sharesBought * (_avgSold - _avgBought);
            }
        }
    }

     netShares = _sharesBought - _sharesSold;
    _runningPnl = netShares * _markedPrice;
    _totalValueBought = _sharesBought * _avgBought ;
    _totalValueSold = _sharesSold * _avgSold ;

    if(_lastOrderId == orderId)
    {
         //then modify the commission
         double lastOrderCommission = _lastOrderCommission;
        _lastOrderCommission = commission;
        _totalCommission += commission - lastOrderCommission;
    }
    else
    {
        _lastOrderId = orderId;
        _lastOrderCommission = commission;
        _totalCommission += commission;
    }

    _netPnl = _runningPnl + _realizedPnl - _totalCommission;

//    if(_sharesBought == _sharesSold && _sharesBought>200)
//    {
//       double netPnl = _totalValueSold - _totalValueBought - _totalCommission;
//       int k=1;
//    }


    if(_netPnl > 0)
    {
        _profit = _netPnl;
        _loss = 0;
    }
    else if (_netPnl < 0)
    {
        _loss = -_netPnl;
        _profit = 0;
    }

    _indicator = 0;
    if(_netPnl >0 && oldNetPnl<=0)
    {
        _indicator = 1;
    }
    else if(_netPnl<0 && oldNetPnl>=0)
    {
        _indicator = -1;
    }
}
