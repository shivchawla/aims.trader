#include "Platform/Trader/OrderManager.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/View/OpenOrderView.h"
#include "Platform/View/MainWindow.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/TraderAssistant.h"
<<<<<<< HEAD
#include "Platform/Reports/EventReport.h"
=======

>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
#include <iostream>

OrderManager::OrderManager():QObject()
{
<<<<<<< HEAD
    _orderId=0;
    lockOpenOrderMap = new QReadWriteLock();
    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    QObject::connect(this, SIGNAL(orderPlaced(const OrderId, const Order&, const Contract&, const String&)),openOrderView, SLOT(addOrder(const OrderId, const Order&, const Contract&, const String&)));
    QObject::connect(this, SIGNAL(orderDeleted(const OrderId)),openOrderView, SLOT(removeOrder(const OrderId)));
=======
    printf("Creating Order Manager\n");
    _orderId=0;
    lockOpenOrderMap = new QReadWriteLock();
    //QThread* thread = ThreadManager::Instance()->requestThread();
    //connect(thread,SIGNAL(started()),this, SLOT(printThreadId()));
    //moveToThread(thread);
    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    QObject::connect(this,SIGNAL(orderPlaced(const OrderId, const Order&, const Contract&, const String&)),openOrderView,SLOT(addOrder(const OrderId, const Order&, const Contract&, const String&)));
    QObject::connect(this,SIGNAL(orderDeleted(const OrderId)),openOrderView,SLOT(removeOrder(const OrderId)));
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}


OrderManager::~OrderManager()
{}

void OrderManager::updateOrderStatus(const OrderId orderId, const OrderStatus orderStatus)
{
<<<<<<< HEAD
    lockOpenOrderMap->lockForRead();
=======
   // std::cout<<"Acquiring Lock\n";

    lockOpenOrderMap->lockForRead();
   // std::cout<<QThread::currentThreadId()<<" acquired Lock"<<"\n";
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    if(_openOrders.count(orderId)!=0)
    {
        _openOrders[orderId]->setOrderStatus(orderStatus);
    }
<<<<<<< HEAD
    lockOpenOrderMap->unlock();
=======
     //std::cout<<QThread::currentThreadId()<<" releasing Lock"<<"\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Released Lock\n";

>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

void OrderManager::removeOpenOrder(const OrderId orderId)
{
<<<<<<< HEAD
    lockOpenOrderMap->lockForWrite();

=======
//    std::cout<<"Acquiring Lock\n";

    lockOpenOrderMap->lockForWrite();
  //   std::cout<<QThread::currentThreadId()<<" acquired Lock"<<"\n";
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    if(_openOrders.count(orderId)!=0)
    {
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
        emit orderDeleted(orderId);
    }
<<<<<<< HEAD
    lockOpenOrderMap->unlock();
=======
    //std::cout<<QThread::currentThreadId()<<" releasing Lock"<<"\n";
    lockOpenOrderMap->unlock();
    //std::cout<<"Released Lock\n";

>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

void OrderManager::addOpenOrder(const Contract& contract, const Order& order)
{
    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);
    lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
<<<<<<< HEAD
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId);
    //newOpenOrder->setOrderStatus(PendingSubmit);
    _openOrders[orderId] = newOpenOrder;
    lockOpenOrderMap->unlock();
}

