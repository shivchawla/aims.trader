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
#include "Platform/Commission/CommissionFactory.h"

//Instrument::Instrument(const TickerId tickerId, const ATContract& contract, int multiplier)
//{
//    _aTcontract = contract;
//    _tickerId = tickerId;
//    _minuteCount=0;
//    _alarmSet=false;

//    _bidPrice = _askPrice = _closePrice = _openPrice = _highPrice = _lowPrice = _lastPrice = _askSize = _bidSize = _lastSize = _volume = 0;

////    _oneMinuteSnapshot = 0;
////    _twoMinuteSnapshot = 0;
////    _fiveMinuteSnapshot = 0;
////    _tenMinuteSnapshot = 0;

//    //setAlarm();
//}


//Instrument::Instrument(const Contract& instrumentContract):QObject()
//{
//         _contract = instrumentContract;
//         _minuteCount = 0;
//         _alarmSet = false;

//        _bidPrice = _askPrice = _closePrice = _openPrice = _highPrice = _lowPrice = _lastPrice = _askSize = _bidSize = _lastSize = _volume = 0;

//    ////    _oneMinuteSnapshot = 0;
//    ////    _twoMinuteSnapshot = 0;
//    ////    _fiveMinuteSnapshot = 0;
//    ////    _tenMinuteSnapshot = 0;

//}

Instrument::Instrument(const TickerId tickerId, const Contract& contract, const int):QObject()
{
        _tickerId = tickerId;
        _contract = contract;
        _minuteCount = 0;
        _alarmSet = false;

        _bidPrice = _askPrice = _closePrice = _openPrice = _highPrice = _lowPrice = _lastPrice = _askSize = _bidSize = _lastSize = _volume = 0;

    ////    _oneMinuteSnapshot = 0;
    ////    _twoMinuteSnapshot = 0;
    ////    _fiveMinuteSnapshot = 0;
    ////    _tenMinuteSnapshot = 0;

}
Instrument::Instrument(const TickerId tickerId,const InstrumentContract& instrumentContract, int multiplier):QObject()
{
    _tickerId = tickerId;
    _contract.symbol = instrumentContract.symbol.toStdString();
    _contract.exchange = instrumentContract.exchangeCode.toStdString();
    _contract.currency ="USD";
    _contract.multiplier = multiplier;
    _minuteCount = 0;
    _alarmSet = false;

    _bidPrice = _askPrice = _closePrice = _openPrice = _highPrice = _lowPrice = _lastPrice = _askSize = _bidSize = _lastSize = _volume = 0;
}


void Instrument::setAlarm()
{
    //QTime currentTime = QTime::currentTime();
    //QTime nextTime(currentTime.hour(),currentTime.minute(),0);

    //set an alarm at every minute interval
    //nextTime.addSecs(60);
    //int timeout = currentTime.msecsTo(nextTime);
    int timeout = ((60 - QTime::currentTime().second()) * 1000);
    timer.start(timeout,this);
}

Instrument::~Instrument()
{}

///Updates the Instrument with new trade price from Active Tick
void Instrument::onLastPriceUpdate(LPATQUOTESTREAM_TRADE_UPDATE pTradeUpdate)
{
    tickGeneric(LAST, pTradeUpdate->lastPrice.price);
    tickGeneric(LAST_SIZE, pTradeUpdate->lastSize);
}

///Updates the Instrument with new quote price from Active Tick
void Instrument::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
    tickGeneric(BID, pQuoteUpdate->bidPrice.price);
    tickGeneric(BID_SIZE, pQuoteUpdate->bidSize);
    tickGeneric(ASK, pQuoteUpdate->askPrice.price);
    tickGeneric(ASK_SIZE, pQuoteUpdate->askSize);
}

void Instrument::calculateSnapshot(const int nMinutes)
{
    emit snapshotUpdated(_tickerId, _lastPrice, nMinutes);
}

//void Instrument::calculateTwoMinuteSnapshot()
//{
//    emit twoMinuteSnapshotUpdated(_tickerId, _lastPrice);
//   /* mutex.lock();
//    _twoMinuteSnapshot = _lastPrice;
//    mutex.unlock();*/
//}

//void Instrument::calculateFiveMinuteSnapshot()
//{
//    emit fiveMinuteSnapshotUpdated(_tickerId, _lastPrice);
//    /*mutex.lock();
//    _fiveMinuteSnapshot = _lastPrice;
//    mutex.unlock();*/
//}

//void Instrument::calculateTenMinuteSnapshot()
//{
//    emit tenMinuteSnapshotUpdated(_tickerId, _lastPrice);
//    /*
//    mutex.lock();
//    _tenMinuteSnapshot = _lastPrice;
//    mutex.unlock();*/
//}

