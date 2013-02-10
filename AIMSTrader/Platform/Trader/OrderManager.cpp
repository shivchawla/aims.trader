#include "Platform/Trader/OrderManager.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/View/IOInterface.h"
#include <iostream>
#include "Platform/View/IODatabase.h"
#include "Platform/Strategy/StrategyManager.h"

OrderManager::OrderManager()
{
    _orderId=0;
    _lockOpenOrderMap = new QReadWriteLock();
}

OrderManager::~OrderManager()
{
    foreach(OpenOrder* openOrder, _openOrders)
    {
        delete openOrder;
    }
}

void OrderManager::updateOrderStatus(const OrderId orderId, const OrderStatus orderStatus)
{
    _lockOpenOrderMap->lockForWrite();

    if(OpenOrder* openOrder = _openOrders.value(orderId,NULL))
    {
        openOrder->setOrderStatus(orderStatus);
    }
    _lockOpenOrderMap->unlock();
}

void OrderManager::removeOpenOrder(const OrderId orderId)
{
    _lockOpenOrderMap->lockForWrite();

    if(OpenOrder* openOrder = _openOrders.value(orderId,NULL))
    {
        delete openOrder;
        _openOrders.remove(orderId);
        //removeOrderFromOutputs(orderId);
    }
    _lockOpenOrderMap->unlock();
}



// We want to know list of pending order for a tickerId-strategyId
//
void OrderManager::addOpenOrder(const OrderId orderId, const StrategyId strategyId, const TickerId tickerId, const Order& order, const Contract& contract)
{  
    _lockOpenOrderMap->lockForWrite();
    OpenOrder* newOpenOrder = new OpenOrder(orderId, strategyId, tickerId, order, contract);
    _openOrders[orderId] = newOpenOrder;
//    _orderIdToTickerId[orderId] = tickerId;

//    QHash<TickerId, int > pendingQuantityForTickerId =  _pendingQuantityForTickerAndStrategy.value(strategyId, QHash<TickerId, int>());
//    if(pendingQuantityForTickerId == QHash<TickerId, int>())
//    {
//        _pendingQuantityForTickerAndStrategy[strategyId] = QHash<TickerId, int>();
//    }

//    int pendingQuantity = _pendingQuantityForTickerAndStrategy[strategyId].value(tickerId,0);
//    _pendingQuantityForTickerAndStrategy[strategyId][tickerId] = pendingQuantity + order.action.compare("BUY") ? order.totalQuantity : -order.totalQuantity;

    //_orderIdToStrategy[orderId] = strategy;
    OrderDetail orderDetail = newOpenOrder->getOrderDetail();
    _lockOpenOrderMap->unlock();
    addOrderInOutputs(orderId, orderDetail);
    //return orderId;
}

void OrderManager::updateOpenOrderOnExecution(const OrderId orderId, /*const Contract& contract,*/ const Execution& execution)
{
    OrderStatus orderStatus;
    OrderDetail orderDetail;

    _lockOpenOrderMap->lockForRead();
    if (OpenOrder* openOrder = _openOrders.value(orderId,NULL))
    {    
        //now get the order corresponding to the reqId
        openOrder->updateOrder(execution);
        orderDetail = openOrder->getOrderDetail();

//        StrategyId strategyId = orderDetail.strategyId;
//        TickerId tickerId = _orderIdToTickerId.value(orderId,0);

//        QHash<TickerId, int > pendingQuantityForTickerId =  _pendingQuantityForTickerAndStrategy.value(strategyId, QHash<TickerId, int>());
//        if(pendingQuantityForTickerId == QHash<TickerId, int>())
//        {
//            _pendingQuantityForTickerAndStrategy[strategyId] = QHash<TickerId, int>();
//        }

//        int pendingQuantity = _pendingQuantityForTickerAndStrategy[strategyId].value(tickerId,0);
//        _pendingQuantityForTickerAndStrategy[strategyId][tickerId] = pendingQuantity + orderDetail.order.action.compare("BUY") ? -orderDetail.filledShares : orderDetail.filledShares;
    }
    _lockOpenOrderMap->unlock();

    String message("Order Execution Received for OrderId: ");
    message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(orderDetail.order.orderType)).append(" Quantity: ").append(QString::number(execution.shares)).append(" Side: ").append(QString::fromStdString(execution.side));
    reportEvent(message);

    updateStrategyForExecution(orderId, orderDetail);
    updateOrderExecutionInOutputs(orderId, orderDetail);

    orderStatus = orderDetail.status;

    if(orderStatus==Canceled || orderStatus==FullyFilled)
    {
        _lockOpenOrderMap->lockForWrite();
        delete _openOrders[orderId];
        _openOrders.remove(orderId);
        //removeOrderFromOutputs(orderId);
        _lockOpenOrderMap->unlock();
    }


}

//void OrderManager::placeOrder(const Order& order, const TickerId tickerId, Strategy* strategy)//, const bool isClosingOrder)
//{
//    if(order.totalQuantity==0)
//    {
//        return;
//    }

