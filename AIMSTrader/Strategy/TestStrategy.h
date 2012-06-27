#pragma once
#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H

#include "Strategy/OneMinuteMomentum.h"

class TestStrategy: public OneMinuteMomentum
{
     public:
        TestStrategy(const String&);
        ~TestStrategy();

     private:
        void initialize();

     public slots:
        void startStrategy();

};

#endif // TESTSTRATEGY_H
