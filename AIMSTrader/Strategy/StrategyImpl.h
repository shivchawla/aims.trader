#ifndef STRATEGYIMPL_H
#define STRATEGYIMPL_H
#include <Platform/Strategy/Strategy.h>

template<class Derived>
class StrategyImpl: public Strategy
{
    public:
        StrategyImpl():Strategy(){}
        StrategyImpl(const QString& strategyName):Strategy(strategyName){}

};

#endif // STRATEGYIMPL_H
