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

class Instrument: public QObject
{
    Q_OBJECT
    private:
        ContractDetails _contractDetails;
        Contract _contract;
        long _tickerId;
        double _bid, _ask, _close, _high, _low, _last, _askSize, _bidSize, _lastSize;
        int multiplier;

    public:
        Instrument(); 
        ~Instrument(); 
        Instrument(const TickerId ,const Contract&, int multiplier);

    public slots:
        const long getTickerId() const;
        const std::string toString() const; 
        void setTickerId(const int tickerId);
        void setBid(const double bid);
        void setAsk(const double ask);
        void setLast(const double last);
        void setHigh(const double high);
        void setClose(const double close);
        void setLow(const double low);
        void setLastSize(const int size);
        void setAskSize(const int size);
        void setBidSize(const int size);
        void setContractDetails(const ContractDetails&);

    public:
        void linkInstrument(QObject*);

    signals:
        void lastPriceUpdated(const TickerId, const double);

};

#endif
