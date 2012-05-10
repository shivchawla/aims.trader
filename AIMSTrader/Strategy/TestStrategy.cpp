#include "Strategy/TestStrategy.h"
#include "Platform/Utils/TimeZone.h"
#include "Platform/Utils/TradingSchedule.h"
#include <iostream>

int x = TestStrategy::addStrategy();

TestStrategy::TestStrategy():OneMinuteMomentum("TestStrategy")
{
    QString start = "00:00";
    QString end = "24:00";
    _tradingSchedule = new TradingSchedule(start, end , EST);
}

TestStrategy::~TestStrategy()
{}

void TestStrategy::initialize()
{
    OneMinuteMomentum::initialize();
    std::vector<String> instruments;
    instruments.push_back("MSFT");
    instruments.push_back("INTC");
    setup(instruments, 1);
}

void TestStrategy::startStrategy()
{
    reportEvent("Starting Test strategy!!!");
    OneMinuteMomentum::startStrategy();
    /*Contract contract;
    //contract.conId = 36;
    contract.symbol = "MSFT";
    contract.secType = "STK";
    contract.expiry = "";
    contract.strike = 0.0;
    contract.right = "";
    contract.multiplier = "0";
    contract.exchange = "SMART";
    contract.primaryExchange = "ISLAND";
    contract.currency = "USD";

    //requestMarketDataFromAPI(contract);
    //requestMarketData(contract,IB);

    Order o;
    //int i=0;
<<<<<<< HEAD
    o.totalQuantity=5;
    o.orderType = "MKT";
    o.action = "BUY";
    o.lmtPrice=0;
    o.referencePriceType=0;
    placeOrder(contract,o);

    Order o1;
    o1.totalQuantity=5;
    o1.orderType = "MKT";
=======
        o.totalQuantity=5;
        //std::string orderType("MKT");
        o.orderType = "MKT";
        //std::string action("BUY");
        o.action = "BUY";
        o.lmtPrice=0;
        o.referencePriceType=0;
        placeOrder(contract,o);
       // std::cout<<o.orderType<<"\n";

    Order o1;
    o1.totalQuantity=5;
    //std::string orderType("MKT");
    o1.orderType = "MKT";
    //std::string action("BUY");
>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
    o1.action = "BUY";
    o1.lmtPrice=0;
    o1.referencePriceType=0;

<<<<<<< HEAD
    contract.symbol="INTC";
    placeOrder(contract,o1);*/
=======
        contract.symbol="INTC";
        placeOrder(contract,o1);

    //contract.symbol="APPL";
      //      placeOrder(contract,o);



>>>>>>> 6d5e798e2e8d358148ad8d04e8f285b6e36f6806
}
