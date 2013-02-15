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
   _parentPosition = NULL;
   _tickerId = 0;
   _strategyId = 0;
   initialize();
}

/*
 *
 */
Position::Position(const TickerId tickerId):_tickerId(tickerId)
{
    _parentPosition = NULL;
    initialize();
}

/*
 *
 */
Position::Position(const TickerId tickerId, const StrategyId strategyId):_tickerId(tickerId),_strategyId(strategyId)
{
    _parentPosition = NULL;
    initialize();
}

Position::Position(const Position* position)
{

    _tickerId = position->_tickerId;
    _strategyId = position->_strategyId;
     _netPositionDetail = position->_netPositionDetail;
     _status = position->_status;

    //initialize();
}

Position::Position(const Position& pos)
{
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
    _status = Open;
    //newTrade = 0;

    _netPositionDetail.reset();
    _lastUpdated = QDateTime::currentDateTime();

    _chgTotalCommission = 0;
    _chgValueBought = 0;
    _chgValueSold = 0;
    _chgRealizedPnl = 0;
    _chgRunningPnl = 0;
    _chgProfit = 0;
    _chgLoss = 0;
}

Position::Position(const TickerId tickerId, const StrategyId strategyId, const PositionData& data)
{
    initialize();
    _tickerId = tickerId;
    _strategyId = strategyId;
    _netPositionDetail.sharesBought = data.sharesBought;
    _netPositionDetail.sharesSold = data.sharesSold;
    _netPositionDetail.avgBuyPrice = data.avgBuyPrice;
    _netPositionDetail.avgSellPrice = data.avgSellPrice;
    _netPositionDetail.totalCommission = data.commission;
    _netPositionDetail.createdTime = data.createdDate;
    _lastUpdated = data.updatedDate;

}

void Position::update(const OrderId orderId, const OrderDetail& orderDetail)
{
    _lastUpdated = QDateTime::currentDateTime();

    long quantity = orderDetail.lastFilledShares;
    long filledShares = orderDetail.order.action.compare("SELL") == 0 ? -quantity : quantity;
    double fillPrice = orderDetail.lastFillPrice;
    double commission = orderDetail.commission;

    double oldTotalCommission = getTotalCommission();
    double oldTotalValueBought= getTotalValueBought();
    double oldTotalValueSold = getTotalValueSold();
    double oldRunningPnl = getRunningPnl();
    double oldRealizedPnl = getRealizedPnl();
    double oldTotalProfit = getTotalProfit();
    double oldTotalLoss = getTotalLoss();

    _netPositionDetail.update(orderId, filledShares, fillPrice, commission);

    _chgTotalCommission = getTotalCommission() - oldTotalCommission;
    _chgValueBought = getTotalValueBought() - oldTotalValueBought;
    _chgValueSold = getTotalValueSold() - oldTotalValueSold;
    _chgRunningPnl = getRunningPnl() - oldRunningPnl;
    _chgRealizedPnl= getRealizedPnl() - oldRealizedPnl;
    _chgProfit = getTotalProfit() - oldTotalProfit;
    _chgLoss = getTotalLoss() - oldTotalLoss;

    if(getNetShares()!=0)
    {
        _status = Open;
    }
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

    double oldRunningPnl = getRunningPnl();
    double oldProfit = getTotalProfit();
    double oldLoss = getTotalLoss();

    _netPositionDetail.update(tickType, currentPrice);

    _chgRunningPnl = getRunningPnl() - oldRunningPnl;
    _chgProfit = getTotalProfit() - oldProfit;
    _chgLoss = getTotalLoss() - oldLoss;
}

void Position::update(const OrderId orderId, const int quantity, const double fillPrice, const double avgFillPrice, const double commission)
{
    _lastUpdated = QDateTime::currentDateTime();

    double oldTotalCommission = getTotalCommission();
    double oldTotalValueBought= getTotalValueBought();
    double oldTotalValueSold = getTotalValueSold();
    double oldRunningPnl = getRunningPnl();
    double oldRealizedPnl = getRealizedPnl();
    double oldTotalProfit = getTotalProfit();
    double oldTotalLoss = getTotalLoss();

    _netPositionDetail.update(orderId, quantity, fillPrice, commission);

    _chgTotalCommission = getTotalCommission() - oldTotalCommission;
    _chgValueBought = getTotalValueBought() - oldTotalValueBought;
    _chgValueSold = getTotalValueSold() - oldTotalValueSold;
    _chgRunningPnl = getRunningPnl() - oldRunningPnl;
    _chgRealizedPnl= getRealizedPnl() - oldRealizedPnl;
    _chgProfit = getTotalProfit() - oldTotalProfit;
    _chgLoss = getTotalLoss() - oldTotalLoss;

   if(getNetShares()!=0)
   {
        _status = Open;
   }
}

