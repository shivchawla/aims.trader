#pragma once
#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H

#include "Strategy/OneMinuteMomentum.h"

class TestStrategy: public OneMinuteMomentum
{
     public:
        //TestStrategy();
        TestStrategy(const String&);
        ~TestStrategy();
        void startStrategy();
        void initialize();

};

#endif // TESTSTRATEGY_H
