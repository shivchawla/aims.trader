#include "Platform/Trader/OrderManager.h"
#include "Platform/Position/OpenOrder.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/TraderAssistant.h"
#include "Platform/View/IOInterface.h"
#include <iostream>
#include "Platform/View/IODatabase.h"

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

const OrderId OrderManager::addOpenOrder(const TickerId tickerId, const Order& order, const Contract& contract, Strategy* strategy)
{  
    _lockOpenOrderMap->lockForWrite();
    OrderId orderId = ++_orderId;
    OpenOrder* newOpenOrder = new OpenOrder(orderId, order, tickerId, contract);
    _openOrders[orderId] = newOpenOrder;
    _orderIdToStrategy[orderId] = strategy;
    addOrderInOutputs(newOpenOrder, strategy->getStrategyName());
    _lockOpenOrderMap->unlock();
    return orderId;
}

void OrderManager::updateOpenOrderOnExecution(const OrderId orderId, /*const Contract& contract,*/ const Execution& execution)
{
    OrderStatus orderStatus;

    _lockOpenOrderMap->lockForWrite();

    if (OpenOrder* openOrder = _openOrders.value(orderId,NULL))
    {
        //now get the order corresponding to the reqId
        openOrder->updateOrder(execution);
        updateStrategyForExecution(openOrder);
        updateOrderExecutionInOutputs(openOrder);
        orderStatus = openOrder->getOrderStatus();

        if (orderStatus==Canceled || orderStatus==FullyFilled)
        {
            //_lockOpenOrderMap->lockForWrite();
            delete _openOrders[orderId];
            _openOrders.remove(orderId);
            //removeOrderFromOutputs(orderId);
            //_lockOpenOrderMap->unlock();
        }
    }
    _lockOpenOrderMap->unlock();
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

void OrderManager::placeOrder(const Order& order, const TickerId tickerId, Strategy* strategy)
{
    if(order.totalQuantity==0)
    {
        return;
    }

    Contract contract = Service::service().getInstrumentManager()->getIBContract(tickerId);

    OrderId orderId = addOpenOrder(tickerId, order, contract, strategy);

    Mode mode = Service::service().getMode();

    if(mode == ForwardTest || mode == Trade)
    {
       String message("Sending Place Order Request to IB OrderId: ");
       message.append(QString::number(orderId)).append(" OrderType: ").append(QString::fromStdString(order.orderType)).append(" Contract: ").append(QString::fromStdString(contract.symbol)).append(" StrategyName: ").append(strategy->getStrategyName());
       reportEvent(message);

       //setup contract here
       Service::service().getTrader()->getTraderAssistant()->placeOrder(orderId, order, contract);
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



//void OrderManager::printThreadId()
//{}

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
       isClosingOrder = openOrder->IsClosingOrder();
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

void OrderManager::addOrderInOutputs(const OpenOrder* openOrder, const String& strategyName, const OutputType type)
{
    IOInterface::ioInterface().addOrder(openOrder, strategyName, type);
}

void OrderManager::updateOrderExecutionInOutputs(const OpenOrder* openOrder, const OutputType type)
{
    IOInterface::ioInterface().updateOrderExecution(openOrder, type);
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
    OrderId id = openOrder->getOrderId();
    if(Strategy* strategy = _orderIdToStrategy.value(id, NULL))
    {
        strategy->requestStrategyUpdateForExecution(openOrder);
    }
}

const Order OrderManager::getOrder(const OrderId orderId)
{
    Order order;
    _lockOpenOrderMap->lockForRead();
    if(OpenOrder* openOrder = _openOrders.value(orderId,NULL))
    {
        order = openOrder->getOrder();
    }
    _lockOpenOrderMap->unlock();

    return order;
}

void OrderManager::cancelOrder(const OrderId orderId)
{
    Service::service().getTrader()->getTraderAssistant()->cancelOrder(orderId);
}

