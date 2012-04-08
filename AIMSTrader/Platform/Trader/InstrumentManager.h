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

class MarketDataSubscriber;
class Instrument;

typedef std::map<TickerId, Instrument*> InstrumentMap;
typedef std::map<std::string, TickerId> SymbolToTickerId;

class InstrumentManager : public QObject
{
    Q_OBJECT
    private:
        InstrumentMap _instruments;
        TickerId _tickerId;
        SymbolToTickerId _symbolToTickerId;
        QReadWriteLock* lockForInstrumentMap;

    private:
        //InstrumentView* _instrumentView;

    public:
        InstrumentManager();
        ~InstrumentManager();

    public:
        void tickPrice( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute);
        void tickSize( const TickerId tickerId, const TickType field, const int size);
        void tickGeneric(const TickerId tickerId, const TickType tickType, const double value);

        void setContractDetails(const TickerId, const ContractDetails&);
        void requestMarketData(const Contract&,const DataSource, DataSubscriber*);
        void requestMarketData(const TickerId, const DataSource, DataSubscriber*);
        void removeInstrument(const TickerId);
        void mktDataCancelled(const TickerId);
        void printThreadId();

    public:
        void onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate);
        void onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate);
        const TickerId getTickerId(const Contract&);
        const Contract& getContractForTicker(const TickerId);

    signals:
        void requestMarketDataToTA(const TickerId, const Contract&);
        void requestCancelDataToTA(const TickerId);
        void updateTickerId(const long contractId, const TickerId);
        void lastPriceUpdated(const TickerId, TradeUpdate);
        void quoteUpdated(const TickerId, QuoteUpdate);
        void instrumentAdded(const TickerId, const Contract&);
};

#endif // INSTRUMENTMANAGER_H
