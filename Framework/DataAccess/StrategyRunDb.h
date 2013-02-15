#ifndef STRATEGYRUNDB_H
#define STRATEGYRUNDB_H

#include "../Shared/DataAccess/DbBase.h"
#include "../Shared/AimsTraderDefs/typedefs.h"

class StrategyRunDb : public DbBase
{
    enum StrategyRun
    {
        RunId,
        StrategyId,
        StrategyName,
        NewPositionFlag,
        RunType,
        StartTime,
        EndTime,
        CreatedBy
    };

    public:
        StrategyRunDb();

    public:
        uint generateRunId();
        uint getLastRunId(const Mode);
        uint insertStrategy(const uint runId, const Mode, const DbStrategyId strategyId, const QString&, const bool newPositionFlag);
};

#endif // STRATEGYRUNDB_H
