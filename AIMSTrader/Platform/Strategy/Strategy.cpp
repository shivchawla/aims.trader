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
#include <QTimer>
#include "Platform/Commission/CommissionFactory.h"
#include <stdlib.h>

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
    //connect(this, SIGNAL(positionUpdateOnExecutionRequested(const OpenOrder&)), this, SLOT(updatePositionOnExecution(const OpenOrder&)));
    connect(this, SIGNAL(positionUpdateOnExecutionRequested(const OrderId, const OrderDetail&)), this, SLOT(updatePositionOnExecution(const OrderId, const OrderDetail&)));
}

//sets the alarm for indicator to start and stop based on weekdays, holidays and exchange timings
void Strategy::setTimeout()
{
    if(_isIndicatorRunning)
    {
        //if running set the timeout to stop the indicator
        QDateTime nextStopDateTime(_nextValidDate, _tradingSchedule->getEndTime());
        int msecondsToStop = QDateTime::currentDateTime().msecsTo(nextStopDateTime);
        qDebug()<<_strategyName <<" StopDateTime "<<nextStopDateTime<<" "<<msecondsToStop ;

        if(msecondsToStop < 0)
        {
           msecondsToStop = 0;
        }

       qDebug()<<_strategyName<<"huhahaha"<<"/n";
       QTimer::singleShot(msecondsToStop, this, SLOT(stopIndicator()));
    }
    else
    {
        //if indicator is not running
        //get the timeout to start the strategy
        //QDate nextValidDate = ;
        QDateTime nextStartDateTime(getNextValidDate(), _tradingSchedule->getStartTime());
        int msecondsToStart = QDateTime::currentDateTime().msecsTo(nextStartDateTime);

        qDebug()<<_strategyName <<" StartDateTime "<<nextStartDateTime<<" "<<msecondsToStart ;

        if(msecondsToStart < 0)
        {
            msecondsToStart = 0;
        }

        qDebug()<<_strategyName<<"huhahaha"<<"/n";
        QTimer::singleShot(msecondsToStart, this, SLOT(startIndicator()));
    }
}

void Strategy::startIndicator()
{
//    if(_indicatorSPtr)
//    {
//        QMetaObject::invokeMethod(_indicatorSPtr, "startIndicator");//, Qt::QueuedConnection);
//    }
    emit requestStartIndicator();
    _isIndicatorRunning = true;
    setTimeout();
}


void Strategy::stopIndicator()
{
//    if(_indicatorSPtr)
//    {
//        QMetaObject::invokeMethod(_indicatorSPtr, "stopIndicator");//, Qt::QueuedConnection);
//    }
    emit requestStopIndicator();
    _isIndicatorRunning = false;
    setTimeout();
}

const QDate& Strategy::getNextValidDate()
{
    //return (_nextValidDate = QDate::currentDate());
    bool validDateSet=false;
    QDateTime nextDateTime = QDateTime::currentDateTime();
    nextDateTime.setTime(_tradingSchedule->getEndTime());

    // QDate nextDateTime;
    QDateTime currentDateTime = QDateTime::currentDateTime();

    while(!validDateSet)
    {
        //qDebug()<<nextDateTime;
        int dayOfWeek = nextDateTime.date().dayOfWeek();
        if(dayOfWeek!=6 && dayOfWeek!=7)
        {
            //currentDateTime = QDateTime::currentDateTime();
            int msecondsToStop = currentDateTime.msecsTo(nextDateTime);
            if(msecondsToStop <= 0)
            {
                nextDateTime = nextDateTime.addDays(1);
                qDebug()<<nextDateTime;
            }
            else
            {
                validDateSet = true;
            }
        }
        else if(dayOfWeek == 6)
        {
            //getTo monday
            nextDateTime = nextDateTime.addDays(2);
            validDateSet = true;
        }
        else if(dayOfWeek == 7)
        {
            nextDateTime = nextDateTime.addDays(1);
            validDateSet = true;
        }
    }
    //check for holiday
    return (_nextValidDate = nextDateTime.date());
}

