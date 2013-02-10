#ifndef INSTRUMENTVIEWSUBSCRIBER_H
#define INSTRUMENTVIEWSUBSCRIBER_H

#include "Platform/Utils/DataSubscriber.h"
class InstrumentView2;
class InstrumentView;

class InstrumentViewSubscriber : public DataSubscriber
{
    private:
        InstrumentView2* _instrumentView2;
        InstrumentView* _instrumentView;

    public:
        InstrumentViewSubscriber(InstrumentView2* parent = 0);
        InstrumentViewSubscriber(InstrumentView* parent = 0);

    public:
        void onTickPriceUpdate(const TickerId, const TickType, const double);
};

#endif // INSTRUMENTVIEWSUBSCRIBER_H
