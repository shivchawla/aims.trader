#include "stdafx.h"
#include "Helper.h"
#include "Requestor.h"
#include "Session.h"
#include "Streamer.h"
#include <ActiveTickServerAPI.h>

#include <iostream>
#include <stdio.h>

using namespace std;

void PrintUsage()
{
	cout <<"Available commands:" << endl;
	cout <<"-------------------------------------------------------------" << endl;
	cout <<"?" << endl;
	cout <<"quit" << endl;

	cout <<"init [serverIpAddress] [serverPort] [apiUseridGuid] [userid] [password]" << endl;

	cout <<"getIntradayHistoryBars [symbol] [minutes] [beginTime] [endTime]" << endl;
	cout <<"getDailyHistoryBars [symbol] [beginTime] [endTime]" << endl;
	cout <<"getWeeklyHistoryBars [symbol] [beginTime] [endTime]" << endl;

	cout <<"getTicks [symbol] [beginTime] [endTime]" << endl;

	cout <<"getMarketMovers [symbol] [exchange]" << endl;

	cout <<"getQuoteDb [symbol]" << endl;

	cout <<"subscribeMarketMovers [symbol] [exchange]" << endl;
	cout <<"unsubscribeMarketMovers [symbol] [exchange]" << endl;

	cout <<"subscribeQuoteStream [symbol]" << endl;
	cout <<"unsubscribeQuoteStream [symbol]" << endl;

	cout <<"-------------------------------------------------------------" << endl;
	cout <<"Note: all date/time parameters must be entered in the following format: YYYYMMDDHHMMSS" << endl;
	cout <<"apiUseridGuid must be entered using full 32 characters, ex: EF1C0A768BBB11DFBCB3F923E0D72085" << endl;
	cout <<"-------------------------------------------------------------" << endl;
	cout.flush();
}

int _main()
{
	ATInitAPI();

	{
        APISession session;
        Requestor requestor(session);
		Streamer streamer(session);

		PrintUsage();

        string serverIpAddress, apiUserid, userid, password;
        serverIpAddress="activetick1.activetick.com";
        apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
        userid="shivchawla";
        password= "27as04sh";

		string cmd;
		while(true)
		{
			cin >> cmd;
			if(cmd == "?")
				PrintUsage();
			else if(cmd == "quit")
				break;
			else if(cmd == "init")
			{
                //string serverIpAddress, apiUserid, userid, password;
                uint32_t serverPort = 0;
                //cin >> serverIpAddress >> serverPort >> apiUserid >> userid >> password;
				
                ATGUID guidApiUserid = Helper::StringToATGuid(apiUserid);
				bool rc = session.Init(guidApiUserid, serverIpAddress, serverPort, &Helper::ConvertString(userid).front(), &Helper::ConvertString(password).front());
				printf("init status: %d\n", rc);
			}
			else if(cmd == "getIntradayHistoryBars")
			{
				string symbol;
				string beginTime, endTime;
				uint32_t minutes = 0;
				cin >> symbol >> minutes >> beginTime >> endTime;

				ATTIME atBeginTime = Helper::StringToATTime(beginTime);
				ATTIME atEndTime = Helper::StringToATTime(endTime);

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
				uint64_t hRequest = requestor.SendATBarHistoryDbRequest(atSymbol, BarHistoryIntraday, (uint8_t)minutes, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): Intraday bar history request [%s]\n", hRequest, symbol.c_str());
			}
			else if(cmd == "getDailyHistoryBars" || cmd == "getWeeklyHistoryBars")
			{
				string symbol;
				string beginTime, endTime;
				cin >> symbol >> beginTime >> endTime;

				ATTIME atBeginTime = Helper::StringToATTime(beginTime);
				ATTIME atEndTime = Helper::StringToATTime(endTime);

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
				uint64_t hRequest = requestor.SendATBarHistoryDbRequest(atSymbol, cmd == "getDailyHistoryBars" ? BarHistoryDaily : BarHistoryWeekly,
					0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): %s bar history request [%s]\n", hRequest, cmd == "getDailyHistoryBars" ? "Daily" : "Weekly", symbol.c_str());
			}
			else if(cmd == "getTicks")
			{
				string symbol;
				string beginTime, endTime;
				cin >> symbol >> beginTime >> endTime;

				ATTIME atBeginTime = Helper::StringToATTime(beginTime);
				ATTIME atEndTime = Helper::StringToATTime(endTime);

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
				uint64_t hRequest = requestor.SendATTickHistoryDbRequest(atSymbol, true, true, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): Tick history request [%s]\n", hRequest, symbol.c_str());
			}
			else if(cmd == "getMarketMovers")
			{
				string symbol;
				uint8_t exchange = 0;
				cin >> symbol >> exchange;

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
				atSymbol.symbolType = SymbolTopMarketMovers;
				atSymbol.exchangeType = exchange;

				uint64_t hRequest = requestor.SendATMarketMoversDbRequest(&atSymbol, 1, DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): Market movers request [%s]\n", hRequest, symbol.c_str());
			}
			else if(cmd == "getQuoteDb")
			{
				string symbol;
				cin >> symbol;

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);

				ATQuoteFieldType fields[] = { QuoteFieldLastPrice, QuoteFieldVolume, QuoteFieldFundamentalEquityName };
				uint64_t hRequest = requestor.SendATQuoteDbRequest(&atSymbol, 1, fields, _countof(fields), DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): QuoteDb request [%s]\n", hRequest, symbol.c_str());
			}
			else if(cmd == "subscribeMarketMovers" || cmd == "unsubscribeMarketMovers")
			{
				uint8_t exchange = 0;
				string symbol;
				cin >> symbol >> exchange;

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
				atSymbol.symbolType = SymbolTopMarketMovers;
				atSymbol.exchangeType = exchange;

				ATStreamRequestType requestType = (cmd == "subscribeMarketMovers" ? StreamRequestSubscribe : StreamRequestUnsubscribe);
				uint64_t hRequest = streamer.SendATMarketMoversStreamRequest(atSymbol, requestType, DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): Market movers stream request [%s]\n", hRequest, symbol.c_str());
			}
			else if(cmd == "subscribeQuoteStream" || cmd == "unsubscribeQuoteStream")
			{
				string symbol;
				cin >> symbol;

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);

				ATStreamRequestType requestType = (cmd == "subscribeQuoteStream" ? StreamRequestSubscribe : StreamRequestUnsubscribe);
				uint64_t hRequest = streamer.SendATQuoteStreamRequest(&atSymbol, 1, requestType, DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): Quote stream request [%s]\n", hRequest, symbol.c_str());
			}
		}
	}

	ATShutdownAPI();

	return 0;
}
