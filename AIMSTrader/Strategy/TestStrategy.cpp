#include "Strategy/TestStrategy.h"
#include "Platform/Utils/TimeZone.h"
#include "Platform/Utils/TradingSchedule.h"
#include <iostream>

int x = TestStrategy::addStrategy();

TestStrategy::TestStrategy():Strategy("TestStrategy")
{
    QString start = "09:30";
    QString end = "15:30";
   // _tradingSchedule = new TradingSchedule("09:30","15:30", EST );
    _tradingSchedule = new TradingSchedule(start, end , EST );
}

TestStrategy::~TestStrategy()
{}

void TestStrategy::startStrategy()
{
    Contract contract;
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
    o1.action = "BUY";
    o1.lmtPrice=0;
    o1.referencePriceType=0;

        contract.symbol="INTC";
        placeOrder(contract,o1);

    //contract.symbol="APPL";
      //      placeOrder(contract,o);



}
