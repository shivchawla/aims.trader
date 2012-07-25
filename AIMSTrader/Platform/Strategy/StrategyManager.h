#pragma once
#ifndef STRATEGYMANAGER_H
#define STRATEGYMANAGER_H
#include "AimsTraderDefs/typedefs.h"
#include <map>
#include <QBasicTimer>
#include "Platform/Utils/Singleton.h"
#include "InteractiveBroker/Shared/Order.h"

class Strategy;

typedef std::map<StrategyId, Strategy*> StrategyMap;
typedef StrategyMap::iterator StrategyMapIterator;

class StrategyManager //:public Singleton<StrategyManager>
{
    //friend class Singleton<StrategyManager>;
    private:
        StrategyMap _strategies;
        QBasicTimer _timer;

    public:
        StrategyManager();
        ~StrategyManager();

    private:
        void loadStrategies();

    public:
        const String& getStrategyName(const StrategyId);

    public:
        void launchStrategies();
        void stopStrategy(const StrategyId);
        void closeAllPositionsInStrategy(const StrategyId);
        void closeAllPositionsForTicker(const TickerId);
        void closePosition(const StrategyId, const TickerId);
        void adjustPosition(const StrategyId, const TickerId, const Order&);
        void addPosition(const TickerId, const Order&);
};

static StrategyManager* strategyManager()
{
    static StrategyManager* sm = new StrategyManager();
    return sm;
}

#endif // STRATEGYMANAGER_H
