#ifndef TRADEMODEL_H
#define TRADEMODEL_H

#include "AimsTraderDefs/typedefs.h"
#include "Platform/Model/DataModel.h"

enum TradeModelColumn{
        TradeModelStrategyName=0,
        TradeModelInstrumentName,
        TradeModelBuys,
        TradeModelSells,
        TradeModelBuyPrice,
        TradeModelSellPrice,
        TradeModelDateTime,
        TradeModelTotalBT,
        TradeModelTotalSLD,
        TradeModelCommission
};

class TradeModel : public DataModel<TradeModelColumn>
{
    public:
        TradeModel()
        {
            add(TradeModelStrategyName, "Strategy", true);
            add(TradeModelInstrumentName,  "InstrumentName", true);
            add(TradeModelBuys, "Buys", true);
            add(TradeModelSells, "Sells", true);
            add(TradeModelBuyPrice, "BuyPrice", true);
            add(TradeModelSellPrice, "SellPrice", true);
            add(TradeModelTotalBT, "TotalBT", false);
            add(TradeModelTotalSLD, "TotalSLD", false);
            add(TradeModelCommission, "Commission", false);
            add(TradeModelDateTime,"DateTime", true);
        }
};

#endif // TRADEMODEL_H
