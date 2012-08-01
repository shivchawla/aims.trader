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

typedef QHash<InstrumentId, Instrument*> InstrumentMap;
typedef QHash<String, TickerId> StringSymbolToTickerIdMap;
typedef QHash<String, InstrumentId> StringSymbolToInstrumentIdMap;
typedef QHash<uint, InstrumentId> ATSymbolToInstrumentIdMap;
typedef QHash<TickerId, InstrumentId> TickerIdToInstrumentIdMap;

//typedef QHash<TickerId, String> TickerIdToSymbol;
typedef QHash<InstrumentId, TickerId> InstrumentIdToTickerId;
typedef QHash<TickerId, InstrumentId> TickerIdToInstrumentId;

class InstrumentManager
{
    private:
        TickerId _tickerId;
        InstrumentMap _instruments;
        ATSymbolToInstrumentIdMap _atSymbolToInstrumentId;

        //this can be changed to a vector as tickers are incremental
        TickerIdToInstrumentIdMap _tickerIdToInstrumentId;
        StringSymbolToInstrumentIdMap  _stringSymbolToInstrumentId;
        InstrumentIdToTickerId _instrumentIdToTickerId;

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

        void setContractDetails(const InstrumentId, const ContractDetails&);
        void requestMarketData(const String, DataSubscriber*, const DataSource source = IB, const DataRequestType = RealTime);
        void requestMarketData(const InstrumentId, DataSubscriber* , const DataSource = IB, const DataRequestType requestType = RealTime);
        void requestMarketData(const InstrumentContract&, DataSubscriber* , const DataSource = IB, const DataRequestType requestType = RealTime);

        void cancelMarketData(const InstrumentId);
        void cancelMarketData(const InstrumentContract&);
        void unSubscribeMarketData(const InstrumentId, const DataSubscriber*);

        void removeInstrument(const InstrumentId);
        void mktDataCanceled(const InstrumentId);
        void printThreadId();
        //void timerEvent(QTimerEvent* event);

    public:
        void onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate);
        void onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate);
        //const TickerId getTickerId(const ATContract&);
        const TickerId getTickerId(const InstrumentId);
        const Contract getIBContract(const InstrumentId);


        //const ATContract getContractForTicker(const TickerId);
        const InstrumentContract getInstrumentContract(const InstrumentId);

        //const ContractId getContractIdForTicker(const TickerId);

        //const String getInstrumentId(const TickerId);
        const QString getSymbol(const InstrumentContract&);
        const QString getSymbol(const InstrumentId);

        void generateSnapshot(const int timeInMinutes);
        const double getSnapShot(const InstrumentId, int length = 1);
        const double getLastPrice(const InstrumentId);
        const double getBidPrice(const InstrumentId);
        const double getAskPrice(const InstrumentId);

    public:
        void reportEvent(const String& message, const MessageType mType = INFO);

    signals:
        void requestMarketDataToTA(const TickerId, const ATContract&);
        void requestCancelDataToTA(const TickerId);
        void updateTickerId(const long contractId, const TickerId);
//        void lastPriceUpdated(const TickerId, TradeUpdate);
//        void quoteUpdated(const TickerId, QuoteUpdate);
        void instrumentAdded(const InstrumentId, const InstrumentContract&);

    private:
        //void setAlarm();
        bool isConnected(const DataSource );
        void linkSubscriberToInstrument(const Instrument* instrument, const DataSubscriber* subscriber, const DataRequestType);
        void reqMktData(const InstrumentContract&, const DataSource);
        void linkInstrumentToView(const Instrument*, const InstrumentView*, const InstrumentId, const InstrumentContract&);
        //Instrument* getInstrumentForTicker(const TickerId);
        Instrument* getInstrument(const InstrumentId);
        Instrument* getInstrument(const InstrumentContract&);
        Instrument* getInstrument(const String&);
        //Instrument* getInstrumentForContract(const ATContract&);
        const bool testConnectivity(const DataSource);
        Instrument* addInstrument(const String&);
        Instrument* addInstrument(const InstrumentContract&);
        const InstrumentId getInstrumentIdForATSymbol(const ATSYMBOL&);
};

#endif // INSTRUMENTMANAGER_H
