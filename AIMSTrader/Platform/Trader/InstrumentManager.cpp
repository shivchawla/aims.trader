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
#include "Platform/View/IOInterface.h"

/*
 * Constructor InstrumentManager
 */
InstrumentManager::InstrumentManager():_tickerId(0),_lockForInstrumentMap(new QReadWriteLock())
{
    //_ioInterface = ioInterface();
}

InstrumentManager::~InstrumentManager()
{
    foreach(Instrument* instrument, _instruments)
    {
        instrument->deleteLater();
    }

    delete _lockForInstrumentMap;
}

static uint hash(const wchar16_t *p, int n)
{
    uint h = 0;

    while (n--) {
        h = (h << 4) + *p++;
        h ^= (h & 0xf0000000) >> 23;
        h &= 0x0fffffff;
    }

    return h;
}

inline uint qHash(const ATSYMBOL& symbol)
{
    return hash(symbol.symbol, _countof(symbol.symbol));
}

void InstrumentManager::onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate)
{
    //resolve the instrument from the instrument map
    // lock this map for reads
    //this is serious overhead. you can not have conversion multiple times

    //_lockForInstrumentMap->lockForRead();
    InstrumentId instrumentId = getInstrumentIdForATSymbol(pLastUpdate->symbol);//_atSymbolToInstrumentId.value(pLastUpdate->symbol
    _instruments[instrumentId]->onLastPriceUpdate(pLastUpdate);
    //_lockForInstrumentMap->unlock();
}

void InstrumentManager::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
   // _lockForInstrumentMap->lockForRead();
    InstrumentId instrumentId = getInstrumentIdForATSymbol(pQuoteUpdate->symbol);
    _instruments[instrumentId]->onQuoteUpdate(pQuoteUpdate);
    //_lockForInstrumentMap->unlock();

}

const InstrumentId InstrumentManager::getInstrumentIdForATSymbol(const ATSYMBOL& symbol)
{
    uint hashKey = hash(symbol.symbol, _countof(symbol.symbol));

    _lockForInstrumentMap->lockForRead();
    InstrumentId instrumentId = _atSymbolToInstrumentId.value(hashKey, 0);
    _lockForInstrumentMap->unlock();

    return instrumentId;
}

void InstrumentManager::setContractDetails(const InstrumentId instrumentId, const ContractDetails& contractDetails)
{
    _lockForInstrumentMap->lockForWrite();
    _instruments[instrumentId]->setContractDetails(contractDetails);
    _lockForInstrumentMap->unlock();
}

//void InstrumentManager::requestMarketData(const TickerId tickerId, DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
//{
//    testConnectivity(source);
//    //try to typecast the subscriber to strategy object
//    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
//    InstrumentView* instrumentView = ioInterface()->getInstrumentView();
//    //Instrument* instrument = NULL;
//    //lockForInstrumentMap->lockForRead();
//    const ATContract aTcontract = getContractForTicker(tickerId);
//    //lockForInstrumentMap->unlock();

//    Instrument* instrument = getInstrumentForTicker(tickerId);

//    linkSubscriberToInstrument(instrument, subscriber, requestType);

//    //only if subscriber is a strategy, update the instrument screen and not otherwise
//    //use case: Indicator object will generally subscribe to multiple instruments
//    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
//    //this behavior can be changed if required
//    if(strategy)
//    {
//        linkInstrumentToView(instrument, instrumentView, tickerId, aTcontract.contract);
//    }
//}

void InstrumentManager::requestMarketData(const InstrumentId instrumentId, DataSubscriber *subscriber, const DataSource source, const DataRequestType requestType)
{
    bool isConnected = testConnectivity(source);
    //try to typecast the subscriber to strategy object
    Strategy* const strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = IOInterface::ioInterface().getInstrumentView();

    bool newRequest = false;
    Instrument* instrument = getInstrument(instrumentId);

    if(!instrument)
    {
       //this is bad
        return ;
    }

    InstrumentContract instrumentContract = instrument->getInstrumentContract();

    linkSubscriberToInstrument(instrument, subscriber, requestType);

    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    if(strategy)
    {
        //TickerId ticekrId = getTickerId(instrumentId);
        linkInstrumentToView(instrument, instrumentView, instrumentId, instrumentContract);
    }
}

