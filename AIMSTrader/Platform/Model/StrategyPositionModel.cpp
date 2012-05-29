#include "Platform/Model/StrategyPositionModel.h"

StrategyPositionModel* StrategyPositionModel::_strategyPositionModel = NULL;
const StrategyPositionModelColumn StrategyPositionModel::_defaultModel[StrategyPositionModel::_defaultNumColumns] = { Strategy, InstrumentId, Buys, Sells, Net, AvgBT, AvgSLD, TotalBT, TotalSLD, PL, UnRealizedPL};

