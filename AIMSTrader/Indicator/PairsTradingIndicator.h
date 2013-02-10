#ifndef PAIRSTRADINGINDICATOR_H
#define PAIRSTRADINGINDICATOR_H

#include "Platform/Indicator/Indicator.h"

class PairsTradingIndicator : public Indicator
{
    public:
        PairsTradingIndicator();

    private:
        void setIndicator();

    public:
        void startIndicator();
};

#endif // PAIRSTRADINGINDICATOR_H
