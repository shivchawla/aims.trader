#ifndef STRATEGYPOSITIONVIEWITEM2_H
#define STRATEGYPOSITIONVIEWITEM2_H
#include "Platform/View/TableViewItem2.h"
#include "AimsTraderDefs/typedefs.h"

class StrategyPositionViewItem2 : public TableViewItem2
{
    public:
        StrategyPositionViewItem2(const StrategyId strategyId, const TickerId tickerId)
        {
            _tickerId = tickerId;
            _strategyId = strategyId;
        }

    private:
        TickerId _tickerId;
        StrategyId _strategyId;

    public:
        void setTickerId(const TickerId tickerId)
        {
            _tickerId = tickerId;
        }

        void setStrategyId(const StrategyId strategyId)
        {
            _strategyId = strategyId;
        }

        const TickerId getTickerId()
        {
            return _tickerId;
        }

        const StrategyId getStrategyId()
        {
            return _strategyId;
        }
};

#endif // STRATEGYPOSITIONVIEWITEM2_H
