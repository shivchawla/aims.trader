#ifndef INTRADAYHISTORYBARDB_H
#define INTRADAYHISTORYBARDB_H
#include <DataAccess/DbBase.h>
#include <myglobal.h>

struct HistoryBarData;

class IntradayHistoryBarDb : public DbBase
{
    public:
        IntradayHistoryBarDb();
        ~IntradayHistoryBarDb();
        HistoryBarData* getIntradayHistoryBarById(const InstrumentId &instrumentId);
        ulong insertIntradayHistoryBars(const QList<HistoryBarData*>& list, const InstrumentId &instrumentId);

        //unsigned int insertMinuteHistoryBar(const MinuteHistoryBarData* data);
        //unsigned int insertMinuteHistoryBar(const QDateTime& timeStamp, const float open, const float close, const float high, const float low,
        //                               const uint volume, const uint instrumentId);


};

#endif // INTRADAYHISTORYBARDB_H
