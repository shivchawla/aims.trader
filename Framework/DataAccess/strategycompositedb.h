#ifndef STRATEGYCOMPOSITEDB_H
#define STRATEGYCOMPOSITEDB_H

#include <QObject>
#include <QString>
#include "Data/strategycompositedata.h"
#include "DataAccess/DbBase.h"

class StrategyCompositeDb : public DbBase
{
    public:
        StrategyCompositeDb();
        StrategyCompositeData getStrategyComposite(const QString &strategyName);
};

#endif // STRATEGYCOMPOSITEDB_H
