/*
 *  Instrument.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 12/14/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */
#pragma once
#ifndef Instrument_h
#define Instrument_h

#include <string>
#include <list>
#include "InteractiveBroker/Shared/Contract.h"
#include "AimsTraderDefs/typedefs.h"
//#include "Platform/Enumerations/TickType.h"
#include "../API/ActiveTickFeed/include/Shared/ATServerAPIDefines.h"
#include <QObject>
#include <Qmutex>
#include <QBasicTimer>

class Instrument: public QObject
{
    Q_OBJECT
    private:
        ContractDetails _contractDetails;
        ATContract _aTcontract;
        long _tickerId;
        double _bidPrice, _askPrice, _closePrice, _openPrice, _highPrice, _lowPrice, _lastPrice, _askSize, _bidSize, _lastSize, _volume;
        int _multiplier;

//        double _oneMinuteSnapshot;
//        double _twoMinuteSnapshot;
//        double _fiveMinuteSnapshot;
//        double _tenMinuteSnapshot;

        //TradeUpdate _lastTradeUpdate;
        //QuoteUpdate _lastQuoteUpdate;

        QMutex _mutex;
        QBasicTimer timer;
        int _minuteCount;
        bool _alarmSet;

    public:
        Instrument(); 
        ~Instrument(); 
        Instrument(const TickerId , const ATContract&, int _multiplier);

    private:
        inline void setTickerId(const int tickerId);
        inline void setBid(const double bid);
        inline void setAsk(const double ask);
        inline void setLast(const double last);
        inline void setHigh(const double high);
        inline void setOpen(const double close);
        inline void setClose(const double close);
        inline void setLow(const double low);
        inline void setLastSize(const int size);
        inline void setAskSize(const int size);
        inline void setBidSize(const int size);
        inline void setVolume(const int volume);

    public:
//        void calculateOneMinuteSnapshot();
//        void calculateTwoMinuteSnapshot();
//        void calculateFiveMinuteSnapshot();
//        void calculateTenMinuteSnapshot();
          void calculateSnapshot(const int minute);
        void setAlarm();

    public:
        void tickPrice(const TickType field, const double price, const int canAutoExecute);
        void tickSize(const TickType field, const int size);
        void tickGeneric(const TickType tickType, const double value);
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
        const ATContract& getContract() const;
        const ContractId getContractId();
        const double getLastPrice();
        const double getAskPrice();
        const double getBidPrice();
        const long getTickerId() const;
        const std::string toString() const;
        const double getSnapshot(const int minute);
        void timerEvent(QTimerEvent* event);

    public:
         void onLastPriceUpdate(LPATQUOTESTREAM_TRADE_UPDATE pTradeUpdate);
         void onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate);

    signals:
//        void lastPriceUpdated(const TickerId, TradeUpdate);
//        void quoteUpdated(const TickerId, QuoteUpdate);

        void tickPriceUpdated( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute);
        void tickSizeUpdated( const TickerId tickerId, const TickType field, const int size);
        void tickGenericUpdated(const TickerId tickerId, const TickType tickType, const double value);


        void snapshotUpdated(const TickerId, const double, const int);
//        void oneMinuteSnapshotUpdated(const TickerId, const double);

//        void twoMinuteSnapshotUpdated(const TickerId, const double);
//        void fiveMinuteSnapshotUpdated(const TickerId, const double);
//        void tenMinuteSnapshotUpdated(const TickerId, const double);

};

#endif
