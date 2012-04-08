#ifndef STRATEGYMANAGER_H
#define STRATEGYMANAGER_H
#include "Platform/typedefs.h"
#include <map>
#include <QBasicTimer>

class StrategyManager
{
    private:
        StrategyManager();
    public:
        ~StrategyManager();

    private:
        static std::map<StrategyId, String> _strategies;
        static StrategyManager* _manager;

    private:
        QBasicTimer timer;

    public:
        void launchStrategies();

    public:
        static const String& getStrategyName(const StrategyId);

    private:
        void loadStrategies();

    public:
        static StrategyManager* manager();

};

#endif // STRATEGYMANAGER_H
