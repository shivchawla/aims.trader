#ifndef TRADEVIEWITEM_H
#define TRADEVIEWITEM_H

#include "Platform/Model/StrategyPositionModel.h"
#include "Platform/View/TableViewItem.h"


class TradeViewItem : public TableViewItem<TradeViewItem>
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

        void setActive(const bool isActive) {_isActive = isActive;}
        bool IsActive() const {return _isActive;}

    public:
        TradeViewItem(const int numItems);
};

#endif // TRADEVIEWITEM_H
