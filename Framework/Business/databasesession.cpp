#include "databasesession.h"
#include "DataAccess/PositionDb.h"
#include "DataAccess/strategycompositedb.h"
#include "DataAccess/strategybuylistdb.h"
#include "DataAccess/strategydb.h"
#include "../Shared/DataAccess/InstrumentDb.h"
#include "DataAccess/orderdb.h"
#include "DataAccess/PositionDetailDb.h"
#include "DataAccess/strategyconfigurationdb.h"
#include "DataAccess/StrategyRunDb.h"
#include "DataAccess/StrategyLinkedPositiondb.h"
#include "DataAccess/StrategyLinkedSpreadPositionDb.h"
#include "DataAccess/SpreadPositionDb.h"
#include "DataAccess/SpreadPositionDetailDb.h"
#include "DataAccess/StrategySpreadBuyListDb.h"
#include "DataAccess/StrategyLinkedSpreadDb.h"
#include "DataAccess/SpreadDetailDb.h"
#include "DataAccess/SpreadDb.h"

DatabaseSession::DatabaseSession()
{
    _runId = 0;
    _lastRunId = 0;
    _positionId = 0;
    _spreadPositionId = 0;
}

DatabaseSession::~DatabaseSession()
{}

QList<StrategyData> DatabaseSession::getStrategies() {
    StrategyDb db;
    return db.getStrategies();
}

StrategyCompositeData DatabaseSession::getCompositeStrategy(const QString& strategyName) {
    StrategyCompositeDb db;
    return db.getStrategyComposite(strategyName);
}

QList<PositionData> DatabaseSession::getStrategyLinkedPositions() {
    PositionDb db;
    return db.getStrategyLinkedPositions(_runId);
}

QList<PositionData> DatabaseSession::getOpenStrategyLinkedPositions(const uint strategyId) {
    PositionDb db;
    QList<PositionData> list = db.getOpenStrategyLinkedPositions(_lastRunId, strategyId);
    //load the map first
//    foreach(StrategyLinkedPositionData* position, list) {
//        _positionsMap.insert(QPair<uint, uint>(position->strategyId, position->instrumentId), position->strategyLinkedPositionId);
//    }
    return list;
}

QList<PositionData> DatabaseSession::getOpenStrategyLinkedPositions(const QString& strategyName) {
    PositionDb db;
    QList<PositionData> list = db.getOpenStrategyLinkedPositions(_lastRunId, strategyName);
    //load the map first
//    foreach(StrategyLinkedPositionData* position, list) {
//        _positionsMap.insert(QPair<uint, uint>(position->strategyId, position->instrumentId), position->strategyLinkedPositionId);
//    }
    return list;
}

QList<SpreadPositionData> DatabaseSession::getOpenStrategyLinkedSpreadPositions(const QString& strategyName) {
    SpreadPositionDb db;
    QList<SpreadPositionData> list = db.getOpenStrategyLinkedSpreadPositions(_lastRunId, strategyName);
    //load the map first
//    foreach(StrategyLinkedPositionData* position, list) {
//        _positionsMap.insert(QPair<uint, uint>(position->strategyId, position->instrumentId), position->strategyLinkedPositionId);
//    }
    return list;
}



QList<ATContract> DatabaseSession::getATContractsForStrategy(const QString& strategyName) {
    //StrategyBuyListDb db;
    //return db.getATContractsForStrategy(strategyName);
}

QList<InstrumentData> DatabaseSession :: getStrategyBuyList(const StrategyId strategyId) {
    InstrumentDb db;
    return db.getInstrumentsFromStrategyBuyList(strategyId);
}

QList<InstrumentData> DatabaseSession :: getStrategyBuyList(const String& strategyName) {
    InstrumentDb db;
    return db.getInstrumentsFromStrategyBuyList(strategyName);
}


QList<OrderData> DatabaseSession :: getOrdersByStrategyName(const QString& strategyName) {
    OrderDb db;
    return db.getOrdersByStrategyName(_runId, strategyName);
}

