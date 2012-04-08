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
#include <QTimerEvent>

Instrument::Instrument(const TickerId tickerId, const Contract& contract, int multiplier)
{
	_contract = contract;
    _tickerId = tickerId;
    _minuteCount=0;
    _alarmSet=false;
    //setAlarm();
}

void Instrument::setAlarm()
{
    QTime currentTime = QTime::currentTime();
    QTime nextTime(currentTime.hour(),currentTime.minute(),0);

    //set an alarm at every minute interval
    nextTime.addSecs(60);
    int timeout = currentTime.msecsTo(nextTime);
    timer.start(timeout,this);
}

Instrument::~Instrument()
{
   timer.stop();
}

///Updates the Instrument with new trade price from Active Tick
void Instrument::onLastPriceUpdate(LPATQUOTESTREAM_TRADE_UPDATE pTradeUpdate)
{
    //this is fine
    //but we can make a uniform
   /* mutex.lock();
    _lastTradeUpdate.dateTime = pTradeUpdate->lastDateTime;
    _lastTradeUpdate.lastPrice = pTradeUpdate->lastPrice.price;
    _lastTradeUpdate.lastSize = pTradeUpdate->lastSize;
    emit lastPriceUpdated(_tickerId,_lastTradeUpdate);
    mutex.unlock();*/

    tickGeneric(LAST, pTradeUpdate->lastPrice.price);
    tickGeneric(LAST_SIZE, pTradeUpdate->lastSize);
}

///Updates the Instrument with new quote price from Active Tick
void Instrument::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
   /* mutex.lock();
    _lastQuoteUpdate.dateTime = pQuoteUpdate->quoteDateTime;
    _lastQuoteUpdate.bidPrice = pQuoteUpdate->bidPrice.price;
    _lastQuoteUpdate.bidSize = pQuoteUpdate->bidSize;
    _lastQuoteUpdate.askPrice = pQuoteUpdate->askPrice.price;
    _lastQuoteUpdate.askSize = pQuoteUpdate->askSize;

    //emits a signal to GUI
    //this signal is non-blocking but can be a bottle-neck with high subscrption rate
    //and can block the mutex for longer than desired
    emit quoteUpdated(_tickerId, _lastQuoteUpdate);
    mutex.unlock();*/
    tickGeneric(BID, pQuoteUpdate->bidPrice.price);
    tickGeneric(BID_SIZE, pQuoteUpdate->bidSize);
    tickGeneric(ASK, pQuoteUpdate->askPrice.price);
    tickGeneric(ASK_SIZE, pQuoteUpdate->askSize);
}


void Instrument::calculateOneMinuteSnapshot()
{
    emit oneMinuteSnapshotUpdated(_tickerId, _lastPrice);
    /*mutex.lock();
    _oneMinuteSnapshot = _lastPrice;
    mutex.unlock();*/
}

void Instrument::calculateTwoMinuteSnapshot()
{
    emit twoMinuteSnapshotUpdated(_tickerId, _lastPrice);
   /* mutex.lock();
    _twoMinuteSnapshot = _lastPrice;
    mutex.unlock();*/
}

void Instrument::calculateFiveMinuteSnapshot()
{
    emit fiveMinuteSnapshotUpdated(_tickerId, _lastPrice);
    /*mutex.lock();
    _fiveMinuteSnapshot = _lastPrice;
    mutex.unlock();*/
}

void Instrument::calculateTenMinuteSnapshot()
{
    emit tenMinuteSnapshotUpdated(_tickerId, _lastPrice);
    /*
    mutex.lock();
    _tenMinuteSnapshot = _lastPrice;
    mutex.unlock();*/
}

void Instrument::setTickerId(const int tickerId) 
{
  _tickerId = tickerId;
}

void Instrument::calculateSnapshot(const int minute)
{
    switch(minute)
    {
        case 1: emit oneMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
        case 2: emit twoMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
        case 5: emit fiveMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
        case 10: emit tenMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
    }
}

void Instrument::setBid(const double bid)
{
    mutex.lock();
   _bidPrice = bid;
   mutex.unlock();
}

void Instrument::setAsk(const double ask)
{
    mutex.lock();
    _askPrice = ask;
    mutex.unlock();
}