//    ATContract aTcontract = service()->getInstrumentManager()->getContractForTicker(tickerId);
//    OrderId orderId = addOpenOrder(tickerId, order, aTcontract.contract, strategy);

//    Mode mode = service()->getMode();
//    if(mode == ForwardTest || mode == Trade)
//    {
//        QString message("Sending Place Order Request to IB OrderId: ");
//        message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(aTcontract.contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
//        reportEvent(message);
//        service()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, aTcontract.contract);
//    }
//    else
//    {
//        Execution execution;
//        int quantity = order.totalQuantity;
//        execution.shares = quantity;

//        if(order.action == "BUY")
//        {
//            execution.price = service()->getInstrumentManager()->getAskPrice(tickerId);
//            execution.side = "BOT";
//        }
//        else
//        {
//            execution.price = service()->getInstrumentManager()->getBidPrice(tickerId);
//            execution.side = "SLD";
//        }

//        execution.cumQty = order.totalQuantity;
//        execution.orderId = orderId;
//        //assume instantaneous execution for optimization purposes
//        updateOpenOrderOnExecution(orderId, execution);
//    }
//}

//void OrderManager::placeOrder(const Order& order, const ATContract& aTcontract, Strategy* strategy)//, const bool isClosingOrder)
//{
//    if(order.totalQuantity==0)
//    {
//        return;
//    }

//    TickerId tickerId = service()->getInstrumentManager()->getTickerId(aTcontract);
//    OrderId orderId = addOpenOrder(tickerId, order, aTcontract.contract, strategy);

//    Mode mode = service()->getMode();

//    if(mode == ForwardTest || mode == Trade)
//    {
//       String message("Sending Place Order Request to IB OrderId: ");
//       message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(aTcontract.contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
//       reportEvent(message);
//       service()->getTrader()->getTraderAssistant()->placeOrder(orderId, order, aTcontract.contract);
//    }
//    else
//    {
//        Execution execution;
//        int quantity = order.totalQuantity;
//        execution.shares = quantity;

//        //34#$%$##@^$%&%$&$#*%^#%$^%#^#%^
//        //this fucker is not gettign the right price
//        if(order.action == "BUY")
//        {
//            execution.price = service()->getInstrumentManager()->getAskPrice(tickerId);
//            execution.side ="BOT";
//        }
//        else
//        {
//            execution.price = service()->getInstrumentManager()->getBidPrice(tickerId);
//            execution.side = "SLD";
//        }

//        execution.cumQty = order.totalQuantity;
//        execution.orderId = orderId;
//        //assume instaneous execution for optimization purposes
//        updateOpenOrderOnExecution(orderId, execution);
//    }
//}

void OrderManager::placeOrder(const OrderId orderId, const TickerId tickerId, const StrategyId strategyId, const Order& order)
{
    std::cout<<order.action<<"\n";
    if(order.totalQuantity<=0)
    {
        reportEvent("Negative/Zero Total Qunatity");
        return;
    }

    Contract contract = Service::service().getInstrumentManager()->getIBContract(tickerId);

    addOpenOrder(orderId, strategyId, tickerId, order, contract);

    Mode mode = Service::service().getMode();

    if(mode == ForwardTest || mode == Trade)
    {
       String message("Sending Place Order Request to IB OrderId: ");
       message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(StrategyManager::strategyManager().getStrategyName(strategyId));
       reportEvent(message);

       //setup contract here
       Service::service().getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
    }
    else
    {
        String message("Sending Order Request OrderId: ");
        message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(StrategyManager::strategyManager().getStrategyName(strategyId));
        reportEvent(message);

        Execution execution;
        int quantity = order.totalQuantity;
        execution.shares = quantity;

        //34#$%$##@^$%&%$&$#*%^#%$^%#^#%^
        //this fucker is not gettign the right price
        if(order.action == "BUY")
        {
            execution.price = Service::service().getInstrumentManager()->getAskPrice(tickerId);
            execution.side ="BOT";
        }
        else
        {
            execution.price = Service::service().getInstrumentManager()->getBidPrice(tickerId);
            execution.side = "SLD";
        }

        execution.cumQty = order.totalQuantity;
        execution.orderId = orderId;

        //assume instaneous execution for optimization purposes
        updateOpenOrderOnExecution(orderId, execution);
    }
}

void OrderManager::placeSpreadOrder(const OrderId orderId, const StrategyId strategyId, const Order& order, const Contract& contract)
{
    std::cout<<order.action<<"\n";
    if(order.totalQuantity<=0)
    {
        reportEvent("Negative/Zero Total Qunatity");
        return;
    }

    addOpenOrder(orderId, strategyId, -1, order, contract);

    Mode mode = Service::service().getMode();

    if(mode == ForwardTest || mode == Trade)
    {
       String message("Sending Place Order Request to IB OrderId: ");
       message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(StrategyManager::strategyManager().getStrategyName(strategyId));
       reportEvent(message);

       //setup contract here
       Service::service().getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
    }
    else
    {
        String message("Sending Order Request OrderId: ");
        message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(StrategyManager::strategyManager().getStrategyName(strategyId));
        reportEvent(message);

        Execution execution;
        int quantity = order.totalQuantity;
        execution.shares = quantity;

        //34#$%$##@^$%&%$&$#*%^#%$^%#^#%^
        //this fucker is not gettign the right price
        if(order.action == "BUY")
        {
            //execution.price = Service::service().getInstrumentManager()->getAskPrice();
            execution.side ="BOT";
        }
        else
        {
            //execution.price = Service::service().getInstrumentManager()->getBidPrice(tickerId);
            execution.side = "SLD";
        }

        execution.cumQty = order.totalQuantity;
        execution.orderId = orderId;

        //assume instaneous execution for optimization purposes
        updateOpenOrderOnExecution(orderId, execution);
    }

    //return orderId;
}


