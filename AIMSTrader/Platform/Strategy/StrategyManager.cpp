#include "Platform/Strategy/StrategyManager.h"
#include "Platform/View/OutputInterface.h"
#include "Strategy/TestStrategy.h"
#include <QDebug>
#include "Data/strategyviewdata.h"

StrategyManager::StrategyManager()
{}

StrategyManager::~StrategyManager()
{}

void StrategyManager::launchStrategies()
{
   loadStrategies();
}


//create a map from strategyId to strategy guids
//how will database will resolve the exact column
//1. send name... then what's the point in having prmary key other than a string
//2. send strategyId , then this has to be a primary key
//3. send strategy guid
//startegyGuid    startegyId

void StrategyManager::loadStrategies()
{
      int i=0;
//    DatabaseSession* session = OutputInterface::Instance()->getDatabaseSession();
//    QList<StrategyViewData*> strategies = session->getStrategies();

//    int numStrategies = strategies.length();

//    for(int i = 0 ; i<numStrategies ; ++i)
//    {
//        if(strategies[i]->usedInTrading)
//        {
//            _strategies[i] = new Strategy(strategies[i]->name);
//            _strategies[i]->loadBuyList(strategies[i]-->)
//        }
//    }




      //get all the contracts from the database as QList<InstrumentDataDb>
      //struct InstrumentDataDb{ Contract, guuid};
      //and here I will keep a track of strategyId to quuid/watever I get


     //QHash<StrategyId, uint> _internalStrategyToDbMapper;

    _strategies[i++] = new Strategy("Manual");

    _strategies[i++] = new TestStrategy("TestStrategy");

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

void StrategyManager::closeAllPositionsInStrategy(const StrategyId strategyId)
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

void StrategyManager::closeAllPositionsForTicker(const TickerId tickerId)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;
        strategy->requestClosePosition(tickerId);
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

void StrategyManager::addPosition(const TickerId tickerId, const Order& order)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;

        if(strategy->getStrategyName() == "Manual")
        {

            qDebug()<<strategy->getStrategyName();
            strategy->requestAdjustPosition(tickerId, order);
            break;
        }
    }
}


//void StrategyManager::loadStrategiesFromDB()
//{
//    OutputInterface::Instance()->
//}

