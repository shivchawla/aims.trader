#pragma once
#include <stdint.h>

#pragma pack(push)
#pragma pack(1)


#ifdef _WIN32
	#ifdef ACTIVETICKSERVERAPI_EXPORTS
		#define ACTIVETICKSERVERAPI_API __declspec(dllexport)
	#else
		#define ACTIVETICKSERVERAPI_API __declspec(dllimport)
	#endif

	#define wchar16_t wchar_t

#else
	#ifdef __GNUC__
		#define ACTIVETICKSERVERAPI_API __attribute__ ((visibility("default")))
	#else
		#define ACTIVETICKSERVERAPI_API		
	#endif

	#define wchar16_t uint16_t
#endif

static const int32_t ATFundamentalNameMaxLength = 100;
static const int32_t ATSymbolMaxLength = 11;
static const int32_t ATMarketMoversMaxRecords = 15;

static const int32_t ATTradeConditionsCount = 4;

//ActiveTick enums
enum ATSessionStatusType
{
	SessionStatusDisconnected = 1,
	SessionStatusDisconnectedDuplicateLogin,
	SessionStatusConnected	
};

enum ATLoginResponseType
{
	LoginResponseSuccess = 1,
	LoginResponseInvalidUserid,
	LoginResponseInvalidPassword,
	LoginResponseInvalidRequest,
	LoginResponseLoginDenied,
	LoginResponseServerError
};

enum ATPermissionType
{
	PermissionExchangeNYSE,
	PermissionExchangeNASDAQ,
	PermissionExchangeAMEX,
	PermissionExchangeFOREX,
	PermissionLogin = 10,
	PermissionQuoteDb,
	PermissionQuoteStream,
	PermissionHistoryDb,
	PermissionMarketMoversDb,
	PermissionMarketMoversStream
};

enum ATMarketMoversDbResponseType
{
	MarketMoversDbResponseSuccess = 1,
	MarketMoversDbResponseInvalidRequest,
	MarketMoversDbResponseDenied
};

enum ATQuoteDbResponseType
{
	QuoteDbResponseSuccess = 1,
	QuoteDbResponseInvalidRequest,
	QuoteDbResponseDenied
};

enum ATSymbolStatus
{
	SymbolStatusSuccess = 1,
	SymbolStatusInvalid,
	SymbolStatusUnavailable,
	SymbolStatusNoPermission
};

enum ATFieldStatus
{
	FieldStatusSuccess = 1,
	FieldStatusInvalid,
	FieldStatusUnavailable,
	FieldStatusDenied
};

enum ATQuoteFieldType
{	
	QuoteFieldSymbol = 1,
	QuoteFieldOpenPrice,		
	QuoteFieldPreviousClosePrice,
	QuoteFieldClosePrice,
	QuoteFieldLastPrice,
	QuoteFieldBidPrice,
	QuoteFieldAskPrice,
	QuoteFieldHighPrice,
	QuoteFieldLowPrice,
	QuoteFieldDayHighPrice,
	QuoteFieldDayLowPrice,
	QuoteFieldPreMarketOpenPrice,
	QuoteFieldExtendedHoursLastPrice,
	QuoteFieldAfterMarketClosePrice,
	QuoteFieldBidExchange,
	QuoteFieldAskExchange,
	QuoteFieldLastExchange,
	QuoteFieldLastCondition,
	QuoteFieldQuoteCondition,
	QuoteFieldLastTradeDateTime,
	QuoteFieldLastQuoteDateTime,
	QuoteFieldDayHighDateTime,
	QuoteFieldDayLowDateTime,
	QuoteFieldLastSize,
	QuoteFieldBidSize,
	QuoteFieldAskSize,
	QuoteFieldVolume,
	QuoteFieldPreMarketVolume,
	QuoteFieldAfterMarketVolume,
	QuoteFieldTradeCount,
	QuoteFieldPreMarketTradeCount,
	QuoteFieldAfterMarketTradeCount,
	QuoteFieldFundamentalEquityName,
	QuoteFieldFundamentalEquityPrimaryExchange
};

