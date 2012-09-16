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

class StrategyLinkedPositionData;

class PositionDetail
{
    friend class Position;
    private:
    long _sharesBought;
    long _sharesSold;
    double _avgBought;
    double _avgSold;
    double _totalValueBought;
    double _totalValueSold;
    double _totalCommission;
    double _lastOrderCommission;
    double _realizedPnl;
    double _runningPnl;
    double _netPnl;
    double _markedPrice;

    double _profit;
    double _loss;

    //indicator.i=0 if net pnl is same direction
    int _indicator;

    OrderId _lastOrderId;

    public:
        PositionDetail();
        PositionDetail(const StrategyLinkedPositionData*);
        PositionDetail(const PositionDetail&);

    public:
        void update(const TickType, const double lastPrice);
        void update(const OrderId, const int quantity, const double fillPrice, const double commission);
        void update(const OrderId, const Execution&, const double);
        void reset();

    public:
        const long getSharesBought() const{return _sharesBought;}
        const long getSharesSold() const{return _sharesSold;}
        const double getAvgBought() const {return _avgBought; }
        const double getAvgSold() const{return _avgSold;}
        const double getTotalValueBought() const{return _totalValueBought;}
        const double getTotalValueSold() const{return _totalValueSold;}
        const double getNetValue() const
        {
            long x;
            return ((x =_sharesBought-_sharesSold)>0) ? x*_avgBought : -x*_avgSold;
        }
        const double getTotalCommission() const{return _totalCommission;}
        const double getRealizedPnl() const{return _realizedPnl;}
        const double getRunningPnl() const{return _runningPnl;}
        const double getPnL() const{return _netPnl;}
        const double getNetTotalIncCommission() const{return _totalValueSold-_totalValueBought-_totalCommission;}
        const double getMarkedPrice() const { return _markedPrice;}
        const int getIndicator() const {return _indicator;}

        const double getReturn() const
        {
            return _runningPnl * 100/abs(_totalValueBought - _totalValueSold);
        }

        const long getNetShares() const {return _sharesBought - _sharesSold;}

        const long getTotalProfit() const {return _profit;}
        const long getTotalLoss() const {return _loss;}

};


//When execution comes, it either updates an open position or opens a new position
//Also, it updates the net position
//say you had 100 shares long and 50 short...a net of 50 (this will be reflected in the current position detail)
//say price = 50 and then price jumps to 51....profit =50*1 = 50
//again price jumps to 50.5, profit = 50*0.5 = 25
//but performance has a profit of 50 associated with this position.
//how do we modify it, we send it a change in performance
//running pnl is always associated with a current position

class Position 
{
    private:
        StrategyId _strategyId;
        TickerId _tickerId;

        PositionDetail _currentPositionDetail;
        PositionDetail _netPositionDetail;

        double _chgTotalCommission;
        double _chgValueBought;
        double _chgValueSold;
        double _chgRealizedPnl;
        double _chgRunningPnl;
        double _chgProfit;
        double _chgLoss;

        PositionStatus _status;

        int _newTrade;

        qint64 _lastUpdated;

        QMutex mutex;

    public:
        const StrategyId getStrategyId() const{return _strategyId;}
        const TickerId getTickerId() const {return _tickerId;}

        const PositionDetail& getNetPositionDetail() const
        {
            return _netPositionDetail;
        }

        const PositionDetail& getCurrentPositionDetail() const
        {
            return _currentPositionDetail;
        }

        const PositionStatus getStatus() const {return _status;}
        void setStatus(const PositionStatus status) {_status = status;}

        const double getChgValueBought() const {return _chgValueBought;}
        const double getChgValueSold() const {return _chgValueSold;}
        const double getChgRunningPnl() const {return _chgRunningPnl;}
        const double getChgRealizedPnl() const {return _chgRealizedPnl;}

        const double getChgTotalCommission() const {return _chgTotalCommission;}
        const int getNewTrade() const {return _newTrade;}

        const double getChgProfit() const {return _chgProfit;}
        const double getChgLoss() const {return _chgLoss;}

        const qint64 lastUpdated(){
            return _lastUpdated;
        }

    public:
        void update(const TickType, const double lastPrice);
        void update(const OrderId, const int quantity, const double fillPrice, const double commission);
        void update(const OrderId, const Execution&, const double commission);

	public:
        Position(const TickerId);
        Position(const TickerId, const StrategyId);
        Position(const Position&);
        Position(const TickerId, const StrategyId, const StrategyLinkedPositionData* data);
        Position();
        ~Position();

    private:
        void initialize();
};

#endif
