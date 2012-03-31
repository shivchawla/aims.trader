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

class PerformanceManager;
class Position 
{
	private:
        Contract _contract;
        double _currentPrice;
        int _quantity;
        long _time;
		double _avgFillPrice;
        bool _isPositionClosed;

        double _tradeCommission;
        double _positionValue;
        double _tradeProfit;
        std::string _timeInMarketStart, _timeInMarket;

        PerformanceManager* _performanceManager;
	
	public:
        Position(){}
        Position(const Contract&, PerformanceManager*);
		~Position();
	
	public:
		const Contract& getContract();
		const double getQuantity();
		const double getAvgFillPrice();
		const double getTime();	
        const bool IsPositionClosed();
	
	public:
		void updatePosition(const Execution&);
        void updatePosition(const double currentPrice);
	
};

#endif
