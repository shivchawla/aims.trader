#ifndef DATABASESESSION_H
#define DATABASESESSION_H

#include <QList>
#include <QString>
#include "Data/strategylinkedpositiondata.h"
#include "Data/strategybuylistdata.h"
//#include "Data/orderdata.h"
#include "Data/strategycompositedata.h"
#include "AimsTraderDefs/typedefs.h"

class StrategyData;

class DatabaseSession
{
private:

public:
        DatabaseSession();
        ~DatabaseSession();
//read functions
public:
        QList<StrategyData*> getStrategies();
        StrategyCompositeData* getCompositeStrategy(const QString& strategyName);
        QList<StrategyLinkedPositionData*> getStrategyLinkedPositions();
        QList<StrategyLinkedPositionData*> getPositionsForStrategy(const QString& strategyName);
        QList<StrategyBuyListData*> getStrategyBuyListForStrategy(const QString& strategyName);
        QList<ATContract*> getATContractsForStrategy(const QString& strategyName);
//change functions
//public:
//        void insertStrategyLinkedPosition(const StrategyLinkedPositionData& data);
//        void updateStrategyLinkedPosition(const StrategyLinkedPositionData& data, QUuid id);
//        void deleteStrategyLinkedPosition(QUuid id);
//        void insertOrder(const OrderData& data);
//        void insertOrder(QUuid orderId, float limitPrice, unsigned int quantity, QChar action, QChar status,
//                            QDateTime placedDate, QDateTime updatedDate, QChar orderType,
//                            float avgFillPrice, unsigned int filledQuantity, float commission,
//                            float positionAmount, QUuid exchangeId, QUuid instrumentId,
//                            QDateTime goodTillDate);
//        void updateOrder(const OrderData* data, QUuid id);
    //write functions
};

#endif // DATABASESESSION_H
