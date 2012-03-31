#ifndef MARKETDATASUBSCRIBER_H
#define MARKETDATASUBSCRIBER_H
#include "Platform/typedefs.h"
#include <Shared/ATServerAPIDefines.h>
#include <QObject>

class MarketDataSubscriber : public QObject
{
    Q_OBJECT
    public:
         MarketDataSubscriber();
         virtual ~MarketDataSubscriber();

    public slots:
        virtual void onTradeUpdate(const TickerId, TradeUpdate);
        virtual void onQuoteUpdate(const TickerId, QuoteUpdate);
};

#endif // MARKETDATASUBSCRIBER_H