void InstrumentManager::requestMarketData(const String symbol, DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    reportEvent("Requesting based on symbol is not good!!! Requests should be based on Contract or instrumentId!!");

    bool isConnected = testConnectivity(source);
    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = IOInterface::ioInterface().getInstrumentView();
    bool newRequest = false;
    Instrument* instrument = getInstrument(symbol);

    InstrumentContract instrumentContract = instrument->getInstrumentContract();
//    aTcontract.contract.symbol = symbol.toStdString();
//    aTcontract.contract.secType = "STK";
//    aTcontract.contract.exchange = "SMART";
//    aTcontract.contract.primaryExchange = "ISLAND";
//    aTcontract.contract.currency = "USD";

    if(!instrument)
    {
        //this is a problem
    }

    InstrumentId instrumentId = instrumentContract.instrumentId;

    linkSubscriberToInstrument(instrument, subscriber, requestType);

    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    if(strategy)
    {
        linkInstrumentToView(instrument, instrumentView, instrumentId, instrumentContract);
    }

    //if newRequest is true, request ActiveTick else request Intearctive Broker
    if(newRequest)
    {
        if(isConnected)
        {
            reqMktData(instrumentContract, source);
        }
    }
}

void InstrumentManager::requestMarketData(const InstrumentContract& instrumentContract, DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    bool isConnected = testConnectivity(source);
    Instrument* instrument = getInstrument(instrumentContract.instrumentId);
    bool newRequest = false;
    String symbol;
    if(!instrument)
    {
        newRequest = true;
        instrument = addInstrument(instrumentContract);
    }

    //TickerId tickerId = instrument->getTickerId();

    linkSubscriberToInstrument(instrument, subscriber, requestType);

    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);
    InstrumentView* instrumentView = IOInterface::ioInterface().getInstrumentView();
    if(strategy)
    {
        linkInstrumentToView(instrument, instrumentView, instrumentContract.instrumentId, instrumentContract);
    }

    //if newRequest is true, request ActiveTick else request Intearctive Broker
    if(newRequest)
    {
        if(isConnected)
        {
            reqMktData(instrumentContract, source);
        }
        else if(Service::service().getMode() == Test)
        {
            Service::service().getTestDataGenerator()->reqMarketData(instrumentContract.instrumentId);
        }
    }
}

void InstrumentManager::reqMktData(const InstrumentContract& instrumentContract, const DataSource source)
{
    switch(source)
    {
        case ActiveTick: Service::service().getActiveTickSession()->requestTradeStream(instrumentContract.symbol); break;

        case IB:
        {
            TickerId tickerId = getTickerId(instrumentContract.instrumentId);
            Contract contract = getIBContract(instrumentContract.instrumentId);
            Service::service().getTrader()->getTraderAssistant()->requestMarketData(tickerId, contract); break;
        }

        case Test: Service::service().getTestDataGenerator()->reqMarketData(instrumentContract.instrumentId); break;
    }
}

void InstrumentManager::mktDataCanceled(const InstrumentId instrumentId)
{}

