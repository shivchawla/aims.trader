#pragma once
#include <ActiveTickServerRequestor.h>
#include "ActiveTickFeed/Utils/Session.h"

namespace ActiveTickFeed{

class Requestor : public ActiveTickServerRequestor
{
public:
    Requestor(const APISession* session);
	virtual ~Requestor(void);

private:
	// ActiveTickServerRequestor's virtual overrides
	virtual void		OnATBarHistoryDbResponse(uint64_t hOrigRequest, ATBarHistoryResponseType responseType, LPATBARHISTORY_RESPONSE pResponse);
	virtual void		OnATTickHistoryDbResponse(uint64_t hOrigRequest, ATTickHistoryResponseType responseType, LPATTICKHISTORY_RESPONSE pResponse);

	virtual void		OnATMarketMoversDbResponse(uint64_t hOrigRequest, ATMarketMoversDbResponseType responseType, LPATMARKET_MOVERSDB_RESPONSE pResponse);
	virtual void		OnATQuoteDbResponse(uint64_t hOrigRequest, ATQuoteDbResponseType responseType, LPATQUOTEDB_RESPONSE pResponse, uint32_t responseCount);

	virtual void		OnATMarketMoversStreamResponse(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATMARKET_MOVERS_STREAM_RESPONSE pResponse);
	virtual void		OnATQuoteStreamResponse(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATQUOTESTREAM_RESPONSE pResponse, uint32_t responseCount);

	virtual void		OnATRequestTimeout(uint64_t hOrigRequest);
};
}
