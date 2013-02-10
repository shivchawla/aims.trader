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

/*
 * PositionManager Constructor
 */
PositionManager::PositionManager(Strategy* strategySPtr):_strategyWPtr(strategySPtr)
{
    initialize();
}

/*
 * Initialize
 */
void PositionManager::initialize()
{
    _performanceManager = _strategyWPtr->getPerformanceManager();
    _strategyId = _strategyWPtr->getStrategyId();
    //_positionId = -1;
    _numPositions = 0;
}

/*
 * Destructor
 */
PositionManager::~PositionManager()
{}

/*
 * Update position for order execution
 */
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

/*
 * Update position for order execution. NOT IN USE.
 */
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

/*
 * Updates the position with last traded price
 */
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

         //this flag is only true for single stock strategy
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

/*
 * Add position for tickerId
 */
void PositionManager::addPosition(const TickerId tickerId)
{
    Position* netPosition = _positions.value(tickerId, NULL);
    if(!netPosition)
    {
        netPosition = new Position(tickerId, _strategyId);
        _positions[tickerId] = netPosition;

        //**line below is commented.. I don't think it's required. BUT check
        //subscribeToMktData(tickerId);

        addPositionInOutputs(_strategyId, tickerId);
    }
}

/*
 * Load position from position data
 */
void PositionManager::loadPosition(const TickerId tickerId, const PositionData& data)
{
    Position* historicalPos = new Position(tickerId, _strategyId, data);

    _positions[tickerId] = historicalPos;

    addPositionInOutputs(_strategyId, tickerId, GUI);
    updateOutputsForExecution(historicalPos, historicalPos, GUI);
    _performanceManager->loadPosition(historicalPos);
}

/*
 *Closing all SubPositions for a tickerId
 *Closes a specific position at MKT price
 */
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

/*
 * Close a position for an instrument
 */
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

/*
 * Get position for tickerId
 */
Position* PositionManager::getPosition(const TickerId tickerId)
{
   return _positions.value(tickerId, NULL);
}

/*
 * Close position at MKT order
 */
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

/*
 * Closes all open positions
 * Sends MKT order to close all positions
 */
void PositionManager::closeAllPositions()
{
    //typedef Position* PositionPointer
    Positions::iterator end = _positions.end();
    for(Positions::iterator it=_positions.begin(); it!=end; ++it)
    {
        closePosition(it.value());
    }
}

/*
 * Add new position to outputs
 */
void PositionManager::addPositionInOutputs(const StrategyId strategyId, const TickerId tickerId, const OutputType type)
{
    StrategyOutput::strategyOutput().addPosition(strategyId, tickerId, type);
}

/*
 * Update outputs for execution
 */
void PositionManager::updateOutputsForExecution(const Position* currentPosition, const Position* cumulativePosition, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForExecution(currentPosition, cumulativePosition, type);
}

/*
 * Update outputs for price updates
 */
void PositionManager::updateOutputsForLastPrice(const Position* position, const OutputType type)
{
    StrategyOutput::strategyOutput().updatePositionForLastPrice(position, type);
}

/*
 * Remove position from position view
 */
void PositionManager::removeFromPositionView(const StrategyId strategyId, const PositionId positionId)
{
   // emit positionRemoved(strategyId, positionId);
}

/*
 * Subscribe to market data
 */
void PositionManager::subscribeToMktData(const TickerId instrumentId)
{
    _strategyWPtr->subscribeMarketData(instrumentId);
}

/*
 * Unsubscribe to market data
 */
void PositionManager::unSubscribeToMktData(const TickerId instrumentId)
{
    _strategyWPtr->unSubscribeMarketData(instrumentId);
}

/*
 * Update performance for price update
 *
 */
void PositionManager::updatePerformanceForPrice(const Position* position)
{
    _performanceManager->updatePerformanceForPrice(position);
}

/*
 * Update performance for execution
 */
void PositionManager::updatePerformanceForExecution(const Position* position)
{
    _performanceManager->updatePerformanceForExecution(position);
}

/*
 * Add sub-position for main position
 * There is one main position for every tickerId.
 * For eg: All spread position are child positions of main position
 */
void PositionManager::addSubPosition(const Position* subPosition)
{
    TickerId tickerId = subPosition->getTickerId();
    addPosition(tickerId);
    Position* netPosition = _positions.value(tickerId, NULL);
    if(netPosition)
    {
        netPosition->addChildPosition(subPosition);
    }
}


