#ifndef STRATEGYLINKEDPOSITIONDB_H
#define STRATEGYLINKEDPOSITIONDB_H

#include "../Shared/DataAccess/DbBase.h"

class StrategyLinkedPositionDb : public DbBase
{
    private:
        enum StrategyLinkedPosition
        {
            RunId,
            PositionId,
            InstrumentId,
            StrategyId
        };

    public:
        StrategyLinkedPositionDb();

    public:
        QHash<uint, QHash<uint, uint> > getStrategyLinkedPositions(const uint runId);

        void insertRow(const uint runId, const uint strategyId, const uint instrumentId, const uint positionId);
};

#endif // STRATEGYLINKEDPOSITIONDB_H
