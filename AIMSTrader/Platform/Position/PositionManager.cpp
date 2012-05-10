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
<<<<<<< HEAD
#include "Platform/View/PositionView.h"
#include "Platform/Trader/OrderManager.h"
#include <math.h>
#include <QDebug>


PositionManager::PositionManager():QObject(),_currentPositionId(0)
{
    lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
    //lockForPositionMap= new QMutex();
}

=======

PositionManager::PositionManager():QObject(),_currentPositionId(0)
{
    lockForPositionMap = new QReadWriteLock();
}

>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
PositionManager::PositionManager(Strategy* strategySPtr):QObject()
                                ,_currentPositionId(0)
                                ,_strategyWPtr(strategySPtr)
{
<<<<<<< HEAD
    lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
    //lockForPositionMap= new QMutex();
    QObject::connect(this, SIGNAL(positionCreated(const StrategyId, const PositionId, const TickerId)), MainWindow::mainWindow()->getPositionView(), SLOT(addPosition(const StrategyId, const PositionId, const TickerId)));
    QObject::connect(this, SIGNAL(positionRemoved(const StrategyId, const PositionId)), MainWindow::mainWindow()->getPositionView(), SLOT(removePosition(const StrategyId, const PositionId)));
    QObject::connect(this, SIGNAL(executionUpdated(const StrategyId, const PositionId, const int,const double, const double, const double)), MainWindow::mainWindow()->getPositionView(), SLOT(onExecutionUpdate(const StrategyId, const PositionId, const int, const double, const double, const double)));
    QObject::connect(this, SIGNAL(lastPriceUpdated(const StrategyId, const PositionId, const double, const double)), MainWindow::mainWindow()->getPositionView(), SLOT(onTradeUpdate(const StrategyId, const PositionId, const double, const double)));
=======
    lockForPositionMap=new QReadWriteLock();
    //connect(this, SIGNAL((const Position&)), MainWindow::mainWindow()->getPositionView(), SLOT(onPositionUpdate(const Position&)));
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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

<<<<<<< HEAD
///Updates the position with execution information
void PositionManager::updatePosition(const OrderId orderId, const ExecutionStatus& executionStatus, const bool isClosingOrder)
=======
/*void PositionManager::updatePosition(const OrderId orderId, const Execution& execution)
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    lockForPositionMap->lockForRead();
    //lockForPositionMap->lock();
    //bool isClosingOrder = Service::Instance()->getOrderManager()->IsClosingOrder(orderId);
    //qDebug("entered locked area: update Position execution");

    if(_orderIdToPositionId.count(orderId) != 0)
    {
        PositionId positionId = _orderIdToPositionId[orderId];
        Position* currentPosition = _currentPositions[positionId];

        //update the number of trades
        //this happens for the first time only
        if(!_firstExecutionHasArrived[orderId] && !isClosingOrder)
        {
            //first execution arrived
            //no more a fresh order
            _firstExecutionHasArrived[orderId] = true;
            //if the execution quantity is positive
            //update long trade
            if(executionStatus.execution.cumQty>0)
            {
                _performanceManager->updateLongTrades();
            }
            //if execution quantity is negative
            //update short trades
            else if(executionStatus.execution.cumQty<0)
            {
                _performanceManager->updateShortTrades();
            }

            //subscribe to market data
            subscribeToMktData(currentPosition->getTickerId());
        }

        //calculate realized pnl as a result of closing trade
        double realizedPnl = currentPosition->updatePosition(executionStatus, isClosingOrder);

        updatePositionViewForExecution(positionId, currentPosition);

        //either setup a commission manager who yields commission based on ticker
        //or add a functionality in InstrumentManager to give commissions based on trade size
         _performanceManager->updateOnOrderFill(executionStatus.execution.shares, executionStatus.execution.avgPrice, 0);
        _performanceManager->bookPnL(realizedPnl);


        /*lockForPositionMap->lockForWrite();
        //check if position is closed
        if(_currentPositions[positionId]->getPositionStatus()==Closed)
        {
            _historicalPositions[positionId] = _currentPositions[positionId];
            _currentPositions.erase(positionId);
            removeFromPositionView(_strategyWPtr->getStrategyId(), positionId);
        }
        lockForPositionMap->unlock();*/
    }
<<<<<<< HEAD
    lockForPositionMap->unlock();
    //qDebug("Left locked area: update Position execution");
=======
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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

///Updates the position with last traded price
void PositionManager::updatePosition(const TickerId tickerId, const double lastPrice)
{
<<<<<<< HEAD
    //Position oldPosition, updatedPosition;
    //lock the position map for reads
     //lockForPositionMap->lockForRead();

    lockForPositionMap->lockForRead();
    //qDebug("entered locked area: update Position price");

    if(_tickerIdToPositionId.count(tickerId)!=0)
     {
         PositionId  positionId = _tickerIdToPositionId[tickerId];
         //Position oldPosition(*(_currentPositions[positionId]));

         if(_currentPositions.count(positionId))
         {
             Position* position = _currentPositions[positionId];
             //double oldLastPrice = position->getLastPrice();
             //double oldTradeProfit = position->getTradeProfit();

             position->updatePosition(lastPrice);

             double positionValue = position->getPositionValue();
             //double avgFillPrice = position->getAvgFillPrice();
             double currentPnL = position->getTradeProfit();

             updatePositionViewForLastPrice(_strategyWPtr->getStrategyId(), positionId, positionValue, currentPnL);
             updatePerformance(positionId, currentPnL);
         }
      }

    lockForPositionMap->unlock();
   // qDebug("Left locked area: update Position price");

}

