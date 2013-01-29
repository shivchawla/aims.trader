#ifndef STRATEGYFACTORY_H
#define STRATEGYFACTORY_H
#include <Platform/Strategy/Strategy.h>
#include <Utils/FactoryPattern.h>
#include "Strategy/FractionalMomentum.h"
#include "Strategy/OneMinuteMomentum.h"
#include "Strategy/PercentileMomentum.h"
#include "Platform/Strategy/SpreadStrategy.h"

static void initializeStrategy();

//class StrategyFactoryMapImpl: public BaseFactoryMapImpl<QString, Strategy>
//{
//    public:
//        StrategyFactoryMapImpl():BaseFactoryMapImpl<QString, Strategy>()
//        {
//            initializeStrategy();
//        }
//};

BaseFactoryMapImpl<QString, Strategy>& StrategyFactoryMap()
{
    initializeStrategy();
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

void initializePercentileMomentum()
{
   static StrategyFactoryRegistrar<PercentileMomentum> s("PercentileMomentum");
}

void initializeManualStrategy()
{
    static StrategyFactoryRegistrar<Strategy> s("Manual");
}

void initializeSpreadStrategy()
{
    static StrategyFactoryRegistrar<SpreadStrategy> s("SpreadStrategy");
}

void initializeStrategy()
{
    initializeManualStrategy();
    initializeFractionalMomentum();
    initializeOneMinuteMomentum();
    initializePercentileMomentum();
    initializeSpreadStrategy();
}

#endif // STRATEGYFACTORY_H
