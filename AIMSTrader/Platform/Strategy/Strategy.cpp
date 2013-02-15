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
#include "Platform/Position/SpreadManager.h"
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
#include "Platform/Trader/RiskManager.h"

long Strategy::id = 0;

/*
 * Strategy Constructor
 */
Strategy::Strategy():DataSubscriber()
{
    initialize();
}

/*
 * Strategy Constructor with strategy name
 */
Strategy::Strategy(const String& strategyName):DataSubscriber()
{
    _strategyName = strategyName;
    initialize();
}

/*
 *
 */
void Strategy::setupConnection()
{
//    connect(this, SIGNAL(closeAllPositionsRequested()), this, SLOT(closeAllPositions()));
//    connect(this, SIGNAL(closePositionRequested(const TickerId)), this, SLOT(closePosition(const TickerId)));
//    connect(this, SIGNAL(adjustPositionRequested(const TickerId, const Order&)), this, SLOT(adjustPosition(const TickerId, const Order&)));
//    connect(this, SIGNAL(positionUpdateOnExecutionRequested(const OrderId, const OrderDetail&)), this, SLOT(updatePositionOnExecution(const OrderId, const OrderDetail&)));
}

/*
 *Sets the alarm for indicator to start and stop based on weekdays, holidays and exchange timings
 */
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
           //stop instantaneously
           stopIndicator();
        }
        else
        {
            qDebug()<<_strategyName<<"huhahaha"<<"/n";
            QTimer::singleShot(msecondsToStop, this, SLOT(stopIndicator()));
        }
    }
    else
    {
        //get the timeout to start the strategy
        QDateTime nextStartDateTime(getNextValidDate(), _tradingSchedule->getStartTime());
        int msecondsToStart = QDateTime::currentDateTime().msecsTo(nextStartDateTime.addSecs(-60));

        qDebug()<<_strategyName <<" StartDateTime "<<nextStartDateTime<<" "<<msecondsToStart ;

        if(msecondsToStart < 0)
        {
            //start instantaneously
            startIndicator();
        }
        else
        {
            qDebug()<<_strategyName<<"huhahaha"<<"/n";
            QTimer::singleShot(msecondsToStart, this, SLOT(startIndicator()));
        }
    }
}

/*
 * Request the indicator to start
 */
void Strategy::startIndicator()
{
    QMetaObject::invokeMethod(_indicatorPtr, "startIndicator", Qt::QueuedConnection);
    //emit requestStartIndicator();
    _isIndicatorRunning = true;
    //_isStrategyActive = true;
    setTimeout();
}

/*
 * Request the indicator to stop
 */
void Strategy::stopIndicator()
{
    QMetaObject::invokeMethod(_indicatorPtr, "stopIndicator", Qt::QueuedConnection);
    //emit requestStopIndicator();
    _isIndicatorRunning = false;
    //_isStrategyActive = false;
    setTimeout();
}

/*
 *  Get the next valid start date
 */
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

/*
 * Initialize startegy parameters
 */
void Strategy::initialize()
{
    _strategyId = ++id;
    _strategyType = SingleStock_StrategyType;
    _isIndicatorRunning = false;
    _isStrategyActive = false;
    _canOpenNewPositions = true;
    _targetReturn = 100;
    _stopLossReturn = -100;
    _maxHoldingPeriod = 50000000;
    _isExtensionAllowed = false;

    _maxInvestment = 50000;
    _totalInvested = 0;

    QThread* thread = ThreadManager::threadManager().requestThread();
    connect(thread, SIGNAL(started()), this, SLOT(startStrategy()));
    connect(thread, SIGNAL(finished()), this, SLOT(deleteLater()));

    moveToThread(thread);
    qRegisterMetaType<Contract>("Contract");
    qRegisterMetaType<Order>("Order");

    //these objects are still on MAIN thread and not on strategy Thread
    _performanceManagerSPtr = new PerformanceManager(this);
    _positionManager = new PositionManager(this);
    _tradingSchedule = new TradingSchedule();
    setDefaultDataSource(Test);
}

/*
 * Get pointer to performance manager
 */
PerformanceManager* Strategy::getPerformanceManager()
{
    if(_performanceManagerSPtr == NULL)
    {
        _performanceManagerSPtr = new PerformanceManager(this);
    }

    return _performanceManagerSPtr;
}

/*
 * Get pointer to position manager
 */
PositionManager* Strategy::getPositionManager()
{
    if(_positionManager == NULL)
    {
        _positionManager = new PositionManager(this);
    }

    return _positionManager;
}

/*
 * Get pointer to strategy report
 */
StrategyReport* Strategy::getStrategyReport()
{
    if(_strategyReportSPtr == NULL)
    {
        _strategyReportSPtr = new StrategyReport(_strategyName);
    }

    return _strategyReportSPtr;
}

