#pragma once
#include <ActiveTickStreamListener.h>
#include "ActiveTickFeed/Utils/Requestor.h"
#include "ActiveTickFeed/Utils/Session.h"
namespace ActiveTickFeed{

class Streamer :	public ActiveTickStreamListener,
					public Requestor
{
public:
    Streamer(const APISession* session);
	virtual ~Streamer(void);

private:
	virtual void	OnATStreamTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pUpdate);
	virtual void	OnATStreamQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pUpdate);
	virtual void	OnATStreamTopMarketMoversUpdate(LPATMARKET_MOVERS_STREAM_UPDATE pUpdate);
};
}
