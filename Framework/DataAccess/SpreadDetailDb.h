#ifndef SPREADDETAILDB_H
#define SPREADDETAILDB_H

#include "DataAccess/DbBase.h"

class SpreadDetailDb : public DbBase
{
    enum {
        RunId,
        SpreadDetailId,
        ValueBought,
        ValueSold,
        Commission,
        CreatedDate,
        UpdatedDate
    };

    public:
        SpreadDetailDb();

    public:
        uint insertSpreadDetail(const uint runId, const uint spreadDetailId, const double vBought, const double vSold, const double commission, const QDateTime& createdTime);
        uint updateSpreadDetail(const uint runId, const uint spreadDetailId, const double vBought, const double vSold, const double commission, const QDateTime& lastUpdated);
};

#endif // SPREADDETAILDB_H
