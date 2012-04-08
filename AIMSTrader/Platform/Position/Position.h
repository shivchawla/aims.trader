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

//class PerformanceManager;
class Position 
{
	private:
        TickerId _tickerId;
        double _lastPrice;
        int _quantity;
        std::string _time;
		double _avgFillPrice;
        bool _isPositionClosed;

        double _tradeCommission;
        double _positionValue;
        double _tradeProfit;
        std::string _timeInMarketStart, _timeInMarket;
	
	public:
        Position(const TickerId);
        //Position(const Contract&);
		~Position();
	
	public:
        //const Contract& getContract();
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
};

#endif
