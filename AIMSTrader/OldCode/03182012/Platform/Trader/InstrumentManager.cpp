#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"
#include "Platform/Position/Instrument.h"
#include "Platform/Strategy/Strategy.h"
#include "Platform/Utils/ThreadManager.h"
#include "Platform/Trader/TraderAssistant.h"
#include <iostream>
InstrumentManager::InstrumentManager():_tickerId(0)
{
    std::cout<<"Creating Instrument Manager\n";
    QThread* thread = ThreadManager::Instance()->requestThread();
    connect(thread,SIGNAL(started()),this, SLOT(printThreadId()));
    moveToThread(thread);
}

InstrumentManager::~InstrumentManager()
{
    this->thread()->quit();
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

void InstrumentManager::removeInstrument(const TickerId tickerId)
{
    delete _instruments[tickerId];
    _instruments.erase(tickerId);
}

TickerId InstrumentManager::getTickerId(const Contract& contract)
{
  if(_contractIdToTickerId.count(contract.conId)!=0)
  {     
      TickerId tickerId = _contractIdToTickerId[contract.conId];
  }
  return 0
}

void InstrumentManager::printThreadId()
{
    printf("\nInstrument Manager Thread \t");
    std::cout<<QThread::currentThreadId();
}


