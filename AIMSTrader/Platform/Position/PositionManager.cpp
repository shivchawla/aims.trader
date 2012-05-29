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
#include "Platform/View/StrategyPositionView.h"
#include "Platform/Trader/OrderManager.h"
#include <math.h>
#include <QDebug>
#include "Platform/Startup/OutputService.h"
#include "Platform/View/OutputInterface.h"

PositionManager::PositionManager():QObject()//,_currentPositionId(0)
{
    lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
    //lockForPositionMap= new QMutex();
}

PositionManager::PositionManager(Strategy* strategySPtr):QObject()
                                //,_currentPositionId(0)
                                ,_strategyWPtr(strategySPtr)
{
    lockForPositionMap = new QReadWriteLock(QReadWriteLock::Recursive);
    //lockForPositionMap= new QMutex();
    //QObject::connect(this, SIGNAL(positionCreated(const StrategyId, const PositionId, const TickerId)), MainWindow::mainWindow()->getPositionView(), SLOT(addPosition(const StrategyId, const PositionId, const TickerId)));
    //QObject::connect(this, SIGNAL(positionRemoved(const StrategyId, const PositionId)), MainWindow::mainWindow()->getPositionView(), SLOT(removePosition(const StrategyId, const PositionId)));
    //QObject::connect(this, SIGNAL(executionUpdated(const StrategyId, const PositionId, const int,const double, const double, const double)), MainWindow::mainWindow()->getPositionView(), SLOT(onExecutionUpdate(const StrategyId, const PositionId, const int, const double, const double, const double)));
    //QObject::connect(this, SIGNAL(lastPriceUpdated(const StrategyId, const PositionId, const double, const double)), MainWindow::mainWindow()->getPositionView(), SLOT(onTradeUpdate(const StrategyId, const PositionId, const double, const double)));

    //QObject::connect(this, SIGNAL(positionCreated(const StrategyId, const TickerId)), OutputService::Instance()->getOutputInterface(), SLOT(addPosition(const StrategyId, const TickerId)));
    //QObject::connect(this, SIGNAL(positionRemoved(const StrategyId, const TickerId)), OutputService::Instance()->getOutputInterface(), SLOT(removePosition(const StrategyId, const TickerId)));
    //QObject::connect(this, SIGNAL(executionUpdated(const Position&)), OutputService::Instance()->getOutputInterface(), SLOT(onExecutionUpdate(const Position&)));
    //QObject::connect(this, SIGNAL(lastPriceUpdated(const StrategyId, const TickerId, const double)), OutputService::Instance()->getOutputInterface(), SLOT(onLastPriceUpdate(const StrategyId, const TickerId, const double, const double)));
}

PositionManager::~PositionManager()
{}

void PositionManager::linkPerformanceManager(PerformanceManager* performanceManagerWptr)
{
    _performanceManager = performanceManagerWptr;
    _outputInterface = OutputService::Instance()->getOutputInterface();
}

const PositionPtrMap& PositionManager::getCurrentPositions()
{
	return _currentPositions;
}

///Updates the position with execution information
void PositionManager::updatePosition(const OrderId orderId, const TickerId tickerId, const Execution& execution)
{
    //lockForPositionMap->lockForRead();
    //qDebug("entered locked area: update Position execution");

    //if(_orderIdToTickerId.count(orderId) != 0)
    //{
        //TickerId tickerId = _orderIdToTickerId[orderId];

        if(!_currentPositions.count(tickerId))
        {
            addPosition(orderId, tickerId);
        }
            Position* currentPosition = _currentPositions[tickerId];

            currentPosition->update(execution);
            //subscribe to market data
            subscribeToMktData(currentPosition->getTickerId());

            //calculate realized pnl as a result of closing trade
            //double realizedPnl = currentPosition->updatePosition(executionStatus, isClosingOrder);

            //updatePositionViewForExecution(positionId, currentPosition);
            //updateOutputs(currentPosition->getStrategyId(), currentPosition->getTickerId(), executionStatus);
            updateOutputsForExecution(currentPosition);
            //updateOutputsForLastPrice(currentPosition->getStrategyId(), tickerId, currentPosition);

             //either setup a commission manager who yields commission based on ticker
            //or add a functionality in InstrumentManager to give commissions based on trade size
            //_performanceManager->updateOnOrderFill(executionStatus.execution.shares, executionStatus.execution.avgPrice, 0);
            //_performanceManager->bookPnL(realizedPnl);

    //}
    //lockForPositionMap->unlock();
    //qDebug("Left locked area: update Position execution");

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

         //double positionValue = position->getPositionValue();
         //double currentPnL = position->getTradeProfit();

         updateOutputsForLastPrice(_strategyWPtr->getStrategyId(), tickerId, position);
         //updatePerformance(positionId, currentPnL);
     }

    //lockForPositionMap->unlock();
   //qDebug("Left locked area: update Position price");

}

