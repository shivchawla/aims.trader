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

void Instrument::onLastPriceUpdate(LPATQUOTESTREAM_TRADE_UPDATE pTradeUpdate)
{
    mutex.lock();
    _lastTradeUpdate.dateTime = pTradeUpdate->lastDateTime;
    _lastTradeUpdate.lastPrice = pTradeUpdate->lastPrice.price;
    _lastTradeUpdate.lastSize = pTradeUpdate->lastSize;
    emit lastPriceUpdated(_tickerId,_lastTradeUpdate);
    mutex.unlock();
}

void Instrument::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
    mutex.lock();
    _lastQuoteUpdate.dateTime = pQuoteUpdate->quoteDateTime;
    _lastQuoteUpdate.bidPrice = pQuoteUpdate->bidPrice.price;
    _lastQuoteUpdate.bidSize = pQuoteUpdate->bidSize;
    _lastQuoteUpdate.askPrice = pQuoteUpdate->askPrice.price;
    _lastQuoteUpdate.askSize = pQuoteUpdate->askSize;
    emit quoteUpdated(_tickerId, _lastQuoteUpdate);
    mutex.unlock();
}

/*
void Instrument::setTickerId(const int tickerId) 
{
//	_tickerId = tickerId;
}

void Instrument::setBid(const double bid)
{
  //  _bidPrice = bid;
}

void Instrument::setAsk(const double ask)
{
    //_askPrice = ask;
}

void Instrument::setLast(const double last)
{
    //_lastPrice = last;
}

void Instrument::setHigh(const double high)
{
    //_highPrice = high;
}

void Instrument::setClose(const double close)
{
    //_closePrice = close;
}

void Instrument::setLow(const double low)
{
    //_lowPrice = low;
}

void Instrument::setLastSize(const int size)
{
    //_lastSize=size;
}

void Instrument::setAskSize(const int size)
{
    //_askSize=size;
}

void Instrument::setBidSize(const int size)
{
//    _bidSize = size;
}

*/
void Instrument::setContractDetails(const ContractDetails& contractDetails)
{
    _contractDetails=contractDetails;
}

const long Instrument::getTickerId() const
{
	return _tickerId;
}

const Contract& Instrument::getContract() const
{
    return _contract;
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

void Instrument::tickPrice( const TickerId tickerId, const TickType field, const double price, const int canAutoExecute)
{
   emit tickPriceUpdated(tickerId, field, price, canAutoExecute);
}

void Instrument::tickSize( const TickerId tickerId, const TickType field, const int size)
{
    emit tickSizeUpdated(tickerId, field, size);
}

void Instrument::tickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{
   emit tickGenericUpdated(tickerId, tickType, value);
}
