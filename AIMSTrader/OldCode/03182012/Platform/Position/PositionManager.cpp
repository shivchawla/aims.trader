/*
 *  PositionManager.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/28/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Position/PositionManager.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Position/Position.h"
#include "Platform/Position/Instrument.h"
#include "Platform/Position/OpenOrder.h"
PositionManager::PositionManager():_currentPositionId(0)
{}

PositionManager::PositionManager(Strategy* strategySPtr):
                                 _currentPositionId(0)
                                ,_strategyWPtr(strategySPtr)
{}

PositionManager::~PositionManager()
{}

void PositionManager::linkPerformanceManager(PerformanceManager* performanceManagerWptr)
{
    _performanceManager = performanceManagerWptr;
}

const PositionPtrMap& PositionManager::getCurrentPositions()
{
	return _currentPositions;
}

const PositionPtrMap& PositionManager::getHistoricalPositions()
{
	return _historicalPositions;
}

void PositionManager::updatePosition(const OrderId orderId, const Execution& execution)
{
    PositionId positionId = _orderIdToPositionId[orderId];
    _currentPositions[positionId]->updatePosition(execution);
    //check if position is closed
    if(_currentPositions[positionId]->IsPositionClosed())
    {
        _historicalPositions[positionId] = _currentPositions[positionId];
        _currentPositions.erase(positionId);
	}
}

void PositionManager::setTickerId(const Contract& contract, const TickerId tickerId)
{
    _tickerIdToPositionId[tickerId] = _contractIdToPositionId[contract.conId];
}

void PositionManager::updatePosition(const TickerId tickerId, const double lastPrice)
{
     PositionId  positionId = _tickerIdToPositionId[tickerId];
    _currentPositions[positionId]->updatePosition(lastPrice);
}

void PositionManager::addPosition(const OrderId orderId, const Contract& contract)
{
    //update the tickerId and OrderId maps
    PositionId positionId = ++_currentPositionId;
    _contractIdToPositionId[contract.conId] = positionId;
    _orderIdToPositionId[orderId] = positionId;
    _currentPositions[positionId] =  new Position(contract, _performanceManager);
}

void PositionManager::closePosition(const PositionId positionId)
{
	//create an order to close the position
    //send the order
    //create a MKT order
    Order mktOrder;
    mktOrder.orderType = "MKT";
    mktOrder.action = "CLOSE";
    mktOrder.totalQuantity = (-1)* _currentPositions[positionId]->getQuantity();
    _strategyWPtr->placeOrder(_currentPositions[positionId]->getContract(), mktOrder);
}

void PositionManager::closeAllPositions()
{
	PositionPtrMap::iterator end = _currentPositions.end();
	for(PositionPtrMap::iterator it=_currentPositions.begin(); it!=end; ++it)
	{
		closePosition(it->first);
	}
}



//till this point what do we have in Position Manager
//1. Ability to update position
//2. ability to close one and all positions.

//What Remains??
//1. When closing a position, it's not clear how to identify the closing position
//2. We can have more function which closes position based on certain order characteristics


