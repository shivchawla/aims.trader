#ifndef INTRADAYHISTORYWRITETASK_H
#define INTRADAYHISTORYWRITETASK_H

#include "DataAccess/IntradayHistoryBarDb.h"
#include "Data/DbWriteTask.h"
#include "DataAccess/DailyHistoryBarDb.h"


class DailyHistoryWriteTask : public DbWriteTask<DailyHistoryBarDb, HistoryBarData>
{
    uint _id;

    public:
        DailyHistoryWriteTask(const uint, QList<HistoryBarData*>&);
        void run();
};

class IntradayHistoryWriteTask : public DbWriteTask<IntradayHistoryBarDb, HistoryBarData>
{
    uint _id;
    DataType _type;

    public:
        IntradayHistoryWriteTask(const uint, QList<HistoryBarData*>&, const DataType);
        void run();
};

//template <DataType type>
IntradayHistoryWriteTask :: IntradayHistoryWriteTask(const uint id, QList<HistoryBarData*>& data, const DataType type)
                                                :DbWriteTask<IntradayHistoryBarDb, HistoryBarData> (data),
                                                 _id(id),
                                                 _type(type)
{}

//template <DataType type>
void IntradayHistoryWriteTask :: run()
{
    uint frequency = 0 ;
    switch(_type)
    {
        case IntradayOneMinuteBar: frequency = 1; break;
        case IntradayTwoMinuteBar: frequency = 2; break;
        case IntradayFiveMinuteBar: frequency = 5; break;
        case IntradayTenMinuteBar: frequency = 10; break;
        case IntradayTwentyMinuteBar: frequency = 20;  break;
        case IntradayThirtyMinuteBar: frequency = 30; break;
        case IntradayOneHourBar: frequency = 60; break;
    }

    IntradayHistoryBarDb db;
    int n = db.write(_id, _data, frequency);
    Logger::log() << QDateTime::currentDateTime() << n << " history records written to "<< db.getName()<<" for instrument id" << _id << endl;
}

DailyHistoryWriteTask :: DailyHistoryWriteTask(const uint id, QList<HistoryBarData*>& data)
                                                :DbWriteTask<DailyHistoryBarDb, HistoryBarData> (data),
                                                  _id(id)
{}

//template <DataType type>
void DailyHistoryWriteTask :: run()
{
    DailyHistoryBarDb db;
    int n = db.write(_id, _data, 0);
    Logger::log() << QDateTime::currentDateTime() << n << " history records written to "<< db.getName()<<" for instrument id" << _id << endl;
}


#endif // INTRADAYHISTORYWRITETASK_H
