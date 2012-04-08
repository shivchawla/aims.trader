#include "Platform/Utils/TradingSchedule.h"

TradingSchedule::TradingSchedule(const QString& startTime, const QString& endTime, double timeZone)
{
    timeZone = timeZone;
    start = QDateTime::fromString(startTime,"hh:mm:ss");
    start.addSecs(-timeZone*60*60);
    start.setTimeSpec(Qt::UTC);
    end = QDateTime::fromString(endTime,"hh:mm:ss");
    end.addSecs(-timeZone*60*60);
    end.setTimeSpec(Qt::UTC);

    if (start > end)
    {
        QString message = "End time must be after the start time in trading schedule.";
    }
}

TradingSchedule::~TradingSchedule()
{}

void TradingSchedule::setExclusion(const QString& startExclusionTime, const QString& endExclusionTime)
{
    exclusionStart = QDateTime::fromString(startExclusionTime,"hh:mm:ss");
    exclusionStart.addSecs(-timeZone*60*60);
    exclusionStart.setTimeSpec(Qt::UTC);
    exclusionEnd = QDateTime::fromString(endExclusionTime,"hh:mm:ss");
    exclusionEnd.addSecs(-timeZone*60*60);
    exclusionEnd.setTimeSpec(Qt::UTC);
    bool isValidExclusion = (exclusionStart > start) && (exclusionEnd < end);

    if (!isValidExclusion)
    {
        QString msg = "Exclusion period must be within trading period in trading schedule.";
    }
    else
    {
         hasExclusion = true;
    }
}

const QTime& TradingSchedule::getStartTime()
{
    return start.time();
}

const QTime& TradingSchedule::getEndTime()
{
    return end.time();
}


