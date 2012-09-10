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
#include "Platform/Position/OpenOrder.h"

PositionManager::PositionManager(Strategy* strategySPtr)://QObject()
                                //,_currentPositionId(0)
                                _strategyWPtr(strategySPtr)
{
    initialize();
}

void PositionManager::initialize()
{
    _lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
    _performanceManager = _strategyWPtr->getPerformanceManager();
    _strategyId = _strategyWPtr->getStrategyId();
}

PositionManager::~PositionManager()
{}

const PositionPtrMap& PositionManager::getCurrentPositions()
{
	return _currentPositions;
}

void PositionManager::updatePosition(const OpenOrder& openOrder)
{
    TickerId tickerId = openOrder.getTickerId();
    addPosition(tickerId);

    OrderId orderId = openOrder.getOrderId();

    long quantity = openOrder.getLastFilledShares();
    long filledShares = (openOrder.getOrder().action == "SELL") ? -quantity : quantity;
    double fillPrice = openOrder.getLastFillPrice();
    double commission = openOrder.getCommission();

    if(Position* position = _cumulativePositions[tickerId])
    {
        position->update(orderId, filledShares, fillPrice, commission);
        updateOutputsForExecution(position, position);
        updatePerformanceForExecution(position);

        if(position->getNetPositionDetail().getNetShares()==0)
        {
            removeCurrentPosition(tickerId);
            unSubscribeToMktData(tickerId);
        }
    }
}

void PositionManager::updatePosition(const OrderId orderId, const TickerId tickerId, const int filledShares, const double fillPrice, const double commission)
{
        addPosition(tickerId);

        Position* position = _cumulativePositions[tickerId];
        //Position* currentPosition = _currentPositions[tickerId];

        position->update(orderId, filledShares, fillPrice, commission);
        //currentPosition->update(filledShares, fillPrice, commission);

        updateOutputsForExecution(position, position);
        updatePerformanceForExecution(position);

        if(position->getNetPositionDetail().getNetShares()==0)
        {
            removeCurrentPosition(tickerId);
            unSubscribeToMktData(tickerId);
        }
}

void PositionManager::removeCurrentPosition(const TickerId tickerId)
{
     if(Position* position = _currentPositions[tickerId])
     {
         delete position;
         _currentPositions.erase(tickerId);
     }
}

///Updates the position with last traded price
void PositionManager::updatePosition(const TickerId tickerId, const TickType tickType, const double lastPrice)
{
    if(Position* position = _cumulativePositions[tickerId])
     {
         //Position* position = _currentPositions[tickerId];
         position->update(tickType, lastPrice);

         updateOutputsForLastPrice(position);
         updatePerformanceForPrice(position);

         qint64 time1 = QDateTime::currentMSecsSinceEpoch();
         qint64 time2 = position->lastUpdated();
         qint64 timeElapsed = (time1 - time2)/60000;

         double ret = position->getCurrentPositionDetail().getReturn();
         int maxHoldingPeriod = _strategyWPtr->getMaxHoldingPeriod();
         if(position->getStatus() == Open && ((timeElapsed > maxHoldingPeriod && maxHoldingPeriod!=0) || ret > _strategyWPtr->getTargetReturn() || ret < _strategyWPtr->getStopLossReturn()))
         {          
            position->setStatus(PendingClose);
            closePosition(tickerId);
         }
    }
}

void PositionManager::addPosition(const TickerId tickerId)//, const bool isClosingPosition)
{
    Position* position;
    if(!(position = _cumulativePositions[tickerId]))
    {
       _cumulativePositions[tickerId] =  new Position(tickerId, _strategyId);
       //_performanceManager->addPerformanceStats(tickerId);
       subscribeToMktData(tickerId);

       addPositionInOutputs(_strategyId, tickerId);
    }
}


void PositionManager::loadPosition(const TickerId tickerId, const StrategyLinkedPositionData* data)
{
    //StrategyId strategyId = _strategyWPtr->getStrategyId();
    //Position* currentPos = new Position(tickerId, _strategyId, data);
    Position* historicalPos = new Position(tickerId, _strategyId, data);

    //_currentPositions[tickerId] = currentPos;
    _cumulativePositions[tickerId] = historicalPos;

    addPositionInOutputs(_strategyId, tickerId, GUI);
    updateOutputsForExecution(historicalPos, historicalPos, GUI);
    _performanceManager->loadPosition(historicalPos);
}

const PositionId PositionManager::createNewPosition(const TickerId tickerId)
{
    Position* pos = _cumulativePositions[tickerId];
    if(!pos)
    {
        pos = new Position(tickerId, _strategyId);
        _cumulativePositions[tickerId] =  pos;
    }

    //StrategyId strategyId = _strategyWPtr->getStrategyId();
    //Position* newPosition = new Position(tickerId, _strategyId);
    //_currentPositions[tickerId] =  newPosition;
}

///Closes a specific position at MKT price
void PositionManager::closePosition(const TickerId tickerId)
{
    if(_cumulativePositions.count(tickerId))
    {
        int quantity = _cumulativePositions[tickerId]->getCurrentPositionDetail().getNetShares();
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

    TickerId instrumentId = position->getTickerId();
    int quantity = position->getNetPositionDetail().getNetShares();

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

    PositionPtrMap::iterator end = _cumulativePositions.end();
    for(PositionPtrMap::iterator it=_cumulativePositions.begin(); it!=end; ++it)
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


void PositionManager::updateOutputsForExecution(const Position* currentPosition, const Position* cumulativePosition, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForExecution(currentPosition, cumulativePosition, type);
    //_outputInterface->updatePositionForExecution(position);
}

void PositionManager::updateOutputsForLastPrice(const Position* position, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForLastPrice(position, type);
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

