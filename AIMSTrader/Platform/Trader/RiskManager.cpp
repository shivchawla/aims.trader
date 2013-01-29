#include "Platform/Trader/RiskManager.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"

RiskManager::RiskManager():DataSubscriber()
{
    QThread* thread = ThreadManager::threadManager().requestThread();
    moveToThread(thread);
}

//void RiskManager::addPosition(const Position* position)
//{
//    TickerId tickerId = position->getTickerId();
//    QMetaObject::invokeMethod(this, SLOT(createPosition(const TickerId)), Q_ARG(TickerId, tickerId));
//}

void RiskManager::addPosition(const TickerId tickerId)
{
    _mutex.lock();
    Position* pos = _positionMap.value(tickerId, NULL);
    if(!pos)
    {
        _positionMap[tickerId] = (pos = new Position(tickerId));
    }
    _mutex.unlock();
}

void RiskManager::updatePosition(const OrderId orderId, const TickerId tickerId, const OrderDetail& orderDetail)
{
    bool t = QMetaObject::invokeMethod(this, "updatePositionForExecution"/*(const OrderId, const TickerId, const OrderDetail&))*/,
                              Qt::QueuedConnection,
                              Q_ARG(OrderId, orderId), Q_ARG(TickerId, tickerId), Q_ARG(OrderDetail, orderDetail));

    subscribeMarketData(tickerId);
}

void RiskManager::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double price)
{
    _mutex.lock();
    Position* pos = _positionMap.value(tickerId, NULL);
    if(!pos)
    {
         _positionMap[tickerId] = (pos = new Position(tickerId));
    }

    pos->update(tickType, price);
    _mutex.unlock();
}

void RiskManager::updatePositionForExecution(const OrderId orderId, const TickerId tickerId, const OrderDetail& orderDetail)
{
    _mutex.lock();
    Position* pos = _positionMap.value(tickerId, NULL);
    if(!pos)
    {
        _positionMap[tickerId] = (pos = new Position(tickerId));
    }

    pos->update(orderId, orderDetail);
    _mutex.unlock();
}


//WTF...this is called by another thread...how to manage this
bool RiskManager::canPlaceOrder(const TickerId tickerId, const Order& order)
{
    bool canPlaceOrder = true;
    _mutex.lock();
    Position* pos = _positionMap.value(tickerId, NULL);
    if(pos)
    {
        double currentAmountInvested = pos->getAmountInvested();
        double currentPrice = Service::service().getInstrumentManager()->getLastPrice(tickerId);
        canPlaceOrder = !abs(pos->getAmountInvested() + order.totalQuantity * currentPrice) > 10000;
    }
    _mutex.unlock();

    return canPlaceOrder;
}

