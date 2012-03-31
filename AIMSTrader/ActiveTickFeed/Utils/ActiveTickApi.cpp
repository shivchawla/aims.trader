#include "ActiveTickFeed/Utils/ActiveTickApi.h"
#include <ActiveTickServerAPI.h>

using namespace ActiveTickFeed;

ActiveTickAPI::ActiveTickAPI()
{
    ATInitAPI();
    session = new APISession();
    requestor = new Requestor(session);
    streamer = new Streamer(session);
    connect();
}

ActiveTickAPI::~ActiveTickAPI()
{
    disConnect();
    ATShutdownAPI();
    delete session;
    delete requestor;
    delete streamer;
}

void ActiveTickAPI::requestQuoteStream(const Contract& contract)
{
    //now request the Active tick farm
    ATStreamRequestType requestType = StreamRequestSubscribe;
    ATSYMBOL atSymbol = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
    uint64_t hRequest = streamer->SendATQuoteStreamRequest(&atSymbol, 1, requestType, DEFAULT_REQUEST_TIMEOUT);
    printf("SEND (%llu): Quote stream request [%s]\n", hRequest, contract.symbol.c_str());
}

void ActiveTickAPI::requestTradeStream(const Contract& contract)
{
    //now request the Active tick farm
    ATStreamRequestType requestType = StreamRequestSubscribe;
    ATSYMBOL atSymbol = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
    uint64_t hRequest = streamer->SendATQuoteStreamRequest(&atSymbol, 1, requestType, DEFAULT_REQUEST_TIMEOUT);
    printf("SEND (%llu): Quote stream request [%s]\n", hRequest, contract.symbol.c_str());
}

void ActiveTickAPI::connect()
{
    //string serverIpAddress, apiUserid, userid, password;
    uint32_t serverPort = 0;
    std::string serverIpAddress="activetick1.activetick.com";
    std::string apiUserid="4ca0f31fbc8df528b598dfd41368af3f";
    std::string userid="shivchawla";
    std::string password= "27as04sh";
    ATGUID guidApiUserid = Helper::StringToATGuid(apiUserid);
    bool rc = session->Init(guidApiUserid, serverIpAddress, serverPort, &Helper::ConvertString(userid).front(), &Helper::ConvertString(password).front());
    printf("init status: %d\n", rc);
}

void ActiveTickAPI::disConnect()
{
  session->Destroy();
}