void InstrumentManager::removeInstrument(const InstrumentId instrumentId)
{
    _lockForInstrumentMap->lockForWrite();
    delete _instruments[instrumentId];

    _instruments.remove(instrumentId);
    //_instruments.erase(tickerId);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::cancelMarketData(const InstrumentContract& instrumentContract)
{
    Contract contract = getIBContract(instrumentContract.instrumentId);
    Service::service().getActiveTickSession()->cancelMarketData(contract);

    TickerId tickerId = getTickerId(instrumentContract.instrumentId);
    Service::service().getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

void InstrumentManager::cancelMarketData(const InstrumentId instrumentId)
{
    Contract contract = getIBContract(instrumentId);
    Service::service().getActiveTickSession()->cancelMarketData(contract);

    TickerId tickerId = getTickerId(instrumentId);
    Service::service().getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

void InstrumentManager::unSubscribeMarketData(const InstrumentId instrumentId, const DataSubscriber* subscriber)
{
    _lockForInstrumentMap->lockForRead();
    if(_instruments.count(instrumentId))
    {
        _instruments[instrumentId]->disconnect(subscriber);//removes the subsriber
    }
    _lockForInstrumentMap->unlock();
}

//const TickerId InstrumentManager::getTickerId(const ATContract& aTcontract)
//{
//  TickerId tickerId;

//  String symbol = getSymbol(aTcontract);
//  _lockForInstrumentMap->lockForRead();

//  if(_stringSymbolToTickerId.count(symbol)!=0)
//  {
//      tickerId = _stringSymbolToTickerId[symbol];
//      _lockForInstrumentMap->unlock();
//  }
//  else
//  {
//      _lockForInstrumentMap->unlock();

//      _lockForInstrumentMap->lockForWrite();
//      tickerId =++_tickerId;

//      _stringSymbolToTickerId[symbol] = tickerId;
//      _lockForInstrumentMap->unlock();
//  }

//  return tickerId;
//}


const TickerId InstrumentManager::getTickerId(const InstrumentId instrumentId)
{
  TickerId tickerId;

  _lockForInstrumentMap->lockForRead();
  tickerId = _instrumentIdToTickerId.value(instrumentId, 0);
  _lockForInstrumentMap->unlock();

  _lockForInstrumentMap->lockForWrite();
  if(!tickerId)
  {
      tickerId = ++_tickerId;
      _tickerIdToInstrumentId[tickerId] = instrumentId;
      _instrumentIdToTickerId[instrumentId] = tickerId;
  }

  _lockForInstrumentMap->unlock();

  return tickerId;
}

const Contract InstrumentManager::getIBContract(const InstrumentId instrumentId)
{
    Contract contract;

    return contract;
}


const String InstrumentManager::getSymbol(const InstrumentContract& instrumentContract)
{
    return instrumentContract.symbol;
}

//const String InstrumentManager::getInstrumentId(const TickerId tickerId)
//{
//    String symbol;
//    _lockForInstrumentMap->lockForRead();

//    Instrument* inst = _instruments[tickerId];
//    if(inst)
//    {
//        symbol = QString::fromStdString(inst->getContract().contract.symbol);
//    }
//    //    if(_tickerIdToContract.count(tickerId)!=0)
////    {
////        symbol = _tickerIdToContract[tickerId];
////    }
//    _lockForInstrumentMap->unlock();
//    return symbol;
//}

const QString InstrumentManager::getSymbol(const InstrumentId instrumentId)
{
    QString symbol;
    _lockForInstrumentMap->lockForRead();

    Instrument* inst = _instruments.value(instrumentId, NULL);
    if(inst)
    {
        symbol = inst->getInstrumentContract().symbol;
    }
    //    if(_tickerIdToContract.count(tickerId)!=0)
//    {
//        symbol = _tickerIdToContract[tickerId];
//    }
    _lockForInstrumentMap->unlock();
    return symbol;
}


void InstrumentManager::printThreadId()
{
    //printf("\nInstrument Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
}

void InstrumentManager::tickPrice( TickerId tickerId,  TickType field, double price, int canAutoExecute)
{
    _lockForInstrumentMap->lockForRead();
     InstrumentId instrumentId = _tickerIdToInstrumentId[tickerId];
     if(instrumentId)
     {
        _instruments[instrumentId]->tickPrice(field, price, canAutoExecute);
     }
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::tickSize( TickerId tickerId, TickType field, int size)
{
    _lockForInstrumentMap->lockForRead();
    InstrumentId instrumentId = _tickerIdToInstrumentId[tickerId];
    if(instrumentId)
    {
        _instruments[instrumentId]->tickSize(field, size);
    }
   _lockForInstrumentMap->unlock();
}

void InstrumentManager::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
    _lockForInstrumentMap->lockForRead();
     InstrumentId instrumentId = _tickerIdToInstrumentId[tickerId];
     if(instrumentId)
     {
        _instruments[instrumentId]->tickGeneric(tickType, value);
     }
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::reportEvent(const String& message, const MessageType mType)
{
    IOInterface::ioInterface().reportEvent("InstrumentManager", message, mType);
}

void InstrumentManager::generateSnapshot(const int timeInMinutes)
{
    //check for conectivity
    InstrumentMap::iterator it;

    _lockForInstrumentMap->lockForRead();
    InstrumentMap::iterator end = _instruments.end();
    for(it=_instruments.begin();it!=end;++it)
    {
        Instrument* inst = it.value();

        inst->calculateSnapshot(timeInMinutes);

//        if(timeInMinutes%2==0)
//        {
//            (it->second)->calculateTwoMinuteSnapshot();
//            //_twoMinuteSnapshot[it->first] = (it->second)->getLastPrice();
//        }

//        if(timeInMinutes%5 == 0)
//        {
//            (it->second)->calculateFiveMinuteSnapshot();
//            //_fiveMinuteSnapshot[it->first] = (it->second)->getLastPrice();
//        }

//        if(timeInMinutes%10 == 0)
//        {
//            (it->second)->calculateTenMinuteSnapshot();
//            //_tenMinuteSnapshot[it->first] = (it->second)->getLastPrice();
//        }
    }
    _lockForInstrumentMap->unlock();
}

bool InstrumentManager::isConnected(const DataSource source)
{
    switch(source)
    {
        case IB: return Service::service().getTrader()->getTraderAssistant()->IsConnected(); break;
        case ActiveTick: return Service::service().getActiveTickSession()->IsConnected(); break;
        case Test: return true; break;
    }
}

void InstrumentManager::linkSubscriberToInstrument(const Instrument* instrument, const DataSubscriber* subscriber, const DataRequestType requestType)
{
    if(subscriber)
    {
        if(requestType==RealTime) //if realtime or snaphot = 0
        {
            QObject::connect(instrument, SIGNAL(tickPriceUpdated(const InstrumentId, const TickType, const double, int)), subscriber, SLOT(onTickPriceUpdate(const InstrumentId, const TickType, const double)), Qt::UniqueConnection);
            QObject::connect(instrument, SIGNAL(tickGenericUpdated(const InstrumentId, const TickType, const double)), subscriber, SLOT(onTickPriceUpdate(const InstrumentId, const TickType, const double)), Qt::UniqueConnection);
        }
        else
        {
            QObject::connect(instrument,SIGNAL(snapshotUpdated(const InstrumentId, const double, const int)), subscriber, SLOT(onSnapshotUpdate(const InstrumentId, const double, const int)), Qt::UniqueConnection);
        }
     }
}

void InstrumentManager::linkInstrumentToView(const Instrument* instrument, const InstrumentView* instrumentView, const InstrumentId  instrumentId, const InstrumentContract& instrumentContract)
{
    QObject::connect(instrument,SIGNAL(tickGenericUpdated(const InstrumentId, const TickType, const double)), instrumentView, SLOT(updateTickGeneric(const InstrumentId, const TickType, const double)), Qt::UniqueConnection);
    QObject::connect(instrument,SIGNAL(tickPriceUpdated(const InstrumentId, const TickType, const double,int)), instrumentView, SLOT(updateTickPrice(const InstrumentId, const TickType, const double, const int)), Qt::UniqueConnection);
    QObject::connect(instrument,SIGNAL(tickSizeUpdated(const InstrumentId, const TickType,const int)), instrumentView, SLOT(updateTickSize(const InstrumentId, const TickType,const int)), Qt::UniqueConnection);
    IOInterface::ioInterface().addInstrument(instrumentId, instrumentContract, GUI);
}

const double InstrumentManager::getLastPrice(const InstrumentId instrumentId)
{
    double price = 0;
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument  = _instruments.value(instrumentId, NULL))
    {
        price = instrument->getLastPrice();
    }
    _lockForInstrumentMap->unlock();

    return price;
}

const double InstrumentManager::getAskPrice(const InstrumentId instrumentId)
{
    double askPrice = 0;
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(instrumentId, NULL))
    {
        askPrice = instrument->getAskPrice();
    }
    _lockForInstrumentMap->unlock();

    return askPrice;
}

const double InstrumentManager::getBidPrice(const InstrumentId instrumentId)
{
    double bidPrice = 0;
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(instrumentId, NULL))
    {
        bidPrice = instrument->getBidPrice();
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

Instrument* InstrumentManager::getInstrument(const InstrumentContract& instrumentContract)
{
    //Instrument* instrument = NULL;
    _lockForInstrumentMap->lockForRead();
     Instrument* instrument = _instruments.value(instrumentContract.instrumentId, NULL);
    _lockForInstrumentMap->unlock();

    return instrument;
}

Instrument* InstrumentManager::getInstrument(const String& symbol)
{
    _lockForInstrumentMap->lockForRead();
     Instrument* instrument = _instruments.value(_stringSymbolToInstrumentId.value(symbol, 0), NULL);
    _lockForInstrumentMap->unlock();
    return instrument;
}

Instrument* InstrumentManager::getInstrument(const InstrumentId instrumentId)
{
    _lockForInstrumentMap->lockForRead();
     Instrument* instrument = _instruments.value(instrumentId, NULL);
    _lockForInstrumentMap->unlock();

    return instrument;
}

//Instrument* InstrumentManager::addInstrument(const String& symbol)
//{
//    ATContract aTcontract;
//    aTcontract.contract.symbol = symbol.toStdString();
//    aTcontract.contract.secType = "STK";
//    aTcontract.contract.exchange = "SMART";
//    aTcontract.contract.primaryExchange = "ISLAND";
//    aTcontract.contract.currency = "USD";

//    _lockForInstrumentMap->lockForWrite();
//    TickerId tickerId = (_stringSymbolToTickerId.count(symbol)!=0) ? _stringSymbolToTickerId[symbol] : ++_tickerId;
//    Instrument* nInstrument = new Instrument(tickerId, aTcontract, 1);
//    //lock for writes
//    _stringSymbolToTickerId[symbol] = tickerId;
//    //_tickerIdToContract[tickerId] = aTcontract;
//    _instruments[tickerId] = nInstrument;
//    _lockForInstrumentMap->unlock();

//    return nInstrument;
//}

Instrument* InstrumentManager::addInstrument(const InstrumentContract& instrumentContract)
{
    String symbol = instrumentContract.symbol;
    Instrument* nInstrument = NULL;
    InstrumentId instrumentId = instrumentContract.instrumentId;
    _lockForInstrumentMap->lockForWrite();
    //InstrumentId instrumentId = _stringSymbolToInstrumentId.value(symbol,0);

    if(instrumentId)
    {
       TickerId tickerId = ++_tickerId;
       _stringSymbolToInstrumentId[symbol] = instrumentContract.instrumentId;
       _instrumentIdToTickerId[instrumentContract.instrumentId] = tickerId;
       _tickerIdToInstrumentId[tickerId] = instrumentContract.instrumentId;
        nInstrument = new Instrument(instrumentContract);
       _instruments[instrumentContract.instrumentId] = nInstrument;
    }
    _lockForInstrumentMap->unlock();

    return nInstrument;
}

void InstrumentManager::registerInstrument(const InstrumentContract& instrumentContract)
{
//     String symbol = instrumentContract.symbol;
//     Instrument* nInstrument = NULL;
//     InstrumentId instrumentId = instrumentContract.instrumentId;

//    _lockForInstrumentMap->lockForWrite();
//    //InstrumentId instrumentId = _stringSymbolToInstrumentId.value(symbol,0);
//    if(instrumentId)
//    {
//       if(!_instruments.value(instrumentContract.instrumentId, NULL))
//       {
//            TickerId tickerId = ++_tickerId;
//           _stringSymbolToInstrumentId[symbol] = instrumentContract.instrumentId;
//           _instrumentIdToTickerId[instrumentContract.instrumentId] = tickerId;
//           _tickerIdToInstrumentId[tickerId] = instrumentContract.instrumentId;
//            nInstrument = new Instrument(instrumentContract);
//           _instruments[instrumentContract.instrumentId] = nInstrument;
//        }
//    }
//    _lockForInstrumentMap->unlock();

    requestMarketData(instrumentContract, NULL);
}

const InstrumentContract InstrumentManager::getInstrumentContract(const InstrumentId instrumentId)
{
    _lockForInstrumentMap->lockForRead();
    InstrumentContract ic = _instruments[instrumentId]->getInstrumentContract();
    _lockForInstrumentMap->unlock();

    return ic;
}
