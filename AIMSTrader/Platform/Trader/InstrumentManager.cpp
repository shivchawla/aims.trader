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

InstrumentManager::InstrumentManager():QObject(),_tickerId(0)
{
    std::cout<<"Creating Instrument Manager\n";
   // QThread* thread = ThreadManager::Instance()->requestThread();
    //connect(thread,SIGNAL(started()),this, SLOT(printThreadId()));
    //moveToThread(thread);
    qRegisterMetaType<TradeUpdate>("TradeUpdate");
    qRegisterMetaType<QuoteUpdate>("QuoteUpdate");
    //_instrumentView = new InstrumentView;
    //_instrumentView->show();
}

InstrumentManager::~InstrumentManager()
{
    //this->thread()->quit();
}

void InstrumentManager::onTradeUpdate(TradeUpdate pLastUpdate)
{
    //resolve the instrument from the instrument map
    // lock this map for reads
    readWritelock.lockForRead();
    //std::string symbol = pLast
    TickerId tickerId = _symbolToTickerId[pLastUpdate.symbol];
    _instruments[tickerId]->onLastPriceUpdate(pLastUpdate);
    emit lastPriceUpdated(tickerId, pLastUpdate);
    readWritelock.unlock();

}

void InstrumentManager::onQuoteUpdate(QuoteUpdate pQuoteUpdate)
{
    readWritelock.lockForRead();
    //std::string symbol = ActiveTickFeed::Helper::ConvertString(pQuoteUpdate->symbol.symbol, _countof(pQuoteUpdate->symbol.symbol));
    TickerId tickerId = _symbolToTickerId[pQuoteUpdate.symbol];
    emit quoteUpdated(tickerId, pQuoteUpdate);
    readWritelock.unlock();
}

void InstrumentManager::setBid(const TickerId tickerId, const double bid)
{
    _instruments[tickerId]->setBid(bid);
}

void InstrumentManager::setAsk(const TickerId tickerId, const double ask)
{
    _instruments[tickerId]->setAsk(ask);
}

void InstrumentManager::setLast(const TickerId tickerId, const double last)
{
    _instruments[tickerId]->setLast(last);
    // printf("Last Price %f \n", last);
}

void InstrumentManager::setHigh(const TickerId tickerId, const double high)
{
    _instruments[tickerId]->setHigh(high);
}

void InstrumentManager::setClose(const TickerId tickerId, const double close)
{
    _instruments[tickerId]->setClose(close);
}

void InstrumentManager::setLow(const TickerId tickerId, const double low)
{
   _instruments[tickerId]->setLow(low);
}

void InstrumentManager::setBidSize(const TickerId tickerId, const int size)
{
    _instruments[tickerId]->setBidSize(size);
}

void InstrumentManager::setAskSize(const TickerId tickerId, const int size)
{
    _instruments[tickerId]->setAskSize(size);
}

void InstrumentManager::setLastSize(const TickerId tickerId, const int size)
{
    _instruments[tickerId]->setLastSize(size);
}

void InstrumentManager::setContractDetails(const TickerId tickerId, const ContractDetails& contractDetails)
{
    _instruments[tickerId]->setContractDetails(contractDetails);
}

void InstrumentManager::requestMarketData(const Contract& contract, MarketDataSubscriber* subscriber)
{
    //lock for threads
    std::cout<<subscriber;
    //MarketDataSubscriber* subscriber =qobject_cast<MarketDataSubscriber*>(QObject::sender());
    InstrumentView* instrumentView = MainWindow::mainWindow()->getInstrumentView();

    ATSYMBOL atsymbol  = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
    std::string symbol = ActiveTickFeed::Helper::ConvertString(atsymbol.symbol, _countof(atsymbol.symbol));

    readWritelock.lockForRead();
    if(_symbolToTickerId.count(symbol)!=0)
    {
        Instrument* instrument = _instruments[_symbolToTickerId[symbol]];
        QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, TradeUpdate )), subscriber, SLOT(onTradeUpdate(const TickerId, TradeUpdate)));
        QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, QuoteUpdate)), subscriber, SLOT(onQuoteUpdate(const TickerId, QuoteUpdate)));

        QObject::connect(this, SIGNAL(lastPriceUpdated(const TickerId, TradeUpdate )), instrumentView, SLOT(onTradeUpdate(const TickerId, TradeUpdate)));
        QObject::connect(this, SIGNAL(quoteUpdated(const TickerId, QuoteUpdate)), instrumentView, SLOT(onQuoteUpdate(const TickerId, QuoteUpdate)));

        readWritelock.unlock();
    }
    else
    {
        readWritelock.unlock();

        readWritelock.lockForWrite();
        TickerId tickerId = ++_tickerId;
        _symbolToTickerId[symbol] = tickerId;
        Instrument* instrument = new Instrument(tickerId, contract, 1);
        _instruments[tickerId] = instrument;
        readWritelock.unlock();

        QObject::connect(instrument, SIGNAL(lastPriceUpdated(const TickerId, TradeUpdate)), subscriber, SLOT(onTradeUpdate(const TickerId, TradeUpdate)));
        QObject::connect(instrument, SIGNAL(quoteUpdated(const TickerId, QuoteUpdate)), subscriber, SLOT(onQuoteUpdate(const TickerId, QuoteUpdate)));

        QObject::connect(this, SIGNAL(lastPriceUpdated(const TickerId, TradeUpdate )), instrumentView, SLOT(onTradeUpdate(const TickerId, TradeUpdate)));
        QObject::connect(this, SIGNAL(quoteUpdated(const TickerId, QuoteUpdate)), instrumentView, SLOT(onQuoteUpdate(const TickerId, QuoteUpdate)));

        Service::Instance()->getActiveTickAPI()->requestTradeStream(contract);
    }
}

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
    readWritelock.lockForWrite();
    delete _instruments[tickerId];
    _instruments.erase(tickerId);
    readWritelock.unlock();
}

TickerId InstrumentManager::getTickerId(const Contract& contract)
{ 
  //lock again
  TickerId tickerId=0;

  readWritelock.lockForRead();
  //ActiveTickFeed::ATSYMBOL symbol = ActiveTickFeed::Helper::StringToSymbol(contract.symbol);
  if(_symbolToTickerId.count(contract.symbol)!=0)
  {     
      tickerId = _symbolToTickerId[contract.symbol];
  }
  readWritelock.unlock();

  return tickerId;
}

void InstrumentManager::printThreadId()
{
    printf("\nInstrument Manager Thread \t");
    std::cout<<QThread::currentThreadId();
}