void OrderManager::reportEvent(const String& message, const MessageType mType)
{
    IOInterface::ioInterface().reportEvent("OrderManager", message, mType);
}

bool OrderManager::IsClosingOrder(const OrderId orderId)
{
    bool isClosingOrder = false;
    _lockOpenOrderMap->lockForRead();
    int x = _openOrders.size();

    if(OpenOrder* openOrder = _openOrders.value(orderId,NULL))
    {
       isClosingOrder = openOrder->getOrderDetail().isClosingOrder;
    }
    _lockOpenOrderMap->unlock();
    return isClosingOrder;
}

void OrderManager::setMode(const Mode mode)
{
    _mode = mode;
}

void OrderManager::removeOrderFromOutputs(const OrderId orderId, const OutputType type)
{
   IOInterface::ioInterface().removeOrder(orderId, type);
}

void OrderManager::addOrderInOutputs(const OpenOrder* openOrder, const OutputType type)
{
//    QString strategyName = StrategyManager::strategyManager().getStrategyName(openOrder->getStrategyId());
//    IOInterface::ioInterface().addOrder(openOrder, strategyName, type);
}

void OrderManager::addOrderInOutputs(const OrderId orderId, const OrderDetail& orderDetail, const OutputType type)
{
    //QString strategyName = StrategyManager::strategyManager().getStrategyName(orderDetail.getStrategyId());
    IOInterface::ioInterface().addOrder(orderId, orderDetail, type);
}


void OrderManager::updateOrderExecutionInOutputs(const OpenOrder* openOrder, const OutputType type)
{
    //IOInterface::ioInterface().updateOrderExecution(openOrder->getOrderId(), openOrder->getOrderDetail(), type);
}

void OrderManager::updateOrderExecutionInOutputs(const OrderId orderId, const OrderDetail& orderDetail, const OutputType type)
{
    IOInterface::ioInterface().updateOrderExecution(orderId, orderDetail, type);
}

void OrderManager::updateOrderStatusInOutputs(const OpenOrder* openOrder,const OutputType type)
{
    IOInterface::ioInterface().updateOrderStatus(openOrder, type);
}

//private function...No thread locking
//this design can be improved...
//Order manager Doesn't need strategy information(strategy pointer)
//It can just send this information to StrategyManager via strategyId
void OrderManager::updateStrategyForExecution(const OpenOrder* openOrder)
{
    OrderId orderId = openOrder->getOrderId();
    OrderDetail orderDetail = openOrder->getOrderDetail();
    StrategyManager::strategyManager().updateStrategyForOrderExecution(orderId, orderDetail);
//    if(Strategy* strategy = _orderIdToStrategy.value(id, NULL))
//    {
//        strategy->requestStrategyUpdateForExecution(openOrder);
//    }
}

void OrderManager::updateStrategyForExecution(const OrderId orderId, const OrderDetail& orderDetail)
{
    //OrderId orderId = openOrder->getOrderId();
    //OrderDetail orderDetail = openOrder->getOrderDetail();
    StrategyManager::strategyManager().updateStrategyForOrderExecution(orderId, orderDetail);
//    if(Strategy* strategy = _orderIdToStrategy.value(id, NULL))
//    {
//        strategy->requestStrategyUpdateForExecution(openOrder);
//    }
}


const Order OrderManager::getOrder(const OrderId orderId)
{
    Order order;
    _lockOpenOrderMap->lockForRead();
    if(OpenOrder* openOrder = _openOrders.value(orderId,NULL))
    {
        order = openOrder->getOrderDetail().order;
    }
    _lockOpenOrderMap->unlock();

    return order;
}

void OrderManager::cancelOrder(const OrderId orderId)
{
    Service::service().getTrader()->getTraderAssistant()->cancelOrder(orderId);
}


OrderId OrderManager::requestOrderId()
{
    _lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    _lockOpenOrderMap->unlock();
    return orderId;
}

//int OrderManager::getPendingQuantity(const StrategyId strategyId, const TickerId tickerId)
//{
//    _lockOpenOrderMap->lockForRead();
//    int pendingShares = _pendingQuantityForTickerAndStrategy.value(strategyId, QHash<TickerId, int>()).value(tickerId, 0);
//    _lockOpenOrderMap->unlock();

//    return pendingShares;
//}


