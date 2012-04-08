/*
 *  Instrument.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 12/14/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef Instrument_h
#define Instrument_h

#include <string>
#include <list>
#include "Platform/Shared/Contract.h"
#include "Platform/typedefs.h"
#include "Platform/Enumerations/TickType.h"
#include <QObject>
#include <Qmutex>

class Instrument: public QObject
{
    Q_OBJECT
    private:
        ContractDetails _contractDetails;
        Contract _contract;
        long _tickerId;
        //double _bidPrice, _askPrice, _closePrice, _highPrice, _lowPrice, _lastPrice, _askSize, _bidSize, _lastSize;
        int multiplier;

        TradeUpdate _lastTradeUpdate;
        QuoteUpdate _lastQuoteUpdate;

        QMutex mutex;

    public:
        Instrument(); 
        ~Instrument(); 
        Instrument(const TickerId ,const Contract&, int multiplier);

    public slots:
        /*void setTickerId(const int tickerId);
        void setBid(const double bid);
        void setAsk(const double ask);
        void setLast(const double last);
        void setHigh(const double high);
        void setClose(const double close);
        void setLow(const double low);
        void setLastSize(const int size);
        void setAskSize(const int size);
        void setBidSize(const int size);
        */
        void setContractDetails(const ContractDetails&);

    public:
        void tickPrice( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute);
        void tickSize( const TickerId tickerId, const TickType field, const int size);
        void tickGeneric(const TickerId tickerId, const TickType tickType, const double value);


    public:
        void linkInstrument(QObject*);
        void updateOnQuote();
        void updateOnTrade();

    public:
        const Contract& getContract() const;
        const long getTickerId() const;
        const std::string toString() const;

    public:
        void onLastPriceUpdate(LPATQUOTESTREAM_TRADE_UPDATE pTradeUpdate);
        void onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate);

    signals:
        void lastPriceUpdated(const TickerId, TradeUpdate);
        void quoteUpdated(const TickerId, QuoteUpdate);

        void tickPriceUpdated( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute);
        void tickSizeUpdated( const TickerId tickerId, const TickType field, const int size);
        void tickGenericUpdated(const TickerId tickerId, const TickType tickType, const double value);

};

#endif
