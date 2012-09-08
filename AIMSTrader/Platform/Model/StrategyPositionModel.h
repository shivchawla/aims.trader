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
        StrategyPositionModelUnRealizedPL,
        StrategyPositionModelRealizedPL,
        StrategyPositionModelPL,
        StrategyPositionModelNetInclCommission
};

class StrategyPositionModel : public DataModel<StrategyPositionModelColumn>, public Singleton<StrategyPositionModel>
{
      friend class Singleton<StrategyPositionModel>;
      private:
        StrategyPositionModel()
        {
//            _totalNumColumns = 17;
//            _dataModel.push_back(StrategyPositionModelStrategy);
//            _dataModel.push_back(StrategyPositionModelInstrumentName);
//            _dataModel.push_back(StrategyPositionModelBuys);
//            _dataModel.push_back(StrategyPositionModelSells);
//            _dataModel.push_back(StrategyPositionModelNet);
//            _dataModel.push_back(StrategyPositionModelAvgBT);
//            _dataModel.push_back(StrategyPositionModelAvgSLD);
//            _dataModel.push_back(StrategyPositionModelTotalBT);
//            _dataModel.push_back(StrategyPositionModelTotalSLD);
//            _dataModel.push_back(StrategyPositionModelPL);
//            _dataModel.push_back(StrategyPositionModelUnRealizedPL);

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
            add(StrategyPositionModelUnRealizedPL, "UnRealized P&L", true);
            add(StrategyPositionModelRealizedPL, "Realized P&L", true);
            add(StrategyPositionModelPL, "P&L", false);
            add(StrategyPositionModelNetInclCommission, "Net Inc. Commission", false);
        }

        public:
          //  ~StrategyPositionModel(){}

             //static StrategyPositionModel* Model();

//        const String getColumnName(const StrategyPositionModelColumn column)
//        {
//            switch(column)
//            {
//                case StrategyPositionModelStrategy:return "StrategyId", true); break;
//                case StrategyPositionModelInstrumentName: return "InstrumentId", true); break;
//                case StrategyPositionModelBuys: return "Buys", true); break;
//                case StrategyPositionModelSells:return "Sells", true); break;
//                case StrategyPositionModelNet: return "Net", true); break;
//                case StrategyPositionModelPosition: return "Position", true); break;
//                case StrategyPositionModelAvgBT: return "Avg(BT)", true); break;
//                case StrategyPositionModelAvgSLD:return "Avg(SLD)" ; break;
//                case StrategyPositionModelTotalBT:return "TotalBT", true); break;
//                case StrategyPositionModelTotalSLD:return "TotalSLD", true); break;
//                case StrategyPositionModelNetTotal:return "NetTotal", true); break;
//                case StrategyPositionModelLastPrice: return "LastPrice", true); break;
//                case StrategyPositionModelCommission: return "Commission", true); break;
//                case StrategyPositionModelUnRealizedPL: return "UnRealized P&L", true); break;
//                case StrategyPositionModelRealizedPL: return "Realized P&L", true); break;
//                case StrategyPositionModelPL: return "P&L", true); break;
//                case StrategyPositionModelNetInclCommission: return "Net Inc. Commission", true); break;
//             }
//        }

//        const String getColumnName(const int column)
//        {
//            StrategyPositionModelColumn col = StrategyPositionModelColumn(column);
//            return getColumnName(col);
//        }

};

#endif // POSITIONMODEL_H
