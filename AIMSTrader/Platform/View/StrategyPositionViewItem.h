#pragma once
#ifndef STRATEGYPOSITIONVIEWITEM_H
#define STRATEGYPOSITIONVIEWITEM_H

#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/TableViewItem.h"

class StrategyPositionViewItem : public TableViewItem<StrategyPositionViewItem>
{
    private:
        StrategyId _strategyId;
        TickerId _tickerId;
        bool _isActive;

    public:
        void setTickerId(const TickerId);
        void setStrategyId(const StrategyId);
        const StrategyId getStrategyId() const
        {
            return _strategyId;
        }

        const TickerId getTickerId() const
        {
            return _tickerId;
        }

        void setActive(const bool);
        bool IsActive() const;

    public:
        StrategyPositionViewItem(const int numItems);
        ~StrategyPositionViewItem();

};

#endif // STRATEGYPOSITIONVIEWITEM_H

