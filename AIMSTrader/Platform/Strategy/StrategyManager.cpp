#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Strategy/Strategy.h"

StrategyManager::StrategyManager()
{}

StrategyManager::~StrategyManager()
{}

void StrategyManager::launchStrategies()
{
   loadStrategies();
}

void StrategyManager::loadStrategies()
{
    std::list<Strategy*> strategyList = Strategy::getStrategies();
    std::list<Strategy*>::iterator it;
    std::list<Strategy*>::iterator end = strategyList.end();

    for(it=strategyList.begin();it!=end;++it)
    {
        (*it)->initialize();
    }
}


