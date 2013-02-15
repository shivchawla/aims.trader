#ifndef STRATEGYLINKEDSPREADDB_H
#define STRATEGYLINKEDSPREADDB_H

#include "DataAccess/DbBase.h"

class StrategyLinkedSpreadDb : public DbBase
{
    enum StrategLinkedSpread
    {
        RunId,
        SpreadDetailId,
        StrategyId,
        SpreadId
    };

    public:
        StrategyLinkedSpreadDb();

    public:
        void insertStrategyLinkedSpread(const uint runId, const uint strategyId, const uint spreadId, const int positionDetailId);
};

#endif // STRATEGYLINKEDSPREADDB_H
