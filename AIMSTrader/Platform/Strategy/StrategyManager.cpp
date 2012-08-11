#include "Platform/Strategy/StrategyManager.h"
#include "Platform/View/IOInterface.h"
#include "Strategy/TestStrategy.h"
#include <QDebug>
#include "Strategy/StrategyFactory.h"
#include "Platform/View/IODatabase.h"

const StrategyId manualStrategyId = -1;
const StrategyId testStrategyId = -2;

StrategyManager::StrategyManager()
{}

StrategyManager::~StrategyManager()
{
//    StrategyMapIterator end = _strategies.end();
//    StrategyMapIterator it;
//    for(it=_strategies.begin();it!=end;++it)
//    {
//        Strategy* strategy = it->second;
//        //strategy->deleteLater();
//    }
}

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
    _strategies[manualStrategyId] = new Strategy("Manual");
    _strategies[testStrategyId] = new TestStrategy("TestStrategy");

    //now load strategies from the Database

    /******* Get only non-Null Stratgies*****MAKE A CHANGE*/
      QList<StrategyData*> strategyDataList = IODatabase::ioDatabase().getStrategies();

      foreach(StrategyData* strategyData, strategyDataList)
      {
          if(strategyData->usedInTrading)
          {
              Strategy* strategy = StrategyFactoryMap().Get(strategyData->parentStrategyName);
              strategy->setName(strategyData->name);

              QList<InstrumentData*> strategyBuyList = IODatabase::ioDatabase().getStrategyBuyList(strategyData->name);
              strategy->setBuyList(strategyBuyList);

              _strategies[strategyData->strategyId] = strategy;
          }
      }


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

void StrategyManager::closeAllPositionsForInstrument(const TickerId instrumentId)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;
        strategy->requestClosePosition(instrumentId);
    }
}

void StrategyManager::closePosition(const StrategyId strategyId, const TickerId instrumentId)
{
    if(_strategies.count(strategyId))
    {
        _strategies[strategyId]->requestClosePosition(instrumentId);
    }
}

void StrategyManager::adjustPosition(const StrategyId strategyId, const TickerId instrumentId, const Order& order)
{
    if(_strategies.count(strategyId))
    {
        _strategies[strategyId]->requestAdjustPosition(instrumentId, order);
    }
}

void StrategyManager::addPosition(const TickerId instrumentId, const Order& order)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;

        if(strategy->getStrategyName() == "Manual")
        {
            qDebug()<<strategy->getStrategyName();
            strategy->requestAdjustPosition(instrumentId, order);
            break;
        }
    }
}
