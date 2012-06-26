#include "Platform/Utils/DataSubscriber.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

DataSubscriber::DataSubscriber():QObject()
{}

DataSubscriber::~DataSubscriber()
{}

void DataSubscriber::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
{}

void DataSubscriber::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
{}

void DataSubscriber::onExecutionUpdate(const TickerId, const Execution& execution)
{}

void DataSubscriber::onTickPriceUpdate(const TickerId, const TickType, const double)
{}

void DataSubscriber::onSnapshotUpdate(const TickerId, const double, const int)
{}

//void DataSubscriber::updateOneMinuteSnapShot(const TickerId, const double)
//{}

//void DataSubscriber::updateTwoMinuteSnapShot(const TickerId, const double)
//{}

//void DataSubscriber::updateFiveMinuteSnapShot(const TickerId, const double)
//{}

//void DataSubscriber::updateTenMinuteSnapShot(const TickerId, const double)
//{}

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
     Service::Instance()->getInstrumentManager()->unSubscribeMarketData(tickerId, this);
    _subscriptions.erase(tickerId);

}

///Request MKT data for given contract
void DataSubscriber::subscribeMarketData(const ATContract& aTcontract, const DataSource source)
{
    TickerId tickerId = Service::Instance()->getInstrumentManager()->getTickerId(aTcontract);
    if(!isSubscribed(tickerId))
    {
        setSubscription(tickerId);
        Service::Instance()->getInstrumentManager()->requestMarketData(aTcontract, this, source);
    }
}

///Request MKT data for given tickerId
void DataSubscriber::subscribeMarketData(const TickerId tickerId, const DataSource source)
{
    if(!isSubscribed(tickerId))
    {
        setSubscription(tickerId);
        Service::Instance()->getInstrumentManager()->requestMarketData(tickerId, this, source);
    }
}

void DataSubscriber::stopMarketData(const TickerId tickerId)
{
    Service::Instance()->getInstrumentManager()->cancelMarketData(tickerId);
    cancelMarketDataSubscription(tickerId);
}

void DataSubscriber::unSubscribeMarketData(const TickerId tickerId)
{
    cancelMarketDataSubscription(tickerId);
}



