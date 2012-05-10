#ifndef DATASUBSCRIBER_H
#define DATASUBSCRIBER_H
#include "Platform/typedefs.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Shared/EWrapper.h"
#include <Shared/ATServerAPIDefines.h>
#include <QObject>
<<<<<<< HEAD
#include <map>

class DataSubscriber:public QObject
{
    private:
        std::map<TickerId, bool> _subscriptions;

=======

class DataSubscriber:public QObject
{
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    public:
         DataSubscriber();
         virtual ~DataSubscriber();

<<<<<<< HEAD
    protected:
         const bool isSubscribed(const TickerId);
         void setSubscription(const TickerId);
         void cancelMarketDataSubscription(const TickerId);

=======
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    public slots:
        virtual void onTradeUpdate(const TickerId, const TradeUpdate&);
        virtual void onQuoteUpdate(const TickerId, const QuoteUpdate&);
        virtual void onExecutionUpdate(const OrderId, const Execution&);
        virtual void onTickPriceUpdate(const TickerId, const TickType, const double);
<<<<<<< HEAD
        virtual void updateOneMinuteSnapShot(const TickerId, const double);
        virtual void updateTwoMinuteSnapShot(const TickerId, const double);
        virtual void updateFiveMinuteSnapShot(const TickerId, const double);
        virtual void updateTenMinuteSnapShot(const TickerId, const double);
=======

>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
};

#endif // DATASUBSCRIBER_H
