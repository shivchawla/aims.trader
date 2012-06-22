#pragma once
#ifndef STRATEGYMANAGER_H
#define STRATEGYMANAGER_H
#include "Platform/typedefs.h"
#include <map>
#include <QBasicTimer>
#include "Platform/Utils/Singleton.h"
#include "Platform/Shared/Order.h"

class Strategy;

typedef std::map<StrategyId, Strategy*> StrategyMap;
typedef StrategyMap::iterator StrategyMapIterator;

class StrategyManager :public Singleton<StrategyManager>
{
    friend class Singleton<StrategyManager>;
    private:
        StrategyMap _strategies;
        QBasicTimer _timer;

    private:
        StrategyManager();

    public:
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

#endif // STRATEGYMANAGER_H
