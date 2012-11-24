#ifndef STRATEGYVIEWITEM2_H
#define STRATEGYVIEWITEM2_H

#include "AimsTraderDefs/typedefs.h"
#include "Platform/View/TableViewItem2.h"

class StrategyViewItem2 : public TableViewItem2
{
    private:
        StrategyId _strategyId;

    public:
        StrategyViewItem2(const StrategyId strategyId = -1);
        //StrategyViewItem2(QWidget* parent = 0);
        //~StrategyViewItem2();

   public:
        void setStrategyId(const StrategyId );
        const StrategyId getStrategyId() const
        {
            return _strategyId;
        }
};

#endif // STRATEGYVIEWITEM2_H
