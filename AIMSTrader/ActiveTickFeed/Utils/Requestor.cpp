#include "ActiveTickFeed/Utils/StdAfx.h"
#include "ActiveTickFeed/Utils/Helper.h"
#include "ActiveTickFeed/Utils/Requestor.h"
#include <../API/ActiveTickFeed/include/ATBarHistoryDbResponseParser.h>
#include <../API/ActiveTickFeed/include/ATMarketMoversDbResponseParser.h>
#include <../API/ActiveTickFeed/include/ATQuoteDbResponseParser.h>
#include <../API/ActiveTickFeed/include/ATQuoteStreamResponseParser.h>
#include <../API/ActiveTickFeed/include/ATTickHistoryDbResponseParser.h>

#include <stdio.h>
#include <string>
#include <string.h>
#include "Platform/Startup/Service.h"

using namespace std;

namespace ActiveTickFeed{

Requestor::Requestor(const APISession* session)
: ActiveTickServerRequestor(session->GetSessionHandle())
{
}

Requestor::~Requestor(void)
{
}

/*virtual*/ void Requestor::OnATBarHistoryDbResponse(uint64_t hOrigRequest, ATBarHistoryResponseType responseType, LPATBARHISTORY_RESPONSE pResponse)
{
	string strResponseType;
	switch(responseType)
	{
	case BarHistoryResponseSuccess: strResponseType = "BarHistoryResponseSuccess"; break;
	case BarHistoryResponseInvalidRequest: strResponseType = "BarHistoryResponseInvalidRequest"; break;
	case BarHistoryResponseMaxLimitReached: strResponseType = "BarHistoryResponseMaxLimitReached"; break;
	case BarHistoryResponseDenied: strResponseType = "BarHistoryResponseDenied"; break;
	default: break;
	}

	printf("RECV (%llu): Bar history response [%s]\n--------------------------------------------------------------\n", (uint64_t)hOrigRequest, strResponseType.c_str());

	uint32_t index = 1;
	ATBarHistoryDbResponseParser parser(pResponse);
	if(parser.MoveToFirstRecord())
	{
		while(true)
		{
			ATTIME recordDateTime = parser.GetDateTime();

			printf("[%d/%d] [%0.2d/%0.2d/%0.4d %0.2d:%0.2d:%0.2d] [o:%0.*f h:%0.*f l:%0.*f c:%0.*f vol:%llu]\n",
				index++, parser.GetRecordCount(),
				recordDateTime.wMonth, recordDateTime.wDay, recordDateTime.wYear, recordDateTime.wHour, recordDateTime.wMinute, recordDateTime.wSecond,
				parser.GetOpen().precision, parser.GetOpen().price,
				parser.GetHigh().precision, parser.GetHigh().price,
				parser.GetLow().precision, parser.GetLow().price,
				parser.GetClose().precision, parser.GetClose().price,
				parser.GetVolume());

			if(parser.MoveToNextRecord() == false)
				break;
		}
	}

	printf("--------------------------------------------------------------\nTotal records:%d\n", parser.GetRecordCount());
}

/*virtual*/ void Requestor::OnATTickHistoryDbResponse(uint64_t hOrigRequest, ATTickHistoryResponseType responseType, LPATTICKHISTORY_RESPONSE pResponse)
{
	string strResponseType;
	switch(responseType)
	{
	case TickHistoryResponseSuccess: strResponseType = "TickHistoryResponseSuccess"; break;
	case TickHistoryResponseInvalidRequest: strResponseType = "TickHistoryResponseInvalidRequest"; break;
	case TickHistoryResponseMaxLimitReached: strResponseType = "TickHistoryResponseMaxLimitReached"; break;
	case TickHistoryResponseDenied: strResponseType = "TickHistoryResponseDenied"; break;
	default: break;
	}

	printf("RECV (%llu): Tick history response [%s]\n--------------------------------------------------------------\n", (uint64_t)hOrigRequest, strResponseType.c_str());

	uint32_t index = 1;
	ATTickHistoryDBResponseParser parser(pResponse);

	if(parser.MoveToFirstRecord())
	{
		while(true)
		{
			ATTIME recordDateTime = parser.GetRecordDateTime();

			switch(parser.GetRecordType())
			{
			case TickHistoryRecordTrade:
				printf("[%d/%d] [%0.2d/%0.2d/%0.4d %0.2d:%0.2d:%0.2d] TRADE [last:%0.*f lastsize:%d lastexch:%d cond:%d]\n",
					index++, parser.GetRecordCount(),
					recordDateTime.wMonth, recordDateTime.wDay, recordDateTime.wYear, recordDateTime.wHour, recordDateTime.wMinute, recordDateTime.wSecond,
					parser.GetTradeLastPrice().precision, parser.GetTradeLastPrice().price,
					parser.GetTradeLastSize(), parser.GetTradeLastExchange(), parser.GetTradeCondition(0));
				break;
			case TickHistoryRecordQuote:
				printf("[%d/%d] [%0.2d/%0.2d/%0.4d %0.2d:%0.2d:%0.2d] QUOTE [bid:%0.*f ask:%0.*f bidsize:%d asksize:%d bidexch:%d askexch:%d cond:%d]\n",
					index++, parser.GetRecordCount(),
					recordDateTime.wMonth, recordDateTime.wDay, recordDateTime.wYear, recordDateTime.wHour, recordDateTime.wMinute, recordDateTime.wSecond,
					parser.GetQuoteBidPrice().precision, parser.GetQuoteBidPrice().price,
					parser.GetQuoteAskPrice().precision, parser.GetQuoteAskPrice().price,
					parser.GetQuoteBidSize(), parser.GetQuoteAskSize(),
					parser.GetQuoteBidExchange(), parser.GetQuoteAskExchange(),
					parser.GetQuoteCondition());
				break;
            default: break;
			}

			if(parser.MoveToNextRecord() == false)
				break;
		}
	}

	printf("--------------------------------------------------------------\nTotal records:%d\n", parser.GetRecordCount());
}

/*virtual*/ void Requestor::OnATMarketMoversDbResponse(uint64_t hOrigRequest, ATMarketMoversDbResponseType responseType, LPATMARKET_MOVERSDB_RESPONSE pResponse)
{
	string strResponseType;
	switch(responseType)
	{
	case MarketMoversDbResponseSuccess: strResponseType = "MarketMoversDbResponseSuccess"; break;
	case MarketMoversDbResponseInvalidRequest: strResponseType = "MarketMoversDbResponseInvalidRequest"; break;
	case MarketMoversDbResponseDenied: strResponseType = "MarketMoversDbResponseDenied"; break;
	default: break;
	}

	printf("RECV (%llu): Market movers response [%s]\n--------------------------------------------------------------\n", (uint64_t)hOrigRequest, strResponseType.c_str());

	ATMarketMoversDbResponseParser parser(pResponse);

	if(parser.MoveToFirstRecord() == true)
	{
		while(true)
		{
			printf("Market movers symbol: %s\n------------------\n", Helper::ConvertString(parser.GetRecordSymbol()->symbol, _countof(parser.GetRecordSymbol()->symbol)).c_str());

			if(parser.MoveToFirstItem() == true)
			{
				while(true)
				{
					printf("symbol:%s last:%0.*f volume:%llu\n",
						Helper::ConvertString(parser.GetItemSymbol()->symbol, _countof(parser.GetItemSymbol()->symbol)).c_str(),
						parser.GetItemLastPrice().precision, parser.GetItemLastPrice().price,
						parser.GetItemVolume());

					if(parser.MoveToNextItem() == false)
						break;
				}

				if(parser.MoveToNextRecord() == false)
					break;
			}
		}
	}
}

/*virtual*/ void Requestor::OnATQuoteDbResponse(uint64_t hOrigRequest, ATQuoteDbResponseType responseType, LPATQUOTEDB_RESPONSE pResponse, uint32_t responseCount)
{
	string strResponseType;
	switch(responseType)
	{
	case QuoteDbResponseSuccess: strResponseType = "QuoteDbResponseSuccess"; break;
	case QuoteDbResponseInvalidRequest: strResponseType = "QuoteDbResponseInvalidRequest"; break;
	case QuoteDbResponseDenied: strResponseType = "QuoteDbResponseDenied"; break;
	default: break;
	}

	printf("RECV (%llu): QuoteDb response [%s]\n--------------------------------------------------------------\n", (uint64_t)hOrigRequest, strResponseType.c_str());

	ATQuoteDbResponseParser parser(pResponse, responseCount);

	if(parser.MoveToFirstResponse() == true)
	{
		while(true)
		{
			string symbolStatus;
			switch(parser.GetSymbolStatus())
			{
			case SymbolStatusSuccess: symbolStatus = "SymbolStatusSuccess"; break;
			case SymbolStatusInvalid: symbolStatus = "SymbolStatusInvalid"; break;
			case SymbolStatusUnavailable: symbolStatus = "SymbolStatusUnavailable"; break;
			case SymbolStatusNoPermission: symbolStatus = "SymbolStatusNoPermission"; break;
			default: break;
			}

			printf("Symbol:%s [status:%s]\n-------------------------\n", Helper::ConvertString(parser.GetSymbol()->symbol, _countof(parser.GetSymbol()->symbol)).c_str(), symbolStatus.c_str());

			if(parser.GetSymbolStatus() == SymbolStatusSuccess && parser.MoveToFirstDataItem() == true)
			{
				while(true)
				{
					char data[512] = {0};

					switch(parser.GetDataItemDataType())
					{
					case DataByte:
						sprintf(data, "%c", *(char*)parser.GetDataItemData());
						break;
					case DataByteArray:
						sprintf(data, "byte data");
						break;
					case DataUInteger32:
						sprintf(data, "%u", *(uint32_t*)parser.GetDataItemData());
						break;
					case DataUInteger64:
						sprintf(data, "%llu", *(uint64_t*)parser.GetDataItemData());
						break;
					case DataInteger32:
						sprintf(data, "%d", *(int32_t*)parser.GetDataItemData());
						break;
					case DataInteger64:
						sprintf(data, "%lld", *(int64_t*)parser.GetDataItemData());
						break;
					case DataDouble:
						{
							ATPRICE price = *(LPATPRICE)parser.GetDataItemData();
							sprintf(data, "%0.*f", price.precision, price.price);
						}
						break;
					case DataString:
						{
							char* pString = (char*)parser.GetDataItemData();
							strncpy(data, pString, sizeof(data) - 1);
						}
						break;
					case DataUnicodeString:
						{
							wchar16_t* pString = (wchar16_t*)parser.GetDataItemData();
							std::string s = Helper::ConvertString(pString, Helper::StringLength(pString));
							strncpy(data, s.c_str(), sizeof(data));
						}
						break;
					case DataDateTime:
						{
							LPATTIME pst = (LPATTIME)parser.GetDataItemData();
							sprintf(data, "%0.2d/%0.2d/%0.4d %0.2d:%0.2d:%0.2d",
								pst->wMonth, pst->wDay, pst->wYear, pst->wHour, pst->wMinute, pst->wSecond);
						}
						break;
                    default: break;
					}

					printf("\tATQuoteFieldType:%d\n\tATFieldStatus:%d\n\tATDataType:%d\n\tData:%s\n",
						parser.GetDataItemQuoteFieldType(),
						parser.GetDataItemFieldStatus(),
						parser.GetDataItemDataType(),
						data);

					if(parser.MoveToNextDataItem() == false)
						break;
				}
			}

			if(parser.MoveToNextResponse() == false)
				break;
		}
	}
}

/*virtual*/ void Requestor::OnATMarketMoversStreamResponse(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATMARKET_MOVERS_STREAM_RESPONSE pResponse)
{
	string strResponseType;
	switch(responseType)
	{
	case StreamResponseSuccess: strResponseType = "StreamResponseSuccess"; break;
	case StreamResponseInvalidRequest: strResponseType = "StreamResponseInvalidRequest"; break;
	case StreamResponseDenied: strResponseType = "StreamResponseDenied"; break;
	default: break;
	}

	printf("RECV (%llu): Market movers stream response [%s]\n--------------------------------------------------------------\n", (uint64_t)hOrigRequest, strResponseType.c_str());
}

/*virtual*/ void Requestor::OnATQuoteStreamResponse(uint64_t hOrigRequest, ATStreamResponseType responseType, LPATQUOTESTREAM_RESPONSE pResponse, uint32_t responseCount)
{
    Service::service().getActiveTickSession()->requestProcessed();
	string strResponseType;
	switch(responseType)
	{
	case StreamResponseSuccess: strResponseType = "StreamResponseSuccess"; break;
	case StreamResponseInvalidRequest: strResponseType = "StreamResponseInvalidRequest"; break;
	case StreamResponseDenied: strResponseType = "StreamResponseDenied"; break;
	default: break;
	}

	printf("RECV (%llu): Quote stream response [%s]\n--------------------------------------------------------------\n", (uint64_t)hOrigRequest, strResponseType.c_str());

	if(responseType == StreamResponseSuccess)
	{
		ATQuoteStreamResponseParser parser(pResponse);
		parser.MoveToBeginning();

		if(parser.MoveToFirstDataItem() == true)
		{
			while(true)
			{
				string symbolStatus;
				switch(parser.GetSymbolStatus())
				{
				case SymbolStatusSuccess: symbolStatus = "SymbolStatusSuccess"; break;
				case SymbolStatusInvalid: symbolStatus = "SymbolStatusInvalid"; break;
				case SymbolStatusUnavailable: symbolStatus = "SymbolStatusUnavailable"; break;
				case SymbolStatusNoPermission: symbolStatus = "SymbolStatusNoPermission"; break;
				default: break;
				}

				printf("\tsymbol:%s symbolStatus:%s\n", Helper::ConvertString(parser.GetSymbol()->symbol, _countof(parser.GetSymbol()->symbol)).c_str(), symbolStatus.c_str());

				if(parser.MoveToNextDataItem() == false)
					break;
			}
		}
	}
}

/*virtual*/ void Requestor::OnATRequestTimeout(uint64_t hOrigRequest)
{
	printf("(%llu): Request timed-out\n", (uint64_t)hOrigRequest);
}

}
