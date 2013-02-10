#ifndef DATABASESESSION_H
#define DATABASESESSION_H

#include <QList>
#include <QString>
#include <QMap>
#include <QPair>
#include "Data/positiondata.h"
#include "Data/strategybuylistdata.h"
#include "Data/strategycompositedata.h"
#include "Data/InstrumentData.h"
#include "AimsTraderDefs/typedefs.h"
#include "Data/strategyconfigurationdata.h"
#include "myglobal.h"


class StrategyData;
class InstrumentData;



class DatabaseSession
{
    private:
        QMap<DbStrategyId, QMap<InstrumentId, uint> >  _positionIdMap;
        QMap<DbStrategyId, QMap<DbSpreadId, QMap<InstrumentId, uint> > >  _spreadPositionIdMap;
        QMap<DbStrategyId, QMap<DbSpreadId, uint> > _spreadIdMap;
        QMap<uint, uint> _latestPositionDetailIdMap;
        QMap<uint, uint> _latestSpreadPositionDetailIdMap;
        uint _runId;
        uint _positionId;
        uint _spreadPositionId;
        Mode _mode;

    public:
            DatabaseSession();
            ~DatabaseSession();
    //read functions
    public:
            QList<StrategyData> getStrategies();
            StrategyCompositeData getCompositeStrategy(const QString& strategyName);
            QList<PositionData> getStrategyLinkedPositions();
            QList<PositionData> getPositionsForStrategy(const QString& strategyName);
            //QList<StrategyBuyListData*> getStrategyBuyListForStrategy(const QString& strategyName);
            QList<ATContract> getATContractsForStrategy(const QString& strategyName);
            QList<InstrumentData> getStrategyBuyList(const StrategyId strategyId);
            QList<PositionData> getOpenStrategyLinkedPositions(const uint strategyId);
            QList<OrderData> getOrdersByStrategyName(const QString& strategyName);
            StrategyConfigurationData getStrategyConfiguration(uint strategyId, QString confKey);
            QHash<QString, QString> getStrategyConfigurations(uint strategyId);
            QList<InstrumentData> getInstrumentsWithSimilarSymbol(const QString&);
            uint getTradeRunId(const Mode);
            void setupDatabaseSession(const Mode);
            QList<InstrumentData> getInstrumentData(const QList<InstrumentId>&);
            InstrumentData getInstrumentData(const InstrumentId);

            QList<SpreadData> getStrategySpreadList(const DbStrategyId strategyId);

       //change functions
        public:
            uint updateStrategyLinkedPosition(const uint strategyId, const uint instrumentId, const PositionDetail& );
            uint updateOrder(const uint orderId, const OrderDetail&);
            uint insertOrder(const uint orderId, const uint strategyId, const uint instrumentId, const OrderDetail&);
            uint insertStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue);
            uint updateStrategyConfiguration(const uint &strategyId, const QString &confKey, const QString &confValue);
            uint updateStrategyLinkedSpreadPosition(const uint strategyId, const uint spreadId, const uint instrumentId, const PositionDetail&);
            uint updateStrategyLinkedSpread(const DbStrategyId strategyId, const DbSpreadId spreadId, const SpreadDetail&);
};

#endif // DATABASESESSION_H
