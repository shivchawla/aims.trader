#pragma once
#ifndef STRATEGYMODEL_H
#define STRATEGYMODEL_H

#include "AimsTraderDefs/typedefs.h"
#include "Platform/Model/DataModel.h"
#include "Platform/Utils/Singleton.h"

enum StrategyModelColumn{
 StrategyModelStrategyName = 0,
 StrategyModelTotalTrades,
 StrategyModelWinningTrades,
 StrategyModelLosingTrades,
 StrategyModelProfit,
 StrategyModelLoss,
 StrategyModelProfitFactor,
 StrategyModelTotalBought,
 StrategyModelTotalSold,
 StrategyModelRunningGrossPnL,
 StrategyModelRealizedGrossPnL,
 StrategyModelNetPnL,
 StrategyModelDrawDown,
 StrategyModelMaxDrawDown,
 StrategyModelTotalCommission,
 StrategyModelInvestment
};

class StrategyModel: public DataModel<StrategyModelColumn>, public Singleton<StrategyModel>
{
    friend class Singleton<StrategyModel>;
    public:
        StrategyModel()
        {
            add(StrategyModelStrategyName, "Strategy", true);
            add(StrategyModelTotalTrades, "Total Trades", false);
            add(StrategyModelWinningTrades, "Winning Trades", false);
            add(StrategyModelLosingTrades, "Losing Trades", false);
            add(StrategyModelProfit, "Profit", true);
            add(StrategyModelLoss, "Loss", true);
            add(StrategyModelProfitFactor, "Profit Factor", true);
            add(StrategyModelTotalBought, "Total Bought", true);
            add(StrategyModelTotalSold, "Total Sold", true);
            add(StrategyModelRunningGrossPnL, "Running Gross Pnl", true);
            add(StrategyModelRealizedGrossPnL, "Realized Gross PnL", true);
            add(StrategyModelNetPnL, "Net PnL", false);
            add(StrategyModelDrawDown, "Drawdown", false);
            add(StrategyModelMaxDrawDown, "MaxDrawdown", false);
            add(StrategyModelTotalCommission, "Total Commission", false);
            add(StrategyModelInvestment, "Investment", true);
        }
};

#endif // STRATEGYMODEL_H
