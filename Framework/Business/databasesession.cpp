#include "databasesession.h"
#include "DataAccess/strategylinkedpositiondb.h"
#include "DataAccess/strategycompositedb.h"
#include "DataAccess/strategybuylistdb.h"
#include "DataAccess/strategydb.h"
#include "../Shared/DataAccess/InstrumentDb.h"
#include "DataAccess/orderdb.h"

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

QList<InstrumentData*> DatabaseSession :: getStrategyBuyList(const QString &strategyName) {
    InstrumentDb db;
    return db.getInstrumentsFromStrategyBuyList(strategyName);
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
uint DatabaseSession :: insertOrder(float limitPrice, uint quantity, quint8 action, quint8 status,
                    QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                    float avgFillPrice, uint filledQuantity, float commission,
                    float positionAmount, uint instrumentId, QDateTime goodTillDate, uint originalOrderId) {

    OrderDb db;
    return db.insertOrder(limitPrice, quantity, action, status,
                          placedDate, updatedDate, orderType,
                          avgFillPrice, filledQuantity, commission,
                          positionAmount, instrumentId, goodTillDate, originalOrderId);
}

uint DatabaseSession :: updateOrder(float limitPrice, uint quantity, quint8 action, quint8 status,
                 QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                 float avgFillPrice, uint filledQuantity, float commission,
                 float positionAmount, uint instrumentId,
                 QDateTime goodTillDate, uint originalOrderId) {
    OrderDb db;
    return db.updateOrderBasedOnOriginalId(limitPrice, quantity, action, status,
                          placedDate, updatedDate, orderType,
                          avgFillPrice, filledQuantity, commission,
                          positionAmount, instrumentId, goodTillDate, originalOrderId);
}
