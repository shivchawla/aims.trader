#pragma once
#ifndef ONEMINUTEMOMENTUM_H
#define ONEMINUTEMOMENTUM_H
//#include "Platform/Strategy/Strategy.h"
//#include "Utils/FactoryPattern.h"
#include "Strategy/StrategyImpl.h"

class OneMinuteMomentum: public StrategyImpl<OneMinuteMomentum>
{
    protected:
         int _timeScale;

    public:
        OneMinuteMomentum();
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
