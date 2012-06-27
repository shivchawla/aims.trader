#pragma once
#ifndef ONEMINUTEMOMENTUM_H
#define ONEMINUTEMOMENTUM_H
#include "Platform/Strategy/Strategy.h"

class OneMinuteMomentum: public Strategy
{
    protected:
         int _timeScale;

    public:
        OneMinuteMomentum(const String&);
        ~OneMinuteMomentum();

    public:
        const int getTimeScale() { return _timeScale;}

    private:
        void setupIndicator();

    public:
        void initialize();

     public slots:
        void startStrategy();



};

#endif // ONEMINUTEMOMENTUM_H
