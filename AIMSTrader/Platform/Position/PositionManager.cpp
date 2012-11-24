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

PositionManager::PositionManager(Strategy* strategySPtr):_strategyWPtr(strategySPtr)
{
    initialize();
}

void PositionManager::initialize()
{
    _performanceManager = _strategyWPtr->getPerformanceManager();
    _strategyId = _strategyWPtr->getStrategyId();
}

PositionManager::~PositionManager()
{}

void PositionManager::updatePosition(const OpenOrder& openOrder)
{
    TickerId tickerId;
    addPosition(tickerId);

    OrderId orderId = openOrder.getOrderId();

    OrderDetail orderDetail = openOrder.getOrderDetail();
    long quantity = orderDetail.getLastFilledShares();
    long filledShares = (orderDetail.getOrder().action == "SELL") ? -quantity : quantity;
    double fillPrice = orderDetail.getLastFillPrice();
    double commission = orderDetail.getCommission();
    double avgFillPrice = orderDetail.getAvgFillPrice();

    if(Position* position = _positions[tickerId])
    {
        position->update(orderId, filledShares, fillPrice, avgFillPrice, commission);

        _strategyWPtr->_totalInvested += position->getAmountInvested();
        updateOutputsForExecution(position, position);
        updatePerformanceForExecution(position);

        if(position->getNetPositionDetail().getNetShares()==0)
        {
            unSubscribeToMktData(tickerId);
        }
    }
}

void PositionManager::updatePosition(const OrderId orderId, const OrderDetail& orderDetail)
{
    TickerId tickerId = orderDetail.getTickerId();
    addPosition(tickerId);

    if(Position* position = _positions[tickerId])
    {
        position->update(orderId, orderDetail);
        updateOutputsForExecution(position, position);
        updatePerformanceForExecution(position);

        if(position->getNetPositionDetail().getNetShares()==0)
        {
            unSubscribeToMktData(tickerId);
        }
    }
}

void PositionManager::updatePosition(const OrderId orderId, const TickerId tickerId, const int filledShares, const double fillPrice, const double commission)
{
        addPosition(tickerId);

        Position* position = _positions[tickerId];

        position->update(orderId, filledShares, fillPrice, 0, commission);

        updateOutputsForExecution(position, position);
        updatePerformanceForExecution(position);

        if(position->getNetPositionDetail().getNetShares()==0)
        {
            unSubscribeToMktData(tickerId);
        }
}

///Updates the position with last traded price
void PositionManager::updatePosition(const TickerId tickerId, const TickType tickType, const double lastPrice)
{
    if(Position* position = _positions[tickerId])
     {
         position->update(tickType, lastPrice);

         updateOutputsForLastPrice(position);
         updatePerformanceForPrice(position);

         int minutesSinceLastUpdated = position->lastUpdated().secsTo(QDateTime::currentDateTime())/60;

         double ret = position->getNetPositionDetail().getReturn();
         int maxHoldingPeriod = _strategyWPtr->getMaxHoldingPeriod();

         if(position->getStatus() == Open)
         {          
            if(minutesSinceLastUpdated > maxHoldingPeriod && maxHoldingPeriod!=0)
            {
                position->setStatus(PendingClose);
                closePosition(tickerId);
                String message;
                message.append("TickerId: ").append(Service::service().getInstrumentManager()->getSymbol(tickerId)).append(" Holding Period Exceeded");
                _strategyWPtr->reportEvent(message);
            }
            else if(ret > _strategyWPtr->getTargetReturn())
            {
                position->setStatus(PendingClose);
                closePosition(tickerId);
                String message;
                message.append("TickerId: ").append(Service::service().getInstrumentManager()->getSymbol(tickerId)).append(" Target Return achieved: ").append(QString::number(ret));
                _strategyWPtr->reportEvent(message);
            }
            else if(ret < _strategyWPtr->getStopLossReturn())
            {
                position->setStatus(PendingClose);
                closePosition(tickerId);
                String message;
                message.append("TickerId: ").append(Service::service().getInstrumentManager()->getSymbol(tickerId)).append(" StopLoss triggered: ").append(QString::number(ret));
                _strategyWPtr->reportEvent(message);
            }
         }
    }
}

void PositionManager::addPosition(const TickerId tickerId)
{
    Position* position;
    if(!(position = _positions[tickerId]))
    {
       _positions[tickerId] =  new Position(tickerId, _strategyId);
       subscribeToMktData(tickerId);
       addPositionInOutputs(_strategyId, tickerId);
    }
}

void PositionManager::loadPosition(const TickerId tickerId, const PositionData& data)
{
    Position* historicalPos = new Position(tickerId, _strategyId, data);

    _positions[tickerId] = historicalPos;

    addPositionInOutputs(_strategyId, tickerId, GUI);
    updateOutputsForExecution(historicalPos, historicalPos, GUI);
    _performanceManager->loadPosition(historicalPos);
}

const PositionId PositionManager::createNewPosition(const TickerId tickerId)
{
    Position* pos = _positions[tickerId];
    if(!pos)
    {
        pos = new Position(tickerId, _strategyId);
        _positions[tickerId] =  pos;
    }
}

///Closes a specific position at MKT price
void PositionManager::closePosition(const TickerId tickerId)
{
    if(_positions.count(tickerId))
    {
        int quantity = _positions[tickerId]->getNetPositionDetail().getNetShares();
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

///closes all open positions
/* Sends MKT order to close all positions*/
void PositionManager::closeAllPositions()
{
    PositionPtrMap::iterator end = _positions.end();
    for(PositionPtrMap::iterator it=_positions.begin(); it!=end; ++it)
	{
        closePosition(it->second);
    }
}

void PositionManager::addPositionInOutputs(const StrategyId strategyId, const TickerId tickerId, const OutputType type)
{
    //this is called on strategy thread but strategy output is running on a different thread
    //this function is though called on Strategy Thread

    StrategyOutput::strategyOutput().addPosition(strategyId, tickerId, type);
}

void PositionManager::updateOutputsForExecution(const Position* currentPosition, const Position* cumulativePosition, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForExecution(currentPosition, cumulativePosition, type);
}

void PositionManager::updateOutputsForLastPrice(const Position* position, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForLastPrice(position, type);
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

const PositionDetail PositionManager::getPositionDetail(const TickerId tickerId)
{
    if(Position* position = _positions[tickerId])
    {
        return position->getNetPositionDetail();
    }

    return PositionDetail();
}


