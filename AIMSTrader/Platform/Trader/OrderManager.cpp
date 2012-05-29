#include "Platform/Trader/OrderManager.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/View/OpenOrderView.h"
#include "Platform/View/MainWindow.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/TraderAssistant.h"

#include "Platform/Reports/EventReport.h"
#include "Platform/Startup/OutputService.h"
#include "Platform/View/OutputInterface.h"
#include <iostream>

OrderManager::OrderManager():QObject()
{
    _orderId=0;
    lockOpenOrderMap = new QReadWriteLock();
    _outputInterface = OutputService::Instance()->getOutputInterface();
//    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
//    QObject::connect(this, SIGNAL(orderPlaced(const OrderId, const Order&, const Contract&, const String&)),openOrderView, SLOT(addOrder(const OrderId, const Order&, const Contract&, const String&)));
//    QObject::connect(this, SIGNAL(orderDeleted(const OrderId)),openOrderView, SLOT(removeOrder(const OrderId)));
}


OrderManager::~OrderManager()
{}

void OrderManager::updateOrderStatus(const OrderId orderId, const OrderStatus orderStatus)
{
    lockOpenOrderMap->lockForRead();

    if(_openOrders.count(orderId)!=0)
    {
        _openOrders[orderId]->setOrderStatus(orderStatus);
    }
    lockOpenOrderMap->unlock();
}

void OrderManager::removeOpenOrder(const OrderId orderId)
{
    lockOpenOrderMap->lockForWrite();

    if(_openOrders.count(orderId)!=0)
    {
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
        removeOrderFromOutputs(orderId);
        //emit orderDeleted(orderId);
    }
    lockOpenOrderMap->unlock();
}

void OrderManager::addOpenOrder(const Contract& contract, const Order& order)
{
    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);
    lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId);
    //newOpenOrder->setOrderStatus(PendingSubmit);
    _openOrders[orderId] = newOpenOrder;
    addOrderInOutputs(newOpenOrder);
    lockOpenOrderMap->unlock();
}

void OrderManager::updateOpenOrderOnExecution(const OrderId orderId, /*const Contract& contract,*/ const Execution& execution)
{
    OrderStatus orderStatus;
    lockOpenOrderMap->lockForRead();    
    if (_openOrders.count(orderId) != 0)
    {
        TickerId tickerId = _openOrders[orderId]->getTickerId();
        emit orderUpdated(orderId, tickerId, execution);
        //now get the order corresponding to the reqId
        _openOrders[orderId]->updateOrder(execution);

        updateOrderExecutionInOutputs(_openOrders[orderId]);
        orderStatus = _openOrders[orderId]->getOrderStatus();
    }

    lockOpenOrderMap->unlock();

    if (orderStatus==Cancelled || orderStatus==FullyFilled)
    {
        lockOpenOrderMap->lockForWrite();
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
        removeOrderFromOutputs(orderId);
        lockOpenOrderMap->unlock();
        //emit orderDeleted(orderId);
    }
}

void OrderManager::placeOrder(const Order& order, const TickerId tickerId, Strategy* strategy)//, const bool isClosingOrder)
{
    lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId);
    _openOrders[orderId] = newOpenOrder;
    //newOpenOrder->setOrderStatus(PendingSubmit);
    lockOpenOrderMap->unlock();

    //get contract for tickerId
    Contract contract = Service::Instance()->getInstrumentManager()->getContractForTicker(tickerId);

    QObject::connect(this,SIGNAL(orderUpdated(const OrderId, const TickerId, const Execution&)), strategy,SLOT(onExecutionUpdate(const OrderId, const TickerId, const Execution&)));

    if(_mode == ForwardTest || _mode == Trade)
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

        if(quantity > 0)
        {
            execution.price = Service::Instance()->getInstrumentManager()->getAskPrice(tickerId);
        }
        else
        {
            execution.price = Service::Instance()->getInstrumentManager()->getBidPrice(tickerId);
        }

        execution.cumQty = order.totalQuantity;
        execution.orderId = orderId;
        //assume instantaneous execution for optimization purposes
        updateOpenOrderOnExecution(orderId, execution);
    }

    addOrderInOutputs(newOpenOrder);
    //emit orderPlaced(orderId, order, contract, strategy->getStrategyName());
}

void OrderManager::placeOrder(const Order& order, const Contract& contract, Strategy* strategy)//, const bool isClosingOrder)
{
    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);
    lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId);
    _openOrders[orderId] = newOpenOrder;
    //newOpenOrder->setOrderStatus(PendingSubmit);
    lockOpenOrderMap->unlock();

    //TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);

    /*if(isClosingOrder)
    {
        newOpenOrder->setIsClosingOrder();
    }*/
    QObject::connect(this,SIGNAL(orderUpdated(const OrderId, const TickerId, const Execution&)),strategy,SLOT(onExecutionUpdate(const OrderId, const TickerId, const Execution&)));

//    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
//    connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onExecutionUpdate(const OrderId,const ExecutionStatus&)));
//    connect(newOpenOrder,SIGNAL(statusUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onStatusUpdate(const OrderId,const ExecutionStatus&)));

    //strategy->addPosition(orderId, tickerId, isClosingOrder);

    if(_mode==ForwardTest || _mode == Trade)
    {
       Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);

        addOrderInOutputs(newOpenOrder);
        //emit orderPlaced(orderId, order, contract, strategy->getStrategyName());
    }
    else
    {
        Execution execution;
        int quantity = order.totalQuantity;
        execution.shares = quantity;

        //34#$%$##@^$%&%$&$#*%^#%$^%#^#%^
        //this fucker is not gettign the right price
        if(quantity>0)
        {
            execution.price = Service::Instance()->getInstrumentManager()->getAskPrice(tickerId);
        }
        else
        {
            execution.price = Service::Instance()->getInstrumentManager()->getBidPrice(tickerId);
        }

        execution.cumQty = order.totalQuantity;
        execution.orderId = orderId;
        //assume instaneous execution for optimization purposes
        updateOpenOrderOnExecution(orderId, execution);
    }

    QString message("Sending Place Order Request to IB OrderId: ");
    message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
    reportEvent(message);
}

void OrderManager::printThreadId()
{}

void OrderManager::reportEvent(const String& message)
{
    Service::Instance()->getEventReport()->report("OrderManager", message);

    //printf("\nOrder Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
}

bool OrderManager::IsClosingOrder(const OrderId orderId)
{
    bool isClosingOrder = false;
    lockOpenOrderMap->lockForRead();
    int x = _openOrders.size();

    if(_openOrders.count(orderId)!=0)
    {
       isClosingOrder = _openOrders[orderId]->IsClosingOrder();
    }
    lockOpenOrderMap->unlock();
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
