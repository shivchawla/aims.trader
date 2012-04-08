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

InstrumentManager::InstrumentManager():QObject(),_tickerId(0),lockForInstrumentMap(new QReadWriteLock())
{
    std::cout<<"Creating Instrument Manager\n";
   // QThread* thread = ThreadManager::Instance()->requestThread();
    //connect(thread,SIGNAL(started()),this, SLOT(printThreadId()));
    //moveToThread(thread);
    qRegisterMetaType<TradeUpdate>("TradeUpdate");
    qRegisterMetaType<QuoteUpdate>("QuoteUpdate");
    //_instrumentView = new InstrumentView;
    //_instrumentView->show();
    InstrumentView* instrumentView = MainWindow::mainWindow()->getInstrumentView();
    QObject::connect(this, SIGNAL(instrumentAdded(const TickerId, const Contract&)), instrumentView, SLOT(addInstrument(const TickerId, const Contract&)));
}

InstrumentManager::~InstrumentManager()
{
    //this->thread()->quit();
}

void InstrumentManager::onTradeUpdate(LPATQUOTESTREAM_TRADE_UPDATE pLastUpdate)
{
    //resolve the instrument from the instrument map
    // lock this map for reads
    std::string symbol = Helper::ConvertString(pLastUpdate->symbol.symbol, _countof(pLastUpdate->symbol.symbol));
    lockForInstrumentMap->lockForRead();
    TickerId tickerId = _symbolToTickerId[symbol];
    _instruments[tickerId]->onLastPriceUpdate(pLastUpdate);
    //emit lastPriceUpdated(tickerId, pLastUpdate);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::onQuoteUpdate(LPATQUOTESTREAM_QUOTE_UPDATE pQuoteUpdate)
{
    std::string symbol = ActiveTickFeed::Helper::ConvertString(pQuoteUpdate->symbol.symbol, _countof(pQuoteUpdate->symbol.symbol));

    lockForInstrumentMap->lockForRead();
    TickerId tickerId = _symbolToTickerId[symbol];
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

void InstrumentManager::requestMarketData(const TickerId tickerId, const DataSource source, DataSubscriber* subscriber)
{
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
        lockForInstrumentMap->unlock();  
    }
    else
    {
        lockForInstrumentMap->unlock();
    }

    if(instrument)
    {
        QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const TradeUpdate&)), subscriber, SLOT(onTradeUpdate(const TickerId, const TradeUpdate&)));
        QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, const QuoteUpdate&)), subscriber, SLOT(onQuoteUpdate(const TickerId, const QuoteUpdate&)));
        QObject::connect(instrument,SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double,int)), subscriber, SLOT(onTickPriceUpdate(const TickerId, const TickType, const double)));

        //only if subscriber is a strategy, update the instrument screen and not otherwise
        //use case: Indicator object will generally subscribe to multiple instruments
        //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
        //this behavior can be changed if required
        if(strategy)
        {
            QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const TradeUpdate&)), instrumentView, SLOT(onTradeUpdate(const TickerId, const TradeUpdate&)));
            QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, const QuoteUpdate& )), instrumentView, SLOT(onQuoteUpdate(const TickerId, const QuoteUpdate&)));
            QObject::connect(instrument,SIGNAL(tickGenericUpdated(const TickerId, const TickType, const double)), instrumentView, SLOT(updateTickGeneric(const TickerId, const TickType, const double)));
            QObject::connect(instrument,SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double,int)), instrumentView, SLOT(updateTickPrice(const TickerId, const TickType, const double)));
            QObject::connect(instrument,SIGNAL(tickSizeUpdated(const TickerId, const TickType,const int)), instrumentView, SLOT(updateTickSize(const TickerId, const TickType,const int)));
        }
    }
}

