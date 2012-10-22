#ifndef DATABASESESSION_H
#define DATABASESESSION_H

#include <QList>
#include <QString>
#include <QMap>
#include <QPair>
#include "Data/strategylinkedpositiondata2.h"
#include "Data/strategybuylistdata.h"
//#include "Data/orderdata.h"
#include "Data/strategycompositedata.h"
#include "AimsTraderDefs/typedefs.h"
#include "Data/strategyconfigurationdata.h"

class StrategyData;
class InstrumentData;

class DatabaseSession
{
    private:
        //QMap<StrategyID, QMap<InstrumentID, PositionId> > >
        QMap<uint, QMap<uint, uint> >  _positionIdMap;
        QMap<uint, uint> _latestPositionDetailIdMap;
        uint _runId;

    public:
            DatabaseSession();
            ~DatabaseSession();
    //read functions
    public:
            QList<StrategyData*> getStrategies();
            StrategyCompositeData* getCompositeStrategy(const QString& strategyName);
            QList<StrategyLinkedPositionData2*> getStrategyLinkedPositions();
            QList<StrategyLinkedPositionData2*> getPositionsForStrategy(const QString& strategyName);
            //QList<StrategyBuyListData*> getStrategyBuyListForStrategy(const QString& strategyName);
            QList<ATContract*> getATContractsForStrategy(const QString& strategyName);
            QList<InstrumentData*> getStrategyBuyList(const StrategyId strategyId);
            QList<StrategyLinkedPositionData2*> getOpenStrategyLinkedPositions(const uint strategyId);
            QList<OrderData*> getOrdersByStrategyName(const QString& strategyName);
            StrategyConfigurationData* getStrategyConfiguration(uint strategyId, QString confKey);
            QHash<QString, QString> getStrategyConfigurations(uint strategyId);
            QList<InstrumentData*> getInstrumentsWithSimilarSymbol(const QString&);

    //change functions

        public:
            uint updateStrategyLinkedPosition(const uint strategyId, const uint instrumentId, const PositionDetail& );
            uint updateOrder(const uint orderId, const uint strategyId, const uint instrumentId, const OrderDetail&);
            uint insertOrder(const uint orderId, const uint strategyId, const uint instrumentId, const OrderDetail&);
            uint insertStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue);
            uint updateStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue);
};

#endif // DATABASESESSION_H
