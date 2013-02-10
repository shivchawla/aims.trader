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
#include "Platform/Commission/Commission.h"
#include "Platform/Commission/CommissionFactory.h"
#include "Platform/View/IODatabase.h"

/*
 * Constructor InstrumentManager
 */
InstrumentManager::InstrumentManager():_tickerId(0),_lockForInstrumentMap(new QReadWriteLock(QReadWriteLock::Recursive))
{
    _spreadId = 0;
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

bool operator==(const ATSYMBOL& s1, const ATSYMBOL& s2)
{
    if(_countof(s1.symbol) != _countof(s2.symbol))
        return false;

    int n = _countof(s1.symbol);

    const wchar16_t* first = s1.symbol;
    const wchar16_t* second = s2.symbol;

    while(n--)
    {
        if(*first++ != *second++)
            return false;
    }

    return true;
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
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(_atSymbolToTickerId.value(pLastUpdate->symbol, 0), NULL))
    {
        instrument->onLastPriceUpdate(pLastUpdate);
    }
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
     //TickerId tickerId = getTickerIdForATSymbol(pQuoteUpdate->symbol);
    _lockForInstrumentMap->lockForRead();
     if(Instrument* instrument = _instruments.value(_atSymbolToTickerId.value(pQuoteUpdate->symbol, 0), NULL))
     {
         instrument->onQuoteUpdate(pQuoteUpdate);
     }
    _lockForInstrumentMap->unlock();
}

const TickerId InstrumentManager::getTickerIdForATSymbol(const ATSYMBOL& symbol)
{
    _lockForInstrumentMap->lockForRead();
    TickerId tickerId = _atSymbolToTickerId.value(symbol, 0);
    _lockForInstrumentMap->unlock();

    return tickerId;
}

