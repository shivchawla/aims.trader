#include "stdafx.h"
#include "Helper.h"
#include "Requestor.h"
#include "Session.h"
#include "Streamer.h"
#include <ActiveTickServerAPI.h>

#include <iostream>
#include <stdio.h>
#include <QString>
#include <QStringList>
#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QThread>

#include "HistoryBarDb.h"
#include "InstrumentDb.h"
#include "ConfigurationDb.h"
#include "BootStrapper.h"
#include "UtilBox.h"

using namespace std;
// $(OutDir)ActiveTickServerAPI.lib
void PrintUsage()
{
	cout << "Available commands:" << endl;
	cout << "-------------------------------------------------------------" << endl;
	cout << "?" << endl;
	cout << "quit" << endl;

	cout << "init [serverIpAddress] [serverPort] [apiUseridGuid] [userid] [password]" << endl;

	cout << "getIntradayHistoryBars [symbol] [minutes] [beginTime] [endTime]" << endl;
	cout << "getDailyHistoryBars [symbol] [beginTime] [endTime]" << endl;
	cout << "getWeeklyHistoryBars [symbol] [beginTime] [endTime]" << endl;

	cout << "getTicks [symbol] [beginTime] [endTime]" << endl;
	cout << "getTicksForEntireDay [symbol] [date]" << endl;

	cout << "getMarketMovers [symbol] [exchange]" << endl;

	cout << "getQuoteDb [symbol]" << endl;

	cout << "getSectorList" << endl;
	cout << "getConstituentsList [sectorname] [industryname] (use _ for spaces)" << endl;

	cout << "subscribeMarketMovers [symbol] [exchange]" << endl;
	cout << "unsubscribeMarketMovers [symbol] [exchange]" << endl;

	cout << "subscribeQuoteStream [symbol1,symbol2...]" << endl;
	cout << "unsubscribeQuoteStream [symbol,symbol2...]" << endl;

	cout << "subscribeTradesOnlyQuoteStream [symbol,symbol2...]" << endl;
	cout << "unsubscribeTradesOnlyQuoteStream [symbol,symbol2...]" << endl;

	cout << "subscribeQuotesOnlyQuoteStream [symbol,symbol2...]" << endl;
	cout << "unsubscribeQuotesOnlyQuoteStream [symbol,symbol2...]" << endl;

	cout << "-------------------------------------------------------------" << endl;
	cout << "Note: all date/time parameters must be entered in the following format: YYYYMMDDHHMMSS" << endl;
	cout << "apiUseridGuid must be entered using full 32 characters, ex: EF1C0A768BBB11DFBCB3F923E0D72085" << endl;
	cout << "-------------------------------------------------------------" << endl;
	cout.flush();
}

void GetDailyHistoryBars(Requestor requestor, QString symbol, QDateTime start, QDateTime end) {
    QString format = "yyyyMMddhhmmss";
	//cin >> symbol >> beginTime >> endTime;
	//Convert date time strings to YYYYMMDDHHMMSS format
	string sym = symbol.toStdString();

	ATTIME atBeginTime = Helper::StringToATTime(start.toString(format).toStdString());
	ATTIME atEndTime = Helper::StringToATTime(end.toString(format).toStdString());

	ATSYMBOL atSymbol = Helper::StringToSymbol(sym);
	uint64_t request = requestor.SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily,
		0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

	printf("SEND (%llu): %s bar history request [%s]\n", request, "Daily", sym.c_str());
}

