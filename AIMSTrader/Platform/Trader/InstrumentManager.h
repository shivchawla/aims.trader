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
//#include "Platform/Enumerations/DataSource.h"
#include <QBasicTimer>
#include "Platform/View/MainWindow.h"

class Instrument;
class Strategy;
class OutputInterface;

typedef QHash<TickerId, Instrument*> InstrumentMap;
typedef QHash<String, TickerId> StringSymbolToTickerId;
typedef QHash<TickerId, String> TickerIdToSymbol;
typedef QHash<ContractId, TickerId> ContractIdToTickerId;

class InstrumentManager
{
    private:
        InstrumentMap _instruments;
        TickerId _tickerId;

        StringSymbolToTickerId _stringSymbolToTickerId;
        //ContractIdToTickerId _contractIdToTickerId;

        QReadWriteLock* _lockForInstrumentMap;
        QBasicTimer _timer;

    private:
       OutputInterface* _outputInterface;

    public:
        InstrumentManager();
        ~InstrumentManager();

    public:
        void tickPrice( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute);
        void tickSize( const TickerId tickerId, const TickType field, const int size);
        void tickGeneric(const TickerId tickerId, const TickType tickType, const double value);

        void setContractDetails(const TickerId, const ContractDetails&);
        void requestMarketData(const ATContract&, DataSubscriber* subscriber, const DataSource = IB, const DataRequestType = RealTime);
        void requestMarketData(const TickerId, DataSubscriber* subscriber, const DataSource = IB, const DataRequestType = RealTime);
        void requestMarketData(const String symbol, DataSubscriber* subscriber, const DataSource source = IB, const DataRequestType = RealTime);

        void cancelMarketData(const TickerId);
        void cancelMarketData(const ATContract&);
        void unSubscribeMarketData(const TickerId, DataSubscriber*);

        void removeInstrument(const TickerId);
        void mktDataCanceled(const TickerId);
        void printThreadId();
        //void timerEvent(QTimerEvent* event);

    public:
        void onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate);
        void onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate);
        const TickerId getTickerId(const ATContract&);
        const ATContract getContractForTicker(const TickerId);
        const ContractId getContractIdForTicker(const TickerId);

        const String getInstrumentId(const TickerId);
        const String getSymbol(const ATContract&);
        void generateSnapshot(const int timeInMinutes);
        const double getSnapShot(const TickerId, int length = 1);
        const double getLastPrice(const TickerId);
        const double getBidPrice(const TickerId);
        const double getAskPrice(const TickerId);

    public:
        void reportEvent(const String& message, const MessageType mType = INFO);

    signals:
        void requestMarketDataToTA(const TickerId, const ATContract&);
        void requestCancelDataToTA(const TickerId);
        void updateTickerId(const long contractId, const TickerId);
//        void lastPriceUpdated(const TickerId, TradeUpdate);
//        void quoteUpdated(const TickerId, QuoteUpdate);
        void instrumentAdded(const TickerId, const ATContract&);

    private:
        //void setAlarm();
        bool isConnected(const DataSource );
        void linkSubscriberToInstrument(Instrument* instrument, DataSubscriber* subscriber, const DataRequestType);
        void reqMktData(const TickerId, const ATContract&, const String&, const DataSource);
        void linkInstrumentToView(Instrument*, const InstrumentView*, const TickerId, const Contract&);
        Instrument* getInstrumentForTicker(const TickerId);
        Instrument* getInstrumentForSymbol(const String&);
        Instrument* getInstrumentForContract(const ATContract&);
        const bool testConnectivity(const DataSource);
        Instrument* addInstrument(const String&);
        Instrument* addInstrument(const ATContract&);
};

#endif // INSTRUMENTMANAGER_H