void OrderManager::updateOpenOrderOnExecution(const OrderId orderId, /*const Contract& contract,*/ const Execution& execution)
{
    OrderStatus orderStatus;

    lockOpenOrderMap->lockForRead();
    if (_openOrders.count(orderId) != 0)
    {
        //now get the order corresponding to the reqId
        _openOrders[orderId]->updateOrder(execution);
        orderStatus = _openOrders[orderId]->getOrderStatus();
    }

    lockOpenOrderMap->unlock();

    if (orderStatus==Cancelled || orderStatus==FullyFilled)
    {
        lockOpenOrderMap->lockForWrite();
        delete _openOrders[orderId];
        _openOrders.erase(orderId);
        lockOpenOrderMap->unlock();
        emit orderDeleted(orderId);
=======
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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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

<<<<<<< HEAD
void OrderManager::placeOrder(const Order& order, const TickerId tickerId, Strategy* strategy, const bool isClosingOrder)
{
    lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId);
    _openOrders[orderId] = newOpenOrder;
    //newOpenOrder->setOrderStatus(PendingSubmit);
    lockOpenOrderMap->unlock();


    //get contract for tickerId
    Contract contract = Service::Instance()->getInstrumentManager()->getContractForTicker(tickerId);
    if(isClosingOrder)
    {
        newOpenOrder->setIsClosingOrder();
    }
    QObject::connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId, const ExecutionStatus&, const bool)),strategy,SLOT(onExecutionUpdate(const OrderId, const ExecutionStatus&, const bool)));

    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId, const ExecutionStatus&, const bool)),openOrderView,SLOT(onExecutionUpdate(const OrderId, const ExecutionStatus&)));
    connect(newOpenOrder,SIGNAL(statusUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onStatusUpdate(const OrderId,const ExecutionStatus&)));


    //keep order of TA outgoing queue
    strategy->addPosition(orderId, tickerId, isClosingOrder);

    //if(_mode==ForwardTest || _mode == Trade)
    //{
        Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
    //}

    emit orderPlaced(orderId, order, contract, strategy->getStrategyName());

    QString message("Sending Place Order Request to IB OrderId: ");
    message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
    reportEvent(message);
}
=======
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
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806

void OrderManager::placeOrder(const Order& order, const Contract& contract, Strategy* strategy, const bool isClosingOrder)
{
    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);
<<<<<<< HEAD
    lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId);
    _openOrders[orderId] = newOpenOrder;
    //newOpenOrder->setOrderStatus(PendingSubmit);
    lockOpenOrderMap->unlock();

    //TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(contract);

    if(isClosingOrder)
    {
        newOpenOrder->setIsClosingOrder();
    }
    QObject::connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId, const ExecutionStatus&)),strategy,SLOT(onExecutionUpdate(const OrderId, const ExecutionStatus&)));

    OpenOrderView* openOrderView = MainWindow::mainWindow()->getOpenOrderView();
    connect(newOpenOrder,SIGNAL(orderUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onExecutionUpdate(const OrderId,const ExecutionStatus&)));
    connect(newOpenOrder,SIGNAL(statusUpdated(const OrderId,const ExecutionStatus&)),openOrderView,SLOT(onStatusUpdate(const OrderId,const ExecutionStatus&)));



    //strategy->addPosition(orderId, tickerId, isClosingOrder);

    if(_mode==ForwardTest || _mode == Trade)
    {
        Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
        emit orderPlaced(orderId, order, contract, strategy->getStrategyName());

    }
    else
    {
        Execution execution;
        execution.shares = order.totalQuantity;
        execution.price = Service::Instance()->getInstrumentManager()->getLastPrice(tickerId);
        execution.cumQty = order.totalQuantity;
        execution.orderId = orderId;
        //assume instaneous execution for optimization purposes
        updateOpenOrderOnExecution(orderId, execution);
    }

    QString message("Sending Place Order Request to IB OrderId: ");
    message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
    reportEvent(message);
=======

    //keep order of TA outgoing queue
    strategy->addPosition(orderId, tickerId);
    //std::cout<<contract.symbol;
    //printf("%s\n", order.orderType);
     Service::Instance()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
    //emit requestPlaceOrdertoTA(orderId, order, contract);
    emit orderPlaced(orderId, order,contract, strategy->getStrategyName());
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}

void OrderManager::printThreadId()
{}

void OrderManager::reportEvent(const String& message)
{
<<<<<<< HEAD
    Service::Instance()->getEventReport()->report("OrderManager", message);
=======
    //printf("\nOrder Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
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