/*
 * Set the strategy name
 */
void Strategy::setName(const QString& name)
{
    _strategyName = name;
}

/*
 * Strategy Destructor
 */
Strategy::~Strategy()
{
    //make sure that all the sub components of strategy are deleted too.
    printf("deleting strategy components");
    //before deleting check for any active positions
    //remove all active positions before deleting all the components
    delete _performanceManagerSPtr;
    delete _strategyReportSPtr;
    delete _positionManager;
    delete _tradingSchedule;
}

/*
 * Close all position for a single stock strategy. This is only used for single stock strategy
 */
void Strategy::closeAllPositions()
{
    _positionManager->closeAllPositions();
}

/*
 * Close a position for single stock strategy. This is only used for single stock strategy
 */
void Strategy::closePosition(const TickerId tickerId)
{
    _positionManager->closePosition(tickerId);
}

/*
 * Adjust position for a strategy. This is only used for single stock strategy
 */
void Strategy::adjustPosition(const TickerId tickerId, const Order& order)
{
   long netShares = 0;//_positionManagerSPtr->getMainPosition(tickerId)->getNetShares();
    TradeType tradeType = OPENEXTEND;
    if((netShares > 0 && order.action == "SELL") || (netShares < 0 && order.action == "BUY"))
    {
        tradeType = CLOSEREDUCE;
    }

    if(IsValid(QDateTime::currentDateTime(), tradeType))
    {
        placeOrder(tickerId, order);
        qDebug()<<_strategyName;
    }
}

/*
 * Place closing order for a position. This is only used for single stock strategy
 */
void Strategy::placeClosingOrder(const TickerId tickerId, const Order& order)
{
    OrderManager* om = Service::service().getOrderManager();
    OrderId orderId  = om->requestOrderId();
    om->placeOrder(orderId, tickerId, _strategyId, order);//, true);
    updatePendingQuantity(tickerId, order.action.compare("BUY") ? order.totalQuantity : -order.totalQuantity);
}

/*
 * Places order for a given instrument
 */
void Strategy::placeOrder(const TickerId tickerId, const Order& order)
{
    //THIS HAS BEEN CHNAGED
    long netShares = _positionManager->getPosition(tickerId)->getNetShares();

    TradeType tradeType = OPENEXTEND;
    if((netShares > 0 && order.action == "SELL") || (netShares < 0 && order.action == "BUY"))
    {
        tradeType = CLOSEREDUCE;
    }

    if(IsValid(QDateTime::currentDateTime(), tradeType))
    {
        if( netShares != 0)
        {
            if(_isExtensionAllowed)
            {
                OrderManager* om = Service::service().getOrderManager();
                OrderId orderId  = om->requestOrderId();
                om->placeOrder(orderId, tickerId, _strategyId, order);//, true);
                updatePendingQuantity(tickerId, order.action.compare("BUY") ? order.totalQuantity : -order.totalQuantity);
                subscribeMarketData(tickerId);
            }
            else
            {
                 reportEvent("Extension Not Allowed Strategy: " + _strategyName);
            }
        }
        else if (netShares == 0)
        {
            if(_canOpenNewPositions)
            {
                OrderManager* om = Service::service().getOrderManager();
                OrderId orderId  = om->requestOrderId();
                om->placeOrder(orderId, tickerId, _strategyId, order);//, true);
                updatePendingQuantity(tickerId, order.action.compare("BUY") ? order.totalQuantity : -order.totalQuantity);
                subscribeMarketData(tickerId);
            }
            else
            {
                reportEvent("New Positions Not Allowed Strategy: "+ _strategyName);
            }
        }
    }
}

/*
 * Add position
 */
void Strategy::addPosition(const OrderId orderId, const TickerId tickerId)
{
    //link contractId to orderId
    _positionManager->addPosition(tickerId);
}

/*
 * Get Strategy Name
 */
const String Strategy::getStrategyName()
{
    return _strategyName;
}

/*
 * Get StrategyId
 */
const StrategyId Strategy::getStrategyId()
{
    return _strategyId;
}

/*
 * Update position on price update
 */
void Strategy::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double value)
{
    _positionManager->updatePosition(tickerId, tickType, value);
}

/*
 *
 */
void Strategy::updatePositionOnExecution(const OrderId orderId, const OrderDetail& orderDetail)
{
    if(TickerId tickerId = _orderIdToTickerId.value(orderId))
    {
        String message("Updating Strategy for Order Execution of OrderId: ");
        message.append(QString::number(orderId));//.append(" OrderType: ").append(QString::fromStdString(orderDetail.getOrder().orderType)).append(" Quantity: ").append(execution.shares).append(" Side: ").append(QString::fromStdString(execution.side));
        reportEvent(message);
        _positionManager->updatePosition(orderId, tickerId, orderDetail);
        updatePendingQuantity(tickerId, orderDetail.order.action.compare("BUY") ? -orderDetail.filledShares : orderDetail.filledShares);
    }
}

