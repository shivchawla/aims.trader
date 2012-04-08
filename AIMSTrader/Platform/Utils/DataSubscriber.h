#ifndef DATASUBSCRIBER_H
#define DATASUBSCRIBER_H
#include "Platform/typedefs.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Shared/EWrapper.h"
#include <Shared/ATServerAPIDefines.h>
#include <QObject>

class DataSubscriber:public QObject
{
    public:
         DataSubscriber();
         virtual ~DataSubscriber();

    public slots:
        virtual void onTradeUpdate(const TickerId, const TradeUpdate&);
        virtual void onQuoteUpdate(const TickerId, const QuoteUpdate&);
        virtual void onExecutionUpdate(const OrderId, const Execution&);
        virtual void onTickPriceUpdate(const TickerId, const TickType, const double);

};

#endif // DATASUBSCRIBER_H
