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

class PositionData;

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
        QDateTime _lastUpdated;
        QMutex mutex;

    public:
        const StrategyId getStrategyId() const{return _strategyId;}
        const TickerId getTickerId() const {return _tickerId;}

        const PositionDetail& getNetPositionDetail() const
        {
            return _netPositionDetail;
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

        const QDateTime lastUpdated(){
            return _lastUpdated;
        }

        const double getAmountInvested()
        {
            return (_netPositionDetail._sharesBought > _netPositionDetail._sharesSold)
                    ? (_netPositionDetail._sharesBought - _netPositionDetail._sharesSold) * _netPositionDetail._avgBuyPrice
                    : (-_netPositionDetail._sharesBought + _netPositionDetail._sharesSold) * _netPositionDetail._avgSellPrice;
        }

    public:
        void update(const TickType, const double lastPrice);
        void update(const OrderId, const int quantity, const double fillPrice, const double avgFillPrice, const double commission);
        void update(const OrderId, const OrderDetail&);


	public:
        Position(const TickerId);
        Position(const TickerId, const StrategyId);
        Position(const Position&);
        Position(const TickerId, const StrategyId, const PositionData& data);
        Position();
        ~Position();

    private:
        void initialize();
};

#endif
