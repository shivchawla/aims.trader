#ifndef STRATEGYSPREADBUYLISTDB_H
#define STRATEGYSPREADBUYLISTDB_H

#include "DataAccess/DbBase.h"
#include "Data/InstrumentData.h"

class StrategySpreadBuyListDb : public DbBase
{
    enum{
        DbSpreadId,
        InstrumentId1,
        InstrumentId2
    };

    public:
        StrategySpreadBuyListDb();

    public:
        QList<SpreadData> getStrategySpreadBuyList(const DbStrategyId strategyId);
};

#endif // STRATEGYSPREADBUYLISTDB_H
