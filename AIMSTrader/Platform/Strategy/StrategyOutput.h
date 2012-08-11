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
        void addPosition(const StrategyId, const InstrumentId);
        void updatePositionForExecution(const Position&);
        void updatePositionForLastPrice(const Position&);
        void updatePerformance(const StrategyId, const PerformanceStats&);
};

//static StrategyOutput& strategyOutput()
//{
//    static StrategyOutput so;
//    return so;
//}


#endif // STRATEGYOUTPUT_H