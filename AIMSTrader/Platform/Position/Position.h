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
        double _lastPrice;
        int _quantity;
        String _time;
		double _avgFillPrice;
        bool _isPositionClosed;

        double _tradeCommission;
        double _positionValue;
        double _tradeProfit;
        String _timeInMarketStart, _timeInMarket;

        PositionStatus _status;

        mutable QMutex mutex;
	
	public:
        Position(const TickerId);
        Position(const TickerId, const StrategyId);
        //Position(const Contract&);
		~Position();
	
	public:
        //const Contract& getContract();
        const int getQuantity() const;
        const double getAvgFillPrice() const ;
        const String getTime() const ;
        const bool IsPositionClosed() const;
        const TickerId getTickerId() const;
        const StrategyId getStrategyId() const;
        const double getLastPrice() const ;
        const double getPositionValue() const ;
        const double getCommission() const;
        const double getTradeProfit() const;
        const PositionStatus getPositionStatus() const;

	public:
        const double updatePosition(const ExecutionStatus&, const bool isClosingPosition);
        const double updatePosition(const double currentPrice);
        void updateStatus(const PositionStatus status);


    private:
        void initialize();

};

#endif