/*
 * Start Strategy
 */
void Strategy::startStrategy()
{
    loadOpenPositions();
    populateStrategySpecificPreferences();
    registerBuyList();
    setupIndicator();
    setTimeout();
}

/*
 * Load strategy data from database
 */
void Strategy::loadStrategyDataFromDb()
{
     loadStrategyParameters();
     populateGeneralStrategyPreferences();
     loadBuyList();
     loadOpenPositions();
}

/*
 * Populate general preferences for all strategies
 */
void Strategy::populateGeneralStrategyPreferences()
{
       _defaultTradeDirection = _strategyParams.value("DefaultTradeDirection","BUY");
       _tradingSchedule->setStartTime(QTime::fromString(_strategyParams.value("StartTime","09:30:00"), "hh:mm:ss"));
       _tradingSchedule->setEndTime(QTime::fromString(_strategyParams.value("EndTime","16:00:00"),"hh:mm:ss"));
       _tradingSchedule->setTimezone(_strategyParams.value("Timezone","EST"));

       _maxHoldingPeriod = _strategyParams.value("MaximumHoldingPeriod","5").toDouble();
       _targetReturn = _strategyParams.value("TargetReturn", "0.1").toDouble();
       _timeScale = _strategyParams.value("TradingFrequency","1").toInt();

       _isExtensionAllowed = _strategyParams.value("IsExtensionAllowed", "false") == "false" ? false : true;
       _stopLossReturn = _strategyParams.value("StopLossReturnPct", "-0.5").toDouble();
}

/*
 * Load buylist for a strategy
 */
void Strategy::loadBuyList()
{
    QList<InstrumentData> strategyBuyList = IODatabase::ioDatabase().getStrategyBuyList(_strategyName);
    setBuyList(strategyBuyList);
}

/*
 * Report event
 */
void Strategy::reportEvent(const String& message, const MessageType mType)
{
    IOInterface::ioInterface().reportEvent(_strategyName, message, mType);
}

/*
 *
 */
void Strategy::timerEvent(QTimerEvent* event)
{
//    if(event->timerId()==_basicTimer.timerId()) //timer even triggered by stratgey itself
//    {
//        _basicTimer.stop();
//        //now set a new time-out
//        setTimeout();
//    }
}

/*
 * Stop strategy (this is important in case you see some weird behaviour)
 * this is called by MAIN thread...this is problematic
 */
void Strategy::stopStrategy()
{
    //set flag to ope more positon to false;
    _canOpenNewPositions = false;

    //ask the stratgey Indicator thread to stop
    stopIndicator();

    //ask Position manager to close all existing psoitions
    closeAllPositions();

    //ask the indicator thrad to terminate
    //_indicatorSPtr->stopIndicator();
}

/*
 * Request to close all positions. This call is then transferred to strategy thread.
 */
void Strategy::requestCloseAllPositions()
{ 
    QMetaObject::invokeMethod(this, "closeAll", Qt::QueuedConnection);
    //emit closeAllPositionsRequested();
}

/*
 * Request to close position. This call is then transferred to strategy thread.
 */
void Strategy::requestClosePosition(const TickerId tickerId)
{
    QMetaObject::invokeMethod(this,"closePosition",Qt::QueuedConnection, Q_ARG(TickerId, tickerId));
    //emit closePositionRequested(tickerId);
}

/*
 * Request to adjust position. This call is then transferred to strategy thread.
 */
void Strategy::requestAdjustPosition(const TickerId tickerId, const Order& order)
{
    QMetaObject::invokeMethod(this, "adjustPosition",Qt::QueuedConnection, Q_ARG(TickerId, tickerId), Q_ARG(Order, order));
    //emit adjustPositionRequested(tickerId, order);
}

/*
 * Request the strategy to update position for execution. This call is then transferred to strategy thread.
 */
void Strategy::requestStrategyUpdateForExecution(const OrderId orderId, const OrderDetail& orderDetail)
{
    QMetaObject::invokeMethod(this, "updatePositionOnExecution", Qt::QueuedConnection, Q_ARG(OrderId, orderId), Q_ARG(OrderDetail, orderDetail));
    //emit positionUpdateOnExecutionRequested(orderId, orderDetail);
}

/*
 * Set buylist
 */
void Strategy::setBuyList(const QList<InstrumentData>& buyList)
{
     _buyList = buyList;
}

