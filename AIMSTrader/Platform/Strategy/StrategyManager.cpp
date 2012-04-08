#include "Platform/Strategy/StrategyManager.h"
#include "Platform/Strategy/Strategy.h"

std::map<StrategyId, String> StrategyManager::_strategies;
StrategyManager*  StrategyManager::_manager=NULL;

StrategyManager::StrategyManager()
{
  //timer.start(1000);
  //connect(&timer,SIGNAL(timeout()),this,SLOT());
}

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
        _strategies[(*it)->getStrategyId()] = (*it)->getStrategyName();


        //here before we initialize the strategy , we should check for open positions for this strategy
        //in a db..This is possible when program crashes in the middle. There might be some open positions
        //it's important to load those positions first
        //Reloading will involve multiple steps
        //1. recreate the positon map for a strategy
        //2. relink the data for the positions
        //3.

        // we can skip this for now but this is an important feature of a robust platform

        (*it)->initialize();
    }
}

const String& StrategyManager::getStrategyName(const StrategyId strategyId)
{
    if(_strategies.count(strategyId)!=0)
    {
        return _strategies[strategyId];
    }
    return "";
}

StrategyManager* StrategyManager::manager()
{
    if(_manager=NULL)
    {
        _manager = new StrategyManager();
    }
    return _manager;
}



