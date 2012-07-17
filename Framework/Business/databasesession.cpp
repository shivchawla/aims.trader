#include "databasesession.h"
#include "DataAccess/strategylinkedpositiondb.h"
#include "DataAccess/strategycompositedb.h"
#include "DataAccess/strategybuylistdb.h"
#include "DataAccess/strategydb.h"
#include "../Shared/DataAccess/InstrumentDb.h"

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

QList<StrategyBuyListData*> DatabaseSession::getStrategyBuyListForStrategy(const QString& strategyName) {
    StrategyBuyListDb db;
    return db.getStrategyBuyListsForStrategy(strategyName);
}

QList<ATContract*> DatabaseSession::getATContractsForStrategy(const QString& strategyName) {
    StrategyBuyListDb db;
    return db.getATContractsForStrategy(strategyName);
}

QList<InstrumentData*> DatabaseSession :: getInstrumentsFromStrategyBuyList(const QString &strategyName) {
    InstrumentDb db;
    return db.getInstrumentsFromStrategyBuyList(strategyName);
}
