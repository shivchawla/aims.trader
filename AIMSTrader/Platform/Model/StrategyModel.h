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
    private:
        //StrategyModel();
        //const String getColumnName(const StrategyModelColumn col);

        StrategyModel()//:DataModel<StrategyModelColumn>()
        {
//            _totalNumColumns = 14;
//            _dataModel.push_back(StrategyModelStrategyName);
//            _dataModel.push_back(StrategyModelTotalBought);
//            _dataModel.push_back(StrategyModelTotalSold);
//            _dataModel.push_back(StrategyModelTotalCommission);
//            _dataModel.push_back(StrategyModelNetPnL);
//            _dataModel.push_back(StrategyModelInvestment);

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

//     public:
//        const String getColumnName(const StrategyModelColumn col)
//        {
//            switch(col)
//            {
//                case StrategyModelStrategyName: return "Strategy", true); break;
//                case StrategyModelTotalTrades: return "TotalTrades", true); break;
//                case StrategyModelWinningTrades: return "WinningTrades", true); break;
//                case StrategyModelLosingTrades: return "LosingTrades", true); break;
//                case StrategyModelProfitFactor: return "ProfitFactor", true); break;
//                case StrategyModelTotalBought: return "TotalBought", true); break;
//                case StrategyModelTotalSold: return "TotalSold", true); break;
//                case StrategyModelUnRealizedGrossPnL: return "UnRealizedGrossPnl", true);break;
//                case StrategyModelRealizedGrossPnL: return "RealizedGrossPnL", true); break;
//                case StrategyModelNetPnL: return "NetPnL", true);break;
//                case StrategyModelDrawDown: return "Drawdown", true);break;
//                case StrategyModelMaxDrawDown: return "MaxDrawdown", true);break;
//                case StrategyModelTotalCommission: return "TotalCommission", true);break;
//                case StrategyModelInvestment: return "Investment", true); break;
//            }
//        }

//        const String getColumnName(const int col)
//        {
//            StrategyModelColumn column = (StrategyModelColumn)(col);
//            return getColumnName(column);
//        }

};


#endif // STRATEGYMODEL_H
