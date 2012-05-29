#ifndef INSTRUMENTPOSITIONMODEL_H
#define INSTRUMENTPOSITIONMODEL_H

namespace InstrumentPosiionModel{

enum InstrumentPositionModelColumn{
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
        NetInclCommission
};

inline const char* getInstrumentPositionModelColumnName(const InstrumentPositionModelColumn column)
{
    switch(column)
    {
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

inline const int getInstrumentPositionModelNumColumns()
{
    return 16;
}



}//namespace ends

#endif // INSTRUMENTPOSITIONMODEL_H
