#ifndef DATABASESESSION_H
#define DATABASESESSION_H

#include <QList>
#include <QString>
#include <QMap>
#include <QPair>
#include "Data/strategylinkedpositiondata.h"
#include "Data/strategybuylistdata.h"
//#include "Data/orderdata.h"
#include "Data/strategycompositedata.h"
#include "AimsTraderDefs/typedefs.h"

class StrategyData;
class InstrumentData;

class DatabaseSession
{
private:
    QMap<QPair<uint, uint>, uint> _positionsMap;

public:
        DatabaseSession();
        ~DatabaseSession();
//read functions
public:
        QList<StrategyData*> getStrategies();
        StrategyCompositeData* getCompositeStrategy(const QString& strategyName);
        QList<StrategyLinkedPositionData*> getStrategyLinkedPositions();
        QList<StrategyLinkedPositionData*> getPositionsForStrategy(const QString& strategyName);
        //QList<StrategyBuyListData*> getStrategyBuyListForStrategy(const QString& strategyName);
        QList<ATContract*> getATContractsForStrategy(const QString& strategyName);
        QList<InstrumentData*> getStrategyBuyList(const QString &strategyName);
        QList<StrategyLinkedPositionData*> getOpenStrategyLinkedPositions(const uint strategyId);
        QList<OrderData*> getOrdersByStrategyName(const QString& strategyName);
//change functions
public:
        uint insertStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought, float avgAmountSold,
                                          float totalAmountCommission, QDateTime createdDate, QDateTime updatedDate, uint strategyId,
                                          uint instrumentId);
        uint updateStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought,
                                          float avgAmountSold, float totalAmountCommission, QDateTime createdDate, QDateTime updatedDate,
                                          uint strategyId, uint instrumentId);

        //this method will provide the new inserted primary key and not the count of rows inserted
        uint insertOrder(float limitPrice, uint quantity, quint8 action, quint8 status,
                            QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                            float avgFillPrice, uint filledQuantity, float commission,
                            float positionAmount, uint instrumentId,
                            QDateTime goodTillDate, uint originalOrderId);
        uint updateOrder(float limitPrice, uint quantity, quint8 action, quint8 status,
                         QDateTime placedDate, QDateTime updatedDate, quint8 orderType,
                         float avgFillPrice, uint filledQuantity, float commission,
                         float positionAmount, uint instrumentId,
                         QDateTime goodTillDate, uint originalOrderId);
};

#endif // DATABASESESSION_H
