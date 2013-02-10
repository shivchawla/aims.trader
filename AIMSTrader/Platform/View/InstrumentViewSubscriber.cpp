#include "Platform/View/InstrumentViewSubscriber.h"
#include "Platform/View/InstrumentView2.h"
#include "Platform/View/InstrumentView.h"

InstrumentViewSubscriber::InstrumentViewSubscriber(InstrumentView2* instrumentView) : DataSubscriber()
{
    _instrumentView2= instrumentView;
}

InstrumentViewSubscriber::InstrumentViewSubscriber(InstrumentView* instrumentView) : DataSubscriber()
{
    _instrumentView= instrumentView;
    _datasource = Test;
}


void InstrumentViewSubscriber::onTickPriceUpdate(const TickerId tickerId, const TickType tickType, const double price)
{
    _instrumentView->updateTickGeneric(tickerId, tickType, price);
    //_instrumentView2->updateTickGeneric(tickerId, tickType, price);
}

