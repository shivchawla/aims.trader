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
        QReadWriteLock readWritelock;

    private:
        //InstrumentView* _instrumentView;

    public:
        InstrumentManager();
        ~InstrumentManager();

    public slots:
        void setBid(const TickerId, const double bid);
        void setAsk(const TickerId, const double ask);
        void setLast(const TickerId, const double last);
        void setHigh(const TickerId, const double high);
        void setClose(const TickerId, const double close);
        void setLow(const TickerId, const double low);
        void setBidSize(const TickerId, const int size);
        void setAskSize(const TickerId, const int size);
        void setLastSize(const TickerId, const int size);
        void setContractDetails(const TickerId, const ContractDetails&);
        void requestMarketData(const Contract&, MarketDataSubscriber*);
        void removeInstrument(const TickerId);
        void printThreadId();

    public:
        void onTradeUpdate(TradeUpdate pLastUpdate);
        void onQuoteUpdate(QuoteUpdate pQuoteUpdate);
        TickerId getTickerId(const Contract&);

    signals:
        void requestMarketDataToTA(const TickerId, const Contract&);
        void requestCancelDataToTA(const TickerId);
        void updateTickerId(const long contractId, const TickerId);
        void lastPriceUpdated(const TickerId, TradeUpdate);
        void quoteUpdated(const TickerId, QuoteUpdate);
};

#endif // INSTRUMENTMANAGER_H
