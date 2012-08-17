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
#include "Platform/View/IOInterface.h"
#include <QDir>
#include <QDebug>
#include "Platform/View/IODatabase.h"

int Strategy::_id = -1;
Strategy::Strategy():DataSubscriber()
{
    initialize();
    setupConnection();
}

Strategy::Strategy(const String& strategyName):DataSubscriber()
{
    _strategyName = strategyName;
    initialize();
    setupConnection();
}

void Strategy::setupConnection()
{
    connect(this, SIGNAL(closeAllPositionsRequested()), this, SLOT(closeAllPositions()));
    connect(this, SIGNAL(closePositionRequested(const TickerId)), this, SLOT(closePosition(const TickerId)));
    connect(this, SIGNAL(adjustPositionRequested(const TickerId, const Order&)), this, SLOT(adjustPosition(const TickerId, const Order&)));
    connect(this, SIGNAL(positionUpdateForExecutionRequested(const TickerId,int,double)), this, SLOT(updatePositionForExecution(const TickerId,int,double)));
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
    _strategyId = ++_id;
    _running=false;
    _canOpenNewPositions = true;
    _targetReturn = 100;
    _stopLossReturn = -100;
    _maxHoldingPeriod = 0;
    _isExtensionAllowed = true;

    QThread* thread = ThreadManager::threadManager().requestThread();
    connect(thread, SIGNAL(started()), this, SLOT(startStrategy()));
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));

    moveToThread(thread);
    qRegisterMetaType<Contract>("Contract");
    qRegisterMetaType<Order>("Order");

    //these objects are still on MAIN thread and not on strategy Thread
    _performanceManagerSPtr = new PerformanceManager(this);
    _positionManagerSPtr = new PositionManager(this);
    _strategyReportSPtr = new StrategyReport(_strategyName);
    //linkWorkers();
}

//void Strategy::linkWorkers()
//{
//    _positionManagerSPtr->linkPerformanceManager(_performanceManagerSPtr);
//}

PerformanceManager* Strategy::getPerformanceManager()
{
    if(_performanceManagerSPtr == NULL)
    {
        _performanceManagerSPtr = new PerformanceManager(this);
    }

    return _performanceManagerSPtr;
}

PositionManager* Strategy::getPositionManager()
{
    if(_positionManagerSPtr == NULL)
    {
        _positionManagerSPtr = new PositionManager(this);
    }
    return _positionManagerSPtr;
}

StrategyReport* Strategy::getStrategyReport()
{
    if(_strategyReportSPtr == NULL)
    {
        _strategyReportSPtr = new StrategyReport(_strategyName);
    }
    return _strategyReportSPtr;
}

void Strategy::setName(const QString& name)
{
    _strategyName = name;
}

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
    //delete _tradingSchedule;

    //_indicatorSPtr->deleteLater();

    //delete the buyList instruments
    foreach(InstrumentData* id, _buyList)
    {
        delete id;
    }
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

    qDebug()<<_strategyName;
}

/////Places order for a given contract
//void Strategy::placeOrder(const ATContract& aTcontract, const Order& order)
//{
//    if(_canOpenNewPositions)
//    {
//        subscribeMarketData(aTcontract,IB);
//        service()->getOrderManager()->placeOrder(order, aTcontract, this);
//    }
//    else
//    {
//        reportEvent("Cannot trade this strategy anymore.");
//    }
//    //Service::Instance()->getOrderManager()->placeOrder(order, contract, this);
//    //requestMarketData(contract,IB);
//}

//void Strategy::placeClosingOrder(const ATContract& aTcontract, const Order& order)
//{
//    service()->getOrderManager()->placeOrder(order, aTcontract, this);//, true);
//}

void Strategy::placeClosingOrder(const TickerId tickerId, const Order& order)
{
    Service::service().getOrderManager()->placeOrder(order, tickerId, this);//, true);
}

///Places order for a given tickerId
void Strategy::placeOrder(const TickerId tickerId, const Order& order)
{
    if(_canOpenNewPositions)
    {
        subscribeMarketData(tickerId, IB);
        Service::service().getOrderManager()->placeOrder(order, tickerId, this);
    }
    else
    {
        reportEvent("Cannot trade this strategy anymore.");
    }
}

