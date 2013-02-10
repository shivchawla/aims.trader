#ifndef PAIRSTADING_H
#define PAIRSTADING_H

#include "Platform/Strategy/SpreadStrategy.h"
#include "Indicator/PairsTradingIndicator.h"

class PairsTrading : public SpreadStrategy
{
    public:
        PairsTrading();

    private:
        void createIndicator();

    public:

};

PairsTrading::PairsTrading():SpreadStrategy()
{
    createIndicator();
}

void PairsTrading::createIndicator()
{
    _indicatorPtr = new PairsTradingIndicator();
}


#endif // PAIRSTADING_H
