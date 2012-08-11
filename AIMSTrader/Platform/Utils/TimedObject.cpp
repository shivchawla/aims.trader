#include "Platform/Utils/TimedObject.h"
#include "Platform/Utils/ThreadManager.h"

#include "Platform/Utils/Timer.h"
TimedObject::TimedObject():QObject()
{
    //_timer = new Timer(this);
    _minuteCounter = 0;
    _snoozeAfter = 0;
    _alarmReached = false;
    //QObject::connect(QThread::currentThread(), SIGNAL(started()), this, SLOT(start()));
}

TimedObject::~TimedObject()
{
    _bTimer.stop();
}

/*void TimedObject::minuteAlarmReceived()
{
    _minuteCounter++;
}*/

void TimedObject::onSnooze()
{}

/*void TimedObject::setAlarm(const int minutes, const int snoozeAfter)
{
    if(!_timer)
    {
        _timer = new Timer(this);
    }
    _timer->setAlarm(minutes, snoozeAfter);
}

void TimedObject::initializeTimer()
{
    if(!_timer)
    {
        _timer = new Timer(this);
    }
}*/

/*void TimedObject::setSnooze(const int minutes)
{
    initializeTimer();
    _timer->setSnooze(minutes);
}

/*void TimedObject::setSnooze(const QDateTime& startTime, const int minutes)
{
    initializeTimer();
    _timer->setAlarmWithSnooze(minutes);
}*/

/*
void TimedObject::setAlarm(const QDateTime& dateTime, const int snoozeAfter)
{
    initializeTimer();
    _timer->setAlarm(dateTime, snoozeAfter);
}

void TimedObject::cancelAlarm()
{
    initializeTimer();
    _timer->cancelAlarm();
}

void TimedObject::cancelSnooze()
{
    initializeTimer();
    _timer->cancelSnooze();
}*/

void TimedObject::setAlarm(const QDateTime& dateTime, const float snoozeAfter)
{
    _snoozeAfter = snoozeAfter;
    QTime currentTime = QTime::currentTime();
    QTime temp(currentTime.hour(),currentTime.minute());

    //set an alarm at every minute interval
    QTime nextTime = temp.addSecs(60);
    int timeout = currentTime.msecsTo(nextTime);
    _bTimer.start(timeout, this);
    //_alarmSet = true;
    //_minuteCount=0;
}

void TimedObject::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == _bTimer.timerId())
    {
        if(!_alarmReached)
        {
            _bTimer.stop();
            if(_snoozeAfter>0)
            {
                _bTimer.start(_snoozeAfter*60*1000, this);
            }
            _alarmReached = true;
        }
        onSnooze();
    }
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

void TimedObject::setSnooze(const float snoozeAfter)
{
    _snoozeAfter = snoozeAfter;
    QTime currentTime = QTime::currentTime();
    QTime temp(currentTime.hour(),currentTime.minute());

    //set an alarm at every minute interval
    QTime nextTime = temp.addSecs(60);
    int timeout = currentTime.msecsTo(nextTime);
    _bTimer.start(timeout, this);
    //_alarmSet = true;
    //_minuteCount=0;
}

void TimedObject::cancelSnooze()
{
    _bTimer.stop();
}

void TimedObject::cancelAlarm()
{
    _bTimer.stop();
}


/*void TimedObject::start()
{
    setSnooze();
}*/