QList<InstrumentData> DatabaseSession::getInstrumentData(const QList<InstrumentId>& instrumentIdList)
{
    InstrumentDb db;
    return db.getInstruments(instrumentIdList);
}

InstrumentData DatabaseSession::getInstrumentData(const InstrumentId instrumentId)
{
    InstrumentDb db;
    return db.getInstrument(instrumentId);
}


QList<SpreadData> DatabaseSession::getStrategySpreadList(const uint strategyId)
{
    StrategySpreadBuyListDb db;
    //return db.getStrategySpreadBuyList(strategyId);
}

QList<SpreadData> DatabaseSession::getStrategySpreadList(const String& strategyName)
{
    StrategySpreadBuyListDb db;
    return db.getStrategySpreadBuyList(strategyName);
}


uint DatabaseSession::updateStrategyLinkedPosition(const uint strategyId, const uint instrumentId, const PositionDetail& detail)
{
        if(_positionIdMap[strategyId] == QMap<uint, uint>())
        {
            _positionIdMap.insert(strategyId, QMap<uint, uint>());
        }

        uint sharesBought = detail.sharesBought;
        uint sharesSold = detail.sharesSold;
        double avgBuyPrice = detail.avgBuyPrice;
        double avgSellPrice = detail.avgSellPrice;
        double totalCommission = detail.totalCommission;
        QDateTime updatedDate = QDateTime::currentDateTime();
        bool isNewLeg = detail.isNewLeg;

        if(uint positionId = _positionIdMap[strategyId].value(instrumentId,0))
        {
            PositionDb positionDb;
            uint retVal = positionDb.updateStrategyLinkedPosition(_runId, positionId, sharesBought, sharesSold, avgBuyPrice,
                                                     avgSellPrice, totalCommission, updatedDate);

            //get the latest DetailId
            uint positionDetailId = _latestPositionDetailIdMap.value(positionId, 0);
//            if(positionDetailId == 0)
//            {
//                _latestPositionDetailIdMap[positionId] = (positionDetailId = 1);
//            }

            PositionDetailDb positionDetailDb;

            if(isNewLeg)
            {
                _latestPositionDetailIdMap[positionId] = ++positionDetailId ;
                //QDateTime createdTime = ;
                retVal = positionDetailDb.insertStrategyLinkedPositionDetail(_runId, positionId, positionDetailId, detail.latestLegSharesBought, detail.latestLegSharesSold, detail.latestLegBuyPrice,
                                                               detail.latestLegSellPrice, detail.latestLegOrderCommission, detail.createdTime);
            }
            else
            {
               positionDetailDb.updateStrategyLinkedPositionDetail(_runId, positionId, positionDetailId, detail.latestLegSharesBought, detail.latestLegSharesSold, detail.latestLegBuyPrice,
                                                                   detail.latestLegSellPrice, detail.latestLegOrderCommission, updatedDate);
            }
        }
        else
        {
            _positionId++;

            StrategyLinkedPositionDb strategyLinkedPositionDb;
            strategyLinkedPositionDb.insertRow(_runId, strategyId, instrumentId, _positionId);

            QDateTime createdTime = detail.createdTime;

            PositionDb positionDb;
            uint retVal = positionDb.insertStrategyLinkedPosition(_runId, _positionId, sharesBought, sharesSold, avgBuyPrice,
                                                          avgSellPrice, totalCommission, createdTime);

            _positionIdMap[strategyId][instrumentId] = _positionId;

            //PositionDetailDb positionDetailDb;
            //uint positionDetailId = 1;
            //retVal = positionDetailDb.insertStrategyLinkedPositionDetail(_runId, _positionId, positionDetailId, sharesBought, sharesSold, avgBuyPrice,
              //                                             avgSellPrice, totalCommission, createdTime);

            //_latestPositionDetailIdMap[positionId] = positionDetailId;
        }

     return 1;
}

