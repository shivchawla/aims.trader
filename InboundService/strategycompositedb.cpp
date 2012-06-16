#include "strategycompositedb.h"
#include "strategyviewdb.h"
#include "strategylinkedpositiondb.h"
#include "orderdb.h"

StrategyCompositeDb::StrategyCompositeDb() // (QObject *parent) :  QObject(parent)
{
}

///////////////////////////////////////////////////////////////////////
// Gets a complete Strategy with its open positions and orders by name
///////////////////////////////////////////////////////////////////////
StrategyCompositeData* StrategyCompositeDb::getStrategyComposite(QString strategyName) {
    StrategyCompositeData* str = new StrategyCompositeData;
    StrategyViewDb viewDb;
    str->strategy = viewDb.getStrategyViewByName(strategyName);
    if (str->strategy == NULL) {
        qDebug() << "Unable to load Strategy! Stopping further loading" << endl;
        return NULL;
    }
    StrategyLinkedPositionDb positionsDb;
    str->positions = positionsDb.getPositionsForStrategy(str->strategy->strategyId);

    OrderDb orderDb;
    str->orders = orderDb.getOrdersForStrategy(str->strategy->strategyId);
    return str;
}

StrategyCompositeDb::~StrategyCompositeDb() // (QObject *parent) :  QObject(parent)
{
}
