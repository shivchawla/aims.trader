#include <Platform/View/IODatabase.h>

void IODatabase::addPosition(const StrategyId strategyId, const TickerId tickerId)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }
}

void IODatabase::updatePositionForExecution(const Position& position)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }

    _session->updateStrategyLinkedPosition(position.getSharesBought(), position.getSharesSold(),position.getAvgBought(), position.getAvgSold(),position.getTotalCommission(),
                                           QDateTime::currentDateTime(),position.getStrategyId(), position.getTickerId());

}

void IODatabase::updatePositionForLastPrice(const StrategyId, const TickerId, const double runningPnL, const double Pnl)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }

}

int IODatabase::addOrder(const OpenOrder& order, const String& strategyName)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }

    Order o = order.getOrder();
    return 1;
//    return _session->insertOrder(o.lmtPrice,o.totalQuantity,o.action,order.getOrderStatus(),,,o.orderType,order.getAvgFillPrice(),
//                          order.getFilledShares(),,0,order.getTickerId(),QDateTime::fromString(QString::fromStdString(o.goodTillDate),Qt::ISODate),
//                          order.getOrderId());
}

void IODatabase::onExecutionUpdate(const OrderId, const long, const long, const double, const double)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }

}

void IODatabase::onStatusUpdate(const OrderId orderId, const OrderStatus orderStatus)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }
}

void IODatabase::updateStrategy(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }

}

int IODatabase::updateOrder(const OpenOrder& order)
{
    if(!_session)
    {
        _session = new DatabaseSession();
    }

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

//QList<StrategyLinkedPositionData*> IODatabase::getPositionsForStrategy(const QString& strategyName)
//{
//    return _session->getPositionsForStrategy(strategyName);
//}

//QList<StrategyBuyListData*> getStrategyBuyListForStrategy(const QString& strategyName);
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


