#pragma once
#ifndef STRATEGYPOSITIONVIEWITEM_H
#define STRATEGYPOSITIONVIEWITEM_H

#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/TableViewItem.h"

class StrategyPositionViewItem : public TableViewItem<StrategyPositionViewItem>
{
    private:
        StrategyId _strategyId;
        InstrumentId _instrumentId;

    public:
        void setInstrumentId(const InstrumentId);
        void setStrategyId(const StrategyId);
        const StrategyId getStrategyId() const
        {
            return _strategyId;
        }

        const InstrumentId getInstrumentId() const
        {
            return _instrumentId;
        }

    public:
        StrategyPositionViewItem(const int numItems);
        ~StrategyPositionViewItem();

};

#endif // STRATEGYPOSITIONVIEWITEM_H

