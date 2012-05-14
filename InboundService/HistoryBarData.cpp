#include "stdafx.h"
#include "HistoryBarData.h"


HistoryBarData::HistoryBarData(void)
{
}


HistoryBarData::~HistoryBarData(void)
{
}

HistoryBarData :: HistoryBarData(HistoryBarData *copy) {
    historyBarId = copy->historyBarId;
    historyDate = copy->historyDate;
    open = copy->open;
    close = copy->close;
    high = copy->high;
    low = copy->low;
    volume = copy->volume;
    instrumentId = copy->instrumentId;
}
