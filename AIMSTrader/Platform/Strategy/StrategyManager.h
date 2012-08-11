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

class StrategyManager
{
    //friend class Singleton<StrategyManager>;
    private:
        StrategyMap _strategies;
        QBasicTimer _timer;

    public:
        StrategyManager();
        ~StrategyManager();
        static StrategyManager& strategyManager()
        {
            static StrategyManager sm;
            return sm;
        }


    private:
        void loadStrategies();

    public:
        const String& getStrategyName(const StrategyId);

    public:
        void launchStrategies();
        void stopStrategy(const StrategyId);
        void closeAllPositionsInStrategy(const StrategyId);
        void closeAllPositionsForInstrument(const TickerId);
        void closePosition(const StrategyId, const TickerId);
        void adjustPosition(const StrategyId, const TickerId, const Order&);
        void addPosition(const TickerId, const Order&);
};

#endif // STRATEGYMANAGER_H
