#include "ActiveTickFeed/Utils/ActiveTickApi.h"
#include <ActiveTickServerAPI.h>
//#include "Platform/Startup/Service.h"
//#include "Platform/Reports/EventReport.h"
#include "Platform/View/OutputInterface.h"

using namespace ActiveTickFeed;

ActiveTickSession::ActiveTickSession()
{
    ATInitAPI();
    session = new APISession();
    requestor = new Requestor(session);
    streamer = new Streamer(session);
    connect();
}

ActiveTickSession::~ActiveTickSession()
{
    disConnect();
    ATShutdownAPI();
    delete session;
    delete requestor;
    delete streamer;
}

void ActiveTickSession::requestQuoteStream(const Contract& contract)
{
    //now request the Active tick farm
    ATStreamRequestType requestType = StreamRequestSubscribe;
    ATSYMBOL atSymbol = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
    uint64_t hRequest = streamer->SendATQuoteStreamRequest(&atSymbol, 1, requestType, DEFAULT_REQUEST_TIMEOUT);

    String message("Send Quote Stream Request to Active Tick");
    message.append(" RequetId: ").append(QString::number(hRequest));
    message.append(" Contract: ").append(QString::fromStdString(contract.symbol));
    reportEvent(message);

   // printf("SEND (%llu): Quote stream request [%s]\n", hRequest, contract.symbol.c_str());
}

void ActiveTickSession::requestTradeStream(const String& symbol)
{
    //now request the Active tick farm
    ATStreamRequestType requestType = StreamRequestSubscribe;
    ATSYMBOL atSymbol = ActiveTickFeed::Helper::StringToSymbol(symbol.toStdString());
    uint64_t hRequest = streamer->SendATQuoteStreamRequest(&atSymbol, 1, requestType, DEFAULT_REQUEST_TIMEOUT);
    String message("Send Trade Stream Request to Active Tick");
    message.append(" RequetId: ").append(QString::number(hRequest));
    message.append(" Contract: ").append(symbol);
    //message.append(" Contract: ").append(QString::fromStdString(contract.symbol));
    reportEvent(message);
    //printf("SEND (%llu): Quote stream request [%s]\n", hRequest, contract.symbol.c_str());
}


void ActiveTickSession::requestTradeStream(const Contract& contract)
{
    //now request the Active tick farm
    ATStreamRequestType requestType = StreamRequestSubscribe;
    ATSYMBOL atSymbol = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
    uint64_t hRequest = streamer->SendATQuoteStreamRequest(&atSymbol, 1, requestType, DEFAULT_REQUEST_TIMEOUT);
    String message("Send Trade Stream Request to Active Tick");
    message.append(" RequetId: ").append(QString::number(hRequest));
    message.append(" Contract: ").append(QString::fromStdString(contract.symbol));
    reportEvent(message);
    //printf("SEND (%llu): Quote stream request [%s]\n", hRequest, contract.symbol.c_str());
}

void ActiveTickSession::connect()
{
    //string serverIpAddress, apiUserid, userid, password;
    uint32_t serverPort = 0;
    std::string serverIpAddress="activetick1.activetick.com";
    std::string apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
    std::string userid="shivchawla";
    std::string password= "27as04sh";
    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserid);
    bool rc = session->Init(guidApiUserid, serverIpAddress, serverPort, &Helper::ConvertString(userid).front(), &Helper::ConvertString(password).front());

    String message("Active Tick initialization status: ");
    message.append(QString::number(rc));
    reportEvent(message);
    //printf("init status: %d\n", rc);
}

void ActiveTickSession::disConnect()
{
  session->Destroy();
}

void ActiveTickSession::reportEvent(const String& message, const MessageType mType)
{
    OutputInterface::Instance()->reportEvent("ActiveTickAPI", message, mType);
}

void ActiveTickSession::cancelQuoteStream(ATSYMBOL& atSymbol)
{
    //now request the Active tick farm
    ATStreamRequestType requestType = StreamRequestUnsubscribe;
    uint64_t hRequest = streamer->SendATQuoteStreamRequest(&atSymbol, 1, requestType, DEFAULT_REQUEST_TIMEOUT);

    /*String message("Send Trade Stream Request to Active Tick");
    message.append(" RequetId: ").append(QString::number(hRequest));
    message.append(" Contract: ").append(QString::fromStdString(contract.symbol));
    reportEvent(message);*/
}


void ActiveTickSession::cancelMarketData(const Contract& contract)
{
    ATSYMBOL atSymbol = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
    cancelQuoteStream(atSymbol);
}

