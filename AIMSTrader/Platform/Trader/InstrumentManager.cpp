#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"
#include "Platform/Position/Instrument.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Trader/TraderAssistant.h"
#include <QReadWriteLock>
#include <iostream>
#include "ActiveTickFeed/Utils/Helper.h"
#include "ActiveTickFeed/Utils/ActiveTickApi.h"
#include <QMetaType>
#include "Platform/View/MainWindow.h"
#include "Platform/View/InstrumentView.h"
#include "Platform/Utils/Converter.h"

/*
 * Constructor InstrumentManager
 */
InstrumentManager::InstrumentManager():QObject(),_tickerId(0),lockForInstrumentMap(new QReadWriteLock())
{
    qRegisterMetaType<TradeUpdate>("TradeUpdate");
    qRegisterMetaType<QuoteUpdate>("QuoteUpdate");
    InstrumentView* instrumentView = MainWindow::mainWindow()->getInstrumentView();
    QObject::connect(this, SIGNAL(instrumentAdded(const TickerId, const Contract&)), instrumentView, SLOT(addInstrument(const TickerId, const Contract&)));

    //setAlarm();
}

InstrumentManager::~InstrumentManager()
{}

/*
 *
 */
void InstrumentManager::onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate)
{
    //resolve the instrument from the instrument map
    // lock this map for reads

    //this is serious overhead. you can not have conversion multiple times

    String symbol = AimsTrader::Converter::ConvertString(pLastUpdate->symbol.symbol, _countof(pLastUpdate->symbol.symbol));
    lockForInstrumentMap->lockForRead();
    TickerId tickerId = _stringSymbolToTickerId[symbol];
    _instruments[tickerId]->onLastPriceUpdate(pLastUpdate);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
    String symbol = AimsTrader::Converter::ConvertString(pQuoteUpdate->symbol.symbol, _countof(pQuoteUpdate->symbol.symbol));

    lockForInstrumentMap->lockForRead();
    TickerId tickerId = _stringSymbolToTickerId[symbol];
    _instruments[tickerId]->onQuoteUpdate(pQuoteUpdate);
    //emit quoteUpdated(tickerId, pQuoteUpdate);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::setContractDetails(const TickerId tickerId, const ContractDetails& contractDetails)
{
    lockForInstrumentMap->lockForWrite();
    _instruments[tickerId]->setContractDetails(contractDetails);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::requestMarketData(const TickerId tickerId, DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    //check the connectivity with the datasource******
    if(!isConnected(source))
    {
        switch(source)
        {
            case IB: reportEvent("Interactive Broker not Connected"); break;
            case ActiveTick: reportEvent("Active Tick not Connected"); break;
        }
        return;
    }

    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = MainWindow::mainWindow()->getInstrumentView();
    Instrument* instrument = NULL;
    Contract contract;

    lockForInstrumentMap->lockForRead();
    if(_instruments.count(tickerId)!=0)
    {
        instrument = _instruments[tickerId];
        contract = _instruments[tickerId]->getContract();
    }
    lockForInstrumentMap->unlock();

    linkSubscriberToInstrument(instrument, subscriber, requestType);

    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    if(strategy)
    {
        linkInstrumentToView(instrument, instrumentView, tickerId, contract);
    }

}

void InstrumentManager::requestMarketData(const String symbol, DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    //check the connectivity with the datasource******
    if(!isConnected(source))
    {
        switch(source)
        {
            case IB: reportEvent("Interactive Broker not Connected"); break;
            case ActiveTick: reportEvent("Active Tick not Connected"); break;
        }
        return;
    }

    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = MainWindow::mainWindow()->getInstrumentView();
    bool newRequest = false;
    Instrument* instrument = NULL;

    lockForInstrumentMap->lockForRead();
    if(_stringSymbolToTickerId.count(symbol)!=0)
    {
        instrument = _instruments[_stringSymbolToTickerId[symbol]];
    }
    lockForInstrumentMap->unlock();

    TickerId tickerId;
    Contract contract;
    contract.symbol = symbol.toStdString();
    contract.secType = "STK";
    contract.exchange = "SMART";
    contract.primaryExchange = "ISLAND";
    contract.currency = "USD";

    if(!instrument)
    {
        newRequest=true;
        //unlock the read lock
        //lockForInstrumentMap->unlock();

        tickerId = (_stringSymbolToTickerId.count(symbol)!=0) ? _stringSymbolToTickerId[symbol] : ++_tickerId;
        instrument = new Instrument(tickerId, contract, 1);
        //lock for writes
        lockForInstrumentMap->lockForWrite();
        _stringSymbolToTickerId[symbol] = tickerId;
        _tickerIdToSymbol[tickerId] = symbol;
        _instruments[tickerId] = instrument;
        lockForInstrumentMap->unlock();
        //same as above
    }

    linkSubscriberToInstrument(instrument, subscriber, requestType);


    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    if(strategy)
    {
        linkInstrumentToView(instrument, instrumentView, tickerId, contract);
    }

    //if newRequest is true, request ActiveTick else request Intearctive Broker
    if(newRequest)
    {
        reqMktData(tickerId, contract,symbol, source);
    }
}


void InstrumentManager::requestMarketData(const Contract& contract, DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    //check the connectivity with the datasource******
    if(!isConnected(source))
    {
        switch(source)
        {
            case IB: reportEvent("Interactive Broker not Connected"); break;
            case ActiveTick: reportEvent("Active Tick not Connected"); break;
        }
        return;
    }

    InstrumentView* instrumentView = MainWindow::mainWindow()->getInstrumentView();

    bool newRequest = false;
    String symbol = String::fromStdString(contract.symbol);
    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);

    Instrument* instrument = NULL;
    TickerId tickerId;
    lockForInstrumentMap->lockForRead();
    if(_stringSymbolToTickerId.count(symbol)!=0)
    {
        instrument = _instruments[_stringSymbolToTickerId[symbol]];
    }

    lockForInstrumentMap->unlock();
    if(!instrument)
    {
        newRequest=true;
        //unlock the read lock
        //lockForInstrumentMap->unlock();

        tickerId = (_stringSymbolToTickerId.count(symbol)!=0) ? _stringSymbolToTickerId[symbol] : ++_tickerId;
        instrument = new Instrument(tickerId, contract, 1);
        //lock for writes
        lockForInstrumentMap->lockForWrite();
        _stringSymbolToTickerId[symbol] = tickerId;
        _tickerIdToSymbol[tickerId] = symbol;
        _instruments[tickerId] = instrument;
        lockForInstrumentMap->unlock();
        //same as above
    }

    linkSubscriberToInstrument(instrument, subscriber, requestType);

    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    if(strategy)
    {
        linkInstrumentToView(instrument, instrumentView, tickerId, contract);
    }

    //if newRequest is true, request ActiveTick else request Intearctive Broker
    if(newRequest)
    {
        reqMktData(tickerId, contract, symbol, source);
    }
}

void InstrumentManager::reqMktData(const TickerId tickerId, const Contract& contract, const String& symbol, const DataSource source)
{
    switch(source)
    {
        case ActiveTick: Service::Instance()->getActiveTickAPI()->requestTradeStream(symbol); break;
        case IB: Service::Instance()->getTrader()->getTraderAssistant()->requestMarketData(tickerId, contract); break;
    }

}

void InstrumentManager::mktDataCancelled(const TickerId tickerId)
{}

void InstrumentManager::removeInstrument(const TickerId tickerId)
{
    lockForInstrumentMap->lockForWrite();
    delete _instruments[tickerId];
    _instruments.erase(tickerId);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::cancelMarketData(const Contract& contract)
{
    Service::Instance()->getActiveTickAPI()->cancelMarketData(contract);

    TickerId tickerId = getTickerId(contract);
    Service::Instance()->getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

void InstrumentManager::cancelMarketData(const TickerId tickerId)
{
    Contract contract = getContractForTicker(tickerId);
    Service::Instance()->getActiveTickAPI()->cancelMarketData(contract);
    Service::Instance()->getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

const TickerId InstrumentManager::getTickerId(const Contract& contract)
{ 
  TickerId tickerId;

  String symbol = getSymbol(contract);
  lockForInstrumentMap->lockForRead();
  if(_stringSymbolToTickerId.count(symbol)!=0)
  {     
      tickerId = _stringSymbolToTickerId[symbol];
  }
  else
  {
      lockForInstrumentMap->unlock();

      lockForInstrumentMap->lockForWrite();
      tickerId =++_tickerId;
      _stringSymbolToTickerId[symbol] = tickerId;
      _tickerIdToSymbol[tickerId]  = symbol;
      lockForInstrumentMap->unlock();
  }

  return tickerId;
}

const String InstrumentManager::getSymbol(const Contract& contract)
{
    return  String::fromStdString(contract.symbol);
}

const String InstrumentManager::getInstrumentId(const TickerId tickerId)
{
    String symbol;
    lockForInstrumentMap->lockForRead();
    if(_tickerIdToSymbol.count(tickerId)!=0)
    {
        symbol = _tickerIdToSymbol[tickerId];
    }
    lockForInstrumentMap->unlock();
    return symbol;
}

void InstrumentManager::printThreadId()
{
    //printf("\nInstrument Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
}

const Contract& InstrumentManager::getContractForTicker(const TickerId tickerId)
{
    lockForInstrumentMap->lockForRead();
    Contract c = _instruments[tickerId]->getContract();
    lockForInstrumentMap->unlock();
    return c;
}

void InstrumentManager::tickPrice( TickerId tickerId,  TickType field, double price, int canAutoExecute)
{
    lockForInstrumentMap->lockForRead();
    _instruments[tickerId]->tickPrice(field, price, canAutoExecute);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::tickSize( TickerId tickerId, TickType field, int size)
{
    lockForInstrumentMap->lockForRead();
   _instruments[tickerId]->tickSize(field, size);
   lockForInstrumentMap->unlock();
}

void InstrumentManager::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
    lockForInstrumentMap->lockForRead();
    _instruments[tickerId]->tickGeneric(tickType, value);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::reportEvent(const String& message)
{
    Service::Instance()->getEventReport()->report("InstrumentManager", message);
}

/*void InstrumentManager::setAlarm()
{
    QTime currentTime = QTime::currentTime();
    std::cout<<currentTime.hour();
    std::cout<<currentTime.minute();

    QTime temp(currentTime.hour(),currentTime.minute());


    //set an alarm at every minute interval
    QTime nextTime = temp.addSecs(60);
    std::cout<<nextTime.hour();
    std::cout<<nextTime.minute();
    std::cout<<nextTime.second();

    int timeout = currentTime.msecsTo(nextTime);
    timer.start(timeout,this);

    _minuteCount=0;
}*/

void InstrumentManager::generateSnapshot(const int timeInMinutes)
{
    //check for conectivity
    std::map<TickerId, Instrument*>:: iterator end = _instruments.end();
    std::map<TickerId, Instrument*>:: iterator it;

    lockForInstrumentMap->lockForRead();
    for(it=_instruments.begin();it!=end;++it)
    {
        //_oneMinuteSnapshot[it->first] = (it->second)->getLastPrice();
        (it->second)->calculateOneMinuteSnapshot();

        if(timeInMinutes%2==0)
        {
            (it->second)->calculateTwoMinuteSnapshot();
            //_twoMinuteSnapshot[it->first] = (it->second)->getLastPrice();
        }

        if(timeInMinutes%5 == 0)
        {
            (it->second)->calculateFiveMinuteSnapshot();
            //_fiveMinuteSnapshot[it->first] = (it->second)->getLastPrice();
        }

        if(timeInMinutes%10 == 0)
        {
            (it->second)->calculateTenMinuteSnapshot();
            //_tenMinuteSnapshot[it->first] = (it->second)->getLastPrice();
        }
    }
    lockForInstrumentMap->unlock();
}

bool InstrumentManager::isConnected(const DataSource source)
{
    switch(source)
    {
        case IB: return Service::Instance()->getTrader()->getTraderAssistant()->IsConnected(); break;
        case ActiveTick: return true; break;
    }
}

void InstrumentManager::linkSubscriberToInstrument(Instrument* instrument, DataSubscriber* subscriber, DataRequestType requestType)
{
    if(requestType == RealTime)
    {
        QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const TradeUpdate&)), subscriber, SLOT(onTradeUpdate(const TickerId, const TradeUpdate&)));
        QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, const QuoteUpdate&)), subscriber, SLOT(onQuoteUpdate(const TickerId, const QuoteUpdate&)));
        QObject::connect(instrument,SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double, int)), subscriber, SLOT(onTickPriceUpdate(const TickerId, const TickType, const double)));
    }
    else if(requestType == Snapshot)
    {
        QObject::connect(instrument,SIGNAL(oneMinuteSnapshotUpdated(const TickerId, const double)), subscriber, SLOT(updateOneMinuteSnapShot(TickerId,double)));
        QObject::connect(instrument,SIGNAL(twoMinuteSnapshotUpdated(const TickerId, const double)), subscriber, SLOT(updateTwoMinuteSnapShot(TickerId,double)));
        QObject::connect(instrument,SIGNAL(fiveMinuteSnapshotUpdated(const TickerId, const double)), subscriber, SLOT(updateFiveMinuteSnapShot(TickerId,double)));
        QObject::connect(instrument,SIGNAL(tenMinuteSnapshotUpdated(const TickerId, const double)), subscriber, SLOT(updateTenMinuteSnapShot(TickerId,double)));
    }
}

