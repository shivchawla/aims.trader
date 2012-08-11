#include <Platform/View/IODatabase.h>
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

void IODatabase::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
    InstrumentId instrumentId = Service::service().getInstrumentManager()->getInstrumentId(tickerId);

    _session->insertStrategyLinkedPosition(0,0,0,0,0,QDateTime::currentDateTime(), QDateTime::currentDateTime(),strategyId, instrumentId);
}

void IODatabase::updatePositionForExecution(const Position& position)
{

    _session->updateStrategyLinkedPosition(position.getSharesBought(), position.getSharesSold(),position.getAvgBought(), position.getAvgSold(),position.getTotalCommission(),
                                           QDateTime::currentDateTime(),position.getStrategyId(), position.getTickerId());

}

void IODatabase::updatePositionForLastPrice(const StrategyId, const TickerId, const double runningPnL, const double Pnl)
{

}

int IODatabase::addOrder(const OpenOrder& order, const String& strategyName)
{

    Order o = order.getOrder();
    return 1;
//    return _session->insertOrder(o.lmtPrice,o.totalQuantity,o.action,order.getOrderStatus(),,,o.orderType,order.getAvgFillPrice(),
//                          order.getFilledShares(),,0,order.getTickerId(),QDateTime::fromString(QString::fromStdString(o.goodTillDate),Qt::ISODate),
//                          order.getOrderId());
}

void IODatabase::onExecutionUpdate(const OrderId, const long, const long, const double, const double)
{

}

void IODatabase::onStatusUpdate(const OrderId orderId, const OrderStatus orderStatus)
{
}

void IODatabase::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats)
{

}

int IODatabase::updateOrder(const OpenOrder& order)
{

    Order o = order.getOrder();
    return 1;
    //return _session->insertOrder(o.lmtPrice,o.totalQuantity,o.action,order.getOrderStatus(),,,o.orderType,order.getAvgFillPrice(),
                          //order.getFilledShares(),,0,order.getTickerId(),QDateTime::fromString(QString::fromStdString(o.goodTillDate),Qt::ISODate),
                          //order.getOrderId());
}


QList<StrategyData*> IODatabase::getStrategies()
{
    return _session->getStrategies();
}

StrategyCompositeData* IODatabase::getCompositeStrategy(const QString& strategyName)
{
    return _session->getCompositeStrategy(strategyName);
}

QList<StrategyLinkedPositionData*> IODatabase::getStrategyLinkedPositions()
{
    return _session->getStrategyLinkedPositions();
}

QList<ATContract*> IODatabase::getATContractsForStrategy(const QString& strategyName)
{
    return _session->getATContractsForStrategy(strategyName);
}

QList<InstrumentData*> IODatabase::getStrategyBuyList(const QString &strategyName)
{
    return _session->getStrategyBuyList(strategyName);
}

QList<StrategyLinkedPositionData*> IODatabase::getOpenStrategyLinkedPositions(const uint strategyId)
{
    return _session->getOpenStrategyLinkedPositions(strategyId);
}

QList<OrderData*> IODatabase::getOrdersByStrategyName(const QString& strategyName)
{
    return _session->getOrdersByStrategyName(strategyName);
}


