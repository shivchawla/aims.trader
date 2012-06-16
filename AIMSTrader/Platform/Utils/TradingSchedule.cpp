#include "Platform/Utils/TradingSchedule.h"

TradingSchedule::TradingSchedule(const QString& startTime, const QString& endTime, double timeZone)
{
    timeZone = timeZone;
    _start = QDateTime::fromString(startTime,"hh:mm:ss");
    _start.addSecs(-timeZone*60*60);
    _start.setTimeSpec(Qt::UTC);
    _end = QDateTime::fromString(endTime,"hh:mm:ss");
    _end.addSecs(-timeZone*60*60);
    _end.setTimeSpec(Qt::UTC);

    if (_start > _end)
    {
        QString message = "End time must be after the start time in trading schedule.";
    }
}

TradingSchedule::~TradingSchedule()
{}

void TradingSchedule::setExclusion(const QString& startExclusionTime, const QString& endExclusionTime)
{
    _exclusionStart = QDateTime::fromString(startExclusionTime,"hh:mm:ss");
    _exclusionStart.addSecs(-_timeZone*60*60);
    _exclusionStart.setTimeSpec(Qt::UTC);
    _exclusionEnd = QDateTime::fromString(endExclusionTime,"hh:mm:ss");
    _exclusionEnd.addSecs(-_timeZone*60*60);
    _exclusionEnd.setTimeSpec(Qt::UTC);
    bool isValidExclusion = (_exclusionStart > _start) && (_exclusionEnd < _end);

    if (!isValidExclusion)
    {
        QString msg = "Exclusion period must be within trading period in trading schedule.";
    }
    else
    {
         _hasExclusion = true;
    }
}

const QTime& TradingSchedule::getStartTime()
{
    return _start.time();
}

const QTime& TradingSchedule::getEndTime()
{
    return _end.time();
}


