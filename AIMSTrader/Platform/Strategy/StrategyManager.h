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
        StrategyManager();
    public:
        ~StrategyManager();


    private:
        StrategyMap _strategies;
        //static StrategyManager* _manager;

    private:
        QBasicTimer _timer;

    public:
        void launchStrategies();

    public:
        const String& getStrategyName(const StrategyId);

    private:
        void loadStrategies();

    public:
        void stopStrategy(const StrategyId);
        void closeAllPositions(const StrategyId);
        void closePosition(const StrategyId, const TickerId);
        void adjustPosition(const StrategyId, const TickerId, const Order&);
};

#endif // STRATEGYMANAGER_H
