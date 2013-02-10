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


/*
 *Create a map from strategyId to strategy guids
 *How will database will resolve the exact column
 *1. send name... then what's the point in having prmary key other than a string
 *2. send strategyId , then this has to be a primary key
 *3. send strategy guid
 *startegyGuid    startegyId
 */
void StrategyManager::loadStrategies()
{
    //_strategies[testStrategyId] = new TestStrategy("TestStrategy");
    //now load strategies from the Database
    /******* Get only non-Null Stratgies*****MAKE A CHANGE*/
      QList<StrategyData> strategyDataList = IODatabase::ioDatabase().getStrategies();

      foreach(StrategyData strategyData, strategyDataList)
      {
          if(strategyData.usedInTrading)
          {
              Strategy* strategy = StrategyFactoryMap().Get(strategyData.parentStrategyName);
              _strategyIdToDbId[strategy->getStrategyId()] = strategyData.strategyId ;

               strategy->setupStrategy(strategyData);
              _strategies[strategy->getStrategyId()] = strategy;
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

/*
 *
 */
const String StrategyManager::getStrategyName(const StrategyId strategyId)
{
    String name="";

    if(Strategy* strategy = _strategies[strategyId])
    {
        name = strategy->getStrategyName();
    }

    return name;
}

/*
 *
 */
void StrategyManager::stopStrategy(const StrategyId strategyId)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;
        if(strategy->getStrategyId() == strategyId)
        {
            QMetaObject::invokeMethod(strategy, "stopStrategy", Qt::QueuedConnection);
            break;
        }
    }
}

/*
 *
 */
void StrategyManager::closeAllPositionsInStrategy(const StrategyId strategyId)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;
        if(strategy->getStrategyId() == strategyId)
        {
            QMetaObject::invokeMethod(strategy, "closeAll", Qt::QueuedConnection);
            break;
        }
    }
}

/*
 *
 */
void StrategyManager::closeAllPositionsForInstrument(const TickerId tickerId)
{
    StrategyMapIterator end = _strategies.end();
    StrategyMapIterator it;
    for(it=_strategies.begin();it!=end;++it)
    {
        Strategy* strategy = it->second;
        QMetaObject::invokeMethod(strategy,"closePosition", Qt::QueuedConnection, Q_ARG(TickerId, tickerId));
        //strategy->requestClosePosition(tickerId);
    }
}

/*
 *
 */
void StrategyManager::closePosition(const StrategyId strategyId, const TickerId tickerId)
{
    if(_strategies.count(strategyId))
    {
        Strategy* strategy = _strategies[strategyId];
        QMetaObject::invokeMethod(strategy, "closePosition", Qt::QueuedConnection, Q_ARG(TickerId, tickerId));
    }
}

/*
 *
 */
void StrategyManager::adjustPosition(const StrategyId strategyId, const TickerId tickerId, const Order& order)
{
    if(_strategies.count(strategyId))
    {
        Strategy* strategy = _strategies[strategyId];
        QMetaObject::invokeMethod(strategy, "adjustPosition", Qt::QueuedConnection, Q_ARG(TickerId, tickerId), Q_ARG(Order, order));
                //->requestAdjustPosition(tickerId, order);
    }
}

/*
 *
 */
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
            QMetaObject::invokeMethod(strategy, "adjustPosition", Qt::QueuedConnection, Q_ARG(TickerId, tickerId), Q_ARG(Order, order));
            break;
        }
    }
}

/*
 *
 */
DbStrategyId StrategyManager::getDatabaseStrategyId(const StrategyId strategyId)
{
    return _strategyIdToDbId[strategyId];
}

/*
 *
 */
void StrategyManager::updateStrategyForOrderExecution(const OrderId orderId, const OrderDetail& orderDetail)
{
    StrategyId strategyId = orderDetail.strategyId;
    if(_strategies.count(strategyId))
    {
        QMetaObject::invokeMethod(_strategies[strategyId], "updatePositionOnExecution", Qt::QueuedConnection, Q_ARG(OrderId, orderId), Q_ARG(OrderDetail, orderDetail));
        //->requestStrategyUpdateForExecution(orderId, orderDetail);
    }
}

/*
 *
 */
StrategyType StrategyManager::getStrategyType(const StrategyId strategyId)
{
    StrategyType type = SingleStock_StrategyType;
    if(Strategy* strategy = _strategies[strategyId])
    {
        type = strategy->getStrategyType();
    }

    return type;
}


