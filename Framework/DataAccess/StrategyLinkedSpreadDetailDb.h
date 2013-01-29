#ifndef STRATEGYLINKEDSPREADDETAILDB_H
#define STRATEGYLINKEDSPREADDETAILDB_H

#include "DataAccess/DbBase.h"

class StrategyLinkedSpreadDetailDb : public DbBase
{
    enum {
        RunId,
        StrategyId,
        SpreadId,
        SpreadDetailId
    };

    public:
        StrategyLinkedSpreadDetailDb();

    public:
        uint insertSpreadDetail(const uint runId, const uint spreadDetailId, const double vBought, const double vSold, const double commission, const QDateTime& createdTime);
        uint updateSpreadDetail(const uint runId, const uint spreadDetailId, const double vBought, const double vSold, const double commission, const QDateTime& lastUpdated);
};

#endif // STRATEGYLINKEDSPREADDETAILDB_H
