#pragma once
#ifndef DATASUBSCRIBER_H
#define DATASUBSCRIBER_H
//#include "Platform/Enumerations/DataSource.h"
#include "AimsTraderDefs/typedefs.h"
#include "InteractiveBroker/Shared/Execution.h"
#include "InteractiveBroker/Shared/EWrapper.h"
#include <../API/ActiveTickFeed/include/Shared/ATServerAPIDefines.h>
#include <QObject>
#include <map>
#include <Data/InstrumentData.h>

class DataSubscriber : public QObject
{
    private:
        std::map<InstrumentId, bool> _subscriptions;

    public:
         DataSubscriber();
         ~DataSubscriber();

    private:
         void setSubscription(const InstrumentId);
         void cancelMarketDataSubscription(const InstrumentId);

    public:
        const bool isSubscribed(const InstrumentId);
        void subscribeMarketData(const InstrumentContract&, const DataSource source = ActiveTick);
        void subscribeMarketData(const InstrumentId, const DataSource source = ActiveTick);
        void stopMarketData(const InstrumentId);
        void unSubscribeMarketData(const InstrumentId);

    public slots:
//        virtual void onTradeUpdate(const TickerId, const TradeUpdate&);
//        virtual void onQuoteUpdate(const TickerId, const QuoteUpdate&);
        virtual void onExecutionUpdate(const InstrumentId, const Execution&);
        virtual void onTickPriceUpdate(const InstrumentId, const TickType, const double);
        virtual void onSnapshotUpdate(const InstrumentId, const double, const int);
//        virtual void updateOneMinuteSnapShot(const TickerId, const double);
//        virtual void updateTwoMinuteSnapShot(const TickerId, const double);
//        virtual void updateFiveMinuteSnapShot(const TickerId, const double);
//        virtual void updateTenMinuteSnapShot(const TickerId, const double);
};

#endif // DATASUBSCRIBER_H
