#pragma once
#ifndef INSTRUMENTMANAGER_H
#define INSTRUMENTMANAGER_H
#include <map>
#include "Platform/typedefs.h"
#include "Platform/Shared/Contract.h"
#include <QObject>
#include <Shared/ATServerAPIDefines.h>
#include <stdio.h>
#include <QReadWriteLock>
#include "Platform/View/InstrumentView.h"
#include "Platform/Enumerations/DataSource.h"
#include <QBasicTimer>
#include "Platform/View/MainWindow.h"

class Instrument;
class Strategy;
class OutputInterface;

typedef std::map<TickerId, Instrument*> InstrumentMap;
typedef std::map<String, TickerId> StringSymbolToTickerId;
typedef std::map<wchar16_t*, TickerId> AtSymbolToTickerId;
typedef std::map<TickerId, String> TickerIdToSymbol;

class InstrumentManager //: public QObject
{
    //Q_OBJECT
    private:
        InstrumentMap _instruments;
        TickerId _tickerId;
        StringSymbolToTickerId _stringSymbolToTickerId;
        AtSymbolToTickerId _atSymbolToTickerId;
        TickerIdToSymbol _tickerIdToSymbol;
        QReadWriteLock* _lockForInstrumentMap;
        QBasicTimer _timer;
        //int _minuteCount;
        //bool _alarmSet;

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
        void requestMarketData(const Contract&, DataSubscriber* subscriber, const DataSource = IB, const DataRequestType requestType = RealTime);
        void requestMarketData(const TickerId, DataSubscriber* subscriber, const DataSource = IB, const DataRequestType requestType = RealTime);
        void requestMarketData(const String symbol, DataSubscriber* subscriber, const DataSource source = IB,  const DataRequestType requestType = RealTime);

        void cancelMarketData(const TickerId);
        void cancelMarketData(const Contract&);
        void unSubscribeMarketData(const TickerId, DataSubscriber*);


        void removeInstrument(const TickerId);
        void mktDataCanceled(const TickerId);
        void printThreadId();
        //void timerEvent(QTimerEvent* event);

    public:
        void onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate);
        void onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate);
        const TickerId getTickerId(const Contract&);
        const Contract getContractForTicker(const TickerId);
        const String getInstrumentId(const TickerId);
        const String getSymbol(const Contract&);
        void generateSnapshot(const int timeInMinutes);
        const double getSnapShot(const TickerId, int length = 1);
        const double getLastPrice(const TickerId);
        const double getBidPrice(const TickerId);
        const double getAskPrice(const TickerId);


    public:
        void reportEvent(const String& message);

    signals:
        void requestMarketDataToTA(const TickerId, const Contract&);
        void requestCancelDataToTA(const TickerId);
        void updateTickerId(const long contractId, const TickerId);
        void lastPriceUpdated(const TickerId, TradeUpdate);
        void quoteUpdated(const TickerId, QuoteUpdate);
        void instrumentAdded(const TickerId, const Contract&);

    private:
        //void setAlarm();
        bool isConnected(const DataSource );
        void linkSubscriberToInstrument(Instrument* instrument, DataSubscriber* subscriber, DataRequestType requestType);
        void reqMktData(const TickerId, const Contract&, const String&, const DataSource);
        void linkInstrumentToView(Instrument*, const InstrumentView*, const TickerId, const Contract&);
        Instrument* getInstrumentForTicker(const TickerId);
        Instrument* getInstrumentForSymbol(const String&);
        Instrument* getInstrumentForContract(const Contract&);
        const bool testConnectivity(const DataSource);
        Instrument* addInstrument(const String&);
        Instrument* addInstrument(const Contract&);
};



#endif // INSTRUMENTMANAGER_H
