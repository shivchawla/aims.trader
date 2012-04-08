#include "Platform/Trader/OrderManager.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/View/OpenOrderView.h"
#include "Platform/View/MainWindow.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/TraderAssistant.h"

#include <iostream>

OrderManager::OrderManager():QObject()
{
    printf("Creating Order Manager\n");
    _orderId=0;
    lockOpenOrderMap = new QReadWriteLock();
    //QThread* thread = ThreadManager::Instance()->requestThread();
    //connect(thread,SIGNAL(started()),this, SLOT(printThreadId()));
    //moveToThread(thread);
    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    QObject::connect(this,SIGNAL(orderPlaced(const OrderId, const Order&, const Contract&, const String&)),openOrderView,SLOT(addOrder(const OrderId, const Order&, const Contract&, const String&)));
    QObject::connect(this,SIGNAL(orderDeleted(const OrderId)),openOrderView,SLOT(removeOrder(const OrderId)));
}


OrderManager::~OrderManager()
{}

void OrderManager::updateOrderStatus(const OrderId orderId, const OrderStatus orderStatus)
{
   // std::cout<<"Acquiring Lock\n";

    lockOpenOrderMap->lockForRead();
   // std::cout<<QThread::currentThreadId()<<" acquired Lock"<<"\n";
    if(_openOrders.count(orderId)!=0)
    {
        _openOrders[orderId]->setOrderStatus(orderStatus);
    }
     //std::cout<<QThread::currentThreadId()<<" releasing Lock"<<"\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Released Lock\n";

}

void OrderManager::removeOpenOrder(const OrderId orderId)
{
//    std::cout<<"Acquiring Lock\n";

    lockOpenOrderMap->lockForWrite();
  //   std::cout<<QThread::currentThreadId()<<" acquired Lock"<<"\n";
    if(_openOrders.count(orderId)!=0)
    {
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
        emit orderDeleted(orderId);
    }
    //std::cout<<QThread::currentThreadId()<<" releasing Lock"<<"\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Released Lock\n";

}

void OrderManager::addOpenOrder(const Contract& contract, const Order& order)
{
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, contract);
    newOpenOrder->setOrderStatus(PendingSubmit);
//    std::cout<<"Acquiring Lock\n";
    lockOpenOrderMap->lockForWrite();
  //   std::cout<<QThread::currentThreadId()<<" acquired Lock"<<"\n";
    _openOrders[orderId] = newOpenOrder;
    // std::cout<<QThread::currentThreadId()<<" releasing Lock"<<"\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Released Lock\n";
}

void OrderManager::updateOpenOrderOnExecution(const OrderId orderId, const Contract& contract, const Execution& execution)
{
    OrderStatus orderStatus;
//    std::cout<<"Acquiring Lock\n";

    lockOpenOrderMap->lockForRead();
  //  std::cout<<QThread::currentThreadId()<<" acquired Lock for read"<<"\n";
    if (_openOrders.count(orderId) != 0)
    {
        //now get the order corresponding to the reqId
        _openOrders[orderId]->updateOrder(contract, execution);
        orderStatus = _openOrders[orderId]->getOrderStatus();
    }
    // std::cout<<QThread::currentThreadId()<<" releasing Lock"<<"\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Released Lock\n";


    if (orderStatus==Cancelled || orderStatus==FullyFilled)
    {
      //  std::cout<<"Acquiring Lock\n";

        lockOpenOrderMap->lockForWrite();
        // std::cout<<QThread::currentThreadId()<<" acquired Lock for writing"<<"\n";
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
         //std::cout<<QThread::currentThreadId()<<" releasing Lock"<<"\n";
        lockOpenOrderMap->unlock();
        //std::cout<<"Released \n";

    }
}

void OrderManager::placeOrder(const Order& order, const TickerId tickerId, Strategy* strategy)
{
    OrderId orderId = ++_orderId;

    //get contract for tickerId
    Contract contract = Service::Instance()->getInstrumentManager()->getContractForTicker(tickerId);
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, contract);
    QObject::connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId, const ExecutionStatus&)),strategy,SLOT(onExecutionUpdate(const OrderId, const ExecutionStatus&)));

    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onExecutionUpdate(const OrderId,const ExecutionStatus&)));
    connect(newOpenOrder,SIGNAL(statusUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onStatusUpdate(const OrderId,const ExecutionStatus&)));

    ///std::cout<<"Acquiring Lock\n";
    lockOpenOrderMap->lockForWrite();
     //std::cout<<QThread::currentThreadId()<<" acquired Lock"<<"\n";
    _openOrders[orderId] = newOpenOrder;
    newOpenOrder->setOrderStatus(PendingSubmit);
     //std::cout<<QThread::currentThreadId()<<" realeasing Lock"<<"\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Released Lock\n";

    //keep order of TA outgoing queue
    strategy->addPosition(orderId, tickerId);
    Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);    //
    emit orderPlaced(orderId, order, contract, strategy->getStrategyName());
}

void OrderManager::placeOrder(const Order& order, const Contract& contract, Strategy* strategy)
{
    //Strategy* strategy = qobject_cast<Strategy*>(QObject::sender());
    //Strategy* strategy = static_cast<Strategy*> (subscriber);
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, contract);

    QObject::connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId, const ExecutionStatus&)),strategy,SLOT(onExecutionUpdate(const OrderId, const ExecutionStatus&)));

    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onExecutionUpdate(const OrderId,const ExecutionStatus&)));
    connect(newOpenOrder,SIGNAL(statusUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onStatusUpdate(const OrderId,const ExecutionStatus&)));

    //std::cout<<"Acquiring Lock\n";
    lockOpenOrderMap->lockForWrite();
     //std::cout<<QThread::currentThreadId()<<" acquired Lock\n";
    _openOrders[orderId] = newOpenOrder;
    newOpenOrder->setOrderStatus(PendingSubmit);
     //std::cout<<QThread::currentThreadId()<<" releasing Lock\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Lock Released\n";

    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);

    //keep order of TA outgoing queue
    strategy->addPosition(orderId, tickerId);
    //std::cout<<contract.symbol;
    //printf("%s\n", order.orderType);
     Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
    //emit requestPlaceOrdertoTA(orderId, order, contract);
    emit orderPlaced(orderId, order,contract, strategy->getStrategyName());
}

void OrderManager::printThreadId()
{
    //printf("\nOrder Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
}
