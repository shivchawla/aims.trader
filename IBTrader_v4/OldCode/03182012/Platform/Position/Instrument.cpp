/*
 *  Instrument.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 12/14/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Platform/Position/Instrument.h"
#include "Platform/Strategy/Strategy.h"

Instrument::Instrument(const TickerId tickerId, const Contract& contract, int multiplier)
{
	_contract = contract;
    _tickerId = tickerId;
}

Instrument::~Instrument()
{}

void Instrument::setTickerId(const int tickerId) 
{
	_tickerId = tickerId;
}

void Instrument::setBid(const double bid)
{
    _bid = bid;
}

void Instrument::setAsk(const double ask)
{
    _ask = ask;
}

void Instrument::setLast(const double last)
{
    _last = last;
    emit lastPriceUpdated(_tickerId, last);
}

void Instrument::setHigh(const double high)
{
    _high = high;
}

void Instrument::setClose(const double close)
{
    _close = close;
}

void Instrument::setLow(const double low)
{
    _low = low;
}

void Instrument::setLastSize(const int size)
{
    _lastSize=size;
}

void Instrument::setAskSize(const int size)
{
    _askSize=size;
}

void Instrument::setBidSize(const int size)
{
    _bidSize = size;
}

void Instrument::setContractDetails(const ContractDetails& contractDetails)
{
    _contractDetails=contractDetails;
}


const long Instrument::getTickerId() const
{
	return _tickerId;
}
		
const std::string Instrument::toString() const
{
	IBString s = _contract.symbol + "-" + _contract.exchange;
	if (_contract.currency != "") 
	{
		s += "-" + _contract.currency;
	}
	return s;
}

void Instrument::linkInstrument(QObject* observer)
{
    connect(this,SIGNAL(lastPriceUpdated(const TickerId, const double)), observer, SLOT(updatePosition(const TickerId, const double)));
}
