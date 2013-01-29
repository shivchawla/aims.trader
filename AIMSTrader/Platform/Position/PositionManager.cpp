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
#include "Platform/Trader/RiskManager.h"
#include "Platform/Position/OpenOrder.h"

PositionManager::PositionManager(Strategy* strategySPtr):_strategyWPtr(strategySPtr)
{
    initialize();
}

void PositionManager::initialize()
{
    _performanceManager = _strategyWPtr->getPerformanceManager();
    _strategyId = _strategyWPtr->getStrategyId();
    _positionId = -1;
    _numPositions = 0;
}

PositionManager::~PositionManager()
{}

//void PositionManager::updatePosition(const OpenOrder& openOrder)
//{
//    OrderId orderId = openOrder.getOrderId();
//    TickerId tickerId = _orderIdToTickerId.value(orderId, -1);
//    if(Position* position = getPosition(tickerId))
//    {
//        OrderDetail orderDetail = openOrder.getOrderDetail();
//        long quantity = orderDetail.lastFilledShares;
//        long filledShares = (orderDetail.order.action == "SELL") ? -quantity : quantity;
//        double fillPrice = orderDetail.lastFillPrice;
//        double commission = orderDetail.commission;
//        double avgFillPrice = orderDetail.avgFillPrice;

//        position->update(orderId, filledShares, fillPrice, avgFillPrice, commission);

//        _strategyWPtr->_totalInvested += position->getAmountInvested();
//        updateOutputsForExecution(position, position);
//        updatePerformanceForExecution(position);

//        if(position->getNetShares()==0)
//        {
//            unSubscribeToMktData(tickerId);
//        }
//    }
//}

void PositionManager::updatePosition(const OrderId orderId, const TickerId tickerId, const OrderDetail& orderDetail, const bool unSubscribe)
{
    addPosition(tickerId);
    if(Position* position = getPosition(tickerId))
    {
        position->update(orderId, orderDetail);
        updateOutputsForExecution(position, position);
        updatePerformanceForExecution(position);

        if(position->getNetShares()==0 && unSubscribe)
        {
            unSubscribeToMktData(tickerId);
        }
    }

    //update RiskManager with new order update
    Service::service().getRiskManager()->updatePosition(orderId, tickerId, orderDetail);
}

void PositionManager::updatePosition(const OrderId orderId, const TickerId tickerId, const int filledShares, const double fillPrice, const double commission)
{
    if(Position* position = getPosition(tickerId))
    {
        position->update(orderId, filledShares, fillPrice, 0, commission);

        updateOutputsForExecution(position, position);
        updatePerformanceForExecution(position);

        if(position->getNetShares()==0)
        {
            unSubscribeToMktData(tickerId);
        }
    }
}

///Updates the position with last traded price
void PositionManager::updatePosition(const TickerId tickerId, const TickType tickType, const double lastPrice, const bool testExitConditions)
{
    if(Position* position = getPosition(tickerId))
    {
         position->update(tickType, lastPrice);

         updateOutputsForLastPrice(position);
         updatePerformanceForPrice(position);

         int minutesSinceLastUpdated = position->lastUpdated().secsTo(QDateTime::currentDateTime())/60;

         double ret = position->getReturn();
         int maxHoldingPeriod = _strategyWPtr->getMaxHoldingPeriod();

         if(!testExitConditions)
         {
             return;
         }

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
    Position* netPosition = _positions.value(tickerId, NULL);
    if(!netPosition)
    {
        netPosition = new Position(tickerId, _strategyId);
        _positions[tickerId] = netPosition;
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

//const PositionId PositionManager::createNewPosition(const TickerId tickerId)
//{
//    Position* pos = _subPositions[tickerId];
//    if(!pos)
//    {
//        pos = new Position(tickerId, _strategyId);
//        _subPositions[tickerId] =  pos;
//    }
//}

//Closing all SubPositions for a tickerId
///Closes a specific position at MKT price
void PositionManager::closePositions(const TickerId tickerId)
{
//    if(Position* pos = _netPositionMap.value(tickerId, NULL))
//    {
//        int quantity = pos->getNetShares();
//        //create an order to close the position
//        //send the order
//        //create a MKT order
//        Order mktOrder;
//        mktOrder.orderType = "MKT";

//        mktOrder.totalQuantity = abs(quantity);

//        if(quantity>0)
//        {
//            mktOrder.action = "SELL";
//        }
//        else
//        {
//            mktOrder.action = "BUY";
//        }

//        _strategyWPtr->placeClosingOrder(tickerId, mktOrder);
//    }
}

void PositionManager::closePosition(const TickerId tickerId)
{
    if(Position* pos = getPosition(tickerId))
    {
        int quantity = pos->getNetShares();
          //create an order to close the position
          //send the order
          //create a MKT order
        TickerId tickerId = pos->getTickerId();

        Order mktOrder;
        mktOrder.orderType = "MKT";
        mktOrder.totalQuantity = abs(quantity);

        if(quantity>0)
        {
            mktOrder.action = "SELL";
        }
        else
        {
            mktOrder.action = "BUY";
        }

        _strategyWPtr->placeOrder(tickerId, mktOrder);
        //_orderIdToTickerId[orderId] = tickerId;
    }
}

//Position* PositionManager::getPosition(const PositionId positionId)
//{
//    if(positionId > -1 && positionId < _numPositions)
//    {
//        return _positions[positionId];
//    }

//    return NULL;
//}

Position* PositionManager::getPosition(const TickerId tickerId)
{
   return _positions.value(tickerId, NULL);
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

///closes all open positions
/* Sends MKT order to close all positions*/
void PositionManager::closeAllPositions()
{
//    PositionPtrMap::iterator end = _positions.end();
//    for(PositionPtrMap::iterator it=_positions.begin(); it!=end; ++it)
//	{
//        closePosition(it->second);
//    }
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

void PositionManager::addPosition(const Position* subPosition)
{
    TickerId tickerId = subPosition->getTickerId();
    Position* netPosition = _positions.value(tickerId, NULL);
    if(!netPosition)
    {
        _positions[tickerId] = netPosition = new Position(tickerId, _strategyId);
        _numPositions++;
        addPositionInOutputs(_strategyId, tickerId);
    }

    netPosition->addChildPosition(subPosition);
}


