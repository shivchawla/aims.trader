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
#include <QDebug>
#include "Platform/View/OutputInterface.h"

PositionManager::PositionManager()//:QObject()//,_currentPositionId(0)
{
    _outputInterface = OutputInterface::Instance();
    _lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
    //lockForPositionMap= new QMutex();
}

PositionManager::PositionManager(Strategy* strategySPtr)://QObject()
                                //,_currentPositionId(0)
                                _strategyWPtr(strategySPtr)
{
    _outputInterface = OutputInterface::Instance();
    _lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
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

///Updates the position with execution information
void PositionManager::updatePosition(const TickerId tickerId, const Execution& execution)
{
    qDebug()<<"signal received"+execution.shares;
    if(!_currentPositions.count(tickerId))
    {
        addPosition(tickerId);
    }

    Position* currentPosition = _currentPositions[tickerId];

    currentPosition->update(execution);
    //subscribe to market data
    subscribeToMktData(currentPosition->getTickerId());

    updateOutputsForExecution(currentPosition);
    updatePerformanceForExecution(currentPosition);

    if(currentPosition->getNetShares()==0)
    {
        unSubscribeToMktData(tickerId);
    }
    else if(!_strategyWPtr->isSubscribed(tickerId))
    {
        subscribeToMktData(tickerId);
    }
}

void PositionManager::updatePosition(const TickerId tickerId, const int filledShares, const double fillPrice)
{
    if(!_currentPositions.count(tickerId))
    {
        addPosition(tickerId);
    }

    Position* currentPosition = _currentPositions[tickerId];
    if(currentPosition)
    {
        currentPosition->update(filledShares, fillPrice);
        updateOutputsForExecution(currentPosition);
        updatePerformanceForExecution(currentPosition);

        if(currentPosition->getNetShares()==0)
        {
            unSubscribeToMktData(tickerId);
        }
        else if(!_strategyWPtr->isSubscribed(tickerId))
        {
            subscribeToMktData(tickerId);
        }
    }
}


///Updates the position with last traded price
void PositionManager::updatePosition(const TickerId tickerId, const double lastPrice)
{
    //lockForPositionMap->lockForRead();
    //qDebug("entered locked area: update Position price");

    if(_currentPositions.count(tickerId))
     {
         Position* position = _currentPositions[tickerId];
         position->update(lastPrice);

         updateOutputsForLastPrice(position);
         updatePerformanceForPrice(position);
     }

    //lockForPositionMap->unlock();
   //qDebug("Left locked area: update Position price");
}

void PositionManager::addPosition(const TickerId tickerId)//, const bool isClosingPosition)
{
    //lockForPositionMap->lockForWrite();
    //qDebug("entered locked area: add Position");
    //_orderIdToTickerId[orderId] = tickerId;
    if(!_currentPositions.count(tickerId))
    {
       createNewPosition(tickerId);
    }
    //lockForPositionMap->unlock();
    //qDebug("Left locked area: add Position");
}

const PositionId PositionManager::createNewPosition(const TickerId tickerId)
{
    StrategyId strategyId = _strategyWPtr->getStrategyId();
    Position* newPosition = new Position(tickerId, strategyId);
    _currentPositions[tickerId] =  newPosition;
    addPositionInOutputs(strategyId, tickerId);
}

///Closes a specific position at MKT price
void PositionManager::closePosition(const TickerId tickerId)
{

    if(_currentPositions.count(tickerId))
    {
        int quantity = _currentPositions[tickerId]->getNetShares();
        //create an order to close the position
        //send the order
        //create a MKT order
        Order mktOrder;
        mktOrder.orderType = "MKT";

        /*lockForPositionMap->lock();//ForRead();
        TickerId tickerId = _currentPositions[positionId]->getTickerId();
        int quantity = _currentPositions[positionId]->getQuantity();
        lockForPositionMap->unlock();*/

        mktOrder.totalQuantity = abs(quantity);

        if(quantity>0)
        {
            mktOrder.action = "SELL";
        }
        else
        {
            mktOrder.action = "BUY";
        }
        _strategyWPtr->placeClosingOrder(tickerId, mktOrder);
    }

}

void PositionManager::closePosition(const Position* position)
{
    //create an order to close the position
    //send the order
    //create a MKT order
    Order mktOrder;
    mktOrder.orderType = "MKT";

    TickerId tickerId = position->getTickerId();
    int quantity = position->getNetShares();

    mktOrder.totalQuantity = abs(quantity);

    if(quantity>0)
    {
        mktOrder.action = "SELL";
    }
    else
    {
        mktOrder.action = "BUY";
    }
    _strategyWPtr->placeClosingOrder(tickerId, mktOrder);

}

//void PositionManager::adjustPosition(const TickerId tickerId, const Order& order)
//{
//    if(_currentPositions.count(tickerId))
//    {

//    }
//}


///closes all open positions
/* Sends MKT order to close all positions*/
void PositionManager::closeAllPositions()
{
    //lockForPositionMap->lockForRead();
    //qDebug("entered locked area: close Position");

	PositionPtrMap::iterator end = _currentPositions.end();
	for(PositionPtrMap::iterator it=_currentPositions.begin(); it!=end; ++it)
	{
        //Position* position = it->second;
        closePosition(it->second);
        //(it->second)->updateStatus(Closing);
    }
    //lockForPositionMap->unlock();
   //qDebug("left locked area: close Position");

}

void PositionManager::addPositionInOutputs(const StrategyId strategyId, const TickerId tickerId)
{
    _outputInterface->addPosition(strategyId, tickerId);
}


//void PositionManager::updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability)
//{
//    _performanceManager->updatePerformance(pnl, currentProfitability, lastProfitability);
//}


void PositionManager::updateOutputsForExecution(const Position* position)
{
    _outputInterface->updatePositionForExecution(position);
}

void PositionManager::updateOutputsForLastPrice(const Position* position)
{
    _outputInterface->updatePositionForLastPrice(position);
}

void PositionManager::removeFromPositionView(const StrategyId strategyId, const PositionId positionId)
{
   // emit positionRemoved(strategyId, positionId);
}

void PositionManager::subscribeToMktData(const TickerId tickerId)
{
    _strategyWPtr->subscribeMarketData(tickerId, IB);
}

void PositionManager::unSubscribeToMktData(const TickerId tickerId)
{
    _strategyWPtr->unSubscribeMarketData(tickerId);
}

void PositionManager::updatePerformanceForPrice(const Position* position)
{
    _performanceManager->updatePerformanceForPrice(position);
}

void PositionManager::updatePerformanceForExecution(const Position* position)
{
    _performanceManager->updatePerformanceForExecution(position);
}

