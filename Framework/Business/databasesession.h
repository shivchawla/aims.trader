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
//change functions
public:
        uint insertStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought, float avgAmountSold,
                                          float totalAmountCommission, QDateTime createdDate, QDateTime updatedDate, uint strategyId,
                                          uint instrumentId);
        uint updateStrategyLinkedPosition(uint numberBought, uint numberSold, float avgAmountBought,
                                          float avgAmountSold, float totalAmountCommission, QDateTime createdDate, QDateTime updatedDate,
                                          uint strategyId, uint instrumentId);

//        void insertStrategyLinkedPosition(const StrategyLinkedPositionData& data);
//        void updateStrategyLinkedPosition(const StrategyLinkedPositionData& data, uint id);
//        void insertOrder(const OrderData& data);
//        void insertOrder(uint orderId, float limitPrice, unsigned int quantity, QChar action, QChar status,
//                            QDateTime placedDate, QDateTime updatedDate, QChar orderType,
//                            float avgFillPrice, unsigned int filledQuantity, float commission,
//                            float positionAmount, uint exchangeId, uint instrumentId,
//                            QDateTime goodTillDate);
//        void updateOrder(const OrderData* data, uint id);

};

#endif // DATABASESESSION_H
