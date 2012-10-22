#ifndef ACTIVETICKSESSION_H
#define ACTIVETICKSESSION_H
#include "InteractiveBroker/Shared/Contract.h"
#include "ActiveTickFeed/Utils/Requestor.h"
#include "ActiveTickFeed/Utils/Session.h"
#include "ActiveTickFeed/Utils/Streamer.h"
#include "ActiveTickFeed/Utils/Helper.h"
#include "AimsTraderDefs/typedefs.h"
#include <QMutex>
#include <QWaitCondition>

using namespace ActiveTickFeed;

class ActiveTickSession
{
    private:
        APISession* session;
        Requestor* requestor;
        Streamer* streamer;
        void cancelQuoteStream(ATSYMBOL& symbol);
        bool _isConnected;
        QMutex mutex;
        QMutex outerMutex;
        QWaitCondition condition;

    public:
        ActiveTickSession();
        ~ActiveTickSession();

    public:
        void requestQuoteStream(const Contract&);
        void requestTradeStream(const Contract&);
        void requestTradeStream(const String& symbol);
        //void requestTradeStream(const ATSYMBOL);
        void connect();
        void disConnect();
        //void reportEvent(const String& message);
        void reportEvent(const String& message, const MessageType mType=INFO);
        void cancelMarketData(const Contract& contract);
        bool IsConnected();
        void requestProcessed();
};


#endif // ACTIVETICKAPI_H
