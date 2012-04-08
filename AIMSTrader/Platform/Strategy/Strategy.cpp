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
#include "Platform/Performance/PerformanceManager.h"
#include "Platform/Indicator/IndicatorManager.h"
#include "Platform/Position/PositionManager.h"
#include "Platform/Reports/EventReport.h"
#include "Platform/Reports/StrategyReport.h"
#include "Platform/Utils/TradingSchedule.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Trader/OrderManager.h"
#include <QMetaType>

std::list<Strategy*> Strategy::_strategyRegister;
std::list<Strategy*> Strategy::getStrategies()
{
    return _strategyRegister;
}

void Strategy::registerStrategy(Strategy *ts)
{
    _strategyRegister.push_back(ts);
}

Strategy::Strategy():DataSubscriber()
{}

Strategy::Strategy(const String& strategyName):DataSubscriber()
{
    _strategyName = strategyName;
}

void Strategy::initialize()
{
    QThread* thread = ThreadManager::Instance()->requestThread();
    moveToThread(thread);
    qRegisterMetaType<Contract>("Contract");
    qRegisterMetaType<Order>("Order");
    QObject::connect(thread,SIGNAL(started()),this,SLOT(startStrategy()));

    _performanceManagerSPtr = new PerformanceManager(this);
    _indicatorManagerSPtr = new IndicatorManager(this);
    _positionManagerSPtr = new PositionManager(this);
    _strategyReportSPtr = new StrategyReport(QString::fromStdString(_strategyName));
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
    delete _indicatorManagerSPtr;
    delete _positionManagerSPtr;
    delete _tradingSchedule;
}

void Strategy::closeAllPositions()
{
	_positionManagerSPtr->closeAllPositions();
}

void Strategy::placeOrder(const Contract& contract, const Order& order)
{
    Service::Instance()->getOrderManager()->placeOrder(order, contract, this);
    requestMarketData(contract,IB);
}

void Strategy::placeClosingOrder(const Contract& contract, const Order& order)
{
    Service::Instance()->getOrderManager()->placeOrder(order, contract, this);
}

void Strategy::placeClosingOrder(const TickerId tickerId, const Order& order)
{
    Service::Instance()->getOrderManager()->placeOrder(order, tickerId, this);
}

void Strategy::placeOrder(const TickerId tickerId, const Order& order)
{
    Service::Instance()->getOrderManager()->placeOrder(order, tickerId, this);
    requestMarketData(tickerId,IB);
}

void Strategy::requestMarketData(const Contract& contract, const DataSource source)
{
    Service::Instance()->getInstrumentManager()->requestMarketData(contract,source,this);
}

void Strategy::requestMarketData(const TickerId tickerId, const DataSource source)
{
    Service::Instance()->getInstrumentManager()->requestMarketData(tickerId,source,this);
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

/*void Strategy::setTickerId(const Contract& contract, const TickerId tickerId)
{
    _positionManagerSPtr->setTickerId(contract, tickerId);
}*/

const String& Strategy::getStrategyName()
{
    return _strategyName;
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

void Strategy::onExecutionUpdate(const OrderId orderId, const ExecutionStatus& executionStatus)
{
    _positionManagerSPtr->updatePosition(orderId, executionStatus);
}

void Strategy::startStrategy()
{}




