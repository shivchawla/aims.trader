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
    InstrumentId instrumentId = Service::service().getInstrumentManager()->getInstrumentId(orderDetail.tickerId);
    DbStrategyId dbStrategyId = StrategyManager::strategyManager().getDatabaseStrategyId(orderDetail.strategyId);
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
    return _session->updateOrder(orderId, orderDetail);
}

void IODatabase::updateSpread(const StrategyId strategyId, const SpreadId spreadId, const SpreadDetail& spreadDetail)
{
    DbStrategyId dbStrategyId = StrategyManager::strategyManager().getDatabaseStrategyId(strategyId);
    DbSpreadId dbSpreadId = Service::service().getInstrumentManager()->getDbSpreadId(spreadId);

    _session->updateStrategyLinkedSpread(dbStrategyId, dbSpreadId, spreadDetail);
}

void IODatabase::updateSpreadPosition(const StrategyId strategyId, const SpreadId spreadId, const TickerId tickerId, const PositionDetail& positionDetail)
{
    InstrumentId instrumentId = Service::service().getInstrumentManager()->getInstrumentId(tickerId);
    DbStrategyId dbStrategyId = StrategyManager::strategyManager().getDatabaseStrategyId(strategyId);
    DbSpreadId dbSpreadId = Service::service().getInstrumentManager()->getDbSpreadId(spreadId);

    _session->updateStrategyLinkedSpreadPosition(dbStrategyId, dbSpreadId, instrumentId, positionDetail);
}


QList<StrategyData> IODatabase::getStrategies()
{
    return _session->getStrategies();
}

StrategyCompositeData IODatabase::getCompositeStrategy(const QString& strategyName)
{
    return _session->getCompositeStrategy(strategyName);
}

QHash<QString, QString> IODatabase::getStrategyConfigurations(uint strategyId)
{
    return _session->getStrategyConfigurations(strategyId);
}


QList<PositionData> IODatabase::getStrategyLinkedPositions()
{
    return _session->getStrategyLinkedPositions();
}

QList<ATContract> IODatabase::getATContractsForStrategy(const QString& strategyName)
{
    return _session->getATContractsForStrategy(strategyName);
}

QList<InstrumentData> IODatabase::getStrategyBuyList(const StrategyId strategyId)
{
    return _session->getStrategyBuyList(strategyId);
}

QList<PositionData> IODatabase::getOpenStrategyLinkedPositions(const DbStrategyId dbStrategyId)
{
    return _session->getOpenStrategyLinkedPositions(dbStrategyId);
}

QList<OrderData> IODatabase::getOrdersByStrategyName(const QString& strategyName)
{
    return _session->getOrdersByStrategyName(strategyName);
}

QList<InstrumentData> IODatabase::getInstrumentsWithSimilarSymbol(const QString& symbol)
{
    return _session->getInstrumentsWithSimilarSymbol(symbol);
}

void IODatabase::setupDatabaseSession(const Mode mode)
{
    _session->setupDatabaseSession(mode);
}

QList<SpreadData> IODatabase::getStrategySpreadList(const DbStrategyId dbStrategyId)
{
    return _session->getStrategySpreadList(dbStrategyId);
}

QList<InstrumentData> IODatabase::getInstruments(const QList<InstrumentId>& instrumentIdList)
{
     return _session->getInstrumentData(instrumentIdList);
}

InstrumentData IODatabase::getInstrument(const InstrumentId instrumentId)
{
     return _session->getInstrumentData(instrumentId);
}



