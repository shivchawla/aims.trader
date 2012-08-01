#include "Platform/Strategy/StrategyOutput.h"
#include "AimsTraderGlobal.h"

StrategyOutput::StrategyOutput()
{}

void StrategyOutput::addPosition(const StrategyId strategyId, const InstrumentId instrumentId )
{
   IOInterface::ioInterface().addPosition(strategyId, instrumentId);
}

void StrategyOutput::updatePositionForExecution(const Position& position)
{
    IOInterface::ioInterface().updatePositionForExecution(&position);
}

void StrategyOutput::updatePositionForLastPrice(const Position& position)
{
    IOInterface::ioInterface().updatePositionForLastPrice(&position);
}

void StrategyOutput::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats)
{
    IOInterface::ioInterface().updatePerformance(strategyId, performanceStats);
}







