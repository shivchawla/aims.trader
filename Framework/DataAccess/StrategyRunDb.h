#ifndef STRATEGYRUNDB_H
#define STRATEGYRUNDB_H

#include "../Shared/DataAccess/DbBase.h"
#include "../Shared/AimsTraderDefs/typedefs.h"

class StrategyRunDb : public DbBase
{
    public:
        StrategyRunDb();

    public:
        uint generateRunId(const Mode);

};

#endif // STRATEGYRUNDB_H
