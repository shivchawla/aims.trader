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
        std::map<TickerId, bool> _subscriptions;

    public:
         DataSubscriber();
         ~DataSubscriber();

    private:
         void setSubscription(const TickerId);
         void cancelMarketDataSubscription(const TickerId);

    public:
        const bool IsSubscribed(const TickerId);
        void subscribeMarketData(const InstrumentContract&, const DataSource source = ActiveTick, const DataRequestType=RealTime);
        void subscribeMarketData(const TickerId, const DataSource source = ActiveTick,const DataRequestType=RealTime);
        void subscribeMarketData(const InstrumentId, const DataSource source = ActiveTick,const DataRequestType=RealTime);

        void stopMarketData(const TickerId);
        void unSubscribeMarketData(const TickerId);

    public slots:
//        virtual void onTradeUpdate(const TickerId, const TradeUpdate&);
//        virtual void onQuoteUpdate(const TickerId, const QuoteUpdate&);
        virtual void onExecutionUpdate(const TickerId, const Execution&);
        virtual void onTickPriceUpdate(const TickerId, const TickType, const double);
        virtual void onSnapshotUpdate(const TickerId, const double, const int);
//        virtual void updateOneMinuteSnapShot(const TickerId, const double);
//        virtual void updateTwoMinuteSnapShot(const TickerId, const double);
//        virtual void updateFiveMinuteSnapShot(const TickerId, const double);
//        virtual void updateTenMinuteSnapShot(const TickerId, const double);
};

#endif // DATASUBSCRIBER_H
