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
#include <algorithm>
#include "Data/strategylinkedpositiondata.h"
#include "Platform/Commission/CommissionFactory.h"
#include <algorithm>

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
    //_currentPositionDetail = pos._currentPositionDetail;
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
    //_currentPositionDetail.reset();
    _lastUpdated = QDateTime::currentMSecsSinceEpoch();
    //_currentReturn = 0;
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

    double runningPnl = _netPositionDetail.getRunningPnl();

    double profit = _netPositionDetail.getTotalProfit();
    double loss = _netPositionDetail.getTotalLoss();

 //   _currentPositionDetail.update(tickType, currentPrice);
    _netPositionDetail.update(tickType, currentPrice);

    _chgRunningPnl = _netPositionDetail.getRunningPnl() - runningPnl;
    _chgProfit = _netPositionDetail.getTotalProfit() - profit;
    _chgLoss = _netPositionDetail.getTotalLoss() - loss;

//    long netShares = _netPositionDetail._sharesBought - _netPositionDetail._sharesSold;
//    double value = (netShares > 0) ? netShares * _netPositionDetail._transitAvgBuyPrice : -netShares*_netPositionDetail._transitAvgSellPrice;

     //_currentReturn = (value!=0) ? _netPositionDetail.getRunningPnl()*100/value : 0;
}

void Position::update(const OrderId orderId, const int quantity, const double fillPrice, const double avgFillPrice, const double commission)
{
    double totalCommission = _netPositionDetail._totalCommission;
    double totalBought = _netPositionDetail._totalValueBought;
    double totalSold = _netPositionDetail._totalValueSold;
    double runningPnl = _netPositionDetail.getRunningPnl();
    double realizedPnl = _netPositionDetail.getRealizedPnl();
    double profit = _netPositionDetail.getTotalProfit();
    double loss = _netPositionDetail.getTotalLoss();

    _netPositionDetail.update(orderId, quantity, fillPrice, commission);

    _chgTotalCommission = _netPositionDetail._totalCommission - totalCommission;
    _chgValueBought = _netPositionDetail._totalValueBought - totalBought;
    _chgValueSold = _netPositionDetail._totalValueSold - totalSold;
    _chgRunningPnl = _netPositionDetail.getRunningPnl() - runningPnl;
    _chgRealizedPnl= _netPositionDetail.getRealizedPnl() - realizedPnl;

   if(_netPositionDetail.getNetShares()!=0)
   {
        _status = Open;
   }

//   _newTrade = 0;

//   if(_currentPositionDetail.getNetShares() == 0)
//   {
//       _currentPositionDetail.reset();
//       _newTrade = (quantity > 0) ? 1 : 0;
//   }

   //_currentPositionDetail.update(orderId, quantity, fillPrice, commission);

   _chgProfit = _netPositionDetail.getTotalProfit() - profit;
   _chgLoss = _netPositionDetail.getTotalLoss() - loss;

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
    //_realizedPnl = (_sharesBought >_sharesSold) ? _sharesSold * (_avgSold - _avgBought) : _sharesBought * (_avgBought - _avgSold);
    //_runningPnl = 0;
    //_netPnl = _realizedPnl + _runningPnl - _totalCommission;
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
    //_realizedPnl = positionDetail._realizedPnl;
    //_runningPnl = positionDetail._runningPnl;
    _markedPrice = positionDetail._markedPrice;
    _indicator = positionDetail._indicator;
    //_netPnl = positionDetail._netPnl;
    //_profit = positionDetail._profit;
    //_loss = positionDetail._loss;
    _isNewLeg = positionDetail._isNewLeg;
    _createdTime = positionDetail._createdTime;
    _transitAvgBuyPrice = positionDetail._transitAvgBuyPrice;
    _transitAvgSellPrice = positionDetail._transitAvgSellPrice;
    _runningProfit = positionDetail._runningProfit;
    _runningLoss = positionDetail._runningLoss;
    _realizedProfit = positionDetail._realizedProfit;
    _realizedLoss = positionDetail._realizedLoss;

}

void PositionDetail::reset()
{
    _transitAvgBuyPrice = 0;
    _transitAvgSellPrice = 0;
    _sharesBought = 0;
    _sharesSold = 0;
    _avgBought = 0;
    _avgSold = 0;
    _totalValueBought = 0;
    _totalValueSold = 0;
    _totalCommission = 0;
    //_realizedPnl = 0;
    //_runningPnl = 0;
    _realizedProfit = 0;
    _realizedLoss = 0;
    _runningProfit = 0;
    _runningLoss = 0;
    //_netPnl = 0;
    _markedPrice = 0;
    _indicator = 0;
    _latestLegOrderId = 0;
    //_profit = 0;
    //_loss = 0;
    _isNewLeg = true;
    _createdTime = QDateTime::currentDateTime();
}

//void PositionDetail::swap(const PositionDetail& detail)
//{
//    PositionDetail* tmp = const_cast<PositionDetail*>(&detail);

//    //std::swap(this, tmp);
//    swap(&this, &tmp);
//    delete tmp;
//}

