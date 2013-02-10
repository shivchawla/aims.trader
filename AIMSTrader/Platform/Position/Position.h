/*
 *  Position.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/28/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#pragma once
#ifndef Position_h
#define Position_h

#include "AimsTraderDefs/typedefs.h"
#include <QMutex>
#include <QDateTime>
#include <QHash>

class PositionData;
class Spread;
typedef long SpreadId;

//When execution comes, it either updates an open position or opens a new position
//Also, it updates the net position
//say you had 100 shares long and 50 short...a net of 50 (this will be reflected in the current position detail)
//say price = 50 and then price jumps to 51....profit =50*1 = 50
//again price jumps to 50.5, profit = 50*0.5 = 25
//but performance has a profit of 50 associated with this position.
//how do we modify it, we send it a change in performance
//running pnl is always associated with a current position

//PositionId TickerId StrategyId
//1 IBM 0
//1 MSFT 0
//2 MSFT 0
//2 ORCL 0
//2 GOOG 0

class Position
{
    friend class Spread;
    friend class PositionDetail;
    private:
        TickerId _tickerId;
        StrategyId _strategyId;

        PositionDetail _netPositionDetail;
        PositionStatus _status;
        QDateTime _lastUpdated;

        double _chgTotalCommission;
        double _chgValueBought;
        double _chgValueSold;
        double _chgRealizedPnl;
        double _chgRunningPnl;
        double _chgProfit;
        double _chgLoss;

     private:
        QList<Position const *> _childPositions;
        Position* _parentPosition;


    public:
        const StrategyId getStrategyId() const{return _strategyId;}
        const TickerId getTickerId() const {return _tickerId;}

        const PositionDetail& getNetPositionDetail() const
        {
            return _netPositionDetail;
        }
//        //const int getNewTrade() const {return _newTrade;}

        const PositionStatus getStatus() const {return _status;}
        void setStatus(const PositionStatus status) {_status = status;}

        const QDateTime lastUpdated(){
            return _lastUpdated;
        }

        const double getAmountInvested()
        {
            return (_netPositionDetail.sharesBought > _netPositionDetail.sharesSold)
                    ? (_netPositionDetail.sharesBought - _netPositionDetail.sharesSold) * _netPositionDetail.avgBuyPrice
                    : (-_netPositionDetail.sharesBought + _netPositionDetail.sharesSold) * _netPositionDetail.avgSellPrice;
        }

    public:
        const long getSharesBought() const{return _netPositionDetail.sharesBought;}
        const long getSharesSold() const{return _netPositionDetail.sharesSold;}
        const double getAvgBought() const {return _netPositionDetail.avgBuyPrice; }
        const double getAvgSold() const{return _netPositionDetail.avgSellPrice;}
        const double getTotalValueBought() const{return _netPositionDetail.totalValueBought;}
        const double getTotalValueSold() const{return _netPositionDetail.totalValueSold;}
        const double getNetValue() const
        {
            return _netPositionDetail.netValue;
        }
        const double getTotalCommission() const{return _netPositionDetail.totalCommission;}
        const double getRealizedPnl() const{return _netPositionDetail.realizedProfit - _netPositionDetail.realizedLoss;}
        const double getRealizedLoss() const {return _netPositionDetail.realizedLoss;}
        const double getRealizedProfit() const {return _netPositionDetail.realizedProfit;}
        const double getRunningProfit() const {return _netPositionDetail.runningProfit;}
        const double getRunningLoss() const {return _netPositionDetail.runningLoss;}
        const double getRunningPnl() const{return _netPositionDetail.runningProfit - _netPositionDetail.runningLoss;}
        const double getTotalProfit() const{return _netPositionDetail.runningProfit + _netPositionDetail.realizedProfit;}
        const double getTotalLoss() const{return _netPositionDetail.runningLoss + _netPositionDetail.realizedLoss;}

        const double getNetPnL() const{return getGrossPnL() - _netPositionDetail.totalCommission;}
        const double getGrossPnL() const{return getRealizedPnl() + getRunningPnl();}
        const double getNetTotalIncCommission() const{return _netPositionDetail.totalValueSold-_netPositionDetail.totalValueBought-_netPositionDetail.totalCommission;}
        const double getMarkedPrice() const { return _netPositionDetail.markedPrice;}
        const int getIndicator() const {return _netPositionDetail.indicator;}

        const double getReturn() const
        {
            double absNetValue = abs(_netPositionDetail.netValue);
            return (absNetValue > 0 ) ? getRunningPnl() * 100/absNetValue : 0;
        }

        const long getNetShares() const {return _netPositionDetail.sharesBought - _netPositionDetail.sharesSold;}

//        const double getTotalProfit() const {return _netPositionDetail._realizedProfit+_netPositionDetail._runningProfit;}
//        const double getTotalLoss() const {return _netPositionDetail._realizedLoss+_netPositionDetail._runningLoss;}

        const long getLatestLegQuantityBought() const {return _netPositionDetail.latestLegSharesBought;}
        const long getLatestLegQuantitySold() const {return _netPositionDetail.latestLegSharesSold;}
        const double getLatestLegBuyPrice() const {return _netPositionDetail.latestLegBuyPrice;}
        const double getLatestLegSellPrice() const {return _netPositionDetail.latestLegSellPrice;}
        const double getLatestLegCommission() const {return _netPositionDetail.latestLegOrderCommission;}
        const double IsNewLeg() const {return _netPositionDetail.isNewLeg;}
        const QDateTime getCreatedTime() const {return _netPositionDetail.createdTime;}

        const double getChgValueBought() const {return _chgValueBought;}
        const double getChgValueSold() const {return _chgValueSold;}
        const double getChgRunningPnl() const {return _chgRunningPnl;}
        const double getChgRealizedPnl() const {return _chgRealizedPnl;}

        const double getChgTotalCommission() const {return _chgTotalCommission;}

        const double getChgProfit() const {return _chgProfit;}
        const double getChgLoss() const {return _chgLoss;}


    public:
        void addChildPosition(const Position*);
        void update(const TickType, const double lastPrice);
        void update(const OrderId, const int quantity, const double fillPrice, const double avgFillPrice, const double commission);
        void update(const OrderId, const OrderDetail&);
        void setTickerId(const TickerId tickerId)
        {
            _tickerId = tickerId;
        }

        void setStrategyId(const StrategyId strategyId)
        {
            _strategyId = strategyId;
        }

	public:
        Position(const TickerId);
        Position(const TickerId, const StrategyId);
        Position(const Position&);
        Position(const Position*);
        Position(const TickerId, const StrategyId, const PositionData& data);
        Position();
        ~Position();

    private:
        void initialize();
};

class SpreadPosition : public Position
{
        Spread* _parentSpread;
    public:
        SpreadPosition(Spread*);
        SpreadPosition(const TickerId, const StrategyId, Spread*);

};

//class SpreadDetail
//{
//    friend class Spread;
//    private:
//        SpreadId _spreadId;
//        double _totalValueBought;
//        double _totalValueSold;
//        double _netValue;
//        double _totalCommission;
//        double _realizedProfit;
//        double _realizedLoss;
//        double _runningProfit;
//        double _runningLoss;
//        double _drawDown;
//        double _peakValue;
//        QDateTime _createdTime;
//};

class Spread
{
    private:
        StrategyId _strategyId;
        SpreadId _spreadId;
        SpreadPosition* _firstPosition;
        SpreadPosition* _secondPosition;

    private:
        SpreadDetail _spreadDetail;

    private:
        void initialize();

    public:
        Spread(const TickerId, const TickerId, const StrategyId);
        Spread(const SpreadId, const StrategyId);
        Spread(const SpreadId,const TickerId, const TickerId, const StrategyId);
        Spread();
        ~Spread();

    public:
        void update(const OrderId, const TickerId, const OrderDetail&);
        void update(const OrderId, const OrderDetail&);

        void update(const TickerId, const TickType, const double);
        SpreadPosition* getFirstPosition() const;
        SpreadPosition* getSecondPosition() const;
        SpreadDetail getSpreadDetail() const{
            return _spreadDetail;}

        StrategyId getStrategyId() const{
            return _strategyId;
        }

        SpreadId getSpreadId() const{
            return _spreadId;
        }
};



#endif
