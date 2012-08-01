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
#include <Platform/Strategy/StrategyOutput.h>

PositionManager::PositionManager(Strategy* strategySPtr)://QObject()
                                //,_currentPositionId(0)
                                _strategyWPtr(strategySPtr)
{
    initialize();
}

void PositionManager::initialize()
{
    //_outputInterface = OutputInterface::Instance();
    _lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
    _performanceManager = _strategyWPtr->getPerformanceManager();
}

PositionManager::~PositionManager()
{}

//void PositionManager::linkPerformanceManager(PerformanceManager* performanceManager)
//{
//    _performanceManager =
//}

const PositionPtrMap& PositionManager::getCurrentPositions()
{
	return _currentPositions;
}

///Updates the position with execution information
void PositionManager::updatePosition(const InstrumentId instrumentId, const Execution& execution)
{
    qDebug()<<"signal received"+execution.shares;
    if(!_currentPositions.count(instrumentId))
    {
        addPosition(instrumentId);
    }

    Position* currentPosition = _currentPositions[instrumentId];

    currentPosition->update(execution);
    //subscribe to market data
    subscribeToMktData(currentPosition->getInstrumentId());

    updateOutputsForExecution(currentPosition);
    updatePerformanceForExecution(currentPosition);

    if(currentPosition->getNetShares()==0)
    {
        unSubscribeToMktData(instrumentId);
    }
    else if(!_strategyWPtr->isSubscribed(instrumentId))
    {
        subscribeToMktData(instrumentId);
    }
}

void PositionManager::updatePosition(const InstrumentId instrumentId, const int filledShares, const double fillPrice)
{
    if(!_currentPositions.count(instrumentId))
    {
        addPosition(instrumentId);
    }

    Position* currentPosition = _currentPositions[instrumentId];
    if(currentPosition)
    {
        currentPosition->update(filledShares, fillPrice);
        updateOutputsForExecution(currentPosition);
        updatePerformanceForExecution(currentPosition);

        if(currentPosition->getNetShares()==0)
        {
            unSubscribeToMktData(instrumentId);
        }
        else if(!_strategyWPtr->isSubscribed(instrumentId))
        {
            subscribeToMktData(instrumentId);
        }
    }
}


///Updates the position with last traded price
void PositionManager::updatePosition(const InstrumentId instrumentId, const double lastPrice)
{
    //lockForPositionMap->lockForRead();
    //qDebug("entered locked area: update Position price");

    if(_currentPositions.count(instrumentId))
     {
         Position* position = _currentPositions[instrumentId];
         position->update(lastPrice);

         updateOutputsForLastPrice(position);
         updatePerformanceForPrice(position);
     }

    //lockForPositionMap->unlock();
   //qDebug("Left locked area: update Position price");
}

void PositionManager::addPosition(const InstrumentId instrumentId)//, const bool isClosingPosition)
{
    //lockForPositionMap->lockForWrite();
    //qDebug("entered locked area: add Position");
    //_orderIdToTickerId[orderId] = tickerId;
    if(!_currentPositions.count(instrumentId))
    {
       createNewPosition(instrumentId);
    }
    //lockForPositionMap->unlock();
    //qDebug("Left locked area: add Position");
}

const PositionId PositionManager::createNewPosition(const InstrumentId instrumentId)
{
    StrategyId strategyId = _strategyWPtr->getStrategyId();
    Position* newPosition = new Position(instrumentId, strategyId);
    _currentPositions[instrumentId] =  newPosition;
    addPositionInOutputs(strategyId, instrumentId);
}

///Closes a specific position at MKT price
void PositionManager::closePosition(const InstrumentId instrumentId)
{

    if(_currentPositions.count(instrumentId))
    {
        int quantity = _currentPositions[instrumentId]->getNetShares();
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
        _strategyWPtr->placeClosingOrder(instrumentId, mktOrder);
    }

}

void PositionManager::closePosition(const Position* position)
{
    //create an order to close the position
    //send the order
    //create a MKT order
    Order mktOrder;
    mktOrder.orderType = "MKT";

    InstrumentId instrumentId = position->getInstrumentId();
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
    _strategyWPtr->placeClosingOrder(instrumentId, mktOrder);

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

void PositionManager::addPositionInOutputs(const StrategyId strategyId, const InstrumentId instrumentId)
{
    //this is called on strategy thread but strategy output is running on a different thread
    //this function is though called on Strategy Thread

    StrategyOutput::strategyOutput().addPosition(strategyId, instrumentId);
}


//void PositionManager::updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability)
//{
//    _performanceManager->updatePerformance(pnl, currentProfitability, lastProfitability);
//}


void PositionManager::updateOutputsForExecution(const Position* position)
{
    StrategyOutput::strategyOutput().updatePositionForExecution(*position);
    //_outputInterface->updatePositionForExecution(position);
}

void PositionManager::updateOutputsForLastPrice(const Position* position)
{
    StrategyOutput::strategyOutput().updatePositionForLastPrice(*position);
    //_outputInterface->updatePositionForLastPrice(position);
}

void PositionManager::removeFromPositionView(const StrategyId strategyId, const PositionId positionId)
{
   // emit positionRemoved(strategyId, positionId);
}

void PositionManager::subscribeToMktData(const InstrumentId instrumentId)
{
    _strategyWPtr->subscribeMarketData(instrumentId, IB);
}

void PositionManager::unSubscribeToMktData(const InstrumentId instrumentId)
{
    _strategyWPtr->unSubscribeMarketData(instrumentId);
}
void PositionManager::updatePerformanceForPrice(const Position* position)
{
    _performanceManager->updatePerformanceForPrice(position);
}

void PositionManager::updatePerformanceForExecution(const Position* position)
{
    _performanceManager->updatePerformanceForExecution(position);
}

