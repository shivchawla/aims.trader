#ifndef TIMER_H
#define TIMER_H

#include <QBasicTimer>
#include <QTimerEvent>
#include <QObject>
#include <QDateTime>

class TimedObject;
class Timer //: public QObject
{
   // Q_OBJECT
    private:
        //static Timer* _timer;

    private:
        TimedObject* _timedObject;
        QBasicTimer _bTimer;
        int _minuteCount;
        bool _alarmReached;
        int _snoozeAfter;

    public:
        Timer();
        Timer(TimedObject*);
        ~Timer();

    public:
        void setAlarm(const QDateTime&, const int snoozeTime = 1);
        void setSnooze(const int minutes = 1);
        void cancelSnooze();
        void canceAlarm();

    public:
        void timerEvent(QTimerEvent*);
        //void static startTimer();
        //static Timer* getTimer();
        //void subscribeToMinuteAlarm(TimedObject*);

    private:
        void onAlarm();
        void onSnooze();

    //signals:
      //  void minuteAlarmSignal();
};

#endif // TIMER_H
