#ifndef STREAMER_H
#define STREAMER_H

#include "ActiveTickStreamListener.h"
#include "Requestor.h"
#include "Session.h"

//class InboundService;

class Streamer :	public ActiveTickStreamListener,
					public Requestor
{
public:
    Streamer(const APISession& session);
	virtual ~Streamer(void);

private:
	virtual void	OnATStreamTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pUpdate);
	virtual void	OnATStreamQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pUpdate);
	virtual void	OnATStreamTopMarketMoversUpdate(LPATMARKET_MOVERS_STREAM_UPDATE pUpdate);
};

#endif //STREAMER_H
