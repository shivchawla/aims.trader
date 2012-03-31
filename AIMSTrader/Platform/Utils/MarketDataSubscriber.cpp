#include "Platform/Utils/MarketDataSubscriber.h"

MarketDataSubscriber::MarketDataSubscriber():QObject()
{}

MarketDataSubscriber::~MarketDataSubscriber()
{}

void MarketDataSubscriber::onTradeUpdate(const TickerId tickerId, TradeUpdate pTradeUpdate)
{
int x=1;
}

void MarketDataSubscriber::onQuoteUpdate(const TickerId tickerId, QuoteUpdate pQuoteUpdate)
{
 int x=1;
}
