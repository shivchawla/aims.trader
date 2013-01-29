#ifndef SPREADPOSITIONDETAILDB_H
#define SPREADPOSITIONDETAILDB_H

#include "../Shared/DataAccess/DbBase.h"
#include "Data/positiondetaildata.h"

class SpreadPositionDetailDb : public DbBase
{
    public:
        SpreadPositionDetailDb();

    //Methods

    PositionDetailData getStrategyLinkedSpreadPositionDetailById(const uint id);
    QList<PositionDetailData> getStrategyLinkedSpreadPositionDetails(const uint spreadPositionId);
    uint insertStrategyLinkedSpreadPositionDetail(const PositionDetailData data);
    uint insertStrategyLinkedSpreadPositionDetail(const uint runId, const uint spreadpositionId, const uint detailId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint updateStrategyLinkedSpreadPositionDetail(const uint runId, const uint spreadPositionId, const uint detailId, const uint sharesBought,
                                            const uint sharesSold, const double avgBought,
                                            const double avgSold, const double commission, const QDateTime& createdDateTime);

    uint insertStrategyLinkedSpreadPositionDetails(const QList<PositionDetailData>& list);

};

#endif // SPREADPOSITIONDETAILDB_H
