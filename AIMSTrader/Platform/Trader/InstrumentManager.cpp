#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"
#include "Platform/Position/Instrument.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Trader/TraderAssistant.h"
#include <QReadWriteLock>
#include <iostream>
#include "ActiveTickFeed/Utils/Helper.h"
#include "ActiveTickFeed/Utils/ActiveTickApi.h"
#include <QMetaType>
#include "Platform/Utils/Converter.h"
#include "Platform/Utils/TestDataGenerator.h"
#include <QDebug>
#include "Platform/View/OutputInterface.h"

/*
 * Constructor InstrumentManager
 */
InstrumentManager::InstrumentManager():QObject(),_tickerId(0),_lockForInstrumentMap(new QReadWriteLock())
{
    qRegisterMetaType<TradeUpdate>("TradeUpdate");
    qRegisterMetaType<QuoteUpdate>("QuoteUpdate");
    InstrumentView* instrumentView = MainWindow::Instance()->getInstrumentView();
    QObject::connect(this, SIGNAL(instrumentAdded(const TickerId, const Contract&)), instrumentView, SLOT(addInstrument(const TickerId, const Contract&)));

    //setAlarm();
}

InstrumentManager::~InstrumentManager()
{}

void InstrumentManager::onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate)
{
    //resolve the instrument from the instrument map
    // lock this map for reads
    //this is serious overhead. you can not have conversion multiple times

    String symbol = AimsTrader::Converter::ConvertString(pLastUpdate->symbol.symbol, _countof(pLastUpdate->symbol.symbol));
    _lockForInstrumentMap->lockForRead();
    TickerId tickerId = _stringSymbolToTickerId[symbol];
    _instruments[tickerId]->onLastPriceUpdate(pLastUpdate);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
    String symbol = AimsTrader::Converter::ConvertString(pQuoteUpdate->symbol.symbol, _countof(pQuoteUpdate->symbol.symbol));

    _lockForInstrumentMap->lockForRead();
    TickerId tickerId = _stringSymbolToTickerId[symbol];
    _instruments[tickerId]->onQuoteUpdate(pQuoteUpdate);
    //emit quoteUpdated(tickerId, pQuoteUpdate);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::setContractDetails(const TickerId tickerId, const ContractDetails& contractDetails)
{
    _lockForInstrumentMap->lockForWrite();
    _instruments[tickerId]->setContractDetails(contractDetails);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::requestMarketData(const TickerId tickerId, DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    testConnectivity(source);
    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = MainWindow::Instance()->getInstrumentView();
    //Instrument* instrument = NULL;
    //lockForInstrumentMap->lockForRead();
    const Contract contract = getContractForTicker(tickerId);
    //lockForInstrumentMap->unlock();

    Instrument* instrument = getInstrumentForTicker(tickerId);
    //bool newRequest = false;
    /*if(!instrument)
    {
        newRequest=true;
        //unlock the read lock
        //lockForInstrumentMap->unlock();

        instrument = new Instrument(tickerId, getContractForTicker(tickerId), 1);
        //lock for writes
        lockForInstrumentMap->lockForWrite();
        _stringSymbolToTickerId[symbol] = tickerId;
        _tickerIdToSymbol[tickerId] = symbol;
        _instruments[tickerId] = instrument;
        lockForInstrumentMap->unlock();
        //same as above
    }*/

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
    testConnectivity(source);
    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = MainWindow::Instance()->getInstrumentView();
    bool newRequest = false;
    Instrument* instrument = getInstrumentForSymbol(symbol);

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
        instrument = addInstrument(contract);
    }

    TickerId tickerId = instrument->getTickerId();

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
    bool isConnected = testConnectivity(source);
    Instrument* instrument = getInstrumentForContract(contract);
    bool newRequest = false;
    String symbol;
    if(!instrument)
    {
        newRequest = true;
        instrument = addInstrument(contract);
    }

    TickerId tickerId = instrument->getTickerId();

    linkSubscriberToInstrument(instrument, subscriber, requestType);

    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = MainWindow::Instance()->getInstrumentView();
    if(strategy)
    {
        linkInstrumentToView(instrument, instrumentView, tickerId, contract);
    }

    //if newRequest is true, request ActiveTick else request Intearctive Broker
    if(newRequest)
    {
        if(isConnected)
        {
            reqMktData(tickerId, contract, symbol, source);
        }
        else if(Service::Instance()->getMode() == Test)
        {
            Service::Instance()->getTestDataGenerator()->reqMarketData(tickerId);
        }
    }
}

void InstrumentManager::reqMktData(const TickerId tickerId, const Contract& contract, const String& symbol, const DataSource source)
{
    switch(source)
    {
        case ActiveTick: Service::Instance()->getActiveTickSession()->requestTradeStream(symbol); break;
        case IB: Service::Instance()->getTrader()->getTraderAssistant()->requestMarketData(tickerId, contract); break;
    }
}

void InstrumentManager::mktDataCancelled(const TickerId tickerId)
{}

void InstrumentManager::removeInstrument(const TickerId tickerId)
{
    _lockForInstrumentMap->lockForWrite();
    delete _instruments[tickerId];
    _instruments.erase(tickerId);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::cancelMarketData(const Contract& contract)
{
    Service::Instance()->getActiveTickSession()->cancelMarketData(contract);

    TickerId tickerId = getTickerId(contract);
    Service::Instance()->getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

void InstrumentManager::cancelMarketData(const TickerId tickerId)
{
    const Contract contract = getContractForTicker(tickerId);
    //lockForInstrumentMap->lockForRead();
    //Contract contract = _instruments[tickerId]->getContract();//getContractForTicker(tickerId);
    //lockForInstrumentMap->unlock();

    Service::Instance()->getActiveTickSession()->cancelMarketData(contract);
    Service::Instance()->getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

void InstrumentManager::unSubscribeMarketData(const TickerId tickerId, DataSubscriber* subscriber)
{
    if(_instruments.count(tickerId))
    {
        _instruments[tickerId]->disconnect(subscriber);
    }
}


const TickerId InstrumentManager::getTickerId(const Contract& contract)
{ 
  TickerId tickerId;

  String symbol = getSymbol(contract);
  _lockForInstrumentMap->lockForRead();
  if(_stringSymbolToTickerId.count(symbol)!=0)
  {     
      tickerId = _stringSymbolToTickerId[symbol];
      _lockForInstrumentMap->unlock();
  }
  else
  {
      _lockForInstrumentMap->unlock();

      _lockForInstrumentMap->lockForWrite();
      tickerId =++_tickerId;
      _stringSymbolToTickerId[symbol] = tickerId;
      _tickerIdToSymbol[tickerId]  = symbol;
      _lockForInstrumentMap->unlock();
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
    _lockForInstrumentMap->lockForRead();
    if(_tickerIdToSymbol.count(tickerId)!=0)
    {
        symbol = _tickerIdToSymbol[tickerId];
    }
    _lockForInstrumentMap->unlock();
    return symbol;
}

void InstrumentManager::printThreadId()
{
    //printf("\nInstrument Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
}

const Contract InstrumentManager::getContractForTicker(const TickerId tickerId)
{
    _lockForInstrumentMap->lockForRead();
    Contract c = _instruments[tickerId]->getContract();
    _lockForInstrumentMap->unlock();
    return c;
}

void InstrumentManager::tickPrice( TickerId tickerId,  TickType field, double price, int canAutoExecute)
{
    _lockForInstrumentMap->lockForRead();
    _instruments[tickerId]->tickPrice(field, price, canAutoExecute);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::tickSize( TickerId tickerId, TickType field, int size)
{
    _lockForInstrumentMap->lockForRead();
   _instruments[tickerId]->tickSize(field, size);
   _lockForInstrumentMap->unlock();
}

void InstrumentManager::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
    _lockForInstrumentMap->lockForRead();
    _instruments[tickerId]->tickGeneric(tickType, value);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::reportEvent(const String& message)
{
    OutputInterface::Instance()->reportEvent("InstrumentManager", message);
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
    std::map<TickerId, Instrument*>:: iterator it;

    _lockForInstrumentMap->lockForRead();
    std::map<TickerId, Instrument*>:: iterator end = _instruments.end();
    for(it=_instruments.begin();it!=end;++it)
    {
        Instrument* inst = it->second;
        if(!inst)
        {
            int k=1;
        }
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
    _lockForInstrumentMap->unlock();
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
        QObject::connect(instrument, SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double, int)), subscriber, SLOT(onTickPriceUpdate(const TickerId, const TickType, const double)));
        QObject::connect(instrument, SIGNAL(tickGenericUpdated(const TickerId, const TickType, const double)), subscriber, SLOT(onTickPriceUpdate(const TickerId, const TickType, const double)));
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
    _lockForInstrumentMap->lockForRead();
    if(_instruments.count(tickerId))
    {
        price = _instruments[tickerId]->getLastPrice();
    }
    _lockForInstrumentMap->unlock();

    return price;
}

const double InstrumentManager::getAskPrice(const TickerId tickerId)
{
    double askPrice = 0;
    _lockForInstrumentMap->lockForRead();
    if(_instruments.count(tickerId))
    {
        askPrice = _instruments[tickerId]->getAskPrice();
    }
    _lockForInstrumentMap->unlock();

    return askPrice;
}

const double InstrumentManager::getBidPrice(const TickerId tickerId)
{
    double bidPrice = 0;
    _lockForInstrumentMap->lockForRead();
    if(_instruments.count(tickerId))
    {
        bidPrice = _instruments[tickerId]->getBidPrice();
    }
    _lockForInstrumentMap->unlock();

    return bidPrice;
}

const bool InstrumentManager::testConnectivity(const DataSource source)
{
    bool connected;
    //check the connectivity with the datasource******
    if(!(connected = isConnected(source)))
    {
        switch(source)
        {
            case IB: reportEvent("Interactive Broker not Connected"); break;
            case ActiveTick: reportEvent("Active Tick not Connected"); break;
        }
    }
    return connected;
}

Instrument* InstrumentManager::getInstrumentForContract(const Contract& contract)
{
    String symbol = String::fromStdString(contract.symbol);
    Instrument* instrument = NULL;
    TickerId tickerId;
    _lockForInstrumentMap->lockForRead();
    if(_stringSymbolToTickerId.count(symbol)!=0)
    {
        instrument = _instruments[_stringSymbolToTickerId[symbol]];
    }
    _lockForInstrumentMap->unlock();

    return instrument;
}

Instrument* InstrumentManager::getInstrumentForSymbol(const String& symbol)
{
    Instrument* instrument;
    _lockForInstrumentMap->lockForRead();
    if(_stringSymbolToTickerId.count(symbol)!=0)
    {
        instrument = _instruments[_stringSymbolToTickerId[symbol]];
    }
    _lockForInstrumentMap->unlock();

    return instrument;
}

Instrument* InstrumentManager::getInstrumentForTicker(const TickerId tickerId)
{
    Instrument* instrument;
    _lockForInstrumentMap->lockForRead();
    if(_instruments.count(tickerId)!=0)
    {
        instrument = _instruments[tickerId];
        //contract = _instruments[tickerId]->getContract();
    }
    _lockForInstrumentMap->unlock();

    return instrument;
}

Instrument* InstrumentManager::addInstrument(const String& symbol)
{
    Contract contract;
    contract.symbol = symbol.toStdString();
    contract.secType = "STK";
    contract.exchange = "SMART";
    contract.primaryExchange = "ISLAND";
    contract.currency = "USD";

    _lockForInstrumentMap->lockForWrite();
    TickerId tickerId = (_stringSymbolToTickerId.count(symbol)!=0) ? _stringSymbolToTickerId[symbol] : ++_tickerId;
    Instrument* nInstrument = new Instrument(tickerId, contract, 1);
    //lock for writes
    _stringSymbolToTickerId[symbol] = tickerId;
    _tickerIdToSymbol[tickerId] = symbol;
    _instruments[tickerId] = nInstrument;
    _lockForInstrumentMap->unlock();

    return nInstrument;
}

Instrument* InstrumentManager::addInstrument(const Contract& contract)
{
    String symbol = String::fromStdString(contract.symbol);
    _lockForInstrumentMap->lockForWrite();
    TickerId tickerId = (_stringSymbolToTickerId.count(symbol)!=0) ? _stringSymbolToTickerId[symbol] : ++_tickerId;
    Instrument* nInstrument = new Instrument(tickerId, contract, 1);
    //lock for writes
    _stringSymbolToTickerId[symbol] = tickerId;
    _tickerIdToSymbol[tickerId] = symbol;
    _instruments[tickerId] = nInstrument;
    _lockForInstrumentMap->unlock();

    return nInstrument;
}


//template<class T>
//void requestMarketData(const String symbol, DataSubscriber<T>* subscriber, const DataSource source = IB,  const DataRequestType requestType = RealTime);
