#ifndef TRADINGSCHEDULE_H
#define TRADINGSCHEDULE_H
#include <QDateTime>
#include <QString>
#include <string>
#include "Platform/Utils/TimeZone.h"
/**
 * TradingSchedule defines the time period during which a strategy can trade.
 * Trading can start after the "startTime". Open positions will be closed
 * at the "endTime". The "startTime" and "endTime" times must be specified
 * in the military time format.
 * <p/>
 * Example: A strategy defines the following trading schedule:
 * tradingSchedule = new TradingSchedule("9:35", "15:45", EST);
 * Then the following trading time line is formed:
 * -- start trading at 9:35 EST
 * -- close open positions at 15:45 EST
 * <p/>
 * A particular period of time within the time window between the "startTime" and "endTime"
 * can be excluded.
 * Example: A strategy defines the following trading interval, along with the excluded period:
 * tradingSchedule = new TradingSchedule("9:35", "15:45", EST);
 * tradingSchedule.setExclusion("12:00", "13:00");
 * Then the following trading time line is formed:
 * -- start trading at 9:35 EST
 * -- close open positions at 12:00 EST
 * -- resume trading at 13:00 EST
 * -- close open positions at 15:45 EST
 */

class TradingSchedule
{
    private:
        QDateTime start, end;
        double timeZone;
        QDateTime exclusionStart, exclusionEnd;
        bool hasExclusion;

    public:
        TradingSchedule();
        TradingSchedule(const QString& startTime, const QString& endTime, double timeZone);
        ~TradingSchedule();

    public:
      void setExclusion(const QString& startExclusionTime, const QString& endExclusionTime);
};


#endif // TRADINGSCHEDULE_H
