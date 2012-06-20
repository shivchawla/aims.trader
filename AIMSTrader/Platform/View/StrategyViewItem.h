#pragma once
#ifndef STRATEGYVIEWITEM_H
#define STRATEGYVIEWITEM_H

#include "Platform/typedefs.h"
#include "Platform/View/TableViewItem.h"

class StrategyViewItem : public TableViewItem<StrategyViewItem>
{
    private:
        StrategyId _strategyId;

    public:
        StrategyViewItem(const int numItems);
        ~StrategyViewItem();

   public:
        void setStrategyId(const StrategyId );
        const StrategyId getStrategyId() const
        {
            return _strategyId;
        }

        //void updateSpecial(const double, int itemColumn);

};

#endif // STRATEGYVIEWITEM_H