///Add/Updates a new/old position with placement of an order
void PositionManager::addPosition(const OrderId orderId, const TickerId tickerId, const bool isClosingPosition)
=======
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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
{
    PositionId positionId;

    lockForPositionMap->lockForRead();
    //qDebug("entered locked area: add Position");
    if(_tickerIdToPositionId.count(tickerId)!=0 || isClosingPosition)
    {
        positionId = _tickerIdToPositionId[tickerId];
        _orderIdToPositionId[orderId] = positionId;
        //this map tells whether order execution has started or not
        _firstExecutionHasArrived[orderId] = false;
    }
<<<<<<< HEAD
    else
    {
        lockForPositionMap->unlock();


        lockForPositionMap->lockForWrite();
        positionId = createNewPosition(orderId, tickerId);
        _orderIdToPositionId[orderId] = positionId;
        //this map tells whether order execution has started or not
        _firstExecutionHasArrived[orderId] = false;
    }
    lockForPositionMap->unlock();
   // qDebug("Left locked area: add Position");
=======
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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

const PositionId PositionManager::createNewPosition(const OrderId orderId, const TickerId tickerId)
{
    StrategyId strategyId = _strategyWPtr->getStrategyId();
    //update the tickerId and OrderId maps
    PositionId positionId = ++_currentPositionId;
    _tickerIdToPositionId[tickerId] = positionId;
    Position* newPosition = new Position(tickerId, strategyId);
    _currentPositions[positionId] =  newPosition;
    newPosition->updateStatus(Active);

    //update Position View with new position
    addPositionInView(strategyId, positionId, tickerId);

    return positionId;
}

///Closes a specific position at MKT price
void PositionManager::closePosition(const TickerId tickerId, const int quantity)
{
	//create an order to close the position
    //send the order
    //create a MKT order
    Order mktOrder;
    mktOrder.orderType = "MKT";
<<<<<<< HEAD

    /*lockForPositionMap->lock();//ForRead();
    TickerId tickerId = _currentPositions[positionId]->getTickerId();
    int quantity = _currentPositions[positionId]->getQuantity();
    lockForPositionMap->unlock();*/

    if(abs(quantity)>0)
    {
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
=======
    mktOrder.action = "CLOSE";
    mktOrder.totalQuantity = (-1)* _currentPositions[positionId]->getQuantity();

    //Contract contract = Service::Instance()->getInstrumentManager()->getContractForTicker(_currentPositions[positionId]->getTickerId());
    //_strategyWPtr->placeOrder(_currentPositions[positionId]->getContract(), mktOrder);
    //_stratgeyWPtr->placeClosingOrder(_currentPositions[positionId]->getTickerId(),mktOrder);

    lockForPositionMap->lockForRead();
    TickerId tickerId = _currentPositions[positionId]->getTickerId();
    lockForPositionMap->unlock();

    _strategyWPtr->placeClosingOrder(tickerId, mktOrder);
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

///closes all open positions
/* Sends MKT order to close all positions*/
void PositionManager::closeAllPositions()
{
    lockForPositionMap->lockForRead();
    //qDebug("entered locked area: close Position");

	PositionPtrMap::iterator end = _currentPositions.end();
	for(PositionPtrMap::iterator it=_currentPositions.begin(); it!=end; ++it)
	{
        Position* position = it->second;
        closePosition(position->getTickerId(), position->getQuantity());
        //(it->second)->updateStatus(Closing);
    }
    lockForPositionMap->unlock();
   // qDebug("left locked area: close Position");

}


void PositionManager::addPositionInView(const StrategyId strategyId, const PositionId positionId, const TickerId tickerId)
{
    emit positionCreated(strategyId, positionId, tickerId);
}

void PositionManager::updatePositionViewForLastPrice(const StrategyId strategyId, const PositionId positionId, const double positionValue, const double tradeProfit)
{
    emit lastPriceUpdated(strategyId, positionId, positionValue, tradeProfit);
}

void PositionManager::updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability)
{
    _performanceManager->updatePerformance(pnl, currentProfitability, lastProfitability);
}

void PositionManager::updatePositionViewForExecution(const PositionId positionId, Position* currentPosition)
{
    emit executionUpdated(currentPosition->getStrategyId(), positionId , currentPosition->getQuantity(), currentPosition->getAvgFillPrice(), currentPosition->getPositionValue(), currentPosition->getCommission());
}

void PositionManager::bookPnLOnClosingTrade(const double pnl)
{
    _performanceManager->bookPnL(pnl);
}

void PositionManager::removeFromPositionView(const StrategyId strategyId, const PositionId positionId)
{
    emit positionRemoved(strategyId, positionId);
}

void PositionManager::subscribeToMktData(const TickerId tickerId)
{
    _strategyWPtr->requestMarketData(tickerId, IB);
}

void PositionManager::updatePerformance(const PositionId positionId, const double pnl)
{
    _performanceManager->updatePerformance(positionId, pnl);
}

