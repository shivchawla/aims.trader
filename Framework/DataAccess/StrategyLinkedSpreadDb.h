#ifndef STRATEGYLINKEDSPREADDB_H
#define STRATEGYLINKEDSPREADDB_H

#include "DataAccess/DbBase.h"

class StrategyLinkedSpreadDb : public DbBase
{
    enum{
        RunId,
        StrategyId,
        SpreadId,
        SpreadDetailId
    };

    public:
        StrategyLinkedSpreadDb();

    public:
        void insertStrategyLinkedSpread(const uint runId, const uint strategyId, const uint spreadId, const int positionDetailId);
};

#endif // STRATEGYLINKEDSPREADDB_H