void InstrumentManager::linkInstrumentToView(Instrument* instrument, InstrumentView* instrumentView, const TickerId  tickerId, const Contract& contract)
{
    QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const TradeUpdate&)), instrumentView, SLOT(onTradeUpdate(const TickerId, const TradeUpdate&)));
    QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, const QuoteUpdate& )), instrumentView, SLOT(onQuoteUpdate(const TickerId, const QuoteUpdate&)));
    QObject::connect(instrument,SIGNAL(tickGenericUpdated(const TickerId, const TickType, const double)), instrumentView, SLOT(updateTickGeneric(const TickerId, const TickType, const double)));
    QObject::connect(instrument,SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double,int)), instrumentView, SLOT(updateTickPrice(const TickerId, const TickType, const double, const int)));
    QObject::connect(instrument,SIGNAL(tickSizeUpdated(const TickerId, const TickType,const int)), instrumentView, SLOT(updateTickSize(const TickerId, const TickType,const int)));
    emit instrumentAdded(tickerId, contract);
}

const double InstrumentManager::getLastPrice(const TickerId tickerId)
{
    double price = 0;
    lockForInstrumentMap->lockForRead();
    if(_instruments.count(tickerId))
    {
        price = _instruments[tickerId]->getLastPrice();
    }
    lockForInstrumentMap->unlock();
    return price;
}