enum ATDataType
{
	DataByte = 1,
	DataByteArray,
	DataUInteger32,
	DataUInteger64,
	DataInteger32,
	DataInteger64,	
	DataDouble,
	DataString,
	DataUnicodeString,
	DataDateTime
};

enum ATSymbolType 
{ 
	SymbolStock = 'S', 
	SymbolIndex = 'I',
	SymbolStockOption = 'O', 
	SymbolBond = 'B',
	SymbolMutualFund = 'M',
	SymbolTopMarketMovers = 'T',
	SymbolCurrency = 'C'
};

enum ATExchangeType 
{
	ExchangeAMEX = 'A',
	ExchangeNasdaqOmxBx = 'B',
	ExchangeNationalStockExchange = 'C',
	ExchangeFinraAdf = 'D',
	ExchangeCQS = 'E',
	ExchangeForex = 'F',
	ExchangeInternationalSecuritiesExchange = 'I',
	ExchangeEdgaExchange = 'J',
	ExchangeEdgxExchange = 'K',
	ExchangeChicagoStockExchange = 'M',
	ExchangeNyseEuronext = 'N',
	ExchangeNyseArcaExchange = 'P',
	ExchangeNasdaqOmx = 'Q',
	ExchangeCTS = 'S',
	ExchangeCTANasdaqOMX = 'T',
	ExchangeOTCBB = 'U',
	ExchangeNNOTC = 'u',
	ExchangeChicagoBoardOptionsExchange = 'W',
	ExchangeNasdaqOmxPhlx = 'X',
	ExchangeBatsYExchange = 'Y',
	ExchangeBatsExchange = 'Z',
	ExchangeComposite = ' '		
};

enum ATCountryType
{
	CountryInternational = 'I',
	CountryUnitedStates = 'U'
};

enum ATTradeMessageFlags
{
	TradeMessageFlagRegularMarketLastPrice = 0x1,
	TradeMessageFlagRegularMarketVolume = 0x2,
	
	TradeMessageFlagHighPrice = 0x4,
	TradeMessageFlagLowPrice = 0x8,
	TradeMessageFlagDayHighPrice = 0x10,
	TradeMessageFlagDayLowPrice = 0x20,		

	TradeMessageFlagExtendedMarketLastPrice = 0x40,
	TradeMessageFlagPreMarketVolume = 0x80,		
	TradeMessageFlagAfterMarketVolume = 0x100,
	TradeMessageFlagPreMarketOpenPrice = 0x200,
	TradeMessageFlagOpenPrice = 0x400	
};

enum ATTradeConditionType
{
	TradeConditionRegular,
	TradeConditionAcquisition,
	TradeConditionAveragePrice,
	TradeConditionAutomaticExecution,
	TradeConditionBunched,
	TradeConditionBunchSold,
	TradeConditionCAPElection,
	TradeConditionCash,
	TradeConditionClosing,
	TradeConditionCross,
	TradeConditionDerivativelyPriced,
	TradeConditionDistribution,
	TradeConditionFormT,
	TradeConditionFormTOutOfSequence,
	TradeConditionInterMarketSweep,
	TradeConditionMarketCenterOfficialClose,
	TradeConditionMarketCenterOfficialOpen,
	TradeConditionMarketCenterOpening,
	TradeConditionMarketCenterReOpenning,
	TradeConditionMarketCenterClosing,
	TradeConditionNextDay,
	TradeConditionPriceVariation,
	TradeConditionPriorReferencePrice,
	TradeConditionRule155Amex,
	TradeConditionRule127Nyse,
	TradeConditionOpening,
	TradeConditionOpened,
	TradeConditionRegularStoppedStock,
	TradeConditionReOpening,
	TradeConditionSeller,
	TradeConditionSoldLast,
	TradeConditionSoldLastStoppedStock,
	TradeConditionSoldOutOfSequence,
	TradeConditionSoldOutOfSequenceStoppedStock,
	TradeConditionSplit,
	TradeConditionStockOption,            
	TradeConditionYellowFlag
};

