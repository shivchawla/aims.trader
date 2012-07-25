#include "Platform/Strategy/StrategyOutput.h"
#include "AimsTraderGlobal.h"

StrategyOutput::StrategyOutput()
{}

void StrategyOutput::addPosition(const StrategyId strategyId, const TickerId tickerId )
{
   ioInterface()->addPosition(strategyId, tickerId);
}

void StrategyOutput::updatePositionForExecution(const Position& position)
{
    ioInterface()->updatePositionForExecution(&position);
}

void StrategyOutput::updatePositionForLastPrice(const Position& position)
{
    ioInterface()->updatePositionForLastPrice(&position);
}

void StrategyOutput::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
    ioInterface()->updatePerformance(strategyId, performanceStats);
}







