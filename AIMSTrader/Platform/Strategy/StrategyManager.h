#ifndef STRATEGYMANAGER_H
#define STRATEGYMANAGER_H

class StrategyManager
{
    public:
        StrategyManager();
        ~StrategyManager();

    public:
        void launchStrategies();

    private:
        void loadStrategies();

};

#endif // STRATEGYMANAGER_H