enum ATQuoteConditionType
{
    QuoteConditionRegular,
	QuoteConditionRegularTwoSidedOpen,
	QuoteConditionRegularOneSidedOpen,
	QuoteConditionSlowAsk,
	QuoteConditionSlowBid,
	QuoteConditionSlowBidAsk,
	QuoteConditionSlowDueLRPBid,
	QuoteConditionSlowDueLRPAsk,
	QuoteConditionSlowDueNYSELRP,
	QuoteConditionSlowDueSetSlowListBidAsk,
	QuoteConditionManualAskAutomaticBid,
	QuoteConditionManualBidAutomaticAsk,
	QuoteConditionManualBidAndAsk,
	QuoteConditionOpening,
	QuoteConditionClosing,
	QuoteConditionClosed,
	QuoteConditionResume,
	QuoteConditionFastTrading,
	QuoteConditionTradingRangeIndication,
	QuoteConditionMarketMakerQuotesClosed,
	QuoteConditionNonFirm,
	QuoteConditionNewsDissemination,
	QuoteConditionOrderInflux,
	QuoteConditionOrderImbalance,
	QuoteConditionDueToRelatedSecurityNewsDissemination,
	QuoteConditionDueToRelatedSecurityNewsPending,
	QuoteConditionAdditionalInformation,
	QuoteConditionNewsPending,
	QuoteConditionAdditionalInformationDueToRelatedSecurity,
	QuoteConditionDueToRelatedSecurity,
	QuoteConditionInViewOfCommon,
	QuoteConditionEquipmentChangeover,
	QuoteConditionNoOpenNoResume,
	QuoteConditionSubPennyTrading,
	QuoteConditionAutomatedBidNoOfferNoBid
};

enum ATStreamRequestType
{
	StreamRequestSubscribe = 1,
	StreamRequestUnsubscribe
};

enum ATStreamResponseType
{
	StreamResponseSuccess = 1,
	StreamResponseInvalidRequest,
	StreamResponseDenied
};

enum ATStreamUpdateType
{
	StreamUpdateTrade = 1,
	StreamUpdateQuote,
	StreamUpdateRefresh,
	StreamUpdateTopMarketMovers
};

enum ATBarHistoryType
{
	BarHistoryIntraday,
	BarHistoryDaily,
	BarHistoryWeekly
};

enum ATBarHistoryResponseType
{
	BarHistoryResponseSuccess = 1,
	BarHistoryResponseInvalidRequest,
	BarHistoryResponseMaxLimitReached,
	BarHistoryResponseDenied
};

enum ATTickHistoryRecordType 
{ 
	TickHistoryRecordTrade = 1, 
	TickHistoryRecordQuote
};

enum ATTickHistoryResponseType
{
	TickHistoryResponseSuccess = 1,
	TickHistoryResponseInvalidRequest,
	TickHistoryResponseMaxLimitReached,
	TickHistoryResponseDenied
};

enum ATCursorType
{
	CursorForward = 1,
	CursorBackward
};

//ActiveTick structures
//

typedef struct _ATTIME
{
	uint16_t wYear;
	uint16_t wMonth;
	uint16_t wDayOfWeek;
	uint16_t wDay;
	uint16_t wHour;
	uint16_t wMinute;
	uint16_t wSecond;
	uint16_t wMilliseconds;
} ATTIME, *LPATTIME;

typedef struct _ATGUID
{
	uint32_t	Data1;
	uint16_t	Data2;
	uint16_t	Data3;
	uint8_t		Data4[8];
} ATGUID, *LPATGUID;

typedef struct _ATSYMBOL
{
	wchar16_t		symbol[ATSymbolMaxLength];
	uint8_t			symbolType;
	uint8_t			exchangeType;
	uint8_t			countryType;
} ATSYMBOL, *LPATSYMBOL;

typedef struct _ATPRICE
{
	double			price;
	uint8_t			precision;
} ATPRICE, *LPATPRICE;

