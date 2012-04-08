#ifndef ACTIVETICKAPI_H
#define ACTIVETICKAPI_H
#include "Platform/Shared/Contract.h"
#include "ActiveTickFeed/Utils/Requestor.h"
#include "ActiveTickFeed/Utils/Session.h"
#include "ActiveTickFeed/Utils/Streamer.h"
#include "ActiveTickFeed/Utils/Helper.h"
#include <Platform/typedefs.h>

class ActiveTickAPI
{
    private:
        APISession* session;
        Requestor* requestor;
        Streamer* streamer;
        void cancelQuoteStream(ATSYMBOL& symbol);

    public:
        ActiveTickAPI();
        ~ActiveTickAPI();

    public:
        void requestQuoteStream(const Contract&);
        void requestTradeStream(const Contract&);
        void requestTradeStream(const String& symbol);
        void connect();
        void disConnect();
        void reportEvent(const String& message);
        void cancelMarketData(const Contract& contract);

};


#endif // ACTIVETICKAPI_H
