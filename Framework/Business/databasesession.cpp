#include "databasesession.h"
#include "DataAccess/strategylinkedpositiondb.h"
#include "DataAccess/strategycompositedb.h"
#include "DataAccess/strategybuylistdb.h"
#include "DataAccess/strategydb.h"
#include "../Shared/DataAccess/InstrumentDb.h"
#include "DataAccess/orderdb.h"
#include "DataAccess/strategylinkedpositiondetaildb.h"
#include "DataAccess/strategyconfigurationdb.h"

DatabaseSession::DatabaseSession()
{
}

DatabaseSession::~DatabaseSession()
{
}

QList<StrategyData*> DatabaseSession::getStrategies() {
    StrategyDb db;
    return db.getStrategies();
}

StrategyCompositeData* DatabaseSession::getCompositeStrategy(const QString& strategyName) {
    StrategyCompositeDb db;
    return db.getStrategyComposite(strategyName);
}

QList<StrategyLinkedPositionData*> DatabaseSession::getStrategyLinkedPositions() {
    StrategyLinkedPositionDb db;
    return db.getStrategyLinkedPositions();
}

QList<StrategyLinkedPositionData*> DatabaseSession::getOpenStrategyLinkedPositions(const uint strategyId) {
    StrategyLinkedPositionDb db;
    QList<StrategyLinkedPositionData*> list = db.getOpenStrategyLinkedPositions(strategyId);
    //load the map first
    foreach(StrategyLinkedPositionData* position, list) {
        _positionsMap.insert(QPair<uint, uint>(position->strategyId, position->instrumentId), position->strategyLinkedPositionId);
    }
    return list;
}

/* QList<StrategyBuyListData*> DatabaseSession::getStrategyBuyListForStrategy(const QString& strategyName) {
    StrategyBuyListDb db;
    return db.getStrategyBuyListsForStrategy(strategyName);
} */

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

//returns the newly inserted row's primary key
uint DatabaseSession :: insertStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought, float avgAmountSold,
                                  float totalAmountCommission, QDateTime createdDate, QDateTime updatedDate, uint strategyId,
                                  uint instrumentId) {
    StrategyLinkedPositionDb db;
    uint newKey = db.insertStrategyLinkedPosition(numberBought, numberSold, avgAmountBought, avgAmountSold,
                                                  totalAmountCommission, createdDate, updatedDate, strategyId, instrumentId);
    _positionsMap.insert(QPair<uint, uint>(strategyId, instrumentId), newKey);

    return newKey;
}


uint DatabaseSession :: updateStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought,
                                  float avgAmountSold, float totalAmountCommission, QDateTime updatedDate,
                                  uint strategyId, uint instrumentId) {
    QPair<uint, uint> pair(strategyId, instrumentId);
    uint retVal=0;
    int pkey = _positionsMap.value(pair);
    if (pkey > 0) {
        StrategyLinkedPositionDb db;
        retVal = db.updateStrategyLinkedPosition(pkey, numberBought, numberSold, avgAmountBought,
                                                 avgAmountSold, totalAmountCommission, updatedDate,
                                                 strategyId, instrumentId);
        if (numberBought == numberSold)
            _positionsMap.remove(pair);
    }
    else
    {
        insertStrategyLinkedPosition(numberBought, numberSold, avgAmountBought, avgAmountSold,
                                                          totalAmountCommission, updatedDate, updatedDate, strategyId, instrumentId);
    }

    return retVal;
}

// returns the newly inserted primary key and not count of rows inserted
uint DatabaseSession :: insertOrder(uint orderId, float limitPrice, uint quantity, quint8 action, quint8 status,
                    QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                    float avgFillPrice, uint filledQuantity, float commission,
                    float positionAmount, uint instrumentId, QDateTime goodTillDate, uint strategyId) {

    OrderDb db;
    return db.insertOrder(orderId, limitPrice, quantity, action, status,
                          placedDate, updatedDate, orderType,
                          avgFillPrice, filledQuantity, commission,
                          positionAmount, instrumentId, goodTillDate, strategyId);
}

uint DatabaseSession :: updateOrder(uint orderId, float limitPrice, uint quantity, quint8 action, quint8 status,
                 QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                 float avgFillPrice, uint filledQuantity, float commission,
                 float positionAmount, uint instrumentId,
                 QDateTime goodTillDate, uint originalOrderId) {
    OrderDb db;
    return db.updateOrder(orderId, limitPrice, quantity, action, status,
                          placedDate, updatedDate, orderType,
                          avgFillPrice, filledQuantity, commission,
                          positionAmount, instrumentId, goodTillDate, originalOrderId);
}

uint DatabaseSession :: insertStrategyLinkedPositionDetail(uint sharesBought, uint sharesSold, float avgBought,
                                      float avgSold, float commission, QDateTime createdDateTime,
                                      uint strategyLinkedPositionId) {
    StrategyLinkedPositionDetailDb db;
    return db.insertStrategyLinkedPositionDetail(sharesBought, sharesSold, avgBought, avgSold,
                                                 commission, createdDateTime, strategyLinkedPositionId);
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


//QList<StrategyConfigurationData*> DatabaseSession :: getStrategyConfigurations(uint strategyId) {
//    StrategyConfigurationDb db;
//    return db.getStrategyConfigurations(strategyId);
//}

uint DatabaseSession :: insertStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue) {
    StrategyConfigurationDb db;
    return db.insertStrategyConfiguration(strategyId, confKey, confValue);
}

uint DatabaseSession :: updateStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue) {
    StrategyConfigurationDb db;
    return db.updateStrategyConfiguration(strategyId, confKey, confValue);
}
