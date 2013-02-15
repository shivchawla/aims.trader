#ifndef SPREADDB_H
#define SPREADDB_H

#include "Data/InstrumentData.h"
#include "DataAccess/DbBase.h"

class SpreadDb : public DbBase
{
    private:
        enum Spread
        {
            DbSpreadId,
            InstrumentId1,
            InstrumentId2
        };

    public:
        SpreadDb();

        //METHODS
        SpreadData getSpreadData(const uint spreadId);
};
#endif // SPREADDB_H
