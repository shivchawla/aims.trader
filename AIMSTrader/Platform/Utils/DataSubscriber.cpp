#include "Platform/Utils/DataSubscriber.h"
#include "Platform/Startup/Service.h"
#include "Platform/Trader/InstrumentManager.h"

DataSubscriber::DataSubscriber():QObject()
{}

DataSubscriber::~DataSubscriber()
{}

//void DataSubscriber::onTradeUpdate(const TickerId tickerId, const TradeUpdate& tradeUpdate)
//{}

//void DataSubscriber::onQuoteUpdate(const TickerId tickerId, const QuoteUpdate& quoteUpdate)
//{}

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

const bool DataSubscriber::isSubscribed(const InstrumentId instrumentId)
{
    if(_subscriptions.count(instrumentId)!=0)
    {
        return true;
    }

    return false;
}

void DataSubscriber::setSubscription(const InstrumentId instrumentId)
{
    _subscriptions[instrumentId] = true;
}

void DataSubscriber::cancelMarketDataSubscription(const InstrumentId instrumentId)
{
     Service::service().getInstrumentManager()->unSubscribeMarketData(instrumentId, this);
    _subscriptions.erase(instrumentId);

}

///Request MKT data for given contract
void DataSubscriber::subscribeMarketData(const InstrumentContract& instrumentContract, const DataSource source)
{
    if(!isSubscribed(instrumentContract.instrumentId))
    {
        setSubscription(instrumentContract.instrumentId);
        Service::service().getInstrumentManager()->requestMarketData(instrumentContract, this, source);
    }
}

///Request MKT data for given tickerId
void DataSubscriber::subscribeMarketData(const InstrumentId instrumentId, const DataSource source)
{
    if(!isSubscribed(instrumentId))
    {
        setSubscription(instrumentId);
        Service::service().getInstrumentManager()->requestMarketData(instrumentId, this, source);
    }
}

void DataSubscriber::stopMarketData(const InstrumentId instrumentId)
{
    Service::service().getInstrumentManager()->cancelMarketData(instrumentId);
    cancelMarketDataSubscription(instrumentId);
}

void DataSubscriber::unSubscribeMarketData(const InstrumentId instrumentId)
{
    cancelMarketDataSubscription(instrumentId);
}



