#pragma once

#include "ActiveTickServerAPI.h"

#include <map>
#include <string>

namespace boost { class recursive_mutex; }

/**
* ActiveTickServerRequestor class. 
* This is used as generic requestor for all ActiveTick's API requests. Extend this class and override its virtual functions to easily receive various notifications.
* @note In order to receive stream notifications, derived class must also extend from ActiveTickStreamListener class.
* @see ActiveTickStreamListener
*/
class ACTIVETICKSERVERAPI_API ActiveTickServerRequestor
{
	friend ACTIVETICKSERVERAPI_API bool ATInitAPI();
	friend ACTIVETICKSERVERAPI_API bool	ATShutdownAPI();

public:
	/**
	* ActiveTickServerRequestor constructor.
	* @param session Session handle created by ATCreateSession function.
	* @see ATCreateSession
	*/
	ActiveTickServerRequestor(uint64_t session);

	/**
	* ActiveTickServerRequestor destructor.
	*/
	virtual ~ActiveTickServerRequestor(void);

public:
	/**
	* Closes all outstanding requests.
	* @return uint32_t Number of requests closed.
	*/
	uint32_t			CloseAllATRequests();

	/**
	* Close specific request.
	* @param request Request handle for which to close the request.
	* @return true if successfully closed the request, false otherwise.
	*/
	bool				CloseATRequest(uint64_t request);
	
	/**
	* Send bar history request.
	* @return Handle to newly created request.
	* @see ATCreateBarHistoryDbRequest	
	*/
	uint64_t			SendATBarHistoryDbRequest(const ATSYMBOL& symbol, ATBarHistoryType barHistoryType, uint8_t intradayMinuteCompression, const ATTIME& beginDateTime, 
													const ATTIME& endDateTime, uint32_t timeout);

	/**
	* Send bar history request.
	* @return Handle to newly created request.
	* @see ATCreateBarHistoryDbRequest	
	*/
	uint64_t			SendATBarHistoryDbRequest(const ATSYMBOL& symbol, ATBarHistoryType barHistoryType, uint8_t intradayMinuteCompression, uint32_t recordsWanted, uint32_t timeout);

	/**
	* Send bar history request.
	* @return Handle to newly created request.
	* @see ATCreateBarHistoryDbRequest	
	*/
	uint64_t			SendATBarHistoryDbRequest(const ATSYMBOL& symbol, ATBarHistoryType barHistoryType, uint8_t intradayMinuteCompression, const ATTIME& beginDateTime, 
													uint32_t recordsWanted, ATCursorType cursorType, uint32_t timeout);

	/**
	* Send login request.
	* @return Handle to newly created request.
	* @see ATCreateLoginRequest	
	*/
	uint64_t			SendATLoginRequest(const wchar16_t* userid, const wchar16_t* password, uint32_t timeout);

	/**
	* Send market holidays list request.
	* @return Handle to newly created request.
	* @see ATCreateMarketHolidaysRequest	
	*/
	uint64_t			SendATMarketHolidaysRequest(uint8_t yearsGoingBack, uint8_t yearsGoingForward, uint32_t timeout);

	/**
	* Send market movers snapshot request.
	* @return Handle to newly created request.
	* @see ATCreateMarketMoversDbRequest	
	*/
	uint64_t			SendATMarketMoversDbRequest(const LPATSYMBOL symbols, uint16_t symbolCount, uint32_t timeout);

	/**
	* Send market movers stream request.
	* @note In order to receive stream notifications, extended derived class from ActiveTickStreamListener, and override necessary virtual methods.
	* @return Handle to newly created request.
	* @see ATCreateMarketMoversStreamRequest	
	* @see ActiveTickStreamListener
	*/
	uint64_t			SendATMarketMoversStreamRequest(const ATSYMBOL& symbol, ATStreamRequestType requestType, uint32_t timeout);	

	/**
	* Send quote snapshot request.
	* @return Handle to newly created request.
	* @see ATCreateQuoteDbRequest	
	*/
	uint64_t			SendATQuoteDbRequest(const LPATSYMBOL symbols, uint16_t symbolCount, const ATQuoteFieldType* fields, uint16_t fieldCount, uint32_t timeout);	

	/**
	* Send quote stream request.
	* @return Handle to newly created request.
	* @note In order to receive stream notifications, extended derived class from ActiveTickStreamListener, and override necessary virtual methods.
	* @see ATCreateQuoteStreamRequest	
	* @see ActiveTickStreamListener
	*/
	uint64_t			SendATQuoteStreamRequest(const LPATSYMBOL symbols, uint16_t symbolCount, ATStreamRequestType requestType, uint32_t timeout);	

