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
