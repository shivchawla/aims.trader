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
#include "Data/strategylinkedpositiondata.h"

/*
 * default constructor
 */
Position::~Position()
{}

Position::Position()
{
   _instrumentId = 0;
   _strategyId = 0;
   initialize();
}

/*
 *
 */
Position::Position(const InstrumentId instrumentId):_instrumentId(instrumentId)
{
    initialize();
}


/*
 *
 */
Position::Position(const InstrumentId instrumentId, const StrategyId strategyId):_instrumentId(instrumentId),_strategyId(strategyId)
{
    initialize();
}

Position::Position(const Position& pos)
{
    _instrumentId = pos._instrumentId;
    _strategyId = pos._strategyId;

    _oldSharesBought = 0;
    _oldSharesSold = 0;
    _oldNetShares=0;
    _oldAvgBought = 0;
    _oldAvgSold = 0;
    _oldTotalValueBought = 0;
    _oldTotalValueSold = 0;
    _oldTotalCommision = 0;
    _oldRealizedPnl = 0;
    _oldRunningPnl = 0;

    _sharesBought = pos._sharesBought;
    _sharesSold = pos._sharesSold;
    _netShares=pos._netShares;
    _avgBought = pos._avgBought;
    _avgSold = pos._avgSold;
    _totalValueBought = pos._totalValueBought;
    _totalValueSold = pos._totalValueSold;
    _totalCommision = pos._totalCommision;
    _realizedPnl = pos._realizedPnl;
    _runningPnl = pos._runningPnl;


}

/*
 * initialize the positon intral variabes with defaults
 */
void Position::initialize()
{
    _oldSharesBought = 0;
    _oldSharesSold = 0;
    _oldNetShares=0;
    _oldAvgBought = 0;
    _oldAvgSold = 0;
    _oldTotalValueBought = 0;
    _oldTotalValueSold = 0;
    _oldTotalCommision = 0;
    _oldRealizedPnl = 0;
    _oldRunningPnl = 0;

    _sharesBought = 0;
    _sharesSold = 0;
    _netShares=0;
    _avgBought = 0;
    _avgSold = 0;
    _totalValueBought = 0;
    _totalValueSold = 0;
    _totalCommision = 0;
    _realizedPnl = 0;
    _runningPnl = 0;

}
Position::Position(const StrategyLinkedPositionData* data)
{
    _instrumentId = data->instrumentId;
    _strategyId = data->strategyId;

    _oldSharesBought = 0;
    _sharesBought = data->numberBought;
    _oldSharesSold = 0;
    _sharesSold = data->numberSold;
    _oldNetShares = 0;
    _netShares = data->numberBought - data->numberSold;
    _oldAvgBought = 0;
    _avgBought = data->avgAmountBought;
    _oldAvgSold = 0;
    _avgSold = data->avgAmountSold;
    _oldTotalValueBought = 0;
    _totalValueBought = data->avgAmountBought * data->numberBought;
    _oldTotalValueSold = 0;
    _totalValueSold = data->avgAmountSold * data->numberSold;
    _oldTotalCommision = 0;
    _totalCommision = data->totalAmountCommission;
    _oldRealizedPnl = 0;
    _realizedPnl = (_netShares > 0) ? _sharesSold * (_avgSold - _avgBought) : _sharesBought * (_avgBought - _avgSold);
    _oldRunningPnl = _runningPnl = 0;
}

/*
 * Updates a positon with new trade price
 */
void Position::update(const double currentPrice)
{
    _oldRunningPnl = _runningPnl;
    if(_netShares>0)
    {
        _runningPnl = _netShares * (currentPrice - _avgBought);
    }
    else
    {
        _runningPnl = _netShares * (_avgSold-currentPrice);
    }
}

void Position::update(const Execution& execution)
{

    int quantity = execution.shares;
     double fillPrice = execution.price;
    //_time = executionStatus.execution.time;
    _oldAvgBought = _avgBought;
    _oldSharesBought = _sharesBought;
    _oldAvgSold = _avgSold;
    _oldSharesSold = _sharesSold;
    _oldNetShares = _netShares;

    _oldTotalValueBought = _totalValueBought;

    _oldTotalValueSold = _totalValueSold;

    _oldRealizedPnl = _realizedPnl;
    _oldRunningPnl = _runningPnl;

    if(execution.side == "BOT")
    {
        if(_netShares<0 )
        {
            if(abs(_netShares) > quantity)
            {
                _realizedPnl += quantity * (_avgSold - fillPrice);
            }
            else
            {
                _realizedPnl += abs(_netShares) * (_avgSold - fillPrice);
            }
        }
        _avgBought = (quantity*fillPrice +_sharesBought*_avgBought)/(_sharesBought += quantity);
    }
    else
    {
        if(_netShares>0)
        {
            if(_netShares > quantity)
            {
                _realizedPnl += quantity * (fillPrice - _avgBought);
            }
            else
            {
                _realizedPnl += _netShares * (fillPrice - _avgBought);
            }
        }
        _avgSold = (quantity*fillPrice + _sharesSold*_avgSold)/(_sharesSold += quantity);
    }

    _netShares = _sharesBought - _sharesSold;

    if(_netShares == 0)
    {
        _runningPnl = 0;

    }
    _totalValueBought = _sharesBought * _avgBought ;
    _totalValueSold = _sharesSold * _avgSold ;

}

void Position::update(const int quantity, const double fillPrice)
{
    _oldAvgBought = _avgBought;
    _oldSharesBought = _sharesBought;
    _oldAvgSold = _avgSold;
    _oldSharesSold = _sharesSold;
    _oldNetShares = _netShares;

    _oldTotalValueBought = _totalValueBought;

    _oldTotalValueSold = _totalValueSold;

    _oldRealizedPnl = _realizedPnl;
    _oldRunningPnl = _runningPnl;

    int absQuantity = std::abs(quantity);

    if(quantity>0)
    {
        if(_netShares<0 )
        {
            if(abs(_netShares) > absQuantity)
            {
                _realizedPnl += absQuantity * (_avgSold - fillPrice);
            }
            else
            {
                _realizedPnl += std::abs(_netShares) * (_avgSold - fillPrice);
            }
        }
        _avgBought = (absQuantity*fillPrice +_sharesBought*_avgBought)/(_sharesBought += absQuantity);
    }
    else
    {
        if(_netShares>0)
        {
            if(_netShares > absQuantity)
            {
                _realizedPnl += absQuantity * (fillPrice - _avgBought);
            }
            else
            {
                _realizedPnl += _netShares * (fillPrice - _avgBought);
            }
        }
        _avgSold = (absQuantity*fillPrice + _sharesSold*_avgSold)/(_sharesSold += absQuantity);
    }

    _netShares = _sharesBought - _sharesSold;

    if(_netShares == 0)
    {
        _runningPnl = 0;

    }
    _totalValueBought = _sharesBought * _avgBought ;
    _totalValueSold = _sharesSold * _avgSold ;

}

