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

class Position 
{
    private:
        StrategyId _strategyId;
        TickerId _tickerId;
        long _oldSharesBought;
        long _oldSharesSold;
        long _oldNetShares;
        double _oldAvgBought;
        double _oldAvgSold;
        double _oldTotalValueBought;
        double _oldTotalValueSold;
        double _oldTotalCommision;
        double _oldRealizedPnl;
        double _oldRunningPnl;

        long _sharesBought;
        long _sharesSold;
        long _netShares;
        double _avgBought;
        double _avgSold;
        double _totalValueBought;
        double _totalValueSold;
        double _totalCommision;
        double _realizedPnl;
        double _runningPnl;

        qint64 _lastUpdated;

        QMutex mutex;

    public:
        const StrategyId getStrategyId() const{return _strategyId;}
        //const TickerId getTickerId() const{return _tickerId;}
        const TickerId getTickerId() const {return _tickerId;}
        const long getSharesBought() const{return _sharesBought;}
        const long getSharesSold() const{return _sharesSold;}
        const long getNetShares() const{return (_sharesBought-_sharesSold);}
        const double getAvgBought() const {return _avgBought; }
        const double getAvgSold() const{return _avgSold;}
        const double getTotalValueBought() const{return _totalValueBought;}
        const double getTotalValueSold() const{return _totalValueSold;}
        const double getNetTotal() const{return (_totalValueBought-_totalValueSold);}
        const double getTotalCommission() const{return _totalCommision;}
        const double getRealizedPnl() const{return _realizedPnl;}
        const double getRunningPnl() const{return _runningPnl;}
        const double getPnL() const{return _realizedPnl+_runningPnl;}
        const double getNetTotalIncCommission() const{return _totalValueBought-_totalValueSold-_totalCommision;}

        //const long getOldSharesBought() const{return _oldSharesBought;}
        //const long getOldSharesSold() const{return _oldSharesSold;}
        //const long getOldNetShares() const{return (_oldSharesBought-_oldSharesSold);}
        //const double getOldAvgBought() const {return _oldAvgBought; }
        //const double getOldAvgSold() const{return _oldAvgSold;}
        const double getOldTotalValueBought() const{return _oldTotalValueBought;}
        const double getOldTotalValueSold() const{return _oldTotalValueSold;}
        //const double getOldNetTotal() const{return (_oldTotalValueBought-_oldTotalValueSold);}
        const double getOldTotalCommission() const{return _oldTotalCommision;}
        const double getOldRealizedPnl() const{return _oldRealizedPnl;}
        const double getOldRunningPnl() const{return _oldRunningPnl;}
        const double getOldPnL() const{return _oldRealizedPnl+_oldRunningPnl;}
        //const double getOldNetTotalIncCommission() const{return _oldTotalValueBought-_oldTotalValueSold-_oldTotalCommision;}

        const qint64 lastUpdated(){
            return _lastUpdated;
        }

        const double getReturn()
        {
            return _runningPnl * 100/abs(_totalValueBought - _totalValueSold);
        }

    public:
        void update(const TickType, const double lastPrice);
        void update(const int quantity, const double fillPrice);
        void update(const Execution&);


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
