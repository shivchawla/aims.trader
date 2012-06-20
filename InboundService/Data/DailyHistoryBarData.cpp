#include "stdafx.h"
#include "DailyHistoryBarData.h"

DailyHistoryBarData::DailyHistoryBarData(void)
{
}


DailyHistoryBarData::~DailyHistoryBarData(void)
{
}

DailyHistoryBarData :: DailyHistoryBarData(DailyHistoryBarData *copy) {
    dailyHistoryBarId = copy->dailyHistoryBarId;
    historyDate = copy->historyDate;
    open = copy->open;
    close = copy->close;
    high = copy->high;
    low = copy->low;
    volume = copy->volume;
    instrumentId = copy->instrumentId;
}