typedef struct _ATLOGIN_RESPONSE
{
	ATLoginResponseType	loginResponse;
	uint8_t				permissions[255];
} ATLOGIN_RESPONSE, *LPATLOGIN_RESPONSE;

//HistoryDb structures
typedef struct _ATBARHISTORY_RESPONSE
{
	ATSYMBOL				symbol;
	ATSymbolStatus			status;
	uint32_t				recordCount;
} ATBARHISTORY_RESPONSE, *LPATBARHISTORY_RESPONSE;

typedef struct _ATBARHISTORY_RECORD
{
	ATPRICE					open;
	ATPRICE					high;
	ATPRICE					low;
	ATPRICE					close;
	uint64_t				volume;
	ATTIME					barTime;
} ATBARHISTORY_RECORD, *LPATBARHISTORY_RECORD;

typedef struct _ATTICKHISTORY_RESPONSE
{
	ATSYMBOL				symbol;
	ATSymbolStatus			status;
	uint32_t				recordCount;
} ATTICKHISTORY_RESPONSE, *LPATTICKHISTORY_RESPONSE;

typedef struct _ATTICKHISTORY_TRADE_RECORD
{
	ATTickHistoryRecordType	recordType;
	ATExchangeType			lastExchange;
	ATTradeConditionType	lastCondition[ATTradeConditionsCount];	
	ATPRICE					lastPrice;
	uint32_t				lastSize;
	ATTIME					lastDateTime;		
} ATTICKHISTORY_TRADE_RECORD, *LPATTICKHISTORY_TRADE_RECORD;

typedef struct _ATTICKHISTORY_QUOTE_RECORD
{
	ATTickHistoryRecordType	recordType;
	ATExchangeType			bidExchange;
	ATExchangeType			askExchange;
	ATQuoteConditionType	quoteCondition;
	ATPRICE					bidPrice;
	ATPRICE					askPrice;
	uint32_t				bidSize;
	uint32_t				askSize;
	ATTIME					quoteDateTime;	
} ATTICKHISTORY_QUOTE_RECORD, *LPATTICKHISTORY_QUOTE_RECORD;

typedef struct _ATTICKHISTORY_RECORD
{
	union
	{
		ATTickHistoryRecordType		recordType;
		ATTICKHISTORY_TRADE_RECORD	trade;
		ATTICKHISTORY_QUOTE_RECORD	quote;
	};
} ATTICKHISTORY_RECORD, *LPATTICKHISTORY_RECORD;

//Market holidays structures
typedef struct _ATMARKET_HOLIDAYSLIST_ITEM
{
	ATSymbolType		symbolType;
	ATTIME				beginDateTime;
	ATTIME				endDateTime;
} ATMARKET_HOLIDAYSLIST_ITEM, *LPATMARKET_HOLIDAYSLIST_ITEM;

//MarketMovers structures
typedef struct _ATMARKET_MOVERS_ITEM
{
	ATSYMBOL			symbol;
	wchar16_t			name[ATFundamentalNameMaxLength];
	ATPRICE				lastPrice;
	ATPRICE				closePrice;
	uint64_t			volume;
	ATTIME				lastDateTime;	
} ATMARKET_MOVERS_ITEM, *LPATMARKET_MOVERS_ITEM;

typedef struct _ATMARKET_MOVERS_RECORD
{
	ATSYMBOL				symbol;
	ATSymbolStatus			status;
	ATMARKET_MOVERS_ITEM	items[ATMarketMoversMaxRecords];
} ATMARKET_MOVERS_RECORD, *LPATMARKET_MOVERS_RECORD;

typedef struct _ATMARKET_MOVERSDB_RESPONSE
{
	ATMarketMoversDbResponseType	responseType;
	uint32_t						marketMoversCount;
} ATMARKET_MOVERSDB_RESPONSE, *LPATMARKET_MOVERSDB_RESPONSE;

typedef struct _ATMARKET_MOVERS_STREAM_RESPONSE
{
	ATStreamResponseType		responseType;
} ATMARKET_MOVERS_STREAM_RESPONSE, *LPATMARKET_MOVERS_STREAM_RESPONSE;