//void PositionDetail::swap(PositionDetail** a, PositionDetail** b)
//{
//    PositionDetail* tmp = *a;
//    *a = *b;
//    *b = *tmp;
//}

void PositionDetail::update(const TickType tickType, const double currentPrice)
{
    double netShares = _sharesBought - _sharesSold;
    double runningPnL = 0;
    if(netShares>0 && tickType == ASK)
    {
        //double avgPrice = (netShares*currentPrice + _sharesSold*_avgSold)/(_sharesBought);
        runningPnL = netShares * (currentPrice - _transitAvgBuyPrice);
    }
    else if(netShares<0 && tickType == BID)
    {
        //double avgPrice = (-netShares*currentPrice + _sharesBought*_avgBought)/(_sharesSold);
        runningPnL = netShares * (currentPrice - _transitAvgSellPrice);
    }

    _markedPrice = currentPrice;
    _runningProfit = (runningPnL>=0) ? runningPnL : 0;
    _runningLoss = (runningPnL<0) ? -runningPnL : 0;


    //double oldNetPnl = _netPnl;
    //_netPnl = _runningPnl + _realizedPnl - _totalCommission;

//    if(_netPnl > 0)
//    {
//        _profit = _netPnl;
//        _loss = 0;
//    }
//    else if (_netPnl < 0)
//    {
//        _loss = -_netPnl;
//        _profit = 0;
//    }

//    _indicator = 0;

//    if(_netPnl >0 && oldNetPnl<=0)
//    {
//        _indicator = 1;
//    }
//    else if(_netPnl<0 && oldNetPnl>=0)
//    {
//        _indicator = -1;
//    }
}

void PositionDetail::update(const OrderId orderId, const int quantity, const double fillPrice, const double commission)
{
    if(orderId != _latestLegOrderId)
    {
        _isNewLeg = true;
        _latestLegSharesBought = 0;
        _latestLegBuyPrice = 0;
        _latestLegSharesSold = 0;
        _latestLegSellPrice = 0;
        _latestLegOrderCommission = commission;
        _latestLegOrderId = orderId;
    }
    else
    {
        _isNewLeg = false;
    }

    if(quantity>0)
    {
        _latestLegBuyPrice = fillPrice ;
        _latestLegSharesBought += quantity;
    }
    else
    {
        _latestLegSharesSold -= quantity;
        _latestLegSellPrice = fillPrice;
    }

    //double oldNetPnl = _netPnl;
    long netShares = _sharesBought - _sharesSold;
    double realizedPnl = 0;
    if(quantity>0)
    {
        int absQuantity = quantity;
        _avgBought = (absQuantity*fillPrice +_sharesBought*_avgBought)/(_sharesBought += absQuantity);

        if(netShares < 0)
        {
            if(-netShares > absQuantity)
            {
                realizedPnl = _sharesBought * (_avgSold - _avgBought);
            }
            else
            {
                realizedPnl = _sharesSold * (_avgSold - _avgBought);
            }
        }
        else
        {
            _transitAvgBuyPrice = (_transitAvgBuyPrice*netShares + quantity*fillPrice)/(netShares + quantity);
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
                realizedPnl = _sharesSold * (_avgSold - _avgBought);
            }
            else
            {
                realizedPnl = _sharesBought * (_avgSold - _avgBought);
            }
        }
        else
        {
             _transitAvgSellPrice = (-_transitAvgSellPrice*netShares + absQuantity*fillPrice)/(-netShares + absQuantity);
        }
    }

     netShares = _sharesBought - _sharesSold;

     _realizedProfit = (realizedPnl>=0) ? realizedPnl : 0;
     _realizedLoss = (realizedPnl<0) ? -realizedPnl : 0;

    double runningPnl = (_markedPrice!=0) ? ((netShares>0) ? netShares * (_markedPrice - _transitAvgBuyPrice) : netShares * (_transitAvgSellPrice - _markedPrice)) : 0;
    _runningProfit = (runningPnl>=0) ? runningPnl : 0;
    _runningLoss = (runningPnl<0) ? -runningPnl : 0;

    _totalValueBought = _sharesBought * _avgBought ;
    _totalValueSold = _sharesSold * _avgSold ;

    if(!_isNewLeg)
    {
         //then modify the commission
         double lastOrderCommission = _latestLegOrderCommission;
        _latestLegOrderCommission = commission;
        _totalCommission += commission - lastOrderCommission;
    }
    else
    {
        //_latestLegOrderId = orderId;
        _latestLegOrderCommission = commission;
        _totalCommission += commission;
    }

    //_netPnl = _runningPnl + _realizedPnl - _totalCommission;

//    if(_netPnl > 0)
//    {
//        _profit = _netPnl;
//        _loss = 0;
//    }
//    else if (_netPnl < 0)
//    {
//        _loss = -_netPnl;
//        _profit = 0;
//    }

//    _indicator = 0;
//    if(_netPnl >0 && oldNetPnl<=0)
//    {
//        _indicator = 1;
//    }
//    else if(_netPnl<0 && oldNetPnl>=0)
//    {
//        _indicator = -1;
//    }
}
