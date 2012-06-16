/*
 *  Strategy.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 11/24/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Strategy/Strategy.h"
#include "Platform/Startup/Service.h"
#include "Platform/Indicator/IndicatorManager.h"
#include "Platform/Position/PositionManager.h"
#include "Platform/Reports/StrategyReport.h"
#include "Platform/Utils/TradingSchedule.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/View/OutputInterface.h"

StrategyId Strategy::_id = -1;
Strategy::Strategy():DataSubscriber()
{
    _id++;
    _running=false;
    _canOpenNewPositions = true;

    QThread* thread = ThreadManager::Instance()->requestThread();
    moveToThread(thread);
    qRegisterMetaType<Contract>("Contract");
    qRegisterMetaType<Order>("Order");
    connect(thread, SIGNAL(started()), this, SLOT(startStrategy()));
    connect(this, SIGNAL(closeAllPositionsRequested()), this, SLOT(closeAllPositions()));
    connect(this, SIGNAL(closePositionRequested(TickerId)), this, SLOT(closePosition(TickerId)));
    connect(this, SIGNAL(adjustPositionRequested(TickerId,Order)), this, SLOT(adjustPosition(TickerId, Order)));
}

Strategy::Strategy(const String& strategyName):DataSubscriber()
{
    _id++;
    _strategyName = strategyName;
    _running=false;
    _canOpenNewPositions = true;

    QThread* thread = ThreadManager::Instance()->requestThread();
    moveToThread(thread);
    qRegisterMetaType<Contract>("Contract");
    qRegisterMetaType<Order>("Order");
    connect(thread, SIGNAL(started()), this, SLOT(startStrategy()));
    connect(this, SIGNAL(closeAllPositionsRequested()), this, SLOT(closeAllPositions()));
    connect(this, SIGNAL(closePositionRequested(TickerId)), this, SLOT(closePosition(TickerId)));
    connect(this, SIGNAL(adjustPositionRequested(TickerId,Order)), this, SLOT(adjustPosition(TickerId, Order)));
}

//sets the alarm for strategy to start and stop based on weekdays, holidays and exchange timings
void Strategy::setTimeout()
{
    QDate nextDate = getNextValidDate();
    QDateTime nextStartDateTime(nextDate, _tradingSchedule->getStartTime());
    QDateTime nextStopDateTime(nextDate, _tradingSchedule->getEndTime());
    int msecondsToStart = QDateTime::currentDateTime().msecsTo(nextStartDateTime);
    int msecondsToStop = QDateTime::currentDateTime().msecsTo(nextStopDateTime);

    if(msecondsToStart<=0 && msecondsToStop > 0)//already running condition
    {
        _running = true;
        _timeout = msecondsToStop;//*1000;
        _basicTimer.start(_timeout,this);
    }
    else
    {
        _running = false;
        _timeout = msecondsToStart;//*1000;
        _basicTimer.start(_timeout,this);
    }
}

/*const QDateTime& Strategy::getNextValidStartTime()
{
    QDate nextDate = getNextValidDate();
    QDate nextDateTime(nextDate, _tradingSchedule->getStartTime());
    int secondsToStart = QDateTime::currentDateTime().secsTo(_tradingSchedule->getStartTime());

    if(secondsToStart<0)

}*/

const QDate& Strategy::getNextValidDate()
{
    bool validDateSet=false;
    QDate nextDate = QDate::currentDate();
   // QDate nextDateTime;
    QTime currentTime;

    while(!validDateSet)
    {
        int dayOfWeek = nextDate.dayOfWeek();
        if(dayOfWeek!=6 && dayOfWeek!=7)
        {
            currentTime = QDateTime::currentDateTime().time();
            int secondsToStop = currentTime.secsTo(_tradingSchedule->getEndTime());
            if(secondsToStop<0)
            {
                nextDate.addDays(1);
            }
            else
            {
                validDateSet = true;
            }
        }
        else if(dayOfWeek == 6)
        {
            //getTo moday
            nextDate.addDays(2);
            //validDateSet = true;
        }
        else if(dayOfWeek == 7)
        {
            nextDate.addDays(1);
            //validDateSet = true;
        }

    }
    //check for holiday
    return nextDate;
}


void Strategy::initialize()
{
    //these objects are still on MAIN thread and not on strategy Thread
    _performanceManagerSPtr = new PerformanceManager(this);
    _positionManagerSPtr = new PositionManager(this);
    _strategyReportSPtr = new StrategyReport(_strategyName);
    linkWorkers();
}

void Strategy::linkWorkers()
{
    _positionManagerSPtr->linkPerformanceManager(_performanceManagerSPtr);
}

void Strategy::setName()
{}