void Position::addPosition(const PositionData& data)
{
    _netPositionDetail.addPositionDetail(data);
}


void Position::addChildPosition(const Position* position)
{
    _childPositions.append(position);
}

PositionDetail::PositionDetail()
{
    reset();
}

PositionDetail::PositionDetail(const PositionDetail& detail)
{
    transitAvgBuyPrice = detail.transitAvgBuyPrice;
    transitAvgSellPrice = detail.transitAvgSellPrice;
    sharesBought = detail.sharesBought;
    sharesSold = detail.sharesSold;
    avgBuyPrice = detail.avgBuyPrice;
    avgSellPrice = detail.avgSellPrice;
    totalValueBought = detail.totalValueBought;
    totalValueSold = detail.totalValueSold;
    totalCommission = detail.totalCommission;
    realizedProfit = detail.realizedProfit;
    realizedLoss = detail.realizedLoss;
    runningProfit = detail.runningProfit;
    runningLoss = detail.runningLoss;
    markedPrice = detail.markedPrice;
    indicator = detail.indicator;
    latestLegOrderId = detail.latestLegOrderId;
    isNewLeg = detail.isNewLeg;
    createdTime = detail.createdTime;
    netValue = detail.netValue;
    latestLegSharesBought = detail.latestLegSharesBought;
    latestLegSharesSold = detail.latestLegSharesSold;
    latestLegBuyPrice = detail.latestLegBuyPrice;
    latestLegSellPrice = detail.latestLegSellPrice;
    latestLegOrderCommission = detail.latestLegOrderCommission;
    drawDown = detail.drawDown;
    peakPositionValue = detail.peakPositionValue;
}

//PositionDetail::PositionDetail(const PositionData& data)
//{
//     reset();
//    sharesBought = data.sharesBought;
//    sharesSold = data.sharesSold;
//    avgBuyPrice = data.avgBuyPrice;
//    avgSellPrice = data.avgSellPrice;
//    totalValueBought = data.avgBuyPrice * data.sharesBought;
//    totalValueSold = data.avgSellPrice * data.sharesSold;
//    totalCommission = data.totalAmountCommission;
//    chgTotalCommission = data.chgTotalCommission;
//    chgValueBought = data.chgValueBought;
//    chgValueSold = data.chgValueSold;
//    chgRealizedPnl = data.chgRealizedPnl;
//    chgRunningPnl = pos.chgRunningPnl;
//    chgProfit = pos.chgProfit;
//    chgLoss = pos.chgLoss;

//}

void PositionDetail::reset()
{
    transitAvgBuyPrice = 0;
    transitAvgSellPrice = 0;
    sharesBought = 0;
    sharesSold = 0;
    avgBuyPrice = 0;
    avgSellPrice = 0;
    totalValueBought = 0;
    totalValueSold = 0;
    totalCommission = 0;
    realizedProfit = 0;
    realizedLoss = 0;
    runningProfit = 0;
    runningLoss = 0;
    markedPrice = 0;
    indicator = 0;
    latestLegOrderId = 0;
    latestLegSharesBought = 0;
    latestLegSharesSold = 0;
    latestLegBuyPrice = 0;
    latestLegSellPrice = 0;
    latestLegOrderCommission = 0;
    isNewLeg = true;
    createdTime = QDateTime::currentDateTime();
    netValue = 0;
    drawDown = 0;
    peakPositionValue = 0;
}

void PositionDetail::update(const TickType tickType, const double currentPrice)
{
    double netShares = sharesBought - sharesSold;
    double runningPnL = 0;
    if(netShares>0 && tickType == ASK)
    {
        runningPnL = netShares * (currentPrice - transitAvgBuyPrice);
        runningProfit = (runningPnL>=0) ? runningPnL : 0;
        runningLoss = (runningPnL<0) ? -runningPnL : 0;
        markedPrice = currentPrice;
    }
    else if(netShares<0 && tickType == BID)
    {
        runningPnL = netShares * (currentPrice - transitAvgSellPrice);
        runningProfit = (runningPnL>=0) ? runningPnL : 0;
        runningLoss = (runningPnL<0) ? -runningPnL : 0;
        markedPrice = currentPrice;
    }
    //double pnl = runningPnL + getRealizedPnl();

    //peakProfit =
    //drawDown = peakProfit > 0 ? 100.0 * (peakProfit - pnl)/ peakProfit  : 0;

}

