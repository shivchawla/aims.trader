#pragma once
#ifndef STRATEGYMODEL_H
#define STRATEGYMODEL_H

#include "Platform/typedefs.h"
#include "Platform/Model/DataModel.h"
#include "Platform/Utils/Singleton.h"

enum StrategyModelColumn{
 StrategyModelStrategyName = 0,
 StrategyModelTotalTrades,
 StrategyModelWinningTrades,
 StrategyModelLosingTrades,
 StrategyModelProfitFactor,
 StrategyModelTotalBought,
 StrategyModelTotalSold,
 StrategyModelUnRealizedGrossPnL,
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
            add(StrategyModelTotalTrades, "TotalTrades", false);
            add(StrategyModelWinningTrades, "WinningTrades", false);
            add(StrategyModelLosingTrades, "LosingTrades", false);
            add(StrategyModelProfitFactor, "ProfitFactor", true);
            add(StrategyModelTotalBought, "TotalBought", true);
            add(StrategyModelTotalSold, "TotalSold", true);
            add(StrategyModelUnRealizedGrossPnL, "UnRealizedGrossPnl", true);
            add(StrategyModelRealizedGrossPnL, "RealizedGrossPnL", true);
            add(StrategyModelNetPnL, "NetPnL", false);
            add(StrategyModelDrawDown, "Drawdown", false);
            add(StrategyModelMaxDrawDown, "MaxDrawdown", false);
            add(StrategyModelTotalCommission, "TotalCommission", false);
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
