#ifndef MINUTEHISTORYBARDB_H
#define MINUTEHISTORYBARDB_H
#include <DataAccess/DbBase.h>
#include <myglobal.h>

struct HistoryBarData;

class MinuteHistoryBarDb : public DbBase
{
    public:
        MinuteHistoryBarDb();
        ~MinuteHistoryBarDb();
        HistoryBarData* getDailyHistoryBarById(const InstrumentId instrumentId);
        //unsigned int insertMinuteHistoryBar(const MinuteHistoryBarData* data);
        //unsigned int insertMinuteHistoryBar(const QDateTime& timeStamp, const float open, const float close, const float high, const float low,
        //                               const uint volume, const uint instrumentId);
        int insertMinuteHistoryBars(const QList<HistoryBarData*>& list, const InstrumentId instrumentId);

};

#endif // MINUTEHISTORYBARDB_H
