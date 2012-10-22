#include "databasesession.h"
#include "DataAccess/StrategyLinkedPositionDb2.h"
#include "DataAccess/strategycompositedb.h"
#include "DataAccess/strategybuylistdb.h"
#include "DataAccess/strategydb.h"
#include "../Shared/DataAccess/InstrumentDb.h"
#include "DataAccess/orderdb.h"
#include "DataAccess/StrategyLinkedPositionDetailDb2.h"
#include "DataAccess/strategyconfigurationdb.h"
//#include <QDateTime>


DatabaseSession::DatabaseSession()
{
    //_positionIdMap.insert(_runId, QMap<uint, uint>());
}

DatabaseSession::~DatabaseSession()
{}

QList<StrategyData*> DatabaseSession::getStrategies() {
    StrategyDb db;
    return db.getStrategies();
}

StrategyCompositeData* DatabaseSession::getCompositeStrategy(const QString& strategyName) {
    StrategyCompositeDb db;
    return db.getStrategyComposite(strategyName);
}

QList<StrategyLinkedPositionData2*> DatabaseSession::getStrategyLinkedPositions() {
    StrategyLinkedPositionDb2 db;
    return db.getStrategyLinkedPositions();
}

QList<StrategyLinkedPositionData2*> DatabaseSession::getOpenStrategyLinkedPositions(const uint strategyId) {
    StrategyLinkedPositionDb2 db;
    QList<StrategyLinkedPositionData2*> list = db.getOpenStrategyLinkedPositions(strategyId);
    //load the map first
//    foreach(StrategyLinkedPositionData* position, list) {
//        _positionsMap.insert(QPair<uint, uint>(position->strategyId, position->instrumentId), position->strategyLinkedPositionId);
//    }
    return list;
}

QList<ATContract*> DatabaseSession::getATContractsForStrategy(const QString& strategyName) {
    StrategyBuyListDb db;
    return db.getATContractsForStrategy(strategyName);
}

QList<InstrumentData*> DatabaseSession :: getStrategyBuyList(const StrategyId strategyId) {
    InstrumentDb db;
    return db.getInstrumentsFromStrategyBuyList(strategyId);
}

QList<OrderData*> DatabaseSession :: getOrdersByStrategyName(const QString& strategyName) {
    OrderDb db;
    return db.getOrdersByStrategyName(strategyName);
}


uint DatabaseSession::updateStrategyLinkedPosition(const uint strategyId, const uint instrumentId, const PositionDetail& detail)
{
        QMap<uint, uint> instrumentIdToPositionIdMap = _positionIdMap[strategyId];

        if(instrumentIdToPositionIdMap == QMap<uint, uint>())
        {
            _positionIdMap.insert(strategyId, QMap<uint, uint>());
        }

        uint sharesBought = detail.getSharesBought();
        uint sharesSold = detail.getSharesSold();
        double avgBuyPrice = detail.getAvgBought();
        double avgSellPrice = detail.getAvgSold();
        double totalCommission = detail.getTotalCommission();
        QDateTime updatedDate = QDateTime::currentDateTime();
        bool isNewLeg = detail.IsNewLeg();

        if(uint positionId = instrumentIdToPositionIdMap.value(instrumentId,0))
        {
            StrategyLinkedPositionDb2 StrategyLinkedPositionDb2;
            uint retVal = StrategyLinkedPositionDb2.updateStrategyLinkedPosition(_runId, positionId, sharesBought, sharesSold, avgBuyPrice,
                                                     avgSellPrice, totalCommission, updatedDate);

            //get the latest DetailId
            uint positionDetailId = _latestPositionDetailIdMap.value(positionId, 0);
            if(positionDetailId == 0)
            {
                _latestPositionDetailIdMap[positionDetailId] = (positionDetailId = 1);
            }

            StrategyLinkedPositionDetailDb2 StrategyLinkedPositionDetailDb2;

            if(isNewLeg)
            {
                _latestPositionDetailIdMap[positionDetailId] = ++positionDetailId ;
                QDateTime createdTime = detail.getCreatedTime();
                retVal = StrategyLinkedPositionDetailDb2.insertStrategyLinkedPositionDetail(_runId, positionId, positionDetailId, sharesBought, sharesSold, avgBuyPrice,
                                                               avgSellPrice, totalCommission, createdTime);
            }
            else
            {
                StrategyLinkedPositionDetailDb2.updateStrategyLinkedPositionDetail(_runId, positionId, positionDetailId, sharesBought, sharesSold, avgBuyPrice,
                                                      avgSellPrice, totalCommission, updatedDate);
            }
        }
        else
            {
                positionId = 1;
                QDateTime createdTime = detail.getCreatedTime();

                StrategyLinkedPositionDb2 StrategyLinkedPositionDb2;
                uint retVal = StrategyLinkedPositionDb2.insertStrategyLinkedPosition(_runId, positionId, sharesBought, sharesSold, avgBuyPrice,
                                                              avgSellPrice, totalCommission, createdTime);

                instrumentIdToPositionIdMap[instrumentId] = positionId;

                StrategyLinkedPositionDetailDb2 StrategyLinkedPositionDetailDb2;
                uint positionDetailId = 1;
                retVal = StrategyLinkedPositionDetailDb2.insertStrategyLinkedPositionDetail(_runId, positionId, positionDetailId, sharesBought, sharesSold, avgBuyPrice,
                                                               avgSellPrice, totalCommission, createdTime);

                _latestPositionDetailIdMap[instrumentId] = positionDetailId;
            }

     return 1;
}