	/**
	* Send tick history data request.
	* @return Handle to newly created request.
	* @see ATCreateTickHistoryDbRequest	
	*/
	uint64_t			SendATTickHistoryDbRequest(const ATSYMBOL& symbol, bool selectTrades, bool selectQuotes, const ATTIME& beginDateTime, const ATTIME& endDateTime, uint32_t timeout);

	/**
	* Send tick history data request.
	* @return Handle to newly created request.
	* @see ATCreateTickHistoryDbRequest	
	*/
	uint64_t			SendATTickHistoryDbRequest(const ATSYMBOL& symbol, bool selectTrades, bool selectQuotes, uint32_t recordsWanted, uint32_t timeout);

	/**
	* Send tick history data request.
	* @return Handle to newly created request.
	* @see ATCreateTickHistoryDbRequest	
	*/
	uint64_t			SendATTickHistoryDbRequest(const ATSYMBOL& symbol, bool selectTrades, bool selectQuotes, const ATTIME& beginDateTime, uint32_t recordsWanted, ATCursorType cursorType, uint32_t timeout);

protected:

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATBarHistoryResponseCallback
	*/
	virtual void		OnATBarHistoryDbResponse(uint64_t hOrigRequest, ATBarHistoryResponseType responseType, LPATBARHISTORY_RESPONSE pResponse) {}

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATLoginResponseCallback
	*/
	virtual void		OnATLoginResponse(uint64_t hOrigRequest, LPATLOGIN_RESPONSE pResponse) {}

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATMarketHolidaysResponseCallback
	*/
	virtual void		OnAtMarketHolidaysResponse(uint64_t hOrigRequest, LPATMARKET_HOLIDAYSLIST_ITEM pItems, uint32_t itemsCount) {}

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATMarketMoversDbResponseCallback
	*/
	virtual void		OnATMarketMoversDbResponse(uint64_t hOrigRequest, ATMarketMoversDbResponseType responseType, LPATMARKET_MOVERSDB_RESPONSE pResponse) {}

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATMarketMoversStreamResponseCallback
	*/
	virtual void		OnATMarketMoversStreamResponse(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATMARKET_MOVERS_STREAM_RESPONSE pResponse) {}

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATQuoteDbResponseCallback
	*/
	virtual void		OnATQuoteDbResponse(uint64_t hOrigRequest, ATQuoteDbResponseType responseType, LPATQUOTEDB_RESPONSE pResponse, uint32_t responseCount) {}

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATQuoteStreamResponseCallback
	*/
	virtual void		OnATQuoteStreamResponse(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATQUOTESTREAM_RESPONSE pResponse, uint32_t responseCount) {}

	/**
	* Virtual override callback that will be called when time out occurs.
	* @see ATRequestTimeoutCallback
	*/
	virtual void		OnATRequestTimeout(uint64_t hOrigRequest) {}

	/**
	* Virtual override callback that will be called once response has been received from the server.
	* @see ATTickHistoryResponseCallback
	*/
	virtual void		OnATTickHistoryDbResponse(uint64_t hOrigRequest, ATTickHistoryResponseType responseType, LPATTICKHISTORY_RESPONSE pResponse) {}	

private:
	static void 		ATBarHistoryDbResponseCallback(uint64_t hOrigRequest, ATBarHistoryResponseType responseType, LPATBARHISTORY_RESPONSE pResponse);
	static void 		ATLoginResponseCallback(uint64_t hSession, uint64_t hRequest, LPATLOGIN_RESPONSE pResponse);
	static void 		ATMarketHolidaysResponseCallback(uint64_t hOrigRequest, LPATMARKET_HOLIDAYSLIST_ITEM pItems, uint32_t itemsCount);
	static void 		ATMarketMoversDbResponseCallback(uint64_t hOrigRequest, ATMarketMoversDbResponseType responseType, LPATMARKET_MOVERSDB_RESPONSE pResponse);
	static void 		ATMarketMoversStreamResponseCallback(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATMARKET_MOVERS_STREAM_RESPONSE pResponse);
	static void 		ATQuoteDbResponseCallback(uint64_t hOrigRequest, ATQuoteDbResponseType responseType, LPATQUOTEDB_RESPONSE pResponse, uint32_t responseCount);
	static void 		ATQuoteStreamResponseCallback(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATQUOTESTREAM_RESPONSE pResponse, uint32_t responseCount);
	static void 		ATRequestTimeoutCallback(uint64_t hOrigRequest);
	static void 		ATTickHistoryResponseCallback(uint64_t hOrigRequest, ATTickHistoryResponseType responseType, LPATTICKHISTORY_RESPONSE pResponse);	
	
private:
	static void			Init();
	static void			Destroy();

private:
	uint64_t			m_hSession;
	
	static std::map<uint64_t, ActiveTickServerRequestor*>*	s_mapRequests;
	static boost::recursive_mutex*							s_mutexRequests;
};