uint DatabaseSession::updateStrategyLinkedSpread(const DbStrategyId strategyId, const DbSpreadId spreadId, const SpreadDetail& spreadDetail)
{
    if(_spreadIdMap[strategyId] == QMap<DbSpreadId, uint>())
    {
        _spreadIdMap.insert(strategyId, QMap<DbSpreadId, uint>());
    }

    double valueBought = spreadDetail.totalValueBought;
    double valueSold = spreadDetail.totalValueSold;
    QDateTime createdTime  = spreadDetail.createdTime;
    QDateTime updatedTime = spreadDetail.updatedTime;
    double commission = spreadDetail.totalCommission;

    if(uint id = _spreadIdMap[strategyId].value(spreadId, 0))
    {
        SpreadDetailDb db;
        db.updateSpreadDetail(_runId, id, spreadDetail.totalValueBought, spreadDetail.totalValueSold, spreadDetail.totalCommission, spreadDetail.updatedTime);
    }
    else
    {
        uint id = 1;
        _spreadIdMap[strategyId][spreadId] = id;
        StrategyLinkedSpreadDb db;
        db.insertStrategyLinkedSpread(_runId, strategyId, spreadId, id);

        SpreadDetailDb db1;
        db1.insertSpreadDetail(_runId, id, 0,0,0, QDateTime::currentDateTime());
    }

}

uint DatabaseSession::updateStrategyLinkedSpreadPosition(const uint strategyId, const uint spreadId, const uint instrumentId, const PositionDetail& detail)
{
    if(_spreadPositionIdMap[strategyId] == QMap<uint, QMap<uint, uint> >())
    {
        _spreadPositionIdMap.insert(strategyId, QMap<uint, QMap<uint, uint> >());
    }

    uint sharesBought = detail.sharesBought;
    uint sharesSold = detail.sharesSold;
    double avgBuyPrice = detail.avgBuyPrice;
    double avgSellPrice = detail.avgSellPrice;
    double totalCommission = detail.totalCommission;
    QDateTime updatedDate = QDateTime::currentDateTime();
    bool isNewLeg = detail.isNewLeg;

    if(_spreadPositionIdMap[strategyId][spreadId] == QMap<uint, uint>())
    {
        _spreadPositionIdMap[strategyId].insert(spreadId, QMap<uint, uint>());
    }

    if(uint spreadPositionId = _spreadPositionIdMap[strategyId][spreadId].value(instrumentId, 0))
    {
        SpreadPositionDb spreadPositionDb;
        uint retVal = spreadPositionDb.updateStrategyLinkedSpreadPosition(_runId, spreadPositionId, sharesBought, sharesSold, avgBuyPrice,
                                                 avgSellPrice, totalCommission, updatedDate);

        //get the latest DetailId
        uint positionDetailId = _latestSpreadPositionDetailIdMap.value(spreadPositionId, 0);
//            if(positionDetailId == 0)
//            {
//                _latestPositionDetailIdMap[positionId] = (positionDetailId = 1);
//            }

        SpreadPositionDetailDb positionDetailDb;

        if(isNewLeg)
        {
            _latestSpreadPositionDetailIdMap[spreadPositionId] = ++positionDetailId ;
            retVal = positionDetailDb.insertStrategyLinkedSpreadPositionDetail(_runId, spreadPositionId, positionDetailId, detail.latestLegSharesBought, detail.latestLegSharesSold, detail.avgBuyPrice,
                                                                               detail.avgSellPrice, detail.latestLegOrderCommission, detail.createdTime);
        }
        else
        {
           positionDetailDb.updateStrategyLinkedSpreadPositionDetail(_runId, spreadPositionId, positionDetailId, detail.latestLegSharesBought, detail.latestLegSharesSold, detail.avgBuyPrice,
                                                               detail.avgSellPrice, detail.latestLegOrderCommission, updatedDate);
        }
    }
    else
    {
        ++_spreadPositionId;

        StrategyLinkedSpreadPositionDb strategyLinkedSpreadPositionDb;
        strategyLinkedSpreadPositionDb.insertRow(_runId, strategyId, spreadId, instrumentId, _spreadPositionId);

        QDateTime createdTime = detail.createdTime;

        SpreadPositionDb spreadPositionDb;
        uint retVal = spreadPositionDb.insertStrategyLinkedSpreadPosition(_runId, _spreadPositionId, sharesBought, sharesSold, avgBuyPrice,
                                                      avgSellPrice, totalCommission, createdTime);

        _spreadPositionIdMap[strategyId][spreadId][instrumentId] = _positionId;

        //PositionDetailDb positionDetailDb;
        //uint positionDetailId = 1;
        //retVal = positionDetailDb.insertStrategyLinkedPositionDetail(_runId, _positionId, positionDetailId, sharesBought, sharesSold, avgBuyPrice,
          //                                             avgSellPrice, totalCommission, createdTime);

        //_latestPositionDetailIdMap[positionId] = positionDetailId;
    }

 return 1;

}