/*
 *
 */
void Strategy::connectIndicatorSignals()
{
//    connect(_indicatorSPtr, SIGNAL(closeAllPositions()), this, SLOT(closeAllPositions()));
//    connect(_indicatorSPtr, SIGNAL(requestPlaceOrderToStrategy(const TickerId, const Order&)), this, SLOT(onOrderRequestFromIndicator(const TickerId, const Order&)));
//    connect(this, SIGNAL(requestStartIndicator()), _indicatorSPtr, SLOT(startIndicator()));
//    connect(this, SIGNAL(requestStopIndicator()), _indicatorSPtr, SLOT(stopIndicator()));
}

/*
 * On order request from indicator
 */
void Strategy::onOrderRequestFromIndicator(const TickerId tickerId, const Order& order)
{
    qDebug()<<"Order Received from Indicator at "<<QDateTime::currentDateTime()<<_strategyName;

    long netShares = _positionManager->getPosition(tickerId)->getNetShares();
    TradeType tradeType = OPENEXTEND;
    if((netShares > 0 && order.action == "SELL") || (netShares < 0 && order.action == "BUY"))
    {
        tradeType = CLOSEREDUCE;
    }

    //this is called when opening a new position
    if(IsValid(QDateTime::currentDateTime(), tradeType))
    {
        double lastPrice = Service::service().getInstrumentManager()->getLastPrice(tickerId);
        if(lastPrice<=0)
        {
            reportEvent("No Price Available. Cannot place order");
            return;
        }

        placeOrder(tickerId, order);
    }
}

/*
 * Load positions from database for this strategy
 */
void Strategy::loadOpenPositions()
{
    QList<PositionData> positions = IODatabase::ioDatabase().getOpenStrategyLinkedPositions(_strategyName);

    foreach(PositionData data, positions)
    {
          TickerId tickerId = Service::service().getInstrumentManager()->getTickerId(data.instrumentId);
          _positionManager->loadPosition(tickerId, data);
          subscribeMarketData(tickerId);
    }
}

/*
 * Get target return for a strategy
 */
const double Strategy::getTargetReturn()
{
    return _targetReturn;
}

/*
 * Get stop-loss return for a strategy
 */
const double Strategy::getStopLossReturn()
{
    return _stopLossReturn;
}

/*
 * Get maximum holding period for a strategy
 */
const int Strategy::getMaxHoldingPeriod()
{
    return _maxHoldingPeriod;
}

/*
 * Setup strategy
 */
void Strategy::setupStrategy(const StrategyId strategyId, const StrategyData& strategyData)
{
    setStrategyId(strategyId);
    setName(strategyData.strategyName);
    _strategyReportSPtr = new StrategyReport(_strategyName);
    loadStrategyDataFromDb();
}

/*
 * Test if a date-time is valid for a trade-type
 */
bool Strategy::IsValid(const QDateTime& dateTime, const TradeType tradeType)
{
    return ((dateTime.date() == _nextValidDate) && _tradingSchedule->IsValid(dateTime.time(), tradeType));
}

/*
 * Register a buylist
 */
void Strategy::registerBuyList()
{
    foreach(InstrumentContract c, _buyList)
    {
        Service::service().getInstrumentManager()->registerInstrument(c, _datasource);
    }
}

/*
 * Test if order can be placed for this strategy.
 * Order in opposite direction is not allowed
 */
bool Strategy::canPlaceOrder(const TickerId tickerId, const Order& order)
{
    //first find out if there exist a position in opposite direction
    int quantity = order.action.compare("BUY") ? order.totalQuantity : -order.totalQuantity;
    Position* pos = _positionManager->getPosition(tickerId);

    int existingPos = pos ? pos->getNetShares() : 0;
    int alreadyOrderedQuantity = _pendingOrderQuantity.value(tickerId, 0);

    if(quantity * existingPos < 0 || quantity * alreadyOrderedQuantity < 0)
    {
        return false;
    }

    RiskManager* riskManager = Service::service().getRiskManager();
    return riskManager->canPlaceOrder(tickerId, order);
}

/*
 * Get the strategy type
 */
StrategyType Strategy::getStrategyType()
{
    return _strategyType;
}

/*
 * Load the strategy parameters
 */
void Strategy::loadStrategyParameters()
{
    _strategyParams = IODatabase::ioDatabase().getStrategyConfigurations(_strategyName);
}

void Strategy::updatePendingQuantity(const TickerId tickerId, int quantity)
{
    int pendingQuantity = _pendingOrderQuantity.value(tickerId, 0);

    _pendingOrderQuantity[tickerId] = pendingQuantity + quantity;
}

void Strategy::closeAll()
{
    closeAllPositions();
}








