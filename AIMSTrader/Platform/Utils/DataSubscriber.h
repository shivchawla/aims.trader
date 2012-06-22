#pragma once
#ifndef DATASUBSCRIBER_H
#define DATASUBSCRIBER_H
#include "Platform/Enumerations/DataSource.h"
#include "Platform/typedefs.h"
#include "Platform/Shared/Execution.h"
#include "Platform/Shared/EWrapper.h"
#include <Shared/ATServerAPIDefines.h>
#include <QObject>
#include <map>

class DataSubscriber : public QObject
{
    private:
        std::map<TickerId, bool> _subscriptions;

    public:
         DataSubscriber();
         ~DataSubscriber();

    private:
         void setSubscription(const TickerId);
         void cancelMarketDataSubscription(const TickerId);

    public:
        const bool isSubscribed(const TickerId);
        void subscribeMarketData(const Contract&, const DataSource source = ActiveTick);
        void subscribeMarketData(const TickerId, const DataSource source = ActiveTick);
        void stopMarketData(const TickerId);
        void unSubscribeMarketData(const TickerId);

    public slots:
        virtual void onTradeUpdate(const TickerId, const TradeUpdate&);
        virtual void onQuoteUpdate(const TickerId, const QuoteUpdate&);
        virtual void onExecutionUpdate(const TickerId, const Execution&);
        virtual void onTickPriceUpdate(const TickerId, const TickType, const double);
        virtual void updateOneMinuteSnapShot(const TickerId, const double);
        virtual void updateTwoMinuteSnapShot(const TickerId, const double);
        virtual void updateFiveMinuteSnapShot(const TickerId, const double);
        virtual void updateTenMinuteSnapShot(const TickerId, const double);
};

#endif // DATASUBSCRIBER_H