void PositionManager::addPosition(const OrderId orderId, const TickerId tickerId)//, const bool isClosingPosition)
{
    //lockForPositionMap->lockForWrite();
    //qDebug("entered locked area: add Position");
    //_orderIdToTickerId[orderId] = tickerId;
    if(!_currentPositions.count(tickerId))
    {
       createNewPosition(orderId, tickerId);
    }
    //lockForPositionMap->unlock();
    //qDebug("Left locked area: add Position");
}

const PositionId PositionManager::createNewPosition(const OrderId orderId, const TickerId tickerId)
{
    StrategyId strategyId = _strategyWPtr->getStrategyId();
    Position* newPosition = new Position(tickerId, strategyId);
    _currentPositions[tickerId] =  newPosition;
    addPositionInOutputs(strategyId, tickerId);
}

///Closes a specific position at MKT price
void PositionManager::closePosition(const TickerId tickerId, const int quantity)
{
	//create an order to close the position
    //send the order
    //create a MKT order
    Order mktOrder;
    mktOrder.orderType = "MKT";

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
}

///closes all open positions
/* Sends MKT order to close all positions*/
void PositionManager::closeAllPositions()
{
    //lockForPositionMap->lockForRead();
    //qDebug("entered locked area: close Position");

	PositionPtrMap::iterator end = _currentPositions.end();
	for(PositionPtrMap::iterator it=_currentPositions.begin(); it!=end; ++it)
	{
        Position* position = it->second;
        closePosition(position->getTickerId(), position->getNetShares());
        //(it->second)->updateStatus(Closing);
    }
    //lockForPositionMap->unlock();
   //qDebug("left locked area: close Position");

}

void PositionManager::addPositionInOutputs(const StrategyId strategyId, const TickerId tickerId)
{
    _outputInterface->addPosition(strategyId, tickerId);
    //emit positionCreated(strategyId, positionId, tickerId);
    //emit positionCreated(strategyId, tickerId);
}


void PositionManager::updatePerformance(const double pnl, const bool currentProfitability, const bool lastProfitability)
{
    _performanceManager->updatePerformance(pnl, currentProfitability, lastProfitability);
}


void PositionManager::updateOutputsForExecution(const Position* position)
{
    _outputInterface->updatePositionForExecution(position);
    //emit executionUpdated(*position);
}

void PositionManager::updateOutputsForLastPrice(const StrategyId strategyId, const TickerId tickerId, const Position* position)
{
    _outputInterface->updatePositionForLastPrice(strategyId, tickerId, position);
    //emit lastPriceUpdated(strategyId, tickerId, position->getRunningPnl(), position->getPnL());
}

/*void PositionManager::updateOutputs(const StrategyId strategyId, const TickerId tickerId, const double lastPrice)
{
     //emit lastPriceUpdated(strategyId, tickerId, lastPrice);
}

void PositionManager::updateOutputs(const StrategyId strategyId, const TickerId tickerId, const ExecutionStatus status)
{
     //emit  executionUpdated(strategyId, tickerId, status);
}*/

void PositionManager::bookPnLOnClosingTrade(const double pnl)
{
    _performanceManager->bookPnL(pnl);
}

void PositionManager::removeFromPositionView(const StrategyId strategyId, const PositionId positionId)
{
   // emit positionRemoved(strategyId, positionId);
}

void PositionManager::subscribeToMktData(const TickerId tickerId)
{
    _strategyWPtr->requestMarketData(tickerId, IB);
}

void PositionManager::updatePerformance(const PositionId positionId, const double pnl)
{
    _performanceManager->updatePerformance(positionId, pnl);
}

