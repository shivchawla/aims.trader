#include "Platform/Strategy/StrategyOutput.h"
#include "AimsTraderGlobal.h"

StrategyOutput::StrategyOutput()
{}

void StrategyOutput::addPosition(const StrategyId strategyId, const InstrumentId instrumentId, const OutputType type)
{
   IOInterface::ioInterface().addPosition(strategyId, instrumentId, type);
}

void StrategyOutput::updatePositionForExecution(const Position& position, const OutputType type)
{
    IOInterface::ioInterface().updatePositionForExecution(&position, type);
}

void StrategyOutput::updatePositionForLastPrice(const Position& position, const OutputType type)
{
    IOInterface::ioInterface().updatePositionForLastPrice(&position, type);
}

void StrategyOutput::updatePerformance(const StrategyId strategyId, const PerformanceStats& performanceStats,  const OutputType type)
{
    IOInterface::ioInterface().updatePerformance(strategyId, performanceStats, type);
}







