#include "Platform/Trader/OrderManager.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/Startup/OutputService.h"
#include "Platform/View/OutputInterface.h"
#include <iostream>

OrderManager::OrderManager():QObject()
{
    _orderId=0;
    _lockOpenOrderMap = new QReadWriteLock();
    _outputInterface = OutputService::Instance()->getOutputInterface();
}

OrderManager::~OrderManager()
{}

void OrderManager::updateOrderStatus(const OrderId orderId, const OrderStatus orderStatus)
{
    _lockOpenOrderMap->lockForRead();

    if(_openOrders.count(orderId)!=0)
    {
        _openOrders[orderId]->setOrderStatus(orderStatus);
    }
    _lockOpenOrderMap->unlock();
}

void OrderManager::removeOpenOrder(const OrderId orderId)
{
    _lockOpenOrderMap->lockForWrite();

    if(_openOrders.count(orderId)!=0)
    {
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
        //removeOrderFromOutputs(orderId);
    }
    _lockOpenOrderMap->unlock();
}

const OrderId OrderManager::addOpenOrder(const TickerId tickerId, const Contract& contract, const Order& order)
{  
    _lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId, contract);
    _openOrders[orderId] = newOpenOrder;
    addOrderInOutputs(newOpenOrder);
    _lockOpenOrderMap->unlock();
    return orderId;
}


void OrderManager::updateOpenOrderOnExecution(const OrderId orderId, /*const Contract& contract,*/ const Execution& execution)
{
    OrderStatus orderStatus;
    _lockOpenOrderMap->lockForRead();
    if (_openOrders.count(orderId) != 0)
    {
        TickerId tickerId = _openOrders[orderId]->getTickerId();
        std::cout<<"signal invoked"<<std::endl;
        emit orderUpdated(orderId, tickerId, execution);
        //now get the order corresponding to the reqId
        _openOrders[orderId]->updateOrder(execution);

        updateOrderExecutionInOutputs(_openOrders[orderId]);
        orderStatus = _openOrders[orderId]->getOrderStatus();
    }

    _lockOpenOrderMap->unlock();

    if (orderStatus==Canceled || orderStatus==FullyFilled)
    {
        _lockOpenOrderMap->lockForWrite();
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
        //removeOrderFromOutputs(orderId);
        _lockOpenOrderMap->unlock();
    }
}

void OrderManager::placeOrder(const Order& order, const TickerId tickerId, Strategy* strategy)//, const bool isClosingOrder)
{
    Contract contract = Service::Instance()->getInstrumentManager()->getContractForTicker(tickerId);
    OrderId orderId = addOpenOrder(tickerId, contract, order);

    //get contract for tickerId

    QObject::connect(this,SIGNAL(orderUpdated(const OrderId, const TickerId, const Execution&)), strategy,SLOT(onExecutionUpdate(const OrderId, const TickerId, const Execution&)), Qt::UniqueConnection);

    Mode mode = Service::Instance()->getMode();
    if(mode == ForwardTest || mode == Trade)
    {
        QString message("Sending Place Order Request to IB OrderId: ");
        message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
        reportEvent(message);
        Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
    }
    else
    {
        Execution execution;
        int quantity = order.totalQuantity;
        execution.shares = quantity;

        if(order.action == "BUY")
        {
            execution.price = Service::Instance()->getInstrumentManager()->getAskPrice(tickerId);
            execution.side = "BOT";
        }
        else
        {
            execution.price = Service::Instance()->getInstrumentManager()->getBidPrice(tickerId);
            execution.side = "SLD";
        }

        execution.cumQty = order.totalQuantity;
        execution.orderId = orderId;
        //assume instantaneous execution for optimization purposes
        updateOpenOrderOnExecution(orderId, execution);
    }
}

void OrderManager::placeOrder(const Order& order, const Contract& contract, Strategy* strategy)//, const bool isClosingOrder)
{
    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);
    OrderId orderId = addOpenOrder(tickerId,contract, order);
    QObject::connect(this,SIGNAL(orderUpdated(const OrderId, const TickerId, const Execution&)),strategy,SLOT(onExecutionUpdate(const OrderId, const TickerId, const Execution&)), Qt::UniqueConnection);

    Mode mode = Service::Instance()->getMode();

    if(mode == ForwardTest || mode == Trade)
    {
       String message("Sending Place Order Request to IB OrderId: ");
       message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
       reportEvent(message);
       Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
    }
    else
    {
        Execution execution;
        int quantity = order.totalQuantity;
        execution.shares = quantity;

        //34#$%$##@^$%&%$&$#*%^#%$^%#^#%^
        //this fucker is not gettign the right price
        if(order.action == "BUY")
        {
            execution.price = Service::Instance()->getInstrumentManager()->getAskPrice(tickerId);
            execution.side ="BOT";
        }
        else
        {
            execution.price = Service::Instance()->getInstrumentManager()->getBidPrice(tickerId);
            execution.side = "SLD";
        }

        execution.cumQty = order.totalQuantity;
        execution.orderId = orderId;
        //assume instaneous execution for optimization purposes
        updateOpenOrderOnExecution(orderId, execution);
    }
}

void OrderManager::printThreadId()
{}

void OrderManager::reportEvent(const String& message)
{
    OutputInterface::Instance()->reportEvent("OrderManager", message);

    //printf("\nOrder Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
}

bool OrderManager::IsClosingOrder(const OrderId orderId)
{
    bool isClosingOrder = false;
    _lockOpenOrderMap->lockForRead();
    int x = _openOrders.size();

    if(_openOrders.count(orderId)!=0)
    {
       isClosingOrder = _openOrders[orderId]->IsClosingOrder();
    }
    _lockOpenOrderMap->unlock();
    return isClosingOrder;
}

void OrderManager::setMode(const Mode mode)
{
    _mode = mode;
}

void OrderManager::removeOrderFromOutputs(const OrderId orderId)
{
   _outputInterface->removeOrder(orderId);
}

void OrderManager::addOrderInOutputs(const OpenOrder* openOrder)
{
    _outputInterface->addOrder(openOrder);
}

void OrderManager::updateOrderExecutionInOutputs(const OpenOrder* openOrder)
{
    _outputInterface->updateOrderExecution(openOrder);
}

void OrderManager::updateOrderStatusInOutputs(const OpenOrder* openOrder)
{
    _outputInterface->updateOrderStatus(openOrder);
}
