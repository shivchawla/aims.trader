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
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/View/MainWindow.h"

PositionManager::PositionManager():QObject(),_currentPositionId(0)
{
    lockForPositionMap = new QReadWriteLock();
}

PositionManager::PositionManager(Strategy* strategySPtr):QObject()
                                ,_currentPositionId(0)
                                ,_strategyWPtr(strategySPtr)
{
    lockForPositionMap=new QReadWriteLock();
    //connect(this, SIGNAL((const Position&)), MainWindow::mainWindow()->getPositionView(), SLOT(onPositionUpdate(const Position&)));
}

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

/*void PositionManager::updatePosition(const OrderId orderId, const Execution& execution)
{
    if(_orderIdToPositionId.count(orderId)!=0)
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
}*/

void PositionManager::updatePosition(const OrderId orderId, const ExecutionStatus& executionStatus)
{
    if(_orderIdToPositionId.count(orderId) != 0)
    {
        PositionId positionId = _orderIdToPositionId[orderId];

        //update the number of trades
        //this happens for the first time only
        if( _currentPositions[positionId]->getQuantity()==0)
        {
            if(executionStatus.execution.cumQty>0)
            {
                _performanceManager->updateLongTrades();
            }
            else if(executionStatus.execution.cumQty<0)
            {
                _performanceManager->updateShortTrades();
            }
        }
        Position* currentPosition = _currentPositions[positionId];
        currentPosition->updatePosition(executionStatus);
        emit positionUpdated(*currentPosition);

        //either setup a commission manager who yields commission based on ticker
        //or add a functionality in InstrumentManager to give commissions based on trade size

         double commission=0;
        _performanceManager->updateOnOrderFill(executionStatus.execution.shares, executionStatus.execution.avgPrice, commission);
        //check if position is closed
        if(_currentPositions[positionId]->IsPositionClosed())
        {
            _historicalPositions[positionId] = _currentPositions[positionId];
            _currentPositions.erase(positionId);
        }
    }
}

void PositionManager::updatePosition(const TickerId tickerId, const double lastPrice)
{
    bool isPositionUpdated=false;
    //Position oldPosition, updatedPosition;
    //lock the position map for reads
     lockForPositionMap->lockForRead();
     if(_tickerIdToPositionId.count(tickerId)!=0)
     {
         PositionId  positionId = _tickerIdToPositionId[tickerId];
         Position oldPosition = *(_currentPositions[positionId]);

         _currentPositions[positionId]->updatePosition(lastPrice);
         Position updatedPosition = *(_currentPositions[positionId]);
         isPositionUpdated=true;
         lockForPositionMap->unlock();
         emit positionUpdated(updatedPosition);

         if(isPositionUpdated)
         {
           _performanceManager->updatePerformance(oldPosition, updatedPosition);
         }
      }
      else
      {
         lockForPositionMap->unlock();
      }
      //do we really need copies
      //can performance be updated in here

}

/*void PositionManager::addPosition(const OrderId orderId, const Contract& contract)
{
    //update the tickerId and OrderId maps
    PositionId positionId = ++_currentPositionId;
    TickerId tickerId;
    if(!(tickerId=Service::Instance()->getInstrumentManager()->getTickerId(contract)))
    {
        _tickerIdToPositionId[tickerId] = positionId;
    }
    _orderIdToPositionId[orderId] = positionId;
    _currentPositions[positionId] =  new Position(contract, _performanceManager);
}*/

void PositionManager::addPosition(const OrderId orderId, const TickerId tickerId)
{
    lockForPositionMap->lockForWrite();
    if(_tickerIdToPositionId.count(tickerId)!=0)
    {
        //update the tickerId and OrderId maps
        PositionId positionId = ++_currentPositionId;
        _tickerIdToPositionId[tickerId] = positionId;
        _orderIdToPositionId[orderId] = positionId;
        _currentPositions[positionId] =  new Position(tickerId);
        emit positionUpdated(*(_currentPositions[positionId]));
    }

    lockForPositionMap->unlock();
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

    //Contract contract = Service::Instance()->getInstrumentManager()->getContractForTicker(_currentPositions[positionId]->getTickerId());
    //_strategyWPtr->placeOrder(_currentPositions[positionId]->getContract(), mktOrder);
    //_stratgeyWPtr->placeClosingOrder(_currentPositions[positionId]->getTickerId(),mktOrder);

    lockForPositionMap->lockForRead();
    TickerId tickerId = _currentPositions[positionId]->getTickerId();
    lockForPositionMap->unlock();

    _strategyWPtr->placeClosingOrder(tickerId, mktOrder);
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


