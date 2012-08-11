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

const bool DataSubscriber::IsSubscribed(const TickerId tickerId)
{
    if(_subscriptions.count(tickerId)!=0)
    {
        return true;
    }

    return false;
}

void DataSubscriber::setSubscription(const TickerId instrumentId)
{
    _subscriptions[instrumentId] = true;
}

void DataSubscriber::cancelMarketDataSubscription(const TickerId instrumentId)
{
     Service::service().getInstrumentManager()->unSubscribeMarketData(instrumentId, this);
    _subscriptions.erase(instrumentId);
}

///Request MKT data for given contract
void DataSubscriber::subscribeMarketData(const InstrumentContract& instrumentContract, const DataSource source,const DataRequestType requestType)
{
    TickerId tickerId = Service::service().getInstrumentManager()->getTickerId(instrumentContract.instrumentId);
    if(!IsSubscribed(tickerId))
    {
        setSubscription(instrumentContract.instrumentId);
        Service::service().getInstrumentManager()->requestMarketData(instrumentContract, this, source, requestType);
    }
}

///Request MKT data for given tickerId
void DataSubscriber::subscribeMarketData(const TickerId tickerId, const DataSource source, const DataRequestType requestType)
{
    if(!IsSubscribed(tickerId))
    {
        setSubscription(tickerId);
        Service::service().getInstrumentManager()->requestMarketData(tickerId, this, source, requestType);
    }
}

void DataSubscriber::subscribeMarketData(const InstrumentId instrumentId, const DataSource source,const DataRequestType requestType)
{
    TickerId tickerId = Service::service().getInstrumentManager()->getTickerId(instrumentId);
    if(!IsSubscribed(tickerId))
    {
        setSubscription(tickerId);
        Service::service().getInstrumentManager()->requestMarketData(tickerId, this, source, requestType);
    }
}

void DataSubscriber::stopMarketData(const TickerId instrumentId)
{
    Service::service().getInstrumentManager()->cancelMarketData(instrumentId);
    cancelMarketDataSubscription(instrumentId);
}

void DataSubscriber::unSubscribeMarketData(const TickerId instrumentId)
{
    cancelMarketDataSubscription(instrumentId);
}



