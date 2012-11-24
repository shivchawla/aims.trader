#ifndef MYGLOBAL_H
#define MYGLOBAL_H
#include <QFile>

inline QFile* logFile()
{
    static QFile*  fPtr = new QFile("log.txt");
    return fPtr;
}

enum DataType
{
    IntradayOneMinuteBar,
    IntradayTwoMinuteBar,
    IntradayFiveMinuteBar,
    IntradayTenMinuteBar,
    IntradayTwentyMinuteBar,
    IntradayThirtyMinuteBar,
    IntradayOneHourBar,
    DailyBar,
    WeeklyBar
};

typedef uint InstrumentId;


#endif // MYGLOBAL_H
