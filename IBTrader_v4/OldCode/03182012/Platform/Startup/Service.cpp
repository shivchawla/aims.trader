//
//  Dispatcher.cpp
//  IBTrader
//
//  Created by Shiv Chawla on 1/28/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#include <iostream>
#include "Platform/Startup/Service.h"
#include "Platform/Trader/OrderManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include <QObject>
#include "Platform/Trader/TraderAssistant.h"
#include <QMetaType>

Service* Service::_instance = NULL;

Service::Service()
{}

void Service::setupConnections()
{
    //start all strategies
    TraderAssistant* ta = _traderSPtr->getTraderAssistant();

    qRegisterMetaType<TickerId>("TickerId");
    qRegisterMetaType<ContractDetails>("ContractDetails");
    qRegisterMetaType<OrderId>("OrderId");
    qRegisterMetaType<OrderStatus>("OrderStatus");
    qRegisterMetaType<Execution>("Execution");


    QObject::connect(ta, SIGNAL(updateBid(const TickerId, const double)), _instrumentManager,SLOT(setBid(const TickerId, const double)));
    QObject::connect(ta, SIGNAL(updateAsk(const TickerId, const double)), _instrumentManager,SLOT(setAsk(const TickerId, const double)));
    QObject::connect(ta, SIGNAL(updateLast(const TickerId, const double)), _instrumentManager,SLOT(setLast(const TickerId, const double)));
    QObject::connect(ta, SIGNAL(updateHigh(const TickerId, const double)), _instrumentManager,SLOT(setHigh(const TickerId, const double)));
    QObject::connect(ta, SIGNAL(updateClose(const TickerId, const double)), _instrumentManager,SLOT(setClose(const TickerId, const double)));
    QObject::connect(ta, SIGNAL(updateLow(const TickerId, const double)), _instrumentManager,SLOT(setLow(const TickerId, const double)));
    QObject::connect(ta, SIGNAL(updateBidSize(const TickerId, const int)), _instrumentManager,SLOT(setBidSize(const TickerId, const int)));
    QObject::connect(ta, SIGNAL(updateAskSize(const TickerId, const int)), _instrumentManager,SLOT(setAskSize(const TickerId, const int)));
    QObject::connect(ta, SIGNAL(updateLastSize(const TickerId, const int)), _instrumentManager,SLOT(setLastSize(const TickerId, const int)));
    QObject::connect(ta, SIGNAL(updateContractDetails(const TickerId, const ContractDetails&)),_instrumentManager,SLOT(setContractDetails(const TickerId, const ContractDetails&)));

    //QObject::connect(_instrumentManager, SIGNAL(requestMarketDataToTA(const TickerId, const Contract&)), ta, SLOT(requestMarketData(const TickerId, const Contract& )));
    //QObject::connect(_instrumentManager, SIGNAL(requestCancelDataToTA(const TickerId)), ta, SLOT(cancelMarketData(const TickerId)));

    QObject::connect(ta, SIGNAL(updateOpenOrder(const OrderId&, const Contract&, const Execution&)),_orderManager,SLOT(updateOpenOrderExecution(const OrderId&, const Contract&, const Execution&)));
    QObject::connect(ta, SIGNAL(requestAddOpenOrder(const Contract& , const Order&)),_orderManager,SLOT(addOpenOrder(const Contract&, const Order&)));
    QObject::connect(ta, SIGNAL(updateOrderStatus(const OrderId, const OrderStatus)),_orderManager,SLOT(updateOrderStatus(const OrderId, const OrderStatus)));
    //QObject::connect(_orderManager, SIGNAL(requestPlaceOrdertoTA(const OrderId,const Order&, const Contract&)), ta, SLOT(placeOrder(const OrderId,const Order&, const Contract&)));

}

Service::~Service()
{
    delete _eventReportSPtr;
    delete _traderSPtr;
    delete _orderManager;
    delete _instrumentManager;
}

Service* Service::Instance()
{
    if(_instance == NULL)
    {
        _instance = new Service();
    }
    return _instance;
}

void Service::startService()
{
    _eventReportSPtr = new EventReport();
    _traderSPtr = new Trader();
    _orderManager = new OrderManager();
    _instrumentManager = new InstrumentManager();
    _mode=ForwardTest;
    setupConnections();
}

EventReport* Service::getEventReport()
{
    return _eventReportSPtr;
}

Trader* Service::getTrader()
{
    return _traderSPtr;
}

/*Mode Service::getMode()
{
    return _mode;
}*/

void Service::exit()
{
   if (_traderSPtr != NULL)
   {
       _traderSPtr->Disconnect();
   }
}

void Service::setMode(const Mode mode)
{
    if (_mode != mode)
    {
        String s =  "Running mode changed to: ";
        s.append(getModeName(mode));
        _eventReportSPtr->report("AIMSTrader", s);
    }

    _mode = mode;

     // Disable all reporting when JA runs in optimization mode. The optimizer
     // runs thousands of strategies, and the amount of data to report would be enormous
    if (mode == Optimization)
    {
        _eventReportSPtr->disable();
    }
    else
    {
        _eventReportSPtr->enable();
    }

    if (mode == Trade || mode == ForwardTest)
    {
        _traderSPtr->Connect();
           // MonitoringServer.start();
    }
    else
    {
        _traderSPtr->Disconnect();
    }
}

InstrumentManager* Service::getInstrumentManager()
{
    return _instrumentManager;
}

OrderManager* Service::getOrderManager()
{
    return _orderManager;
}



