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
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

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
    else if(!_strategyWPtr->IsSubscribed(tickerId))
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
        else if(!_strategyWPtr->IsSubscribed(tickerId))
        {
            subscribeToMktData(tickerId);
        }
    }
}


///Updates the position with last traded price
void PositionManager::updatePosition(const TickerId tickerId, const TickType tickType, const double lastPrice)
{
    //lockForPositionMap->lockForRead();
    //qDebug("entered locked area: update Position price");

    if(_currentPositions.count(tickerId))
     {
         Position* position = _currentPositions[tickerId];
         position->update(tickType, lastPrice);

         updateOutputsForLastPrice(position);
         updatePerformanceForPrice(position);

         qint64 time1 = QDateTime::currentMSecsSinceEpoch();
         qint64 time2 = position->lastUpdated();
         qint64 timeElapsed = (time1 - time2)/60000;

         double ret = position->getReturn();
          int maxHoldingPeriod = _strategyWPtr->getMaxHoldingPeriod();
         if((timeElapsed > maxHoldingPeriod && maxHoldingPeriod!=0) || ret > _strategyWPtr->getTargetReturn() || ret < _strategyWPtr->getStopLossReturn())
         {          
            closePosition(tickerId);
         }
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


void PositionManager::loadPosition(const TickerId tickerId, const StrategyLinkedPositionData* data)
{
    StrategyId strategyId = _strategyWPtr->getStrategyId();
    Position* pos = new Position(tickerId, strategyId, data);
    _currentPositions[tickerId] = pos;

    addPositionInOutputs(strategyId, tickerId, GUI);
    updateOutputsForExecution(pos, GUI);
    _performanceManager->loadPosition(pos);
}

const PositionId PositionManager::createNewPosition(const TickerId tickerId)
{
    StrategyId strategyId = _strategyWPtr->getStrategyId();
    Position* newPosition = new Position(tickerId, strategyId);
    _currentPositions[tickerId] =  newPosition;
    addPositionInOutputs(strategyId, tickerId);
}

///Closes a specific position at MKT price
void PositionManager::closePosition(const TickerId instrumentId)
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

    TickerId instrumentId = position->getTickerId();
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

void PositionManager::addPositionInOutputs(const StrategyId strategyId, const TickerId instrumentId, const OutputType type)
{
    //this is called on strategy thread but strategy output is running on a different thread
    //this function is though called on Strategy Thread

    StrategyOutput::strategyOutput().addPosition(strategyId, instrumentId, type);
}


//void PositionManager::updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability)
//{
//    _performanceManager->updatePerformance(pnl, currentProfitability, lastProfitability);
//}


void PositionManager::updateOutputsForExecution(const Position* position, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForExecution(*position, type);
    //_outputInterface->updatePositionForExecution(position);
}

void PositionManager::updateOutputsForLastPrice(const Position* position, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForLastPrice(*position, type);
    //_outputInterface->updatePositionForLastPrice(position);
}

void PositionManager::removeFromPositionView(const StrategyId strategyId, const PositionId positionId)
{
   // emit positionRemoved(strategyId, positionId);
}

void PositionManager::subscribeToMktData(const TickerId instrumentId)
{
    _strategyWPtr->subscribeMarketData(instrumentId);
}

void PositionManager::unSubscribeToMktData(const TickerId instrumentId)
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

//const Position* PositionManager::getPosition(const TickerId tickerId)
//{
//    Position* position = NULL;
//    if(_currentPositions.count(tickerId))
//    {
//        position = _currentPositions[tickerId];
//    }

//    return position;
//}

