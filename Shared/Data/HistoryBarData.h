#ifndef HISTORYBARDATA_H
#define HISTORYBARDATA_H

#include <QString>
#include <QDateTime>

struct HistoryBarData
{
        QDateTime historyDateTime;
        float open;
        float close;
        float high;
        float low;
        uint volume;
        //uint instrumentId;
        uint frequency;

       HistoryBarData()
       {
           historyDateTime = QDateTime();
           open = 0;
           close = 0;
           high = 0;
           low = 0;
           volume = 0;
           frequency = 0;
           //instrumentId = 0;

       }

       HistoryBarData(const HistoryBarData& copy)
       {
           historyDateTime = copy.historyDateTime;
           open = copy.open;
           close = copy.close;
           high = copy.high;
           low = copy.low;
           volume = copy.volume;
           frequency = copy.frequency;
           //instrumentId = copy->instrumentId;
       }
};

typedef QList<HistoryBarData> QListHistoryBarData;


#endif // HISTORYBARDATA_H