typedef struct _ATMARKET_MOVERS_STREAM_UPDATE
{
	ATMARKET_MOVERS_RECORD		marketMovers;
	ATTIME						lastUpdateTime;
} ATMARKET_MOVERS_STREAM_UPDATE, *LPATMARKET_MOVERS_STREAM_UPDATE;

//QuoteDb structures
typedef struct _ATQUOTEDB_RESPONSE
{
	ATSYMBOL				symbol;
	ATSymbolStatus			status;
	uint16_t				dataItemCount;
} ATQUOTEDB_RESPONSE, *LPATQUOTEDB_RESPONSE; 

typedef struct _ATQUOTEDB_DATA_ITEM
{
	ATQuoteFieldType		fieldType;
	ATFieldStatus			fieldStatus;
	ATDataType				dataType;
} ATQUOTEDB_DATA_ITEM, *LPATQUOTEDB_DATA_ITEM;

//QuoteStream structures
typedef struct _ATQUOTESTREAM_RESPONSE
{
	ATStreamResponseType	responseType;
	uint16_t				dataItemCount;
} ATQUOTESTREAM_RESPONSE, *LPATQUOTESTREAM_RESPONSE; 

typedef struct _ATQUOTESTREAM_DATA_ITEM
{
	ATSYMBOL				symbol;
	ATSymbolStatus			symbolStatus;
} ATQUOTESTREAM_DATA_ITEM, *LPATQUOTESTREAM_DATA_ITEM;

typedef struct _ATQUOTESTREAM_TRADE_UPDATE
{
	ATSYMBOL				symbol;
	ATTradeMessageFlags		flags;
	ATTradeConditionType	condition[ATTradeConditionsCount];
	ATExchangeType			lastExchange;
	ATPRICE					lastPrice;
	uint32_t				lastSize;
	ATTIME					lastDateTime;
} ATQUOTESTREAM_TRADE_UPDATE, *LPATQUOTESTREAM_TRADE_UPDATE;

typedef struct _ATQUOTESTREAM_QUOTE_UPDATE
{
	ATSYMBOL				symbol;
	ATQuoteConditionType	condition;
	ATExchangeType			bidExchange;
	ATExchangeType			askExchange;
	ATPRICE					bidPrice;
	ATPRICE					askPrice;
	uint32_t				bidSize;
	uint32_t				askSize;
	ATTIME					quoteDateTime;
} ATQUOTESTREAM_QUOTE_UPDATE, *LPATQUOTESTREAM_QUOTE_UPDATE;

typedef struct _ATQUOTESTREAM_REFRESH_UPDATE
{
	ATSYMBOL				symbol;
	ATTradeConditionType	lastCondition[ATTradeConditionsCount];
    ATQuoteConditionType	quoteCondition;
	ATPRICE					openPrice;
    ATPRICE					lastPrice;
    ATPRICE					highPrice;
    ATPRICE					lowPrice;    
	ATPRICE					closePrice;
	ATPRICE					prevClosePrice;
	ATPRICE					afterMarketClosePrice;
	ATPRICE					bidPrice;
    ATPRICE					askPrice;     
    ATExchangeType			lastExchange;
    ATExchangeType			bidExchange;
    ATExchangeType			askExchange;            
    uint32_t				bidSize;
    uint32_t				askSize;
    uint32_t				lastSize;
    uint64_t				volume;	
} ATQUOTESTREAM_REFRESH_UPDATE, *LPATQUOTESTREAM_REFRESH_UPDATE;

typedef struct _ATSTREAM_UPDATE
{
	ATStreamUpdateType	updateType;

	union
	{
		ATQUOTESTREAM_TRADE_UPDATE		trade;
		ATQUOTESTREAM_QUOTE_UPDATE		quote;
		ATQUOTESTREAM_REFRESH_UPDATE	refresh;
		ATMARKET_MOVERS_STREAM_UPDATE	marketMovers;
	};
} ATSTREAM_UPDATE, *LPATSTREAM_UPDATE; 

#pragma pack(pop)
