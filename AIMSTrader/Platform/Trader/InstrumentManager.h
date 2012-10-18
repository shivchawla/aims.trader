#pragma once
#ifndef INSTRUMENTMANAGER_H
#define INSTRUMENTMANAGER_H
#include <map>
#include "AimsTraderDefs/typedefs.h"
#include "InteractiveBroker/Shared/Contract.h"
#include <QObject>
#include <../API/ActiveTickFeed/include/Shared/ATServerAPIDefines.h>
#include <stdio.h>
#include <QReadWriteLock>
#include "Platform/View/InstrumentView.h"
#include <QBasicTimer>
#include "Platform/View/MainWindow.h"
#include <Data/InstrumentData.h>

class Instrument;
class Strategy;
class IOInterface;

typedef QHash<TickerId, Instrument*> InstrumentMap;
typedef QHash<String, TickerId> StringSymbolToTickerIdMap;
//typedef QHash<String, InstrumentId> StringSymbolToInstrumentIdMap;
typedef QHash<ATSYMBOL, TickerId> ATSymbolToTickerIdMap;
typedef QHash<TickerId, InstrumentId> TickerIdToInstrumentIdMap;

//typedef QHash<TickerId, String> TickerIdToSymbol;
typedef QHash<InstrumentId, TickerId> InstrumentIdToTickerIdMap;
//typedef QHash<TickerId, InstrumentId> TickerIdToInstrumentId;

class InstrumentManager
{
    private:
        TickerId _tickerId;
        InstrumentMap _instruments;
        ATSymbolToTickerIdMap _atSymbolToTickerId;

        //this can be changed to a vector as tickers are incremental
        TickerIdToInstrumentIdMap _tickerIdToInstrumentId;
        StringSymbolToTickerIdMap  _stringSymbolToTickerId;
        InstrumentIdToTickerIdMap _instrumentIdToTickerId;

        QReadWriteLock* _lockForInstrumentMap;
        QBasicTimer _timer;

    private:
       IOInterface* _ioInterface;

    public:
        InstrumentManager();
        ~InstrumentManager();

    public:
        void tickPrice( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute);
        void tickSize( const TickerId tickerId, const TickType field, const int size);
        void tickGeneric(const TickerId tickerId, const TickType tickType, const double value);

        void setContractDetails(const TickerId, const ContractDetails&);
        void requestMarketData(const String, const DataSubscriber*, const DataSource = Test, const DataRequestType requestType = RealTime);
        void requestMarketData(const TickerId, const DataSubscriber* , const DataSource = Test, const DataRequestType requestType = RealTime);
        void requestMarketData(const InstrumentId, const DataSubscriber* , const DataSource = Test, const DataRequestType requestType = RealTime);
        void requestMarketData(const InstrumentContract*, const DataSubscriber* , const DataSource = Test, const DataRequestType requestType = RealTime);

        void cancelMarketData(const TickerId);
        void cancelMarketData(const InstrumentId);
        void cancelMarketData(const InstrumentContract*);
        void unSubscribeMarketData(const TickerId, const DataSubscriber*);
        void unSubscribeMarketData(const InstrumentId, const DataSubscriber*);

        void removeInstrument(const TickerId);
        void removeInstrument(const InstrumentId);

        void mktDataCanceled(const TickerId);
        void mktDataCanceled(const InstrumentId);

        const double getCommission(const TickerId, const int shares, const double price);


        void printThreadId();
        //void timerEvent(QTimerEvent* event);
        void registerInstrument(const InstrumentContract*, const DataSource);

    public:
        void onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate);
        void onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate);

        const TickerId getTickerId(const InstrumentId);
        const TickerId getTickerId(const String&);

        const InstrumentId getInstrumentId(const TickerId);

        const InstrumentContract getInstrumentContract(const TickerId);
        const InstrumentContract getInstrumentContract(const InstrumentId);

        const Contract getIBContract(const TickerId);
        const Contract getIBContract(const InstrumentId);

        const QString getSymbol(const InstrumentContract*);
        const QString getSymbol(const TickerId);
        const QString getSymbol(const InstrumentId);

        const Prices getPrices(const TickerId);

        void generateSnapshot(const int timeInMinutes);
        const double getSnapShot(const TickerId, int length = 1);
//        const double getLastPrice(const InstrumentId);
//        const double getBidPrice(const InstrumentId);
//        const double getAskPrice(const InstrumentId);

        const double getLastPrice(const TickerId);
        const double getBidPrice(const TickerId);
        const double getAskPrice(const TickerId);

    public:
        void reportEvent(const String& message, const MessageType mType = INFO);

    private:
        bool isConnected(const DataSource);
        void linkSubscriberToInstrument(const TickerId, const DataSubscriber* subscriber, const DataRequestType);
        //void reqMktData(const Contract&, const DataSource);
        //void reqMktData(const InstrumentContract&, const DataSource);
        void reqMktData(const TickerId, const DataSource);

        void linkInstrumentToView(const Instrument*, const InstrumentView*, const TickerId);

        //Instrument* getInstrument(const TickerId);
        Instrument* getInstrument(const InstrumentContract*);
        Instrument* getInstrument(const String&);
        Instrument* getInstrument(const InstrumentId);

        const bool testConnectivity(const DataSource);
        const TickerId addInstrument(const String&);
        const TickerId addInstrument(const InstrumentContract*, const DataSource);
        const TickerId addInstrument(const TickerId);

        const TickerId getTickerIdForATSymbol(const ATSYMBOL&);
        const ATSYMBOL getATSymbol(const TickerId);

};

#endif // INSTRUMENTMANAGER_H
