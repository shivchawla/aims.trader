#ifndef STRATEGYOUTPUT_H
#define STRATEGYOUTPUT_H

#include "AimsTraderDefs/typedefs.h"
#include "Platform/Position/Position.h"

class StrategyOutput
{
    public:
        StrategyOutput();
        static StrategyOutput& strategyOutput()
        {
            static StrategyOutput so;
            return so;
        }

    
    public:
        void addPosition(const StrategyId, const TickerId, const OutputType type = ALL);
        void updatePositionForExecution(const Position*, const Position*, const OutputType type = ALL);
        void updatePositionForLastPrice(const Position&, const OutputType type = ALL);
        void updatePerformance(const StrategyId, const PerformanceStats&, const OutputType type = ALL);
};

//static StrategyOutput& strategyOutput()
//{
//    static StrategyOutput so;
//    return so;
//}


#endif // STRATEGYOUTPUT_H
