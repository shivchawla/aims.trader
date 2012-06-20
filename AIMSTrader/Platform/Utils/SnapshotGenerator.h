#pragma once
#ifndef SNAPSHOTGENERATOR_H
#define SNAPSHOTGENERATOR_H

#include "Platform/Utils/TimedObject.h"

class SnapshotGenerator : public TimedObject
{
    Q_OBJECT
    public:
        SnapshotGenerator();
        ~SnapshotGenerator();

     public:
        //virtual void onAlarm();
        virtual void onSnooze();

    //public slots:
      //  void minuteAlarmReceived();

    private:
        void generateSnapshot(const int);

    public slots:
        void start();

};

#endif // SNAPSHOTGENERATOR_H
