#include <Platform/View/IODatabase.h>
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Strategy/StrategyManager.h"

void IODatabase::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
    InstrumentId instrumentId = Service::service().getInstrumentManager()->getInstrumentId(tickerId);
    DbStrategyId dbStrategyId = StrategyManager::strategyManager().getDatabaseStrategyId(strategyId);
    _session->updateStrategyLinkedPosition(dbStrategyId, instrumentId, PositionDetail());
}

void IODatabase::updatePositionForExecution(const StrategyId strategyId, const TickerId tickerId, const PositionDetail& positionDetail)
{
     InstrumentId instrumentId = Service::service().getInstrumentManager()->getInstrumentId(tickerId);
     DbStrategyId dbStrategyId = StrategyManager::strategyManager().getDatabaseStrategyId(strategyId);
     _session->updateStrategyLinkedPosition(dbStrategyId, instrumentId, positionDetail);
}

void IODatabase::updatePositionForLastPrice(const StrategyId, const TickerId, const double runningPnL, const double Pnl)
{

}

int IODatabase::addOrder(const OrderId orderId, const OrderDetail& orderDetail, const String& strategyName)
{

//    Order o = order.getOrder();
    return 1;
//    return _session->insertOrder(o.lmtPrice,o.totalQuantity,o.action,order.getOrderStatus(),,,o.orderType,order.getAvgFillPrice(),
//                          order.getFilledShares(),,0,order.getTickerId(),QDateTime::fromString(QString::fromStdString(o.goodTillDate),Qt::ISODate),
//                          order.getOrderId());
}

int IODatabase::addOrder(const OrderId orderId, const OrderDetail& orderDetail)
{
    InstrumentId instrumentId = Service::service().getInstrumentManager()->getInstrumentId(orderDetail.getTickerId());
    DbStrategyId dbStrategyId = StrategyManager::strategyManager().getDatabaseStrategyId(orderDetail.getStrategyId());
    _session->insertOrder(orderId, dbStrategyId, instrumentId, orderDetail);
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

int IODatabase::updateOrder(const OrderId orderId, const OrderDetail& orderDetail)
{

    //Order o = order.getOrder();
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

QHash<QString, QString> IODatabase::getStrategyConfigurations(uint strategyId)
{
    return _session->getStrategyConfigurations(strategyId);
}


QList<StrategyLinkedPositionData*> IODatabase::getStrategyLinkedPositions()
{
    return _session->getStrategyLinkedPositions();
}

QList<ATContract*> IODatabase::getATContractsForStrategy(const QString& strategyName)
{
    return _session->getATContractsForStrategy(strategyName);
}

QList<InstrumentData*> IODatabase::getStrategyBuyList(const StrategyId strategyId)
{
    return _session->getStrategyBuyList(strategyId);
}

QList<StrategyLinkedPositionData*> IODatabase::getOpenStrategyLinkedPositions(const StrategyId strategyId)
{
    DbStrategyId dbId = StrategyManager::strategyManager().getDatabaseStrategyId(strategyId);
    return _session->getOpenStrategyLinkedPositions(dbId);
}

QList<OrderData*> IODatabase::getOrdersByStrategyName(const QString& strategyName)
{
    return _session->getOrdersByStrategyName(strategyName);
}

QList<InstrumentData*> IODatabase::getInstrumentsWithSimilarSymbol(const QString& symbol)
{
    return _session->getInstrumentsWithSimilarSymbol(symbol);
}