int _main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	//List of installed database drivers
	/*cout << "Connecting to database..." << endl;

    qDebug() << QSqlDatabase :: drivers();*/

	BootStrapper::InitDatabase();

	/*QSqlDatabase db = QSqlDatabase:: addDatabase("QMYSQL3");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("stratTrader");
    db.setUserName("root");
    db.setPassword("proline21");

    if (!db.open()) {
        cout << "Unable to connect to database!!" << endl;
        qDebug() << db.lastError().driverText();
        return a.exec();
    }
    else cout << "Database connected!!" << endl;

    QSqlQuery query;
    query.exec("select BINTOUUID(instrument_id) instrument_id, symbol, short_name, full_name, updated_by, updated_date from instruments");
    while (query.next()) {
        QString id = query.value(0).toString();
        QString symbol = query.value(1).toString();
        QString shortName = query.value(2).toString();
        qDebug() << id << " " << symbol << " " << shortName << endl;
    }
    query.finish();
    db.close();*/

	//

	ATInitAPI();

	{
        APISession session();
		Requestor requestor(session);
		Streamer streamer(session);

		PrintUsage();
		string serverIpAddress, apiUserid, userid, password;

		serverIpAddress="activetick1.activetick.com";
		apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
		userid="shivchawla";
		password= "27as04sh";

		string cmd;
		//while(true)
		//{
			/*cin >> cmd;
			if(cmd == "?")
				PrintUsage();
			else if(cmd == "quit")
			{
				session.Destroy();
				break;
			}
			else if(cmd == "init")
			{*/
				//string serverIpAddress, apiUserid, userid, password;
				uint32_t serverPort = 0;
				//cin >> serverIpAddress >> serverPort >> apiUserid >> userid >> password;
				
                ATGUID guidApiUserid = Helper::StringToATGuid(apiUserid);
				bool rc = session.Init(guidApiUserid, serverIpAddress, serverPort, &Helper::ConvertString(userid).front(), &Helper::ConvertString(password).front());
				printf("init status: %d\n", rc);
				UtilBox::sleep(3);
			//}
			
			/*else if(cmd == "getDailyHistoryBars" || cmd == "getWeeklyHistoryBars")
			{
				string symbol;
				string beginTime, endTime;
				cin >> symbol >> beginTime >> endTime;

				ATTIME atBeginTime = Helper::StringToATTime(beginTime);
				ATTIME atEndTime = Helper::StringToATTime(endTime);

				ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
				uint64_t request = requestor.SendATBarHistoryDbRequest(atSymbol, cmd == "getDailyHistoryBars" ? BarHistoryDaily : BarHistoryWeekly,
					0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

				printf("SEND (%llu): %s bar history request [%s]\n", request, cmd == "getDailyHistoryBars" ? "Daily" : "Weekly", symbol.c_str());
			}*/
			//else if (cmd == "db" || cmd == "database") {
				//First get configuration
				ConfigurationDb confDb;
				ConfigurationData* historyStartDateConf = confDb.GetConfigurationByKey("HistoryStartDate");

				InstrumentDb db;
				QList<InstrumentData> instruments = db.GetInstruments();

				for(int i=0; i< instruments.count(); i++) {
					qDebug() << "Fetching data for " << instruments.at(i).instrumentId << " " << instruments.at(i).symbol << " at " << QDateTime ::currentDateTime() << endl;

					/*GetDailyHistoryBars(requestor,
										instruments.at(i).symbol, 
										QDateTime :: fromString(historyStartDateConf->value, "dd-MMM-yyyy"),
										QDateTime :: QDateTime(QDate::currentDate(), QTime(23, 59, 59)));*/

                    QString format = "yyyyMMddhhmmss";
					//Convert date time strings to YYYYMMDDHHMMSS format
					string sym = instruments.at(i).symbol.toStdString();

					QDateTime start = QDateTime :: fromString(historyStartDateConf->value, "dd-MMM-yyyy");
                    QDateTime end = QDateTime(QDate::currentDate(), QTime(23, 59, 59));

					ATTIME atBeginTime = Helper::StringToATTime(start.toString(format).toStdString());
					ATTIME atEndTime = Helper::StringToATTime(end.toString(format).toStdString());

					ATSYMBOL atSymbol = Helper::StringToSymbol(sym);
					uint64_t request = requestor.SendATBarHistoryDbRequest(atSymbol, BarHistoryDaily,
						0, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

					printf("SEND (%llu): %s bar history request [%s]\n", request, "Daily", sym.c_str());

					UtilBox::sleep(3);
					/*string check;
					cin >> check;*/
				}
			//}
			//else if(cmd == "getIntradayHistoryBars")
			//{
			//	string symbol;
			//	string beginTime, endTime;
			//	uint32_t minutes = 0;
			//	cin >> symbol >> minutes >> beginTime >> endTime;

			//	ATTIME atBeginTime = Helper::StringToATTime(beginTime);
			//	ATTIME atEndTime = Helper::StringToATTime(endTime);

			//	ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
			//	uint64_t request = requestor.SendATBarHistoryDbRequest(atSymbol, BarHistoryIntraday, (uint8_t)minutes, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): Intraday bar history request [%s]\n", request, symbol.c_str());
			//}
			//else if(cmd == "getTicks")
			//{
			//	string symbol;
			//	string beginTime, endTime;
			//	cin >> symbol >> beginTime >> endTime;

			//	ATTIME atBeginTime = Helper::StringToATTime(beginTime);
			//	ATTIME atEndTime = Helper::StringToATTime(endTime);

			//	ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
			//	uint64_t request = requestor.SendATTickHistoryDbRequest(atSymbol, true, true, atBeginTime, atEndTime, DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): Tick history request [%s]\n", request, symbol.c_str());
			//}
			//else if(cmd == "getTicksForEntireDay")
			//{
			//	string symbol;
			//	string dbdate;
			//	cin >> symbol >> dbdate;

			//	if(dbdate.length() < 8)
			//	{
			//		std::cout << "Invalid date\n";
			//		continue;
			//	}

			//	ATTIME atDbDate = {0};
			//	atDbDate.wYear = atol(dbdate.substr(0, 4).c_str());
			//	atDbDate.wMonth = atol(dbdate.substr(4, 2).c_str());
			//	atDbDate.wDay = atol(dbdate.substr(6, 2).c_str());

			//	ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);

			//	//get 10 pages, and start from very end (-1)
			//	uint64_t request = requestor.SendATTickHistoryDbRequest(atSymbol, true, true, 10, -1, atDbDate, DEFAULT_REQUEST_TIMEOUT);
			//}
			//else if(cmd == "getMarketMovers")
			//{
			//	string symbol;
			//	uint8_t exchange = 0;
			//	cin >> symbol >> exchange;

			//	ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
			//	atSymbol.symbolType = SymbolTopMarketMovers;
			//	atSymbol.exchangeType = exchange;

			//	uint64_t request = requestor.SendATMarketMoversDbRequest(&atSymbol, 1, DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): Market movers request [%s]\n", request, symbol.c_str());
			//}
			//else if(cmd == "getQuoteDb")
			//{
			//	string symbol;
			//	cin >> symbol;

			//	ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);

			//	ATQuoteFieldType fields[] = { QuoteFieldLastPrice, QuoteFieldVolume, QuoteFieldProfileShortName, QuoteFieldBalanceSheetsShareDataTotalCommonSharesOutstanding, QuoteFieldProfileIndustry };
			//	uint64_t request = requestor.SendATQuoteDbRequest(&atSymbol, 1, fields, _countof(fields), DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): QuoteDb request [%s]\n", request, symbol.c_str());
			//}
			//else if(cmd == "getSectorList")
			//{
			//	uint64_t request = requestor.SendATSectorListRequest(DEFAULT_REQUEST_TIMEOUT);
			//	printf("SEND (%llu): SectorList request\n", request);
			//}
			//else if(cmd == "getConstituentsList")
			//{
			//	string sector, industry;
			//	cin >> sector >> industry;
			//	
			//	string key = sector + "\n" + industry;

			//	for(string::size_type i = 0; i < key.length(); ++i)
			//	{
			//		if(key[i] == '_')
			//			key[i] = ' ';
			//	}

			//	uint64_t request = requestor.SendATConstituentListRequest(ATConstituentListSector, key.c_str(), DEFAULT_REQUEST_TIMEOUT);
			//	printf("SEND (%llu): ConstituentList request\n", request);
			//}
			//else if(cmd == "subscribeMarketMovers" || cmd == "unsubscribeMarketMovers")
			//{
			//	uint8_t exchange = 0;
			//	string symbol;
			//	cin >> symbol >> exchange;

			//	ATSYMBOL atSymbol = Helper::StringToSymbol(symbol);
			//	atSymbol.symbolType = SymbolTopMarketMovers;
			//	atSymbol.exchangeType = exchange;

			//	ATStreamRequestType requestType = (cmd == "subscribeMarketMovers" ? StreamRequestSubscribe : StreamRequestUnsubscribe);
			//	uint64_t request = streamer.SendATMarketMoversStreamRequest(atSymbol, requestType, DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): Market movers stream request [%s]\n", request, symbol.c_str());
			//}
			//else if(cmd == "subscribeQuoteStream" || cmd == "unsubscribeQuoteStream")
			//{
			//	string s;
			//	cin >> s;
			//	std::vector<ATSYMBOL> symbols = Helper::StringToSymbols(s);

			//	ATStreamRequestType requestType = (cmd == "subscribeQuoteStream" ? StreamRequestSubscribe : StreamRequestUnsubscribe);
			//	uint64_t request = streamer.SendATQuoteStreamRequest(symbols.data(), (uint16_t)symbols.size(), requestType, DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): Quote stream request [%s]\n", request, s.c_str());
			//}
			//else if(cmd == "subscribeTradesOnlyQuoteStream" || cmd == "unsubscribeTradesOnlyQuoteStream")
			//{
			//	string s;
			//	cin >> s;
			//	std::vector<ATSYMBOL> symbols = Helper::StringToSymbols(s);

			//	ATStreamRequestType requestType = (cmd == "subscribeTradesOnlyQuoteStream" ? StreamRequestSubscribeTradesOnly : StreamRequestUnsubscribeTradesOnly);
			//	uint64_t request = streamer.SendATQuoteStreamRequest(symbols.data(), (uint16_t)symbols.size(), requestType, DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): Quote stream (trades only) request [%s]\n", request, s.c_str());
			//}
			//else if(cmd == "subscribeQuotesOnlyQuoteStream" || cmd == "unsubscribeQuotesOnlyQuoteStream")
			//{
			//	string s;
			//	cin >> s;
			//	std::vector<ATSYMBOL> symbols = Helper::StringToSymbols(s);

			//	ATStreamRequestType requestType = (cmd == "subscribeQuotesOnlyQuoteStream" ? StreamRequestSubscribeQuotesOnly : StreamRequestUnsubscribeQuotesOnly);
			//	uint64_t request = streamer.SendATQuoteStreamRequest(symbols.data(), (uint16_t)symbols.size(), requestType, DEFAULT_REQUEST_TIMEOUT);

			//	printf("SEND (%llu): Quote stream (quotes only) request [%s]\n", request, s.c_str());
			//}
			
		//}
	}

	//a.exec();
	ATShutdownAPI();

	BootStrapper :: ShutdownDatabase();

	a.quit();
	return 0;
}
