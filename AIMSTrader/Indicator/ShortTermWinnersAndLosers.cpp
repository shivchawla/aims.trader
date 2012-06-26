#include "Indicator/ShortTermWinnersAndLosers.h"
#include "Platform/Indicator/Indicator.h"
#include "Platform/Trader/InstrumentManager.h"
#include "Platform/Startup/Service.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <QDateTime>

ShortTermWinnersAndLosers::~ShortTermWinnersAndLosers()
{}

ShortTermWinnersAndLosers::ShortTermWinnersAndLosers(/*Strategy* strategy*/):Indicator(/*strategy*/)
{
    //get instruments in index
    //create some kind of index factory
    //I believe active tick has given a huge list of index and corresponding symbols
    //use that
    _updateCount = 0;
    _minuteCount = 0;

    int n=2;//let's assume the length the length of index
    _returnSnapshots.reserve(n);
    for(int i = 0 ; i <n ; ++i)
    {
        _returnSnapshots.push_back(ReturnSnapshot());
    }
}

void ShortTermWinnersAndLosers::calculate()
{
    std::map<TickerId, PriceSnapshot>::iterator end = _priceSnapshots.end();
    std::map<TickerId, PriceSnapshot>::iterator it;
    int i=0;
    for(it=_priceSnapshots.begin();it!=end;++it)
    {
        _returnSnapshots[i].tickerId = it->first;
        PriceSnapshot snapshot = it->second;
        _returnSnapshots[i].returns = (snapshot.current/std::max(0.0000001, snapshot.past))-1;
        i++;
    }

    //std::cout<<_returnSnapshots.size();
    std::sort(_returnSnapshots.begin(), _returnSnapshots.end());

    /*std::vector<ReturnSnapshot>::iterator e = _returnSnapshots.end();
    std::vector<ReturnSnapshot>::iterator itt;
    for(itt = _returnSnapshots.begin();itt!=e;++itt)
    {
        std::cout<<itt->tickerId<<" ";
        std::cout<<itt->returns<<"\n";
    }*/

    //std::cout<<_returnSnapshots[0].tickerId<<" "<<_returnSnapshots[0].returns;
    //std::cout<<"\n"<<_returnSnapshots[1].tickerId<<" "<<_returnSnapshots[1].returns;

    //place order in first stock
    if(++_minuteCount>1)
    {
        closeAllPositions();
        //inititate new positions
        Order o;
        //int i=0;
        o.totalQuantity=5;
        o.orderType = "MKT";
        o.action = "BUY";
        o.goodTillDate = QDateTime::currentDateTime().addSecs(3).toString("YYYYMMDD hh:mm:ss").toStdString();
        //o.lmtPrice=0;
        //o.referencePriceType=0;

        //get the contract for the tickerId
        //either get a contract everytime or add a list of contract in here
        //or create a special map in InstrumentManager
        placeOrder(_returnSnapshots[0].tickerId, o);
    }
}

void ShortTermWinnersAndLosers::OnSnapshotUpdate(const TickerId tickerId, const double snapshot, const int minutes)
{
    if(minutes != _timeScale) {return;}

    if(_priceSnapshots.count(tickerId)!=0)
    {
        _priceSnapshots[tickerId].past = _priceSnapshots[tickerId].current;
        _priceSnapshots[tickerId].current = snapshot;
    }
    else
    {
        _priceSnapshots[tickerId].past = snapshot;
        _priceSnapshots[tickerId].current = snapshot;
    }

    if(++_updateCount == _numInstruments)
    {
        _updateCount=0;
        calculate();
    }
}

void ShortTermWinnersAndLosers::startIndicator()
{
    //place market request for underlying instrument
    ATContract aTcontract;
    //contract.conId = 36;
    aTcontract.contract.symbol = "MSFT";
    aTcontract.contract.secType = "STK";
    aTcontract.contract.expiry = "";
    aTcontract.contract.strike = 0.0;
    aTcontract.contract.right = "";
    aTcontract.contract.multiplier = "0";
    aTcontract.contract.exchange = "SMART";
    aTcontract.contract.primaryExchange = "ISLAND";
    aTcontract.contract.currency = "USD";

    Service::Instance()->getInstrumentManager()->requestMarketData(aTcontract, this, IB, Snapshot);
    aTcontract.contract.symbol="INTC";
    Service::Instance()->getInstrumentManager()->requestMarketData(aTcontract, this, IB, Snapshot);
}