void Strategy::initialize()
{
    _strategyId = ++_id;
    _isIndicatorRunning=false;
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
    _tradingSchedule = new TradingSchedule();
    //linkWorkers();

    setDefaultDataSource(Test);

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
    delete _positionManagerSPtr;
    delete _tradingSchedule;

   // _indicatorSPtr->stopIndicator();
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

void Strategy::placeClosingOrder(const TickerId tickerId, const Order& order)
{
    Service::service().getOrderManager()->placeOrder(tickerId, _strategyId, order);//, true);
}

///Places order for a given tickerId
void Strategy::placeOrder(const TickerId tickerId, const Order& order)
{
    if(_canOpenNewPositions)
    {
        subscribeMarketData(tickerId);
        Service::service().getOrderManager()->placeOrder(tickerId, _strategyId, order);
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
    //_positionManagerSPtr->updatePosition(tickerId, execution);
}

void Strategy::updatePositionOnExecution(const OrderId orderId, const TickerId tickerId, const int filledShares, const double fillPrice, const double commission)
{
    _positionManagerSPtr->updatePosition(orderId, tickerId, filledShares, fillPrice, commission);
}

void Strategy::updatePositionOnExecution(const OpenOrder& openOrder)
{
    _positionManagerSPtr->updatePosition(openOrder);
}

void Strategy::updatePositionOnExecution(const OrderId orderId, const OrderDetail& orderDetail)
{
    String message("Updating Strategy for Order Execution of OrderId: ");
    message.append(QString::number(orderId));//.append(" OrderType: ").append(QString::fromStdString(orderDetail.getOrder().orderType)).append(" Quantity: ").append(execution.shares).append(" Side: ").append(QString::fromStdString(execution.side));
    reportEvent(message);

    _positionManagerSPtr->updatePosition(orderId, orderDetail);
}

void Strategy::startStrategy()
{
    foreach(InstrumentContract* c, _buyList)
    {
        Service::service().getInstrumentManager()->registerInstrument(c, _datasource);
    }

    populateStrategySpecificPreferences();
    setupIndicator();
    setTimeout();
}

void Strategy::loadStrategyDataFromDb(const StrategyData* strategyData)
{
     setName(strategyData->name);
     DbStrategyId id = strategyData->strategyId;
     _strategyParams = IODatabase::ioDatabase().getStrategyConfigurations(id);
     populateGeneralStrategyPreferences();
     loadBuyList(id);
     loadPositions(id);
}

void Strategy::populateGeneralStrategyPreferences()
{
       _defaultTradeDirection = _strategyParams.value("DefaultTradeDirection","BUY");
       _tradingSchedule->setStartTime(QTime::fromString(_strategyParams.value("StartTime","09:30:00"), "hh:mm:ss"));
       _tradingSchedule->setEndTime(QTime::fromString(_strategyParams.value("EndTime","15:30:00"),"hh:mm:ss"));
       _tradingSchedule->setTimezone(_strategyParams.value("Timezone","EST"));

       _maxHoldingPeriod = _strategyParams.value("MaximumHoldingPeriod","5").toDouble();
       _targetReturn = _strategyParams.value("TargetReturn", "0.1").toDouble();
       _timeScale = _strategyParams.value("TradingFrequency","1").toInt();

        //_holdingPeriodUnit = HoldingPeriodUnit
       _isExtensionAllowed = _strategyParams.value("IsExtensionAllowed", "false") == "false" ? false : true;
       _stopLossReturn = _strategyParams.value("StopLossReturnPct", "-0.5").toDouble();
}

void Strategy::loadBuyList(const DbStrategyId strategyId)
{
    QList<InstrumentData*> strategyBuyList = IODatabase::ioDatabase().getStrategyBuyList(strategyId);
    setBuyList(strategyBuyList);
}

void Strategy::reportEvent(const String& message, const MessageType mType)
{
    IOInterface::ioInterface().reportEvent(_strategyName, message, mType);
}

void Strategy::timerEvent(QTimerEvent* event)
{
//    if(event->timerId()==_basicTimer.timerId()) //timer even triggered by stratgey itself
//    {
//        _basicTimer.stop();
//        //now set a new time-out
//        setTimeout();
//    }
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
    _indicatorSPtr->stopIndicator();
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

//void Strategy::requestStrategyUpdateForExecution(const OpenOrder* openOrder)
//{
////    TickerId tickerId = openOrder->getTickerId();
////    OrderId orderId = openOrder->getOrderId();
////    int filledShares =  openOrder->getLastFilledShares();
////    double lastFillPrice = openOrder->getLastFillPrice();
////    double commission = openOrder->getCommission();

////    if (openOrder->getOrder().action == "SELL")
////    {
////        filledShares *= -1;
////    }

//    //emit positionUpdateForExecutionRequested(tickerId, filledShares, lastFillPrice, commission);

//    emit positionUpdateOnExecutionRequested(*openOrder);
//}

void Strategy::requestStrategyUpdateForExecution(const OrderId orderId, const OrderDetail& orderDetail)
{
    emit positionUpdateOnExecutionRequested(orderId, orderDetail);
}

//void Strategy::updatePositionOnExecution(const OpenOrder& openOrder)
//{
//    TickerId tickerId = openOrder.getTickerId();
//    OrderId orderId = openOrder.getOrderId();

//    long quantity = openOrder.getLastFilledShares();
//    long filledShares = (openOrder.getOrder().action == "SLD") ? -quantity : quantity;
//    _positionManagerSPtr->updatePosition(openOrder);
//}



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
}

void Strategy::connectIndicatorSignals()
{
    connect(_indicatorSPtr, SIGNAL(closeAllPositions()), this, SLOT(closeAllPositions()));
    connect(_indicatorSPtr, SIGNAL(requestPlaceOrderToStrategy(const TickerId, const Order&)), this, SLOT(onInstrumentSelectionByIndicator(const TickerId, const Order&)));
    connect(this, SIGNAL(requestStartIndicator()), _indicatorSPtr, SLOT(startIndicator()));
    connect(this, SIGNAL(requestStopIndicator()), _indicatorSPtr, SLOT(stopIndicator()));
}

void Strategy::onInstrumentSelectionByIndicator(const TickerId tickerId, const Order& order)
{
//    Order o;
//    o.action = (tradeDirection == BUY) ? "BUY" : "SELL";
//    o.orderType = "MKT";

    double lastPrice = Service::service().getInstrumentManager()->getLastPrice(tickerId);
    if(lastPrice<=0)
    {
        reportEvent("No Price Available. Cannot place order");
        return;
    }

    //o.totalQuantity = 5000/lastPrice;
    placeOrder(tickerId, order);
    //_canOpenNewPositions = false;
}

void Strategy::loadPositions(const DbStrategyId strategyId)
{
    QList<StrategyLinkedPositionData*> positions = IODatabase::ioDatabase().getOpenStrategyLinkedPositions(strategyId);

    foreach(StrategyLinkedPositionData* data, positions)
    {
        TickerId tickerId = Service::service().getInstrumentManager()->getTickerId(data->instrumentId);
       _positionManagerSPtr->loadPosition(tickerId, data);
        subscribeMarketData(tickerId);
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

void Strategy::setupStrategy(const StrategyData* strategyData)
{
    loadStrategyDataFromDb(strategyData);
}









