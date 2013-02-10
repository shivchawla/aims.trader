#ifndef SPREADMODEL_H
#define SPREADMODEL_H

#include "Platform/Model/DataModel.h"

enum SpreadModelColumn{
 SpreadModelSpreadName = 0,
 SpreadModelTotalBought,
 SpreadModelTotalSold,
 SpreadModelRunningGrossPnL,
 SpreadModelRealizedGrossPnL,
 SpreadModelNetPnL,
 SpreadModelProfit,
 SpreadModelLoss,
 SpreadModelProfitFactor,
 SpreadModelTotalCommission
};


class SpreadModel : public DataModel<SpreadModel>
{
    public:
        SpreadModel()
        {
            add(SpreadModelSpreadName, "SpreadName", true);
            add(SpreadModelTotalBought, "TotalBought", true);
            add(SpreadModelTotalSold, "TotalSold", true);
            add(SpreadModelRunningGrossPnL, "RunningPnL", true);
            add(SpreadModelRealizedGrossPnL, "RealizedPnL", true);
            add(SpreadModelNetPnL, "NetPnL", true);
            add(SpreadModelProfit, "Profit", false);
            add(SpreadModelLoss,, "Loss", false);
            add(SpreadModelProfitFactor, "ProfitFactor", false);
            add(SpreadModelTotalCommission, "Commission", false);
        }
}

#endif // SPREADMODEL_H
