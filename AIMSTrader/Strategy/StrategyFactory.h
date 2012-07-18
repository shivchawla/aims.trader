#ifndef STRATEGYFACTORY_H
#define STRATEGYFACTORY_H
#include <Platform/Strategy/Strategy.h>
#include <Utils/FactoryPattern.h>
#include "Strategy/FractionalMomentum.h"
#include "Strategy/OneMinuteMomentum.h"

void initializeStrategy();

class StrategyFactoryMapImpl: public BaseFactoryMapImpl<QString, Strategy>
{
    public:
        StrategyFactoryMapImpl():BaseFactoryMapImpl<QString, Strategy>()
        {
            initializeStrategy();
        }
};

BaseFactoryMapImpl<QString, Strategy>& StrategyFactoryMap()
{
    return BaseFactoryMap<QString,Strategy>();
}

template <class Impl>
class StrategyFactoryRegistrar: public BaseFactoryRegistrar<QString, Strategy, Impl>
{
    public:
        StrategyFactoryRegistrar(QString strategyName):BaseFactoryRegistrar<QString, Strategy, Impl>(strategyName){}
};

void initializeFractionalMomentum()
{
    static StrategyFactoryRegistrar<FractionalMomentum> s("FractionalMomentum");
}

//extern void initialzeOneMinuteMomentum();
void initializeOneMinuteMomentum()
{
    static StrategyFactoryRegistrar<OneMinuteMomentum> s("OneMinuteMomentum");
}

void initializeStrategy()
{
    initializeFractionalMomentum();
    initializeOneMinuteMomentum();
}



#endif // STRATEGYFACTORY_H