/*void Instrument::onLastPriceUpdate(LPATQUOTESTREAM_TRADE_UPDATE pTradeUpdate)
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
}*/


//void Instrument::setTickerId(const int tickerId)
//{
//  // _tickerId = tickerId;
//}

//void Instrument::calculateSnapshot(const int minute)
//{
//    switch(minute)
//    {
//        case 1: emit oneMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
//        case 2: emit twoMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
//        case 5: emit fiveMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
//        case 10: emit tenMinuteSnapshotUpdated(_tickerId, _lastPrice); break;
//    }
//}

void Instrument::setBid(const double bid)
{
    _mutex.lock();
   _bidPrice = bid;
   _mutex.unlock();
}

void Instrument::setAsk(const double ask)
{
    _mutex.lock();
    _askPrice = ask;
    _mutex.unlock();
}

void Instrument::setOpen(const double open)
{
    _mutex.lock();
    _openPrice = open;
    _mutex.unlock();
}

void Instrument::setVolume(const int volume)
{
    _mutex.lock();
    _volume = volume;
    _mutex.unlock();
}

void Instrument::setLast(const double last)
{
    _mutex.lock();
    _lastPrice = last;
    _mutex.unlock();
}

void Instrument::setHigh(const double high)
{
    _mutex.lock();
    _highPrice = high;
    _mutex.unlock();
}

void Instrument::setClose(const double close)
{
    _mutex.lock();
    _closePrice = close;
    _mutex.unlock();
}

void Instrument::setLow(const double low)
{
    _mutex.lock();
    _lowPrice = low;
    _mutex.unlock();
}

void Instrument::setLastSize(const int size)
{
    _mutex.lock();
    _lastSize=size;
    _mutex.unlock();
}

void Instrument::setAskSize(const int size)
{
    _mutex.lock();
    _askSize=size;
    _mutex.unlock();
}

void Instrument::setBidSize(const int size)
{
    _mutex.lock();
     _bidSize = size;
     _mutex.unlock();
}

///Sets the contract Details for a Contract
void Instrument::setContractDetails(const ContractDetails& contractDetails)
{
    _mutex.lock();
    _contractDetails=contractDetails;
    _mutex.unlock();
}

const TickerId Instrument::getTickerId() const
{
    return _tickerId;
}

//const ATContract& Instrument::getContract() const
//{
//    return _aTcontract;
//}

const Contract Instrument::getContract() const
{
    return _contract;
}


//const ContractId Instrument::getContractId()
//{
//    return _aTcontract.contractId;
//}
		

//const std::string Instrument::toString() const
//{
//    IBString s = _aTcontract.contract.symbol + "-" + _aTcontract.contract.exchange;
//    if (_aTcontract.contract.currency != "")
//	{
//        s += "-" + _aTcontract.contract.currency;
//	}
//	return s;
//}

const QString Instrument::toString()
{
    QString inst;
    inst = QString::fromStdString(_contract.symbol);

    return inst;
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
        //calculateSnapshot(++_minuteCount);
//        calculateOneMinuteSnapshot();
//        emit oneMinuteSnapshotUpdated(_tickerId, _oneMinuteSnapshot);
//        if(_minuteCount%2==0)
//        {
//            calculateTwoMinuteSnapshot();
//            emit twoMinuteSnapshotUpdated(_tickerId, _twoMinuteSnapshot);
//        }

//        if(_minuteCount%5 == 0)
//        {
//            calculateFiveMinuteSnapshot();
//             emit fiveMinuteSnapshotUpdated(_tickerId, _fiveMinuteSnapshot);
//        }

//        if(_minuteCount%10 == 0)
//        {
//            calculateTenMinuteSnapshot();
//            emit tenMinuteSnapshotUpdated(_tickerId, _tenMinuteSnapshot);
//        }
    }
}

//const double Instrument::getSnapshot(const int minute)
//{
//    //do we need lock for this
//    switch(minute)
//    {
//        case 1: return _oneMinuteSnapshot; break;
//        case 2: return _twoMinuteSnapshot; break;
//        case 5: return _fiveMinuteSnapshot; break;
//        case 10: return _tenMinuteSnapshot; break;
//    }
//}

const double Instrument::getLastPrice()
{
    //double price;
    _mutex.lock();
    double lastPrice = _lastPrice;
    _mutex.unlock();
    return lastPrice;
}

const double Instrument::getAskPrice()
{
    //double askPrice;
    _mutex.lock();
    double askPrice = _askPrice;
    _mutex.unlock();
    return askPrice;
}

const double Instrument::getBidPrice()
{
    //double bidPrice;
    _mutex.lock();
    double bidPrice = _bidPrice;
    _mutex.unlock();
    return bidPrice;
}
