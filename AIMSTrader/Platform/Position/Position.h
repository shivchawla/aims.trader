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
<<<<<<< HEAD
#include "Platform/Enumerations/PositionStatus.h"
#include <QMutex>
=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

//class PerformanceManager;
class Position 
{
	private:
<<<<<<< HEAD
        StrategyId _strategyId;
        TickerId _tickerId;
        double _lastPrice;
        int _quantity;
        String _time;
=======
        TickerId _tickerId;
        double _lastPrice;
        int _quantity;
        std::string _time;
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
		double _avgFillPrice;
        bool _isPositionClosed;

        double _tradeCommission;
        double _positionValue;
        double _tradeProfit;
<<<<<<< HEAD
        String _timeInMarketStart, _timeInMarket;

        PositionStatus _status;

        mutable QMutex mutex;
	
	public:
        Position(const TickerId);
        Position(const TickerId, const StrategyId);
=======
        std::string _timeInMarketStart, _timeInMarket;
	
	public:
        Position(const TickerId);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
        //Position(const Contract&);
		~Position();
	
	public:
        //const Contract& getContract();
<<<<<<< HEAD
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

=======
        const double getQuantity() const;
        const double getAvgFillPrice() const ;
        const String& getTime() const ;
        const bool IsPositionClosed() const;
        const TickerId getTickerId() const;
        const double getLastPrice() const ;
        const double getPositionValue() const ;
	
	public:
        void updatePosition(const ExecutionStatus&);
        void updatePosition(const double currentPrice);	
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
};

#endif