Strategy::~Strategy()
{
    //make sure that all the sub components of strategy are deleted too.
    printf("deleting strategy components");
    //before deleting check for any active positions
    //remove all active positions before deleting all the components
    delete _performanceManagerSPtr;
    delete _strategyReportSPtr;
    //delete _indicatorManagerSPtr;
    delete _positionManagerSPtr;
    delete _tradingSchedule;
}

void Strategy::closeAllPositions()
{
	_positionManagerSPtr->closeAllPositions();
}

void Strategy::closePosition(const TickerId tickerId)
{
    _positionManagerSPtr->closePosition(tickerId);
}

void Strategy::adjustPosition(const TickerId tickerId, const Order& order)
{
    placeOrder(tickerId, order);
}

///Places order for a given contract
void Strategy::placeOrder(const Contract& contract, const Order& order)
{
    if(_canOpenNewPositions)
    {
        subscribeMarketData(contract,IB);
        Service::Instance()->getOrderManager()->placeOrder(order, contract, this);
    }
    else
    {
        reportEvent("Cannot trade this strategy anymore.");
    }
    //Service::Instance()->getOrderManager()->placeOrder(order, contract, this);
    //requestMarketData(contract,IB);
}

void Strategy::placeClosingOrder(const Contract& contract, const Order& order)
{
    Service::Instance()->getOrderManager()->placeOrder(order, contract, this);//, true);
}

void Strategy::placeClosingOrder(const TickerId tickerId, const Order& order)
{
    Service::Instance()->getOrderManager()->placeOrder(order, tickerId, this);//, true);
}

///Places order for a given tickerId
void Strategy::placeOrder(const TickerId tickerId, const Order& order)
{
    if(_canOpenNewPositions)
    {
        subscribeMarketData(tickerId,IB);
        Service::Instance()->getOrderManager()->placeOrder(order, tickerId, this);
    }
    else
    {
        reportEvent("Cannot trade this strategy anymore.");
    }
    //Service::Instance()->getOrderManager()->placeOrder(order, tickerId, this);
}

/*void Strategy::addPosition(const OrderId orderId, const Contract& contract)
{
    //link contractId to orderId
    _positionManagerSPtr->addPosition(orderId, contract);
}*/

void Strategy::addPosition(const OrderId orderId, const TickerId tickerId)
{
    //link contractId to orderId
    _positionManagerSPtr->addPosition(orderId, tickerId);
}


///Adds position to the startegy
/*void Strategy::addPosition(const OrderId orderId, const TickerId tickerId)//, const bool isClosingPosition)
{
    _positionManagerSPtr->addPosition(orderId, tickerId);//, isClosingPosition);
}*/

const String& Strategy::getStrategyName()
{
    return _strategyName;
}

const StrategyId Strategy::getStrategyId()
{
    return _id;
}

//SLOTS
void Strategy::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
{
    double lastPrice = tradeUpdate.lastPrice;
    _positionManagerSPtr->updatePosition(tickerId, lastPrice);
}

void Strategy::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
{}

void Strategy::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double value)
{
    switch(tickType)
    {
     case LAST: _positionManagerSPtr->updatePosition(tickerId, value); break;
     default: break;
    }
}

void Strategy::onExecutionUpdate(const OrderId orderId, const TickerId tickerId, const Execution& execution)//, const bool isClosingOrder)
{
    _positionManagerSPtr->updatePosition(orderId, tickerId, execution);//, isClosingOrder);
}

void Strategy::startStrategy()
{
    setTimeout();
    if(_running == true)
    {
        emit startIndicator();
    }
    else
    {
        emit stopIndicator();
    }
}

void Strategy::reportEvent(const String& message)
{
    OutputInterface::Instance()->reportEvent(_strategyName, message);
}

void Strategy::timerEvent(QTimerEvent* event)
{
    if(event->timerId()==_basicTimer.timerId()) //timer even triggered by stratgey itself
    {
        _basicTimer.stop();
        //now set a new time-out
        setTimeout();
    }
}

//this is important in case you see some weird behaviour
void Strategy::stopStrategy()
{
    //set flag to ope more positon to false;
    _canOpenNewPositions=false;

    //ask the stratgey Indicator thread to stop

    //ask Position manager to close all existing psoitions
    closeAllPositions();

    //ask the indicator thrad to terminate
    _indicatorSPtr->stopIndicator();;
}

void Strategy::requestCloseAllPositions()
{
    emit closeAllPositionsRequested();
}

void Strategy::requestClosePosition(const TickerId tickerId)
{
   emit closePositionRequested(tickerId);
}

void Strategy::requestAdjustPosition(const TickerId tickerId, const Order& order)
{
    emit adjustPositionRequested(tickerId, order);
}