void Instrument::setOpen(const double open)
{
    mutex.lock();
    _openPrice = open;
    mutex.unlock();
}

void Instrument::setVolume(const int volume)
{
    mutex.lock();
    _volume = volume;
    mutex.unlock();
}

void Instrument::setLast(const double last)
{
    mutex.lock();
    _lastPrice = last;
    mutex.unlock();
}

void Instrument::setHigh(const double high)
{
    mutex.lock();
    _highPrice = high;
    mutex.unlock();
}

void Instrument::setClose(const double close)
{
    mutex.lock();
    _closePrice = close;
    mutex.unlock();
}

void Instrument::setLow(const double low)
{
    mutex.lock();
    _lowPrice = low;
    mutex.unlock();
}

void Instrument::setLastSize(const int size)
{
    mutex.lock();
    _lastSize=size;
    mutex.unlock();
}

void Instrument::setAskSize(const int size)
{
    mutex.lock();
    _askSize=size;
    mutex.unlock();
}

void Instrument::setBidSize(const int size)
{
    mutex.lock();
     _bidSize = size;
     mutex.unlock();
}


///Sets the contract Details for a Contract
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

//Updates the instrument for new tickPrices from IB
void Instrument::tickPrice(const TickType field, const double price, const int canAutoExecute)
{
   emit tickPriceUpdated(_tickerId, field, price, canAutoExecute);
   switch(field)
   {
        case OPEN: setOpen(price); break;
        case CLOSE: setClose(price); break;
        case HIGH: setHigh(price); break;
        case LOW: setLow(price); break;
        case BID: setBid(price); break;
        case ASK: setAsk(price); break;
        case LAST: setLast(price); break;
   }
}

//Updates the instrument for new tickSize from IB
void Instrument::tickSize(const TickType field, const int size)
{
    emit tickSizeUpdated(_tickerId, field, size);
    switch(field)
    {
        case BID_SIZE: setBidSize(size); break;
        case ASK_SIZE: setAskSize(size); break;
        case LAST_SIZE: setLastSize(size); break;
    }
}

void Instrument::tickGeneric(const TickType tickType, const double value)
{
   emit tickGenericUpdated(_tickerId, tickType, value);
   switch(tickType)
   {
       case OPEN: setOpen(value); break;
       case CLOSE: setClose(value); break;
       case HIGH: setHigh(value); break;
       case LOW: setLow(value); break;
       case BID: setBid(value); break;
       case ASK: setAsk(value); break;
       case LAST: setLast(value); break;
       case BID_SIZE: setBidSize(value); break;
       case ASK_SIZE: setAskSize(value); break;
       case LAST_SIZE: setLastSize(value);break;
       case VOLUME: setVolume(value); break;
   }
}

void Instrument::timerEvent(QTimerEvent* event)
{
    if(event->timerId() == timer.timerId()) //timer even triggered by instrument
    {
        if(!_alarmSet)
        {
            timer.stop();
            timer.start(60000,this);
            _alarmSet=true;
        }
        _minuteCount++;


        calculateOneMinuteSnapshot();
        emit oneMinuteSnapshotUpdated(_tickerId, _oneMinuteSnapshot);
        if(_minuteCount%2==0)
        {
            calculateTwoMinuteSnapshot();
            emit twoMinuteSnapshotUpdated(_tickerId, _twoMinuteSnapshot);
        }

        if(_minuteCount%5 == 0)
        {
            calculateFiveMinuteSnapshot();
             emit fiveMinuteSnapshotUpdated(_tickerId, _fiveMinuteSnapshot);
        }

        if(_minuteCount%10 == 0)
        {
            calculateTenMinuteSnapshot();
            emit tenMinuteSnapshotUpdated(_tickerId, _tenMinuteSnapshot);
        }
    }
}

const double Instrument::getSnapshot(const int minute)
{
    //do we need lock for this
    switch(minute)
    {
        case 1: return _oneMinuteSnapshot; break;
        case 2: return _twoMinuteSnapshot; break;
        case 5: return _fiveMinuteSnapshot; break;
        case 10: return _tenMinuteSnapshot; break;
    }
}

const double Instrument::getLastPrice()
{
    double price;
    mutex.lock();
    price = _lastPrice;
    mutex.unlock();
    return price;
}

