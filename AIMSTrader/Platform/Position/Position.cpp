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
#include "Data/positiondata.h"
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
    _lastUpdated = QDateTime::currentDateTime();
}

Position::Position(const TickerId tickerId, const StrategyId strategyId, const PositionData& data)
{
    _tickerId = tickerId;
    _strategyId = strategyId;
    _lastUpdated = data.updatedDate;
}

void Position::update(const OrderId orderId, const OrderDetail& orderDetail)
{
    _lastUpdated = QDateTime::currentDateTime();

    long quantity = orderDetail.getLastFilledShares();
    long filledShares = orderDetail.getOrderAction().compare("SELL") == 0 ? -quantity : quantity;
    double fillPrice = orderDetail.getLastFillPrice();
    double commission = orderDetail.getCommission();

    PositionDetail oldPositionDetail = _netPositionDetail;

    _netPositionDetail.update(orderId, filledShares, fillPrice, commission);

    _chgTotalCommission = _netPositionDetail._totalCommission - oldPositionDetail._totalCommission;
    _chgValueBought = _netPositionDetail._totalValueBought - oldPositionDetail._totalValueBought;
    _chgValueSold = _netPositionDetail._totalValueSold - oldPositionDetail._totalValueSold;
    _chgRunningPnl = _netPositionDetail.getRunningPnl() - oldPositionDetail.getRunningPnl();
    _chgRealizedPnl= _netPositionDetail.getRealizedPnl() - oldPositionDetail.getRealizedPnl();

   if(_netPositionDetail.getNetShares()!=0)
   {
        _status = Open;
   }

   _chgProfit = _netPositionDetail.getTotalProfit() - oldPositionDetail.getTotalProfit();
   _chgLoss = _netPositionDetail.getTotalLoss() - oldPositionDetail.getTotalLoss();

}

/*
 * Updates a position with new trade price
 */
void Position::update(const TickType tickType, const double currentPrice)
{
    if(currentPrice<=0)
    {
        return;
    }

    double runningPnl = _netPositionDetail.getRunningPnl();

    double profit = _netPositionDetail.getTotalProfit();
    double loss = _netPositionDetail.getTotalLoss();

    _netPositionDetail.update(tickType, currentPrice);

    _chgRunningPnl = _netPositionDetail.getRunningPnl() - runningPnl;
    _chgProfit = _netPositionDetail.getTotalProfit() - profit;
    _chgLoss = _netPositionDetail.getTotalLoss() - loss;

}

void Position::update(const OrderId orderId, const int quantity, const double fillPrice, const double avgFillPrice, const double commission)
{
    _lastUpdated = QDateTime::currentDateTime();

    PositionDetail oldPositionDetail = _netPositionDetail;

    _netPositionDetail.update(orderId, quantity, fillPrice, commission);

    _chgTotalCommission = _netPositionDetail._totalCommission - oldPositionDetail._totalCommission;
    _chgValueBought = _netPositionDetail._totalValueBought - oldPositionDetail._totalValueBought;
    _chgValueSold = _netPositionDetail._totalValueSold - oldPositionDetail._totalValueSold;
    _chgRunningPnl = _netPositionDetail.getRunningPnl() - oldPositionDetail.getRunningPnl();
    _chgRealizedPnl= _netPositionDetail.getRealizedPnl() - oldPositionDetail.getRealizedPnl();

   if(_netPositionDetail.getNetShares()!=0)
   {
        _status = Open;
   }

   _chgProfit = _netPositionDetail.getTotalProfit() - oldPositionDetail.getTotalProfit();
   _chgLoss = _netPositionDetail.getTotalLoss() - oldPositionDetail.getTotalLoss();
}

PositionDetail::PositionDetail()
{
    reset();
}

PositionDetail::PositionDetail(const PositionData& data)
{
     reset();
    _sharesBought = data.sharesBought;
    _sharesSold = data.sharesSold;
    _avgBuyPrice = data.avgBuyPrice;
    _avgSellPrice = data.avgSellPrice;
    _totalValueBought = data.avgBuyPrice * data.sharesBought;
    _totalValueSold = data.avgSellPrice * data.sharesSold;
    _totalCommission = data.totalAmountCommission;
}


