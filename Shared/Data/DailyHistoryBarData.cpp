#include "Data/DailyHistoryBarData.h"
#include <QDebug>

DailyHistoryBarData::DailyHistoryBarData()
{}

DailyHistoryBarData::~DailyHistoryBarData()
{}

DailyHistoryBarData :: DailyHistoryBarData(DailyHistoryBarData *copy)
{
    dailyHistoryBarId = copy->dailyHistoryBarId;
    historyDate = copy->historyDate;
    open = copy->open;
    close = copy->close;
    high = copy->high;
    low = copy->low;
    volume = copy->volume;
    instrumentId = copy->instrumentId;
}

void DailyHistoryBarData :: printDebug() {
    qDebug() << dailyHistoryBarId << " " << historyDate << " " << open << " " << close << " " << high << " " << low << " " << volume << " " << instrumentId << endl;
}
