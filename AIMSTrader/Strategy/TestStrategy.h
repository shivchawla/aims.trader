#ifndef TESTSTRATEGY_H
#define TESTSTRATEGY_H
#include "Platform/Strategy/Strategy.h"

class TestStrategy: public Strategy
{
    Q_OBJECT
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
};



#endif // TESTSTRATEGY_H
