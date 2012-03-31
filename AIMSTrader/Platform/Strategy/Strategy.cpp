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

Strategy::Strategy():MarketDataSubscriber()
{}

Strategy::Strategy(const String& strategyName):MarketDataSubscriber()
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
    Service::Instance()->getOrderManager()->placeOrder(order, contract);
    requestMarketData(contract);
}

void Strategy::requestMarketData(const Contract& contract)
{
    Service::Instance()->getInstrumentManager()->requestMarketData(contract,this);
}

void Strategy::updatePosition(const OrderId orderId, const Execution& execution)
{
    _positionManagerSPtr->updatePosition(orderId, execution);
}

void Strategy::addPosition(const OrderId orderId, const Contract& contract)
{
    //link contractId to orderId
    _positionManagerSPtr->addPosition(orderId, contract);
}

/*void Strategy::setTickerId(const Contract& contract, const TickerId tickerId)
{
    _positionManagerSPtr->setTickerId(contract, tickerId);
}*/

const String& Strategy::getStrategyName()
{
    return _strategyName;
}

void Strategy::onTradeUpdate(const TickerId tickerId, TradeUpdate pTradeUpdate)
{
    double lastPrice = pTradeUpdate.lastPrice;
    _positionManagerSPtr->updatePosition(tickerId, lastPrice);
}

void Strategy::onQuoteUpdate(const TickerId tickerId, QuoteUpdate pQuoteUpdate)
{}

void Strategy::startStrategy()
{}


