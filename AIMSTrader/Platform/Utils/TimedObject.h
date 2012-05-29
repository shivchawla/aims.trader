#ifndef TIMEDOBJECT_H
#define TIMEDOBJECT_H

#include <QObject>
#include <QDateTime>
#include <QBasicTimer>

class TimedObject : public QObject
{
    //Q_OBJECT
    private:
        //Timer* _timer;
        //QThread* _thisThread;
          QBasicTimer _bTimer;

    protected:
        float _snoozeAfter;
        bool _alarmReached;
        int _minuteCounter;
        //void setAlarm(const int);
        void setSnooze(const float minutes = 1.0);
        void setSnooze(const QDateTime&, const int);
        void setAlarm(const QDateTime&, const float snoozeAfter = 1);
        void cancelAlarm();
        void cancelSnooze();

    public:
        TimedObject();
        virtual ~TimedObject();

    public:
        //virtual void onAlarm();
        virtual void onSnooze();
        void timerEvent(QTimerEvent* event);

    /*public slots:
        void start();*/
};

#endif // TIMEDOBJECT_H
