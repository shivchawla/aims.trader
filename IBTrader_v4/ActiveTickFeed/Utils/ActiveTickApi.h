#ifndef ACTIVETICKAPI_H
#define ACTIVETICKAPI_H
#include "Platform/Shared/Contract.h"
#include "ActiveTickFeed/Utils/Requestor.h"
#include "ActiveTickFeed/Utils/Session.h"
#include "ActiveTickFeed/Utils/Streamer.h"
#include "ActiveTickFeed/Utils/Helper.h"

class ActiveTickAPI
{
    private:
        APISession* session;
        Requestor* requestor;
        Streamer* streamer;

    public:
        ActiveTickAPI();
        ~ActiveTickAPI();

    public:
        void requestQuoteStream(const Contract&);
        void requestTradeStream(const Contract&);
        void connect();
        void disConnect();
};


#endif // ACTIVETICKAPI_H
