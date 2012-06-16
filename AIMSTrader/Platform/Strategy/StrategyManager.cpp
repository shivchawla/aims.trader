#include "Platform/Strategy/StrategyManager.h"
#include "Strategy/TestStrategy.h"

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
    _strategies[0] = new TestStrategy("TestStrategy");

//    StrategyMapIterator end = _strategies.end();
//    StrategyMapIterator it;
//    for(it=_strategies.begin();it!=end;++it)
//    {
//         Strategy* strategy = it->second;
//        //here before we initialize the strategy , we should check for open positions for this strategy
//        //in a db..This is possible when program crashes in the middle. There might be some open positions
//        //it's important to load those positions first
//        //Reloading will involve multiple steps
//        //1. recreate the positon map for a strategy
//        //2. relink the data for the positions
//        //3.

//        // we can skip this for now but this is an important feature of a robust platform

//        strategy->initialize();
//    }
}

const String& StrategyManager::getStrategyName(const StrategyId strategyId)
{
    if(_strategies.count(strategyId)!=0)
    {
        return _strategies[strategyId]->getStrategyName();
    }
    return "";
}

/*StrategyManager* StrategyManager::manager()
{
    if(_manager=NULL)
    {
        _manager = new StrategyManager();
    }
    return _manager;
}*/

void StrategyManager::stopStrategy(const StrategyId strategyId)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;
        if(strategy->getStrategyId() == strategyId)
        {
            strategy->stopStrategy();
            break;
        }
    }
}

void StrategyManager::closeAllPositions(const StrategyId strategyId)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;
        if(strategy->getStrategyId() == strategyId)
        {
            strategy->requestCloseAllPositions();
            break;
        }
    }

}

void StrategyManager::closePosition(const StrategyId strategyId, const TickerId tickerId)
{
    if(_strategies.count(strategyId))
    {
        _strategies[strategyId]->requestClosePosition(tickerId);
    }
}


void StrategyManager::adjustPosition(const StrategyId strategyId, const TickerId tickerId, const Order& order)
{
    if(_strategies.count(strategyId))
    {
        _strategies[strategyId]->requestAdjustPosition(tickerId, order);
    }
}