// returns the newly inserted primary key and not count of rows inserted
uint DatabaseSession::insertOrder(const uint orderId, const uint strategyId, const uint instrumentId, const OrderDetail& orderDetail)
{
    Order order = orderDetail.getOrder();
    double limitPrice = order.lmtPrice;
    uint quantity = order.totalQuantity;
    QString action = QString::fromStdString(order.action);
    QString status = getOrderStatusString(orderDetail.getOrderStatus());
    QDateTime placedDate = orderDetail.getPlacedTime();
    QString orderType = QString::fromStdString(order.orderType);
    QDateTime goodTillDate = QDateTime::fromString(QString::fromStdString(order.goodTillDate), QString::fromStdString("yyyymmdd hh:mm:ss"));

    OrderDb db;
    return db.insertOrder(_runId, orderId, strategyId, instrumentId, orderType, limitPrice, quantity, action, status, placedDate, goodTillDate);
}

uint DatabaseSession :: updateOrder(const uint orderId, const uint strategyId, const uint instrumentId, const OrderDetail& orderDetail)
{
    QString status = getOrderStatusString(orderDetail.getOrderStatus());
    QDateTime updatedDate = orderDetail.getLastUpdatedTime();
    double avgFillPrice = orderDetail.getAvgFillPrice();
    uint filledQuantity = orderDetail.getFilledShares();
    double commission = orderDetail.getCommission();

    OrderDb db;
    return db.updateOrder(_runId, orderId, strategyId, instrumentId, status, avgFillPrice, filledQuantity, commission,  updatedDate);
}

StrategyConfigurationData* DatabaseSession :: getStrategyConfiguration(uint strategyId, QString confKey) {
    StrategyConfigurationDb db;
    return db.getStrategyConfiguration(strategyId, confKey);
}

QHash<QString, QString> DatabaseSession :: getStrategyConfigurations(const uint strategyId)
{
    StrategyConfigurationDb db;
    return db.getStrategyConfigurations(strategyId);
}

uint DatabaseSession :: insertStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue) {
    StrategyConfigurationDb db;
    return db.insertStrategyConfiguration(strategyId, confKey, confValue);
}

uint DatabaseSession :: updateStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue) {
    StrategyConfigurationDb db;
    return db.updateStrategyConfiguration(strategyId, confKey, confValue);
}

QList<InstrumentData*> DatabaseSession::getInstrumentsWithSimilarSymbol(const QString& symbol)
{
    InstrumentDb instrumentDb;
    return instrumentDb.getInstrumentsWithSimilarSymbol(symbol);
}