void InstrumentManager::requestMarketData(const Contract& contract, const DataSource source, DataSubscriber* subscriber)
{
    //lock for threads
    std::cout<<subscriber;
    //MarketDataSubscriber* subscriber =qobject_cast<MarketDataSubscriber*>(QObject::sender());
    InstrumentView* instrumentView = MainWindow::mainWindow()->getInstrumentView();
    //ATSYMBOL atsymbol  = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
     //std::string symbol = ActiveTickFeed::Helper::ConvertString(atsymbol.symbol, _countof(atsymbol.symbol));

    bool newRequest = false;
    std:: string symbol = contract.symbol;
    //try to typecast the subscriber to strategy object
    Strategy* strategy = qobject_cast<Strategy*>(subscriber);

    Instrument* instrument = NULL;
    TickerId tickerId;
    lockForInstrumentMap->lockForRead();
    if(_symbolToTickerId.count(symbol)!=0)
    {
        instrument = _instruments[_symbolToTickerId[symbol]];
        lockForInstrumentMap->unlock();
    }
    else
    {
        newRequest=true;
        //unlock the read lock
        lockForInstrumentMap->unlock();

        tickerId = ++_tickerId;
        instrument = new Instrument(tickerId, contract, 1);
        //lock for writes
        lockForInstrumentMap->lockForWrite();
        _symbolToTickerId[symbol] = tickerId;
        _instruments[tickerId] = instrument;
        lockForInstrumentMap->unlock();
        //same as above
    }


    QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const TradeUpdate&)), subscriber, SLOT(onTradeUpdate(const TickerId, const TradeUpdate&)));
    QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, const QuoteUpdate&)), subscriber, SLOT(onQuoteUpdate(const TickerId, const QuoteUpdate&)));
    QObject::connect(instrument,SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double,int)), subscriber, SLOT(onTickPriceUpdate(const TickerId, const TickType, const double)));

    //only if subscriber is a strategy, update the instrument screen and not otherwise
    //use case: Indicator object will generally subscribe to multiple instruments
    //to find the trade-able instruments. We don't want to jam theinstrument view with all instruments
    //this behavior can be changed if required
    if(strategy)
    {
        QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const TradeUpdate&)), instrumentView, SLOT(onTradeUpdate(const TickerId, const TradeUpdate&)));
        QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, const QuoteUpdate& )), instrumentView, SLOT(onQuoteUpdate(const TickerId, const QuoteUpdate&)));
        QObject::connect(instrument,SIGNAL(tickGenericUpdated(const TickerId, const TickType, const double)), instrumentView, SLOT(updateTickGeneric(const TickerId, const TickType, const double)));
        QObject::connect(instrument,SIGNAL(tickPriceUpdated(const TickerId, const TickType, const double,int)), instrumentView, SLOT(updateTickPrice(const TickerId, const TickType, const double, const int)));
        QObject::connect(instrument,SIGNAL(tickSizeUpdated(const TickerId, const TickType,const int)), instrumentView, SLOT(updateTickSize(const TickerId, const TickType,const int)));
    }

    //if API is true, request ActiveTick else request Intearctive Broker
    if(newRequest)
    {
        switch(source)
        {
            case ActiveTick: Service::Instance()->getActiveTickAPI()->requestTradeStream(contract); break;
            case IB: Service::Instance()->getTrader()->getTraderAssistant()->requestMarketData(tickerId, contract); break;
        }
        emit instrumentAdded(tickerId, contract);
     }
}

void InstrumentManager::mktDataCancelled(const TickerId tickerId)
{}


/*
void InstrumentManager::requestMarketData(const Contract& contract)
{
    std::cout<<QThread::currentThreadId();
    Strategy* strategy =qobject_cast<Strategy*>(QObject::sender());
   // connect(instrument, SIGNAL(updateTickerId(const long, const TickerId)), strategy, SLOT(setTickerId(const long, const TickerId)));
    if(_contractIdToTickerId.count(contract.conId)!=0)
    {
        Instrument* instrument = _instruments[_contractIdToTickerId[contract.conId]];

        QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const double)), strategy, SLOT(updatePosition(const TickerId, const double)));
    }
    else
    {
        TickerId tickerId = ++_tickerId;
        _contractIdToTickerId[contract.conId] = tickerId;
        Instrument* instrument = new Instrument(tickerId, contract, 1);
        _instruments[tickerId] = instrument;
        QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, const double)), strategy, SLOT(updatePosition(const TickerId, const double)));
        TraderAssistant* ta = Service::Instance()->getTrader()->getTraderAssistant();
        //emit requestMarketDataToTA(tickerId, contract);
    }
    emit updateTickerId(contract.conId, _contractIdToTickerId[contract.conId]);
}
*/
void InstrumentManager::removeInstrument(const TickerId tickerId)
{
    lockForInstrumentMap->lockForWrite();
    delete _instruments[tickerId];
    _instruments.erase(tickerId);
    lockForInstrumentMap->unlock();
}

const TickerId InstrumentManager::getTickerId(const Contract& contract)
{ 
  //lock again
  TickerId tickerId=0;

  lockForInstrumentMap->lockForRead();
  //ActiveTickFeed::ATSYMBOL symbol = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
  if(_symbolToTickerId.count(contract.symbol)!=0)
  {     
      tickerId = _symbolToTickerId[contract.symbol];
  }
  lockForInstrumentMap->unlock();

  return tickerId;
}

void InstrumentManager::printThreadId()
{
    printf("\nInstrument Manager Thread \t");
    std::cout<<QThread::currentThreadId();
}

const Contract& InstrumentManager::getContractForTicker(const TickerId tickerId)
{
    lockForInstrumentMap->lockForRead();
    _instruments[tickerId]->getContract();
    lockForInstrumentMap->unlock();
}

void InstrumentManager::tickPrice( TickerId tickerId,  TickType field, double price, int canAutoExecute)
{
    lockForInstrumentMap->lockForRead();
    _instruments[tickerId]->tickPrice(tickerId, field, price, canAutoExecute);
    lockForInstrumentMap->unlock();
}

void InstrumentManager::tickSize( TickerId tickerId, TickType field, int size)
{
    lockForInstrumentMap->lockForRead();
   _instruments[tickerId]->tickSize(tickerId, field, size);
   lockForInstrumentMap->unlock();
}

void InstrumentManager::tickGeneric(TickerId tickerId, TickType tickType, double value)
{
    lockForInstrumentMap->lockForRead();
    _instruments[tickerId]->tickGeneric(tickerId, tickType, value);
    lockForInstrumentMap->unlock();
}