void PositionDetail::update(const OrderId orderId, const int quantity, const double fillPrice, const double commission)
{
    if(orderId != latestLegOrderId)
    {
        isNewLeg = true;
        latestLegSharesBought = 0;
        latestLegBuyPrice = 0;
        latestLegSharesSold = 0;
        latestLegSellPrice = 0;
        latestLegOrderCommission = commission;
        latestLegOrderId = orderId;
    }
    else
    {
        isNewLeg = false;
    }

    if(quantity>0)
    {
        latestLegBuyPrice = fillPrice ;
        latestLegSharesBought += quantity;
    }
    else
    {
        latestLegSharesSold -= quantity;
        latestLegSellPrice = fillPrice;
    }

    long netShares = sharesBought - sharesSold;
    double realizedPnl = 0;
    if(quantity>0)
    {
        int absQuantity = quantity;
        avgBuyPrice = (absQuantity*fillPrice +sharesBought*avgBuyPrice)/(sharesBought += absQuantity);

        if(netShares < 0)
        {
            if(-netShares > absQuantity)
            {
                realizedPnl = sharesBought * (avgSellPrice - avgBuyPrice);
            }
            else
            {
                realizedPnl = sharesSold * (avgSellPrice - avgBuyPrice);
            }
        }
        else
        {
            transitAvgBuyPrice = (transitAvgBuyPrice*netShares + quantity*fillPrice)/(netShares + quantity);
        }
    }
    else
    {
        int absQuantity = -quantity;
        avgSellPrice = (absQuantity*fillPrice + sharesSold*avgSellPrice)/(sharesSold += absQuantity);

        if(netShares>0)
        {
            if(netShares > absQuantity)
            {
                realizedPnl = sharesSold * (avgSellPrice - avgBuyPrice);
            }
            else
            {
                realizedPnl = sharesBought * (avgSellPrice - avgBuyPrice);
            }
        }
        else
        {
             transitAvgSellPrice = (-transitAvgSellPrice*netShares + absQuantity*fillPrice)/(-netShares + absQuantity);
        }
    }

     netShares = sharesBought - sharesSold;;

     netValue = netShares > 0 ? netShares*transitAvgBuyPrice : netShares*transitAvgSellPrice;

    realizedProfit = (realizedPnl>=0) ? realizedPnl : 0;
    realizedLoss = (realizedPnl<0) ? -realizedPnl : 0;

    double runningPnl = (markedPrice!=0) ? ((netShares>0) ? netShares * (markedPrice - transitAvgBuyPrice) : netShares * (transitAvgSellPrice - markedPrice)) : 0;
    runningProfit = (runningPnl>=0) ? runningPnl : 0;
    runningLoss = (runningPnl<0) ? -runningPnl : 0;

    totalValueBought = sharesBought * avgBuyPrice ;
    totalValueSold = sharesSold * avgSellPrice ;

    //peakPositionValue = std::max(netValue, )

    if(!isNewLeg)
    {
         //then modify the commission
         double lastOrderCommission = latestLegOrderCommission;
        latestLegOrderCommission = commission;
        totalCommission += commission - lastOrderCommission;
    }
    else
    {
        latestLegOrderCommission = commission;
        totalCommission += commission;
    }
}

void PositionDetail::addPositionDetail(const PositionData& data)
{
    long valueBought = sharesBought*avgBuyPrice;
    long valueSold = sharesSold*avgSellPrice;

    sharesBought += data.sharesBought;
    sharesSold += data.sharesSold;
    avgBuyPrice = (valueBought + data.sharesBought*data.avgBuyPrice)/sharesBought;
    avgSellPrice = (valueSold + data.sharesSold*data.avgSellPrice)/sharesSold;
    totalValueBought = sharesBought*avgBuyPrice;
    totalValueSold = sharesSold*avgSellPrice;
    totalCommission += data.commission;
}


SpreadDetail::SpreadDetail(const SpreadDetail& detail)
{
    spreadId = detail.spreadId;
    totalValueBought = detail.totalValueBought;
    totalValueSold = detail.totalValueSold;
    netValue = detail.netValue;
    totalCommission = detail.totalCommission;
    realizedProfit = detail.realizedProfit;
    realizedLoss = detail.realizedLoss;
    runningProfit = detail.runningProfit;
    runningLoss = detail.runningLoss;
    drawDown = detail.drawDown;
    peakValue = detail.peakValue;
    createdTime = detail.createdTime;
}

SpreadDetail::SpreadDetail()
{
    spreadId = 0;
    totalValueBought = 0;
    totalValueSold = 0;
    netValue = 0;
    totalCommission = 0;
    realizedProfit = 0;
    realizedLoss = 0;
    runningProfit = 0;
    runningLoss = 0;
    drawDown = 0;
    peakValue = 0;
    createdTime = QDateTime();

}



