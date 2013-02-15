#ifndef SPREADPOSITIONDB_H
#define SPREADPOSITIONDB_H

#include "../Shared/DataAccess/DbBase.h"
#include "Data/positiondata.h"

class SpreadPositionDb : public DbBase
{
    private:
        enum SpreadPosition {
            RunId,
            SpreadPositionId,
            SharesBought,
            SharesSold,
            AvgBuyPrice,
            AvgSellPrice,
            Commission,
            CreatedDate,
            UpdatedDate
        };

    public:
        SpreadPositionDb();

    //Methods

    PositionData getStrategyLinkedSpreadPositionById(const uint id);
    QList<PositionData> getStrategyLinkedSpreadPositions();
    QList<PositionData> getOpenStrategyLinkedSpreadPositions(uint strategyId);
    QList<PositionData> getSpreadPositionsForStrategy(const uint strategyId);
    QList<SpreadPositionData> getOpenStrategyLinkedSpreadPositions(const uint runId, const QString& strategyName);

    //returns the newly inserted primary key
    uint insertStrategyLinkedSpreadPosition(const uint runId, const uint spreadPositionId, const uint sharesBought, const uint sharesSold,
                                      const double avgBuyPrice, const double avgSellPrice,
                                      const double totalCommission, const QDateTime& createdDate) ;

    //uint insertStrategyLinkedPosition(const StrategyLinkedPositionData* &data);

    uint updateStrategyLinkedSpreadPosition(const uint runId, const uint spreadPositionId,
                                      const uint sharesBought, const uint sharesSold, const double avgBuyPrice, const double avgSellPrice,
                                      const double totalCommission, const QDateTime& updatedDate);

    //uint updateStrategyLinkedPosition(const StrategyLinkedPositionData* &data);
    uint deleteStrategyLinkedSpreadPosition(const uint spreadPositionId);

};

#endif // SPREADPOSITIONDB_H
