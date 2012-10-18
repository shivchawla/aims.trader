#include "databasesession.h"
#include "DataAccess/strategylinkedpositiondb.h"
#include "DataAccess/strategycompositedb.h"
#include "DataAccess/strategybuylistdb.h"
#include "DataAccess/strategydb.h"
#include "../Shared/DataAccess/InstrumentDb.h"
#include "DataAccess/orderdb.h"
#include "DataAccess/strategylinkedpositiondetaildb.h"
#include "DataAccess/strategyconfigurationdb.h"
//#include <QDateTime>


DatabaseSession::DatabaseSession()
{}

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

QList<StrategyLinkedPositionData*> DatabaseSession::getStrategyLinkedPositions() {
    StrategyLinkedPositionDb db;
    return db.getStrategyLinkedPositions();
}

QList<StrategyLinkedPositionData*> DatabaseSession::getOpenStrategyLinkedPositions(const uint strategyId) {
    StrategyLinkedPositionDb db;
    QList<StrategyLinkedPositionData*> list = db.getOpenStrategyLinkedPositions(strategyId);
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


uint DatabaseSession :: updateStrategyLinkedPosition(const uint strategyId, const uint instrumentId, const PositionDetail& detail)
{
//    //Get the positionID for the strategyId - InstrumentId pair
//    QMap<uint, QMap<uint, uint> > positions = _positionIdMap[_runId];

//    if(positions != QMap<uint, QMap<uint, uint> > ())
//    {
        QMap<uint, uint> instrumentIdToPositionIdMap = positions[strategyId];

        if(instrumentIdToPositionIdMap != QMap<uint, uint>())
        {
            uint sharesBought = detail.getSharesBought();
            uint sharesSold = detail.getSharesSold();
            float avgBuyPrice = detail.getAvgBought();
            float avgSellPrice = detail.getAvgSold();
            float totalCommission = detail.getTotalCommission();
            QDateTime updatedDate = QDateTime::currentDateTime();
            bool isNewLeg = detail.IsNewLeg();

            if(uint positionId = instrumentIdToPositionIdMap.value(instrumentId,0))
            {
                StrategyLinkedPositionDb strategyLinkedPositionDb;
                uint retVal = strategyLinkedPositionDb.updateStrategyLinkedPosition(_runId, positionId, sharesBought, sharesSold, avgBuyPrice,
                                                         avgSellPrice, totalCommission, updatedDate);


                StrategyLinkedPositionDetailDb strategyLinkedPositionDetailDb;

                //get the latest DetailId
                if(uint positionDetailId = _latestPositionDetailIdMap.value(positionId, 0))
                {

                }
                else
                {
                    _latestPositionDetailIdMap[positionDetailId] = (positionDetailId = 1);
                }

                if(isNewLeg)
                {
                    _latestPositionDetailIdMap[positionDetailId] = ++positionDetailId ;
                    QDateTime createdTime = detail.getCreatedTime();
                    retVal = strategyLinkedPositionDetailDb.insertStrategyLinkedPositionDetail(_runId, positionId, positionDetailId, sharesBought, sharesSold, avgBuyPrice,
                                                                   avgSellPrice, totalCommission, createdTime);
                }
                else
                {
                    strategyLinkedPositionDetailDb.updateStrategyLinkedPositionDetail(_runId, positionId, positionDetailId, sharesBought, sharesSold, avgBuyPrice,
                                                          avgSellPrice, totalCommission, updatedDate);
                }


            }
            else
            {
                QDateTime createdTime = detail.getCreatedTime();

                StrategyLinkedPositionDb strategyLinkedPositionDb;
                uint retVal = strategyLinkedPositionDb.insertStrategyLinkedPosition(_runId, strategyId, instrumentId, sharesBought, sharesSold, avgBuyPrice,
                                                              avgSellPrice, totalCommission, createdTime);

                StrategyLinkedPositionDetailDb strategyLinkedPositionDetailDb;
                retVal = strategyLinkedPositionDetailDb.insertStrategyLinkedPositionDetail(_runId, strategyId, instrumentId, sharesBought, sharesSold, avgBuyPrice,
                                                               avgSellPrice, totalCommission, createdTime);

                positions[strategyId].insert(instrumentId, retVal);
            }
        }
        else
        {
            positions.insert(strategyId, QMap<uint, uint>());
            updateStrategyLinkedPosition(strategyId, instrumentId, detail);
        }
    }
    else
    {
        _positionMap.insert(_runId, QMap<uint, QMap<uint, uint> >());
        updateStrategyLinkedPosition(strategyId, instrumentId, detail);
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

//    Order order = orderDetail.getOrder();
//    double limitPrice = order.lmtPrice;

//    uint quantity = order.totalQuantity;
//    QString action = QString::fromStdString(order.action);
    QString status = getOrderStatusString(orderDetail.getOrderStatus());
//    QDateTime placedDate = orderDetail.getPlacedTime();
    QDateTime updatedDate = orderDetail.getLastUpdatedTime();
//    QString orderType = QString::fromStdString(order.orderType);
    double avgFillPrice = orderDetail.getAvgFillPrice();
    uint filledQuantity = orderDetail.getFilledShares();
    double commission = orderDetail.getCommission();
//    double positionAmount = 0;
//    QDateTime goodTillDate = QDateTime::fromString(QString::fromStdString(order.goodTillDate), Qt::ISODate);

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
