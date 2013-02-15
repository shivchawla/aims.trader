#ifndef STRATEGYLINKEDSPREADPOSITIONDB_H
#define STRATEGYLINKEDSPREADPOSITIONDB_H

#include "../Shared/DataAccess/DbBase.h"

class StrategyLinkedSpreadPositionDb : public DbBase
{
    private:
        enum StrategyLinkedSpreadPosition
        {
            RunId,
            StrategyId,
            SpreadId,
            InstrumentId,
            SpreadPositionId
        };


    public:
        StrategyLinkedSpreadPositionDb();

    public:
        QHash<uint, QHash<uint, QHash<uint, uint> > > getStrategyLinkedSpreadPositions(const uint runId);

        void insertRow(const uint runId, const uint spreadId, const uint strategyId, const uint instrumentId, const uint spreadPositionId);
};

#endif // STRATEGYLINKEDSPREADPOSITIONDB_H
