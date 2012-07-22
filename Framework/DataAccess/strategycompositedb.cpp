#include "strategycompositedb.h"
#include "strategylinkedpositiondb.h"
#include "orderdb.h"
#include "strategydb.h"

StrategyCompositeDb::StrategyCompositeDb() // (QObject *parent) :  QObject(parent)
{
}

///////////////////////////////////////////////////////////////////////
// Gets a complete Strategy with its open positions and orders by name
///////////////////////////////////////////////////////////////////////
StrategyCompositeData* StrategyCompositeDb::getStrategyComposite(const QString &strategyName) {
    StrategyCompositeData* str = new StrategyCompositeData;
    StrategyDb strategyDb;
    str->strategy = strategyDb.getStrategyViewByName(strategyName);
    if (str->strategy == NULL) {
        qDebug() << "Unable to load Strategy! Stopping further loading" << endl;
        return NULL;
    }
    StrategyLinkedPositionDb positionsDb;
    str->positions = positionsDb.getPositionsForStrategy(str->strategy->strategyId);

    OrderDb orderDb;
    str->orders = orderDb.getOrdersByStrategyId(str->strategy->strategyId);
    return str;
}

StrategyCompositeDb::~StrategyCompositeDb() // (QObject *parent) :  QObject(parent)
{
}
