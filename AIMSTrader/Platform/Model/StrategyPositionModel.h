#ifndef STRATEGYPOSITIONMODEL_H
#define STRATEGYPOSITIONMODEL_H

#include "Platform/typedefs.h"
#include <vector>

enum StrategyPositionModelColumn{
        Strategy=0,
        InstrumentId,
        Buys,
        Sells,
        Net,
        Position,
        AvgBT,
        AvgSLD,
        TotalBT,
        TotalSLD,
        NetTotal,
        LastPrice,
        Commission,
        UnRealizedPL,
        RealizedPL,
        PL,
        NetInclCommission,
        none = -1
};

class StrategyPositionModel
{
    const static int _numColumns = 17;
    const static int _defaultNumColumns = 11;
    const static StrategyPositionModelColumn _defaultModel[_defaultNumColumns];// = { Strategy, InstrumentId, Buys, Sells, Net, AvgBT, AvgSLD, TotalBT, TotalSLD, LastPrice, NetInclCommission};
    static StrategyPositionModel* _strategyPositionModel;// = NULL;

    StrategyPositionModel()
    {}

    public:
        ~StrategyPositionModel(){}

         StrategyPositionModel* Model()
         {
             if(!_strategyPositionModel)
             {
                 _strategyPositionModel = new StrategyPositionModel();
             }
             return _strategyPositionModel;
         }

    inline static const String getStrategyPositionModelColumnName(const StrategyPositionModelColumn column)
    {
        switch(column)
        {
            case Strategy:return "Strategy"; break;
            case InstrumentId: return "InstrumentId"; break;
            case Buys: return "Buys"; break;
            case Sells:return "Sells"; break;
            case Net: return "Net"; break;
            case Position: return "Position"; break;
            case AvgBT: return "Avg(BT)"; break;
            case AvgSLD:return "Avg(SLD)" ; break;
            case TotalBT:return "TotalBT"; break;
            case TotalSLD:return "TotalSLD"; break;
            case NetTotal:return "NetTotal"; break;
            case LastPrice: return "LastPrice"; break;
            case Commission: return "Commission"; break;
            case UnRealizedPL: return "UnRealized P&L"; break;
            case RealizedPL: return "Realized P&L"; break;
            case PL: return "P&L"; break;
            case NetInclCommission: return "Net Inc. Commission"; break;
         }
    }

    static const int getNumColumns()
    {
        return _numColumns;
    }

    static const int getDefaultNumColumns()
    {
        return _defaultNumColumns;
    }

    const static StrategyPositionModelColumn* getDefaultModel()
    {
        return _defaultModel;
    }
};

#endif // POSITIONMODEL_H
