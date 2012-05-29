/*
 *  Position.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/28/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef Position_h
#define Position_h

#include "Platform/Shared/Contract.h"
#include "Platform/Shared/Execution.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/PositionStatus.h"
#include <QMutex>

//class PerformanceManager;

class Position 
{
    private:
        StrategyId _strategyId;
        TickerId _tickerId;
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

    public:
        const StrategyId getStrategyId() const{return _strategyId;}
        const TickerId getTickerId() const{return _tickerId;}
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

    public:
        void update(const double lastPrice);
        void update(const Execution&);

	public:
        Position(const TickerId);
        Position(const TickerId, const StrategyId);
        //Position(const Position*);
        ~Position();

    private:
        void initialize();

};

#endif
