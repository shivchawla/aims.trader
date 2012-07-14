#ifndef DAILYHISTORYBARDATA_H
#define DAILYHISTORYBARDATA_H

//#include <QObject>
#include <QString>
#include <QDateTime>

class DailyHistoryBarData
{
    public:
        uint dailyHistoryBarId;
        QDateTime historyDate;
        float open;
        float close;
        float high;
        float low;
        uint volume;
        uint instrumentId;

       DailyHistoryBarData();
       ~DailyHistoryBarData();
       DailyHistoryBarData(DailyHistoryBarData *copy);

       void printDebug();
};

#endif //DAILYHISTORYBARDATA_H