void Strategy::addPosition(const OrderId orderId, const TickerId tickerId)
{
    //link contractId to orderId
    _positionManagerSPtr->addPosition(tickerId);
}

const String& Strategy::getStrategyName()
{
    return _strategyName;
}

const StrategyId Strategy::getStrategyId()
{
    return _strategyId;
}

void Strategy::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double value)
{
     _positionManagerSPtr->updatePosition(tickerId, tickType, value);
}

void Strategy::onExecutionUpdate(const TickerId tickerId, const Execution& execution)//, const bool isClosingOrder)
{
    _positionManagerSPtr->updatePosition(tickerId, execution);//, isClosingOrder);
}

void Strategy::updatePositionForExecution(const TickerId tickerId, const int filledShares, const double fillPrice)
{
    _positionManagerSPtr->updatePosition(tickerId, filledShares, fillPrice);
}

void Strategy::startStrategy()
{
    //initialize();
    loadPositions();
}

void Strategy::reportEvent(const String& message, const MessageType mType)
{
    IOInterface::ioInterface().reportEvent(_strategyName, message, mType);
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

void Strategy::requestStrategyUpdateForExecution(const OpenOrder* openOrder)
{
    TickerId tickerId = openOrder->getTickerId();
    int filledShares =  openOrder->getLastFilledShares();
    double lastFillPrice = openOrder->getLastFillPrice();

    if (openOrder->getOrder().action == "SELL")
    {
        filledShares *= -1;
    }

    emit positionUpdateForExecutionRequested(tickerId, filledShares, lastFillPrice);
}


//void Strategy::loadBuyListFromIndex(const String index)
//{
//    QDir directory("/Users/shivkumarchawla/aims.trader/AIMSTrader");

//    QString fileName = directory.path() +"/Symbol Lists/" + index + ".txt";
//    QFile file(fileName);

//    String symbol;
//    if(file.open(QIODevice::ReadOnly))
//    {
//        QTextStream in(&file);
//        while(!file.atEnd())
//        {
//            in >> symbol;
//            _buyList.append(symbol);
//        }
//    }
//    file.close();
//}

void Strategy::setBuyList(const QList<InstrumentData*>& buyList)
{
     _buyList = buyList;

     foreach(InstrumentContract* c, buyList)
     {
         Service::service().getInstrumentManager()->registerInstrument(*c);
     }
}

void Strategy::setupIndicatorConnections()
{
    connect(this, SIGNAL(startIndicator()), _indicatorSPtr, SLOT(startIndicator()));
    connect(this, SIGNAL(stopIndicator()), _indicatorSPtr, SLOT(stopIndicator()));
    connect(_indicatorSPtr, SIGNAL(closeAllPositions()), this, SLOT(closeAllPositions()));
    connect(_indicatorSPtr, SIGNAL(instrumentSelected(const TickerId)), this, SLOT(onInstrumentSelection(const TickerId)));
}

void Strategy::onInstrumentSelection(const TickerId tickerId)
{
    Order o;
    o.action = "BUY";
    o.orderType = "MKT";
    o.totalQuantity = 5000/Service::service().getInstrumentManager()->getLastPrice(tickerId);
    placeOrder(tickerId, o);
    _canOpenNewPositions = false;
}

void Strategy::loadPositions()
{
    QList<StrategyLinkedPositionData*> positions = IODatabase::ioDatabase().getOpenStrategyLinkedPositions(_strategyId);

    foreach(StrategyLinkedPositionData* data, positions)
    {
        TickerId tickerId = Service::service().getInstrumentManager()->getTickerId(data->instrumentId);
       _positionManagerSPtr->loadPosition(tickerId, data);
        subscribeMarketData(tickerId, Test);
    }
}

const double Strategy::getTargetReturn()
{
    return _targetReturn;
}

const double Strategy::getStopLossReturn()
{
    return _stopLossReturn;
}

const int Strategy::getMaxHoldingPeriod()
{
    return _maxHoldingPeriod;
}









