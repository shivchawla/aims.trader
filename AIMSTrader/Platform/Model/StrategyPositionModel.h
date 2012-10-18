#pragma once
#ifndef STRATEGYPOSITIONMODEL_H
#define STRATEGYPOSITIONMODEL_H

#include "AimsTraderDefs/typedefs.h"
#include <vector>
#include "Platform/Utils/Singleton.h"
#include "Platform/Model/DataModel.h"

enum StrategyPositionModelColumn{
        StrategyPositionModelStrategy=0,
        StrategyPositionModelInstrumentName,
        StrategyPositionModelBuys,
        StrategyPositionModelSells,
        StrategyPositionModelNet,
        StrategyPositionModelAvgBT,
        StrategyPositionModelAvgSLD,
        StrategyPositionModelTotalBT,
        StrategyPositionModelTotalSLD,
        StrategyPositionModelNetTotal,
        StrategyPositionModelLastPrice,
        StrategyPositionModelCommission,
        StrategyPositionModelRunningPL,
        StrategyPositionModelRealizedPL,
        StrategyPositionModelPL,
        StrategyPositionModelNetInclCommission
};

class StrategyPositionModel : public DataModel<StrategyPositionModelColumn>, public Singleton<StrategyPositionModel>
{
    friend class Singleton<StrategyPositionModel>;
    friend class StrategyPositionModel2;
    public:
        StrategyPositionModel()
        {
            add(StrategyPositionModelStrategy, "Strategy", true);
            add(StrategyPositionModelInstrumentName,  "InstrumentName", true);
            add(StrategyPositionModelBuys, "Buys", true);
            add(StrategyPositionModelSells, "Sells", true);
            add(StrategyPositionModelNet, "Net", true);
            add(StrategyPositionModelAvgBT, "Avg(BT)", true);
            add(StrategyPositionModelAvgSLD, "Avg(SLD)", true);
            add(StrategyPositionModelTotalBT, "TotalBT", false);
            add(StrategyPositionModelTotalSLD, "TotalSLD", false);
            add(StrategyPositionModelNetTotal, "NetTotal", false);
            add(StrategyPositionModelLastPrice, "LastPrice", false);
            add(StrategyPositionModelCommission, "Commission", false);
            add(StrategyPositionModelRunningPL, "Running P&L", true);
            add(StrategyPositionModelRealizedPL, "Realized P&L", true);
            add(StrategyPositionModelPL, "P&L", false);
            add(StrategyPositionModelNetInclCommission, "Net Inc. Commission", false);
        }
};

#endif // POSITIONMODEL_H
