#include "Platform/Trader/OrderManager.h"
#include "Platform/Startup/Service.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Utils/ThreadManager.h"
#include <iostream>
OrderManager::OrderManager():_orderId(0)
{
    printf("Creating Order Manager\n");
    QThread* thread = ThreadManager::Instance()->requestThread();
    connect(thread,SIGNAL(started()),this, SLOT(printThreadId()));
    moveToThread(thread);
}

OrderManager::~OrderManager()
{
    this->thread()->quit();
}

void OrderManager::updateOrderStatus(const OrderId orderId, const OrderStatus orderStatus)
{
    _openOrders[orderId]->setOrderStatus(orderStatus);
}

void OrderManager::removeOpenOrder(const OrderId orderId)
{
    if(_openOrders.count(orderId)!=0)
    {
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
    }
}

void OrderManager::addOpenOrder(const Contract& contract, const Order& order)
{

}

void OrderManager::updateOpenOrderExecution(const OrderId orderId, const Contract& contract, const Execution& execution)
{
    if (_openOrders.count(orderId) != 0)
    {
        //now get the order corresponding to the reqId
        _openOrders[orderId]->updateOrder(contract, execution);
        OrderStatus orderStatus = _openOrders[orderId]->getOrderStatus();
        if (orderStatus==Cancelled || orderStatus==FullyFilled)
        {
            delete _openOrders[orderId];
            _openOrders.erase(orderId);
        }
    }
}

void OrderManager::placeOrder(const Order& order, const Contract& contract)
{
    Strategy* strategy = qobject_cast<Strategy*>(QObject::sender());
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order);
    _openOrders[++_orderId] = newOpenOrder;
    QObject::connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId, const Execution&)),strategy,SLOT(updatePosition(const OrderId, const Execution&)));
    QObject::connect(this, SIGNAL(orderPlaced(const OrderId, const Contract&)), strategy, SLOT(addPosition(const OrderId, const Contract&)));

    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);

    emit requestTickerId(contract);
    emit requestPlaceOrdertoTA(_orderId, order, contract);
    emit orderPlaced(orderId, contract);
}

void OrderManager::printThreadId()
{
    printf("\nOrder Manager Thread \t");
    std::cout<<QThread::currentThreadId();
}
