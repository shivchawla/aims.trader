#include "Platform/Utils/DataSubscriber.h"

DataSubscriber::DataSubscriber():QObject()
{}

DataSubscriber::~DataSubscriber()
{}

void DataSubscriber::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
{}

void DataSubscriber::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
{}

void DataSubscriber::onExecutionUpdate(const OrderId, const Execution& execution)
{}

void DataSubscriber::onTickPriceUpdate(const TickerId, const TickType, const double)
{}

void DataSubscriber::updateOneMinuteSnapShot(const TickerId, const double)
{}

void DataSubscriber::updateTwoMinuteSnapShot(const TickerId, const double)
{}

void DataSubscriber::updateFiveMinuteSnapShot(const TickerId, const double)
{}

void DataSubscriber::updateTenMinuteSnapShot(const TickerId, const double)
{}

const bool DataSubscriber::isSubscribed(const TickerId tickerId)
{
    if(_subscriptions.count(tickerId)!=0)
    {
        return true;
    }
    return false;
}

void DataSubscriber::setSubscription(const TickerId tickerId)
{
    _subscriptions[tickerId] = true;
}

void DataSubscriber::cancelMarketDataSubscription(const TickerId tickerId)
{
    _subscriptions.erase(tickerId);
}