// returns the newly inserted primary key and not count of rows inserted
uint DatabaseSession::insertOrder(const uint orderId, const uint strategyId, const uint instrumentId, const OrderDetail& orderDetail)
{
    Order order = orderDetail.order;
    double limitPrice = order.lmtPrice;
    uint quantity = order.totalQuantity;
    QString action = QString::fromStdString(order.action);
    QString status = getOrderStatusString(orderDetail.status);
    QDateTime placedDate = orderDetail.placedTime;
    QString orderType = QString::fromStdString(order.orderType);
    QDateTime goodTillDate = QDateTime::fromString(QString::fromStdString(order.goodTillDate), QString::fromStdString("yyyymmdd hh:mm:ss"));

    OrderDb db;
    return db.insertOrder(_runId, orderId, strategyId, instrumentId, orderType, limitPrice, quantity, action, status, placedDate, goodTillDate);
}

uint DatabaseSession :: updateOrder(const uint orderId, const OrderDetail& orderDetail)
{
    QString status = getOrderStatusString(orderDetail.status);
    QDateTime updatedDate = orderDetail.lastUpdatedTime;
    double avgFillPrice = orderDetail.avgFillPrice;
    uint filledQuantity = orderDetail.filledShares;
    double commission = orderDetail.commission;

    OrderDb db;
    return db.updateOrder(_runId, orderId, status, avgFillPrice, filledQuantity, commission,  updatedDate);
}

StrategyConfigurationData DatabaseSession :: getStrategyConfiguration(const QString& strategyName, QString confKey) {
    StrategyConfigurationDb db;
    return db.getStrategyConfiguration(strategyName, confKey);
}

QHash<QString, QString> DatabaseSession :: getStrategyConfigurations(const QString& strategyName)
{
    StrategyConfigurationDb db;
    return db.getStrategyConfigurations(strategyName);
}


uint DatabaseSession :: insertStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue) {
    StrategyConfigurationDb db;
    //return db.insertStrategyConfiguration(strategyId, confKey, confValue);
}

uint DatabaseSession :: updateStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue) {
    StrategyConfigurationDb db;
    //return db.updateStrategyConfiguration(strategyId, confKey, confValue);
}

QList<InstrumentData> DatabaseSession::getInstrumentsWithSimilarSymbol(const QString& symbol)
{
    InstrumentDb instrumentDb;
    return instrumentDb.getInstrumentsWithSimilarSymbol(symbol);
}

uint DatabaseSession::getTradeRunId()
{
    StrategyRunDb strategyRunDb;
    return strategyRunDb.generateRunId();
}

uint DatabaseSession::getLastRunId(const Mode mode)
{
    StrategyRunDb strategyRunDb;
    return strategyRunDb.getLastRunId(mode);
}

void DatabaseSession::setupDatabaseSession(const Mode mode)
{
    _runId = getTradeRunId();
    _lastRunId = getLastRunId(_mode = mode);
}

void DatabaseSession::insertStrategyInStrategyRun(const uint strategyId, const StrategyData& strategyData)
{
    StrategyRunDb strategyRunDb;
    strategyRunDb.insertStrategy(_runId, _mode, strategyId, strategyData.strategyName, strategyData.newPositionFlag);
}

SpreadData DatabaseSession::getSpreadData(const uint spreadId)
{
    SpreadDb db;
    return db.getSpreadData(spreadId);
}



