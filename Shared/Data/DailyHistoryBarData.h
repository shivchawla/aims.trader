#ifndef DAILYHISTORYBARDATA_H
#define DAILYHISTORYBARDATA_H

//#include <QObject>
#include <QString>
#include <QUuid>
#include <QDateTime>

class DailyHistoryBarData
{
    public:
        QUuid dailyHistoryBarId;
        QDateTime historyDate;
        float open;
        float close;
        float high;
        float low;
        qint32 volume;
        QUuid instrumentId;

       DailyHistoryBarData();
       ~DailyHistoryBarData();
        DailyHistoryBarData(DailyHistoryBarData *copy);
};

#endif //DAILYHISTORYBARDATA_H
