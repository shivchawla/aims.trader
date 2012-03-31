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
#include "Platform/Model/StrategyModel.h"
#include "Platform/View/StrategyView.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Trader/OrderManager.h"

std::list<Strategy*> Strategy::_strategyRegister;
std::list<Strategy*> Strategy::getStrategies()
{
    return _strategyRegister;
}

void Strategy::registerStrategy(Strategy *ts)
{
    _strategyRegister.push_back(ts);
}


Strategy::Strategy()
{}

Strategy::Strategy(const String& strategyName)
{
    _strategyName = strategyName;
}

void Strategy::initialize()
{
    moveToThread(ThreadManager::Instance()->requestThread());
    qRegisterMetaType<Contract>("Contract");
    qRegisterMetaType<Order>("Order");

    QObject::connect(this, SIGNAL(requestMarketDataToInstrumentManager(const Contract&)), Service::Instance()->getInstrumentManager(),SLOT(requestMarketData(const Contract&)));
    QObject::connect(this, SIGNAL(requestPlaceOrderToOrderManager(const Order&,const Contract&)), Service::Instance()->getOrderManager(),SLOT(placeOrder(const Order&, const Contract&)));

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

/*void Strategy::createModelnView()
{
    //create the model and the view
    //_strategyModel = new StrategyModel(this);
    _strategyView = new StrategyView();
    //_strategyView->setModel(_strategyModel);
    _strategyView->show();
}*/

Strategy::~Strategy()
{
    //make sure that all the sub components of strategy are deleted too.
    printf("deleting strategy components");
    //before deleting check for any active positions
    //remove all ative positions before deleting all the components
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
    emit requestPlaceOrderToOrderManager(order, contract);
    requestMarketData(contract);
}

void Strategy::requestMarketData(const Contract& contract)
{
    emit requestMarketDataToInstrumentManager(contract);
}

void Strategy::updatePosition(const OrderId orderId, const Execution& execution)
{
    _positionManagerSPtr->updatePosition(orderId, execution);
}

void Strategy::addPosition(const OrderId orderId, const Contract& contract)
{
    //link contractId to orderId
    //_positionManagerSPtr->addPosition(orderId, contract);
}

void Strategy::setTickerId(const Contract& contract, const TickerId tickerId)
{
    _positionManagerSPtr->setTickerId(contract, tickerId);
}

const String& Strategy::getStrategyName()
{
    return _strategyName;
}

void Strategy::updatePosition(const TickerId tickerId, const double lastPrice)
{
    _positionManagerSPtr->updatePosition(tickerId, lastPrice);
}

void Strategy::startStrategy()
{}


