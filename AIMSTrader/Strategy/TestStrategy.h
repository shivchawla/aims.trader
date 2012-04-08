#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H

#include "Strategy/OneMinuteMomentum.h"

class TestStrategy: public OneMinuteMomentum
{
    //Q_OBJECT
    public:
        static int addStrategy()
        {
            Strategy::registerStrategy(new TestStrategy());
            return 1;
        }

     public:
        TestStrategy();
        //TestStrategy(const String&);
        ~TestStrategy();
        void startStrategy();
        void initialize();

};

#endif // TESTSTRATEGY_H
