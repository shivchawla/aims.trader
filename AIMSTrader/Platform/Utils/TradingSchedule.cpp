#include "Platform/Utils/TradingSchedule.h"

TradingSchedule::TradingSchedule()
{}

TradingSchedule::TradingSchedule(const QString& startTime, const QString& endTime, double timeZone)
{
    timeZone = timeZone;
    _start = QTime::fromString(startTime,"hh:mm:ss");
    _start.addSecs(-timeZone*60*60);
    //_start.setTimeSpec(Qt::UTC);
    _end = QTime::fromString(endTime,"hh:mm:ss");
    _end.addSecs(-timeZone*60*60);
    //_end.setTimeSpec(Qt::UTC);

    if (_start > _end)
    {
        QString message = "End time must be after the start time in trading schedule.";
    }

    setExclusion();
}

TradingSchedule::~TradingSchedule()
{}

void TradingSchedule::setExclusion(const QString& startExclusionTime, const QString& endExclusionTime, const TradeType exclusionType)
{
    ExclusionTime time;
    time._startExclusionTime = QTime::fromString(startExclusionTime,"hh:mm:ss");
    //_exclusionStart.addSecs(-_timeZone*60*60);
    //_exclusionStart.setTimeSpec(Qt::UTC);
    time._endExclusionTime = QTime::fromString(endExclusionTime,"hh:mm:ss");
    time._tradeType = exclusionType;
    //_exclusionEnd.addSecs(-_timeZone*60*60);
    //_exclusionEnd.setTimeSpec(Qt::UTC);
    bool isValidExclusion = (time._startExclusionTime > _start) && (time._endExclusionTime < _end);

    if (!isValidExclusion)
    {
        QString msg = "Exclusion period must be within trading period in trading schedule.";
    }
    else
    {
         _hasExclusion = true;
    }
}

void TradingSchedule::setExclusion(const int toStartInMinutes, const int fromEndInMinutes, const TradeType exclusionType)
{
    ExclusionTime exclusionTime;
    exclusionTime._endExclusionTime = _start.addSecs(toStartInMinutes * 60);
    exclusionTime._startExclusionTime = _start;
    exclusionTime._tradeType = exclusionType;

    bool isValidExclusion = (exclusionTime._startExclusionTime > _start) && (exclusionTime._endExclusionTime < _end);

    if (!isValidExclusion)
    {
        QString msg = "Exclusion period must be within trading period in trading schedule.";
    }
    else
    {
         _hasExclusion = true;
         _exclusionTimeList.append(exclusionTime);
    }

    exclusionTime._startExclusionTime = _start.addSecs(-fromEndInMinutes * 60);
    exclusionTime._endExclusionTime = _end;
    exclusionTime._tradeType = exclusionType;

    isValidExclusion = (exclusionTime._startExclusionTime > _start) && (exclusionTime._endExclusionTime < _end);

    if (!isValidExclusion)
    {
        QString msg = "Exclusion period must be within trading period in trading schedule.";
    }
    else
    {
         _hasExclusion = true;
         _exclusionTimeList.append(exclusionTime);
    }
}

const QTime TradingSchedule::getStartTime()
{
    //QTime t = _start.time();
    return _start;
}

const QTime TradingSchedule::getEndTime()
{
    //QTime t = _end.time();
    return _end;
}

void TradingSchedule::setStartTime(const QTime& startTime)
{
    _start = startTime;
}

void TradingSchedule::setEndTime(const QTime& endTime)
{
    _end = endTime;
}

void TradingSchedule::setTimezone(const QString& timezone)
{
    //_timeZone = getTimezone(timeZone);
}

bool TradingSchedule::IsValid(const QTime& time, const TradeType tradeType)
{
    if(time < _start || time > _end)
    {
        return false;
    }

    int length;

    if(length = _exclusionTimeList.length())
    {
        for(int k = 0 ; k<length; ++k)
        {
            if(time > _exclusionTimeList[k]._startExclusionTime && time < _exclusionTimeList[k]._endExclusionTime)
            {
                return !(_exclusionTimeList[k]._tradeType == ALLPOS);

                if((_exclusionTimeList[k]._tradeType == OPENEXTEND) && (tradeType == OPENEXTEND || tradeType == OPENPOS || tradeType == EXTEND))
                     return false;
                else if((_exclusionTimeList[k]._tradeType == CLOSEREDUCE) && (tradeType == CLOSEREDUCE || tradeType == CLOSEPOS || tradeType == REDUCE))
                    return false;

                return !(_exclusionTimeList[k]._tradeType == tradeType);
            }
        }
    }

    return true;
}



