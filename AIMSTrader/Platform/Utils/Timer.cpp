#include "Platform/Utils/Timer.h"
#include "Platform/Startup/Service.h"
#include "platform/Trader/InstrumentManager.h"
#include <QTime>
#include "Platform/Utils/TimedObject.h"

Timer::Timer(TimedObject* object):_timedObject(object)
{
    _alarmReached = false;
    _minuteCount = 0;
    _snoozeAfter=0;

    //QObject::connect(thread,SIGNAL(started()),this,SLOT(setMinuteAlarm()));
}

Timer::~Timer()
{}

void Timer::setAlarm(const QDateTime& dateTime, const int snoozeAfter)
{
    _snoozeAfter = 1;
    QTime currentTime = QTime::currentTime();
    QTime temp(currentTime.hour(),currentTime.minute());

    //set an alarm at every minute interval
    QTime nextTime = temp.addSecs(60);
    int timeout = currentTime.msecsTo(nextTime);
    //_bTimer.start(timeout, this);
    //_alarmSet = true;
    //_minuteCount=0;
}

void Timer::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == _bTimer.timerId())
    {
        if(!_alarmReached)
        {
           // _bTimer.stop();
            if(_snoozeAfter)
            {
               // _bTimer.start(_snoozeAfter*60*1000, this);
            }
            _alarmReached = true;
        }
        onSnooze();
    }
}

void Timer::onSnooze()
{
     _timedObject->onSnooze();
    //emit minuteAlarmSignal();
}

/*void Timer::subscribeToMinuteAlarm(TimedObject* object)
{
    QObject::connect(this, SIGNAL(minuteAlarmSignal()), object, SLOT(minuteAlarmReceived()));
}*/

/*Timer* Timer::getTimer()
{
    if(_timer==NULL)
    {
        _timer = new Timer();
    }
    return _timer;
}

void Timer::startTimer()
{
    if(_timer==NULL)
    {
        _timer = new Timer();
    }
}*/

void Timer::setSnooze(const int snoozeAfter)
{
    _snoozeAfter = snoozeAfter;
    QTime currentTime = QTime::currentTime();
    QTime temp(currentTime.hour(),currentTime.minute());

    //set an alarm at every minute interval
    QTime nextTime = temp.addSecs(60);
    int timeout = currentTime.msecsTo(nextTime);
    //_bTimer.start(timeout, this);
    //_alarmSet = true;
    //_minuteCount=0;
}

void Timer::cancelSnooze()
{
   // _bTimer.stop();
}

void Timer::canceAlarm()
{
    //_bTimer.stop();
}