void InstrumentManager::setContractDetails(const TickerId tickerId, const ContractDetails& contractDetails)
{
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
    {
        instrument->setContractDetails(contractDetails);
    }
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::requestMarketData(const TickerId tickerId, const DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{    
    linkSubscriberToInstrument(tickerId, subscriber, requestType);
    bool newRequest = false;
    QHash<DataSource, bool> dSources = _isSubscribed.value(tickerId, QHash<DataSource, bool>());
    if(dSources ==  QHash<DataSource, bool>())
    {
        dSources = (_isSubscribed[tickerId] = QHash<DataSource, bool>());
    }

    if(!dSources.value(source,false))
    {
        dSources[source] = true;
        newRequest = true;
    }

    if(newRequest)
    {
        bool isConnected = IsConnected(source);
        if(isConnected)
        {
            reqMktData(tickerId, source);
        }
    }
}

void InstrumentManager::requestMarketData(const InstrumentId instrumentId, const DataSubscriber *subscriber, const DataSource source, const DataRequestType requestType)
{
    TickerId tickerId;
    if(!(tickerId = getTickerId(instrumentId)))
    {
        return;
    }

    requestMarketData(tickerId, subscriber, source, requestType);
}

void InstrumentManager::requestMarketData(const String symbol, const DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    TickerId tickerId = getTickerId(symbol);
    if(!(tickerId = getTickerId(symbol)))
    {
        return;
    }

    requestMarketData(tickerId, subscriber, source, requestType);
}

void InstrumentManager::requestMarketData(const InstrumentContract& instrumentContract, const DataSubscriber* subscriber, const DataSource source,  const DataRequestType requestType)
{
    TickerId tickerId;
    if(!(tickerId = getTickerId(instrumentContract.instrumentId)))
    {
        tickerId = addInstrument(instrumentContract);
    }

    requestMarketData(tickerId, subscriber, source, requestType);
}

void InstrumentManager::reqMktData(const TickerId tickerId, const DataSource source)
{
    switch(source)
    {
        case ActiveTick: Service::service().getActiveTickSession()->requestTradeStream(getSymbol(tickerId)); break;

        case IB:
        {
             Service::service().getTrader()->getTraderAssistant()->requestMarketData(tickerId, getIBContract(tickerId)); break;
        }

        case Test: Service::service().getTestDataGenerator()->reqMarketData(tickerId); break;
    }
}


void InstrumentManager::mktDataCanceled(const InstrumentId instrumentId)
{}

void InstrumentManager::mktDataCanceled(const TickerId tickerId)
{}

void InstrumentManager::removeInstrument(const TickerId tickerId)
{
    _lockForInstrumentMap->lockForWrite();
    delete _instruments[tickerId];

    _instruments.remove(tickerId);
    //_instruments.erase(tickerId);
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::cancelMarketData(const InstrumentContract* instrumentContract)
{
    Contract contract = getIBContract(instrumentContract->instrumentId);
    Service::service().getActiveTickSession()->cancelMarketData(contract);

    TickerId tickerId = getTickerId(instrumentContract->instrumentId);
    Service::service().getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

void InstrumentManager::cancelMarketData(const TickerId tickerId)
{
    Contract contract = getIBContract(tickerId);
    Service::service().getActiveTickSession()->cancelMarketData(contract);

    //TickerId tickerId = getTickerId(tickerId);
    Service::service().getTrader()->getTraderAssistant()->cancelMarketData(tickerId);
}

void InstrumentManager::unSubscribeMarketData(const TickerId tickerId, const DataSubscriber* subscriber)
{
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
    {
        bool x = instrument->disconnect(subscriber);//removes the subsriber
    }
    _lockForInstrumentMap->unlock();
}

const TickerId InstrumentManager::getTickerId(const InstrumentId instrumentId)
{
  TickerId tickerId;

  _lockForInstrumentMap->lockForRead();
  tickerId = _instrumentIdToTickerId.value(instrumentId, 0);
  _lockForInstrumentMap->unlock();

  return tickerId;
}

const TickerId InstrumentManager::getTickerId(const String& symbol)
{
  _lockForInstrumentMap->lockForRead();
  TickerId tickerId = _stringSymbolToTickerId.value(symbol, 0);
  _lockForInstrumentMap->unlock();

  if(!tickerId)
  {
      tickerId = addInstrument(IODatabase::ioDatabase().getInstrumentsWithSimilarSymbol(symbol)[0]);
  }

  return tickerId;
}

const InstrumentId InstrumentManager::getInstrumentId(const TickerId tickerId)
{
    _lockForInstrumentMap->lockForRead();
    InstrumentId instrumentId = _tickerIdToInstrumentId.value(tickerId, 0);
    _lockForInstrumentMap->unlock();

    return instrumentId;
}

const Contract InstrumentManager::getIBContract(const InstrumentId instrumentId)
{
    _lockForInstrumentMap->lockForRead();
    TickerId tickerId = _instrumentIdToTickerId.value(instrumentId,0);
    _lockForInstrumentMap->unlock();
    return getIBContract(tickerId);
}

const Contract InstrumentManager::getIBContract(const TickerId tickerId)
{
    Contract contract;
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
    {
        contract = instrument->getContract();
    }
    _lockForInstrumentMap->unlock();

    return contract;

}

const String InstrumentManager::getSymbol(const InstrumentContract& instrumentContract)
{
    return instrumentContract.symbol;
}

const QString InstrumentManager::getSymbol(const InstrumentId instrumentId)
{
    QString symbol;
    _lockForInstrumentMap->lockForRead();


    if(Instrument* instrument = _instruments.value(_instrumentIdToTickerId.value(instrumentId, 0), NULL))
    {
        symbol = instrument->toString();
    }
    _lockForInstrumentMap->unlock();
    return symbol;
}

const QString InstrumentManager::getSymbol(const TickerId tickerId)
{
    String symbol;

    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
    {
        symbol = instrument->toString();
    }
    _lockForInstrumentMap->unlock();

    return symbol;
}

const ATSYMBOL InstrumentManager::getATSymbol(const TickerId tickerId)
{
    ATSYMBOL symbol;

    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
    {
        symbol = Helper::StringToSymbol(instrument->toString().toStdString());
    }
    _lockForInstrumentMap->unlock();

    return symbol;
}




void InstrumentManager::printThreadId()
{
    //printf("\nInstrument Manager Thread \t");
    //std::cout<<QThread::currentThreadId();
}

void InstrumentManager::tickPrice(const TickerId tickerId, const TickType field, const double price, const int canAutoExecute)
{
    _lockForInstrumentMap->lockForRead();
     //InstrumentId instrumentId = _tickerIdToInstrumentId[tickerId];
     if(Instrument* instrument = _instruments.value(tickerId, NULL))
     {
        instrument->tickPrice(field, price, canAutoExecute);
     }
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::tickSize(const TickerId tickerId, const TickType field, const int size)
{
    _lockForInstrumentMap->lockForRead();
    //InstrumentId instrumentId = _tickerIdToInstrumentId[tickerId];
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
    {
        instrument->tickSize(field, size);
    }
   _lockForInstrumentMap->unlock();
}

void InstrumentManager::tickGeneric(const TickerId tickerId, const TickType tickType, const double value)
{
    _lockForInstrumentMap->lockForRead();
     //InstrumentId instrumentId = _tickerIdToInstrumentId[tickerId];
     if(Instrument* instrument = _instruments.value(tickerId, NULL))
     {
        instrument->tickGeneric(tickType, value);
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
//    InstrumentMap::iterator end = _instruments.end();
//    for(it=_instruments.begin();it!=end;++it)
//    {
//        Instrument* inst = it.value();

    foreach(Instrument* inst, _instruments)
    {
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

bool InstrumentManager::IsConnected(const DataSource source)
{
    switch(source)
    {
        case IB: return Service::service().getTrader()->getTraderAssistant()->IsConnected(); break;
        case ActiveTick: return Service::service().getActiveTickSession()->IsConnected(); break;
        case Test: return true; break;
    }
}

void InstrumentManager::linkSubscriberToInstrument(const TickerId tickerId, const DataSubscriber* subscriber, const DataRequestType requestType)
{
    _lockForInstrumentMap->lockForRead();
    Instrument* instrument = _instruments.value(tickerId, NULL);
    if(subscriber && instrument)
    {
        if(requestType == RealTime) //if realtime or snaphot = 0
        {
            QObject::connect(instrument, SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double, int)), subscriber, SLOT(onTickPriceUpdate(const TickerId, const TickType, const double)), Qt::UniqueConnection);
            QObject::connect(instrument, SIGNAL(tickGenericUpdated(const TickerId, const TickType, const double)), subscriber, SLOT(onTickPriceUpdate(const TickerId, const TickType, const double)), Qt::UniqueConnection);
        }
        else
        {
            QObject::connect(instrument,SIGNAL(snapshotUpdated(const TickerId, const double, const int)), subscriber, SLOT(onSnapshotUpdate(const TickerId, const double, const int)), Qt::UniqueConnection);
        }
     }
    _lockForInstrumentMap->unlock();
}

void InstrumentManager::linkInstrumentToView(const Instrument* instrument, const InstrumentView* instrumentView, const TickerId  tickerId)
{
//    if(instrument)
//    {
//        QObject::connect(instrument,SIGNAL(tickGenericUpdated(const TickerId, const TickType, const double)), instrumentView, SLOT(updateTickGeneric(const TickerId, const TickType, const double)), Qt::UniqueConnection);
//        QObject::connect(instrument,SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double,int)), instrumentView, SLOT(updateTickPrice(const TickerId, const TickType, const double, const int)), Qt::UniqueConnection);
//        QObject::connect(instrument,SIGNAL(tickSizeUpdated(const TickerId, const TickType,const int)), instrumentView, SLOT(updateTickSize(const TickerId, const TickType,const int)), Qt::UniqueConnection);
//        IOInterface::ioInterface().addInstrument(tickerId);
//    }
}

const double InstrumentManager::getLastPrice(const TickerId tickerId)
{
    double price = 0;
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument  = _instruments.value(tickerId, NULL))
    {
        price = instrument->getLastPrice();
    }
    _lockForInstrumentMap->unlock();

    return price;
}

const double InstrumentManager::getAskPrice(const TickerId tickerId)
{
    double askPrice = 0;
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
    {
        askPrice = instrument->getAskPrice();

    }
    _lockForInstrumentMap->unlock();

    return askPrice;
}

const double InstrumentManager::getBidPrice(const TickerId tickerId)
{
    double bidPrice = 0;
    _lockForInstrumentMap->lockForRead();
    if(Instrument* instrument = _instruments.value(tickerId, NULL))
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
    if(!(connected = IsConnected(source)))
    {
        switch(source)
        {
            case IB: reportEvent("Interactive Broker not Connected"); break;
            case ActiveTick: reportEvent("Active Tick not Connected"); break;
        }
    }
    return connected;
}

//Instrument* InstrumentManager::getInstrument(const InstrumentContract* instrumentContract)
//{
//    //Instrument* instrument = NULL;
//    _lockForInstrumentMap->lockForRead();
//    TickerId tickerId = _instrumentIdToTickerId.value(instrumentContract->instrumentId, -1);
//    Instrument* instrument = _instruments.value(tickerId, NULL);
//    _lockForInstrumentMap->unlock();

//    return instrument;
//}

//Instrument* InstrumentManager::getInstrument(const String& symbol)
//{
//    _lockForInstrumentMap->lockForRead();
//     Instrument* instrument = _instruments.value(_stringSymbolToTickerId.value(symbol, 0), NULL);
//    _lockForInstrumentMap->unlock();
//    return instrument;
//}

//Instrument* InstrumentManager::getInstrument(const InstrumentId instrumentId)
//{
//    _lockForInstrumentMap->lockForRead();
//    TickerId tickerId = _instrumentIdToTickerId.value(instrumentId, -1);
//    Instrument* instrument = _instruments.value(tickerId, NULL);
//    _lockForInstrumentMap->unlock();

//    return instrument;
//}

//Instrument* InstrumentManager::getInstrument(const TickerId tickerId)
//{
//    _lockForInstrumentMap->lockForRead();
//     Instrument* instrument = _instruments.value(tickerId, NULL);
//    _lockForInstrumentMap->unlock();

//    return instrument;
//}

const TickerId InstrumentManager::addInstrument(const InstrumentContract& instrumentContract)
{
    String symbol = instrumentContract.symbol;
    Instrument* nInstrument = NULL;
    TickerId tickerId;
    InstrumentId instrumentId = instrumentContract.instrumentId;
    _lockForInstrumentMap->lockForWrite();
    tickerId = _stringSymbolToTickerId.value(symbol, 0);
    if(instrumentId &&  tickerId==0)
    {
        tickerId = ++_tickerId;
       _stringSymbolToTickerId[symbol] = tickerId;
       _atSymbolToTickerId[Helper::StringToSymbol(symbol.toStdString())] = tickerId;
       _instrumentIdToTickerId[instrumentId] = tickerId;
       _tickerIdToInstrumentId[tickerId] = instrumentId;
        nInstrument = new Instrument(tickerId, instrumentContract);
       _instruments[tickerId] = nInstrument;
    }
    _lockForInstrumentMap->unlock();

    return tickerId;
}

void InstrumentManager::registerInstrument(const InstrumentContract& instrumentContract, const DataSource source)
{
    if(instrumentContract.symbol=="FSLR")
    {
        int x = 1;
    }

    requestMarketData(instrumentContract, NULL, source);
}

void InstrumentManager::registerSpread(const SpreadData& spreadData)
{
    QPair<InstrumentId, InstrumentId> pair(spreadData.instrumentId1, spreadData.instrumentId2);
    TickerId tickerId1 = getTickerId(spreadData.instrumentId1);
    TickerId tickerId2 = getTickerId(spreadData.instrumentId2);

    QPair<TickerId, TickerId> p(tickerId1, tickerId2);
    _lockForInstrumentMap->lockForWrite();
    _instrumentIdsToSpreadId[pair] = spreadData.spreadId;
    _spreadIdToDbSpreadId[++_spreadId] = spreadData.spreadId;
    _tickerIdsToSpreadId[p] = _spreadId;
    _spreadIdToTickerIds[_spreadId] = p;
    _lockForInstrumentMap->unlock();
}

DbSpreadId InstrumentManager::getDbSpreadId(const TickerId tickerId1, const TickerId tickerId2)
{
    //_lockForInstrumentMap->lockForRead();
    //DBSprea
}

DbSpreadId InstrumentManager::getDbSpreadId(const SpreadId spreadId)
{
    _lockForInstrumentMap->lockForRead();
    DbSpreadId dbSpreadId = _spreadIdToDbSpreadId.value(spreadId, -1);
    _lockForInstrumentMap->unlock();

    return dbSpreadId;
}

const double InstrumentManager::getCommission(const TickerId, const int shares, const double price)
{
    return CommissionFactory::getNorthAmericaStockCommission().getCommission(shares, price, PriceBased);
}

const Prices InstrumentManager::getPrices(const TickerId tickerId)
{
     Prices prices;
    _lockForInstrumentMap->lockForRead();
     if(Instrument* instrument = _instruments.value(tickerId, NULL))
     {
        prices.ask = instrument->getAskPrice();
        prices.bid = instrument->getBidPrice();
        prices.last = instrument->getLastPrice();
     }
    _lockForInstrumentMap->unlock();

    return prices;
}

SpreadId InstrumentManager::getSpreadId(const TickerId tickerId1, const TickerId tickerId2)
{
    QPair<TickerId, TickerId> p(tickerId1, tickerId2);
    _lockForInstrumentMap->lockForRead();
    SpreadId spreadId = _tickerIdsToSpreadId.value(p, 0);
    _lockForInstrumentMap->unlock();
    return spreadId;
}

QPair<TickerId, TickerId> InstrumentManager::getTickerIds(const SpreadId spreadId)
{
    _lockForInstrumentMap->lockForRead();
     QPair<TickerId, TickerId> p = _spreadIdToTickerIds.value(spreadId, QPair<TickerId, TickerId>(0,0));
    _lockForInstrumentMap->unlock();
    return p;
}