void PositionDetail::reset()
{
    _transitAvgBuyPrice = 0;
    _transitAvgSellPrice = 0;
    _sharesBought = 0;
    _sharesSold = 0;
    _avgBuyPrice = 0;
    _avgSellPrice = 0;
    _totalValueBought = 0;
    _totalValueSold = 0;
    _totalCommission = 0;
    _realizedProfit = 0;
    _realizedLoss = 0;
    _runningProfit = 0;
    _runningLoss = 0;
    _markedPrice = 0;
    _indicator = 0;
    _latestLegOrderId = 0;
    _isNewLeg = true;
    _createdTime = QDateTime::currentDateTime();
    _netValue = 0;
}

void PositionDetail::update(const TickType tickType, const double currentPrice)
{
    double netShares = _sharesBought - _sharesSold;
    double runningPnL = 0;
    if(netShares>0 && tickType == ASK)
    {
        runningPnL = netShares * (currentPrice - _transitAvgBuyPrice);
        _runningProfit = (runningPnL>=0) ? runningPnL : 0;
        _runningLoss = (runningPnL<0) ? -runningPnL : 0;

    }
    else if(netShares<0 && tickType == BID)
    {
        runningPnL = netShares * (currentPrice - _transitAvgSellPrice);
        _runningProfit = (runningPnL>=0) ? runningPnL : 0;
        _runningLoss = (runningPnL<0) ? -runningPnL : 0;
    }

    _markedPrice = currentPrice;
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

    long netShares = _sharesBought - _sharesSold;
    double realizedPnl = 0;
    if(quantity>0)
    {
        int absQuantity = quantity;
        _avgBuyPrice = (absQuantity*fillPrice +_sharesBought*_avgBuyPrice)/(_sharesBought += absQuantity);

        if(netShares < 0)
        {
            if(-netShares > absQuantity)
            {
                realizedPnl = _sharesBought * (_avgSellPrice - _avgBuyPrice);
            }
            else
            {
                realizedPnl = _sharesSold * (_avgSellPrice - _avgBuyPrice);
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
        _avgSellPrice = (absQuantity*fillPrice + _sharesSold*_avgSellPrice)/(_sharesSold += absQuantity);

        if(netShares>0)
        {
            if(netShares > absQuantity)
            {
                realizedPnl = _sharesSold * (_avgSellPrice - _avgBuyPrice);
            }
            else
            {
                realizedPnl = _sharesBought * (_avgSellPrice - _avgBuyPrice);
            }
        }
        else
        {
             _transitAvgSellPrice = (-_transitAvgSellPrice*netShares + absQuantity*fillPrice)/(-netShares + absQuantity);
        }
    }

     netShares = _sharesBought - _sharesSold;;

     _netValue = netShares > 0 ? netShares*_transitAvgBuyPrice : netShares*_transitAvgSellPrice;

    _realizedProfit = (realizedPnl>=0) ? realizedPnl : 0;
    _realizedLoss = (realizedPnl<0) ? -realizedPnl : 0;

    double runningPnl = (_markedPrice!=0) ? ((netShares>0) ? netShares * (_markedPrice - _transitAvgBuyPrice) : netShares * (_transitAvgSellPrice - _markedPrice)) : 0;
    _runningProfit = (runningPnl>=0) ? runningPnl : 0;
    _runningLoss = (runningPnl<0) ? -runningPnl : 0;

    _totalValueBought = _sharesBought * _avgBuyPrice ;
    _totalValueSold = _sharesSold * _avgSellPrice ;

    if(!_isNewLeg)
    {
         //then modify the commission
         double lastOrderCommission = _latestLegOrderCommission;
        _latestLegOrderCommission = commission;
        _totalCommission += commission - lastOrderCommission;
    }
    else
    {
        _latestLegOrderCommission = commission;
        _totalCommission += commission;
    }
}
